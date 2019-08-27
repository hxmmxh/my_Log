#include "LogFile.h"
#include "Logging.h"
#include "Timestamp.h"

#include <stdio.h>
#include <unistd.h>
#include <iostream>

off_t kRollSize = 500 * 1000 * 1000;

using namespace std;
using namespace hxmmxh;

int g_total;                  //日志的总字节数
LogFile *g_logFile = nullptr; //通过LogFile存储日志内容

void SyncOutput(const char *msg, int len)
{
    g_total += len;
    g_logFile->append(msg, len);
}

void SyncFlush()
{
  g_logFile->flush();
}

double bench()
{
    Logger::setOutput(SyncOutput);
    Logger::setFlush(SyncFlush);

    const long kBatch = 1000 * 1000 * 10;
    string longstr(100, 'X');

    struct timespec t;
    t.tv_nsec = 1;
    t.tv_sec = 0;

    Timestamp start(Timestamp::now());
    for (long i = 0; i < kBatch; ++i)
    {
        LOG_INFO << longstr;
        //nanosleep(&t,NULL);
    }
    Timestamp end(Timestamp::now());

    double seconds = timeDifference(end, start);
    double speed = kBatch * 100 / seconds / (1024 * 1024);
    printf("Async: %f seconds, %ld bytes, %10.2f msg/s, %.2f MiB/s\n",
           seconds, kBatch * 100, kBatch / seconds, speed);
    return speed;
}

int main()
{
    g_logFile = new LogFile("test_log_st", 500 * 1000 * 1000);
    double result = 0;
    for (int i = 0; i < 5; ++i)
        result += bench();
    std::cout << "average: " << result / 5 << "MiB/s" << std::endl;
}
