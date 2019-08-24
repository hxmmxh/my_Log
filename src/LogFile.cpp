#include "LogFile.h"
#include "Logging.h"

#include <cassert> //assert
#include <time.h>
#include <cstdio>  //FILE,setvbuf,fopen
#include <cstring> //strerror_r
#include <errno.h>
#include <fcntl.h>    //open
#include <sys/stat.h> //open
#include <unistd.h>   //gethostname

using namespace hxmmxh;

//a:打开或新建一个文本文件，只允许在文件末尾追写
//e:O_CLOEXEC
//子进程默认是继承父进程打开的所有fd,如果句柄加入了这个设置,在execve替换进程时就会关闭设置这个选项的所有fd.
AppendFile::AppendFile(StringArg filename)
    : fp_(::fopen(filename.c_str(), "ae")), // 'e' for
      writtenBytes_(0)
{
  assert(fp_);
  //设置fp_文件的缓冲区
  //当Buffer类型是Block Buffered时，往该Stream中写入的信息会先暂存在Buffer中，每次实际的写入都是以块(Block)大小为单位写入目标文件
  // _IOFBF表示Fully Buffered
  //int setvbuf(FILE *stream, char *buf, int mode, size_t size);
  ::setvbuf(fp_, buffer_, _IOFBF, sizeof(buffer_));
}

AppendFile::~AppendFile()
{
  ::fclose(fp_);
}

size_t AppendFile::write(const char *logline, size_t len)
{
  //行为和fwrite一样
  //返回确切写入的字节数，可能比传入的字节数要少
  //不加锁流操作,非线程安全
  //do not use locking (theydo not set locks themselves, and do not test for the presence of locksset by others) and hence are thread-unsafe
  return ::fwrite_unlocked(logline, 1, len, fp_);
}

void AppendFile::flush()
{
  //强迫将缓冲区内的数据写回参数stream 指定的文件中。
  ::fflush(fp_);
}

//写入长度为len的logline日志
void AppendFile::append(const char *logline, const size_t len)
{
  size_t n = write(logline, len);
  size_t remain = len - n;
  //如果一次性没有写完
  while (remain > 0)
  {
    //从已经写完的部分继续写
    size_t x = write(logline + n, remain);
    //还是无法写入则出错
    if (x == 0)
    {
      //如果ferror返回值为0（假），表示未出错。如果返回一个非零值，表示出错
      int err = ferror(fp_);
      if (err)
      {
        //strerror_tl定义在Logging文件
        fprintf(stderr, "AppendFile::append() failed %s\n", strerror_tl(err));
      }
      break;
    }
    n += x;
    remain = len - n; // remain -= x
  }

  writtenBytes_ += len;
}

LogFile::LogFile(const string &basename,
                 off_t rollSize,
                 int flushInterval,
                 int checkEveryN)
    : basename_(basename),
      rollSize_(rollSize),
      flushInterval_(flushInterval),
      checkEveryN_(checkEveryN),
      count_(0),
      startOfPeriod_(0),
      lastRoll_(0),
      lastFlush_(0)
{
  //验证basename是文件名，不是路径名
  assert(basename.find('/') == string::npos);
  rollFile(); //新建一个文件
}

LogFile::~LogFile() = default;

void LogFile::append(const char *logline, int len)
{
    std::lock_guard<std::mutex> lock(mutex_);
    append_unlocked(logline, len);

}

//file_是Append类型对象
//LogFile的工作都转交给它完成

void LogFile::append_unlocked(const char *logline, int len)
{
  //写入长度为len的logline日志
  file_->append(logline, len);

  if (file_->writtenBytes() > rollSize_) //文件写满
  {
    rollFile(); //换一个文件
  }
  else
  {
    ++count_; //写入次数加1
    //写入次数达到一定值时进行check工作
    if (count_ >= checkEveryN_)
    {
      count_ = 0;
      //返回自纪元 Epoch（1970-01-01 00:00:00 UTC）起经过的时间，以秒为单位
      time_t now = ::time(NULL);
      //算出日期，如果过了一天，thisPeroid会加1
      time_t thisPeriod_ = now / kRollPerSeconds_ * kRollPerSeconds_;
      //如果过了一天，则日志滚动，换个新文件写入日志
      if (thisPeriod_ != startOfPeriod_)
      {
        rollFile();
      }
      //和上一次flush的间隔超过过了设定的flushInterval_
      else if (now - lastFlush_ > flushInterval_)
      {
        lastFlush_ = now;
        //把缓冲区中内容写入文件中
        file_->flush();
      }
    }
  }
}

void LogFile::flush()
{
    std::lock_guard<std::mutex> lock(mutex_);
    file_->flush();
}

//日志滚动
bool LogFile::rollFile()
{
  time_t now = 0;
  //获取新建日志的文件名
  string filename = getLogFileName(basename_, &now);
  //转换成天数
  time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;

  if (now > lastRoll_)
  {
    lastRoll_ = now;
    lastFlush_ = now;
    startOfPeriod_ = start;
    //换一个文件
    //释放file_目前指向的对象，并将file_指向新构造的对象
    file_.reset(new AppendFile(filename));
    return true;
  }
  return false;
}

//获取日志名
//格式为
string LogFile::getLogFileName(const string &basename, time_t *now)
{
  string filename;
  //
  filename.reserve(basename.size() + 64);
  filename = basename;

  char timebuf[32];
  struct tm tm;
  *now = time(NULL);
  //把UTC秒数转换成时间结构，有时区转换，得到本地时间
  localtime_r(now, &tm);
  strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
  filename += timebuf;

  char hostname[256];
  if (::gethostname(hostname, sizeof(hostname)) == 0)
  {
    hostname[sizeof(hostname) - 1] = '\0';
    filename += hostname;
  }
  else
      filename += "unknownhost";

  char pidbuf[32];
  snprintf(pidbuf, sizeof pidbuf, ".%d", ::getpid());
  filename += pidbuf;

  filename += ".log";

  return filename;
}
