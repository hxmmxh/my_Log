//使用双缓冲技术实现异步日志

#ifndef HXMMXH_ASYNCLOGGING_RINGBUFFERING_H
#define HXMMXH_ASYNCLOGGING_RINGBUFFERING_H

#include "LogStream.h"
#include "Thread.h"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <memory>
#include <thread>
#include <vector>

namespace hxmmxh
{
class AsyncLoggingRingBuffering
{
public:
    AsyncLoggingRingBuffering(const string &basename,
                                off_t rollSize,
                                int flushInterval = 3);
    ~AsyncLoggingRingBuffering()
    {
        if (running_)
        {
            stop();
        }
    }
    //前端和后端的接口，往缓冲区里写消息
    //logline代表一条完整的日志消息,len是它的长度
    void append(const char *logline, int len);

    void start()
    {
        running_ = true;
        thread_.start();
        latch_.wait(); //等待直到计数器为0
    }

    void stop()
    {
        running_ = false;
        cond_.notify_one();
        thread_.join();
    }

private:
    void threadFunc();

    typedef detail::FixedBuffer<detail::kLargeBuffer> Buffer;
    typedef std::unique_ptr<Buffer> BufferPtr;
    typedef std::vector<BufferPtr> BufferVector;

    const int flushInterval_; //每隔flushInterval_秒进行一个交换写入操作
    std::atomic<bool> running_;
    const std::string basename_;
    const off_t rollSize_;
    Thread thread_;
    CountDownLatch latch_; //计数器

    std::mutex mutex_;
    std::condition_variable cond_;
    BufferPtr currentBuffer_; //当前缓冲
    BufferPtr nextBuffer_;    //预备缓冲
    BufferVector fullbuffers_;    //待后端写入文件的缓冲块
};
} // namespace hxmmxh

#endif