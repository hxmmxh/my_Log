#include "AsyncLogging.h"
#include "LogFile.h"
#include "../Time/Timestamp.h"

#include <stdio.h>
#include <utility>//move
#include <chrono>

using namespace hxmmxh;

AsyncLoggingDoubleBuffering::AsyncLoggingDoubleBuffering(const string &basename, off_t rollSize, int flushInterval)
    : flushInterval_(flushInterval),
      running_(false),
      basename_(basename),
      rollSize_(rollSize),
      thread_(std::bind(&AsyncLoggingDoubleBuffering::threadFunc, this), "Logging"),
      latch_(1),
      currentBuffer_(new Buffer),
      nextBuffer_(new Buffer),
      fullbuffers_()
{
    currentBuffer_->bzero();
    nextBuffer_->bzero();
    fullbuffers_.reserve(16);
}

//往缓冲区里写消息
void AsyncLoggingDoubleBuffering::append(const char *logline, int len)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (currentBuffer_->avail() > len)
    {
        currentBuffer_->append(logline, len);
    }
    else
    {
        fullbuffers_.push_back(std::move(currentBuffer_));

        if (nextBuffer_)
        {
            currentBuffer_ = std::move(nextBuffer_);
        }
        //写入速度太快，备用缓冲也没有了的话，新建一个缓冲区
        else
        {
            currentBuffer_.reset(new Buffer); 
        }
        currentBuffer_->append(logline, len);
        //通知开始写入文件
        cond_.notify_one();
    }
}

void AsyncLoggingDoubleBuffering::threadFunc()
{
  assert(running_ == true);
  latch_.countDown();//计数器减一
  LogFile output(basename_, rollSize_, false);//创建输出文件
  //后端的两个缓冲区
  BufferPtr newBuffer1(new Buffer);
  BufferPtr newBuffer2(new Buffer);
  newBuffer1->bzero();
  newBuffer2->bzero();
  //后端的待写入文件缓冲块数组
  BufferVector buffersToWrite;
  //预留存储空间
  buffersToWrite.reserve(16);
  while (running_)
  {
    //初始状态，后端的两个缓冲块存在且为空，待写入缓冲块数组为空
    assert(newBuffer1 && newBuffer1->length() == 0);
    assert(newBuffer2 && newBuffer2->length() == 0);
    assert(buffersToWrite.empty());

    {
      std::unique_lock<std::mutex> lock(mutex_);
      //前端写频率不高，buffers_为空，则超时三秒后交换，写入文件
      if (fullbuffers_.empty()) 
      {
          //等待超时或收到信号
          cond_.wait_for(lock, std::chrono::seconds(flushInterval_));
      }
      fullbuffers_.push_back(std::move(currentBuffer_));
      currentBuffer_ = std::move(newBuffer1);
      buffersToWrite.swap(fullbuffers_);
      if (!nextBuffer_)
      {
        nextBuffer_ = std::move(newBuffer2);
      }
    }

    assert(!buffersToWrite.empty());

    //写入的buffer过多，可能出现了问题
    //处理日志堆积，直接丢弃多余的日志buffer
    if (buffersToWrite.size() > 25)
    {
      char buf[256];
      snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
               Timestamp::now().toFormattedString().c_str(),
               buffersToWrite.size()-2);
      fputs(buf, stderr);
      output.append(buf, static_cast<int>(strlen(buf)));
      buffersToWrite.erase(buffersToWrite.begin()+2, buffersToWrite.end());
    }

    //写入LOG文件
    for (const auto& buffer : buffersToWrite)
    {
      output.append(buffer->data(), buffer->length());
    }

    //正常情况只要两个元素空间
    if (buffersToWrite.size() > 2)
    {
      buffersToWrite.resize(2);
    }

    if (!newBuffer1)
    {
      assert(!buffersToWrite.empty());
      newBuffer1 = std::move(buffersToWrite.back());
      buffersToWrite.pop_back();
      newBuffer1->reset();
    }

    if (!newBuffer2)
    {
      assert(!buffersToWrite.empty());
      newBuffer2 = std::move(buffersToWrite.back());
      buffersToWrite.pop_back();
      newBuffer2->reset();
    }

    buffersToWrite.clear();
    output.flush();
  }
  output.flush();
}