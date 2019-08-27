#include "AsyncLogging.h"
#include "Logging.h"
#include "Timestamp.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>

off_t kRollSize = 500 * 1000 * 1000;

using namespace hxmmxh;
using namespace std;

int g_total; //日志的总字节数
AsyncLoggingDoubleBuffering *g_asyncLog = nullptr;

void asyncOutput(const char *msg, int len)
{
    g_total += len;
    g_asyncLog->append(msg, len);
}

double bench()
{
    Logger::setOutput(asyncOutput);

    const long kBatch = 1000 * 1000 * 10;
    string longstr(100, 'X');

    struct timespec t;
    t.tv_nsec = 1;
    t.tv_sec = 0;

    Timestamp start = Timestamp::now();
    for (long i = 0; i < kBatch; ++i)
    {
        LOG_INFO << longstr;
        //nanosleep(&t,NULL);
    }
    Timestamp end = Timestamp::now();
    //返回的是秒
    double seconds = timeDifference(end, start);
    double speed = kBatch * 100 / seconds / (1024 * 1024);
    printf("Async: %f seconds, %ld bytes, %10.2f msg/s, %.2f MiB/s\n",
           seconds, kBatch * 100, kBatch / seconds, speed);
    return speed;
}

int main()
{

    AsyncLoggingDoubleBuffering log("Async Logging", kRollSize);
    log.start();
    g_asyncLog = &log;
    int result = 0;
    for (int i = 0; i < 5; ++i)
        result += bench();
    std::cout << "average: " << result / 5 << "MiB/s" << std::endl;
}
