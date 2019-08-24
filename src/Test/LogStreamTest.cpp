//测试各种格式的输入

#include "../LogStream.h"

#include <iostream>
#include <limits>

using namespace std;
using namespace hxmmxh;
static int test_count = 0;
static int test_pass = 0;

#define CHECKEQUAL(expect, actual)                                                            \
    do                                                                                        \
    {                                                                                         \
        test_count++;                                                                         \
        if ((expect) == (actual))                                                             \
            test_pass++;                                                                      \
        else                                                                                  \
        {                                                                                     \
            std::cout << "line: " << __LINE__ << "error:   " << #expect << " != " << #actual; \
            std::cout << "    " #expect << " = " << expect << std::endl;                      \
        }                                                                                     \
    } while (0)

void testLogStreamBooleans()
{
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();
    CHECKEQUAL(buf.toString(), string(""));
    os << true; //bool值为true,输入”1“
    CHECKEQUAL(buf.toString(), string("1"));
    os << '\n';
    CHECKEQUAL(buf.toString(), string("1\n"));
    os << false;
    CHECKEQUAL(buf.toString(), string("1\n0"));
}

void testLogStreamIntegers()
{
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();
    CHECKEQUAL(buf.toString(), string(""));
    os << 1;
    CHECKEQUAL(buf.toString(), string("1"));
    os << 0;
    CHECKEQUAL(buf.toString(), string("10"));
    os << -1;
    CHECKEQUAL(buf.toString(), string("10-1"));
    os.resetBuffer();
    os << 0 << " " << 123 << 'x' << 0x64; //96+4
    CHECKEQUAL(buf.toString(), string("0 123x100"));
}
void testLogStreamIntegerLimits()
{
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();
    //int从-2147483648到2147483647，-2^31到2^31-1
    os << -2147483647;
    CHECKEQUAL(buf.toString(), string("-2147483647"));
    os << static_cast<int>(-2147483647 - 1);
    CHECKEQUAL(buf.toString(), string("-2147483647-2147483648"));
    os << ' ';
    os << 2147483647;
    CHECKEQUAL(buf.toString(), string("-2147483647-2147483648 2147483647"));
    os.resetBuffer();
    //2^15
    os << std::numeric_limits<int16_t>::min();
    CHECKEQUAL(buf.toString(), string("-32768"));
    os.resetBuffer();
    os << std::numeric_limits<int16_t>::max();
    CHECKEQUAL(buf.toString(), string("32767"));
    os.resetBuffer();
    //unsigned
    os << std::numeric_limits<uint16_t>::min();
    CHECKEQUAL(buf.toString(), string("0"));
    os.resetBuffer();
    //2^16-1
    os << std::numeric_limits<uint16_t>::max();
    CHECKEQUAL(buf.toString(), string("65535"));
    os.resetBuffer();

    os << std::numeric_limits<int32_t>::min();
    CHECKEQUAL(buf.toString(), string("-2147483648"));
    os.resetBuffer();

    os << std::numeric_limits<int32_t>::max();
    CHECKEQUAL(buf.toString(), string("2147483647"));
    os.resetBuffer();

    os << std::numeric_limits<uint32_t>::min();
    CHECKEQUAL(buf.toString(), string("0"));
    os.resetBuffer();

    os << std::numeric_limits<uint32_t>::max();
    CHECKEQUAL(buf.toString(), string("4294967295"));
    os.resetBuffer();

    os << std::numeric_limits<int64_t>::min();
    CHECKEQUAL(buf.toString(), string("-9223372036854775808"));
    os.resetBuffer();

    os << std::numeric_limits<int64_t>::max();
    CHECKEQUAL(buf.toString(), string("9223372036854775807"));
    os.resetBuffer();

    os << std::numeric_limits<uint64_t>::min();
    CHECKEQUAL(buf.toString(), string("0"));
    os.resetBuffer();

    os << std::numeric_limits<uint64_t>::max();
    CHECKEQUAL(buf.toString(), string("18446744073709551615"));
    os.resetBuffer();

    int16_t a = 0;
    int32_t b = 0;
    int64_t c = 0;
    os << a;
    os << b;
    os << c;
    CHECKEQUAL(buf.toString(), string("000"));
}

void testLogStreamFloats()
{
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();

    os << 0.0;
    CHECKEQUAL(buf.toString(), string("0"));
    os.resetBuffer();

    os << 1.0;
    CHECKEQUAL(buf.toString(), string("1"));
    os.resetBuffer();

    os << 0.1;
    CHECKEQUAL(buf.toString(), string("0.1"));
    os.resetBuffer();

    os << 0.05;
    CHECKEQUAL(buf.toString(), string("0.05"));
    os.resetBuffer();

    os << 0.15;
    CHECKEQUAL(buf.toString(), string("0.15"));
    os.resetBuffer();

    double a = 0.1;
    os << a;
    CHECKEQUAL(buf.toString(), string("0.1"));
    os.resetBuffer();

    double b = 0.05;
    os << b;
    CHECKEQUAL(buf.toString(), string("0.05"));
    os.resetBuffer();

    double c = 0.15;
    os << c;
    CHECKEQUAL(buf.toString(), string("0.15"));
    os.resetBuffer();

    os << a + b;
    CHECKEQUAL(buf.toString(), string("0.15"));
    os.resetBuffer();

    os << 1.23456789;
    CHECKEQUAL(buf.toString(), string("1.23456789"));
    os.resetBuffer();

    os << 1.234567;
    CHECKEQUAL(buf.toString(), string("1.234567"));
    os.resetBuffer();

    os << -123.456;
    CHECKEQUAL(buf.toString(), string("-123.456"));
    os.resetBuffer();
}

void testLogStreamVoid()
{
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();

    os << static_cast<void *>(0);
    CHECKEQUAL(buf.toString(), string("0x0"));
    os.resetBuffer();

    os << reinterpret_cast<void *>(8888); //8888的16进制就是0x22BB
    CHECKEQUAL(buf.toString(), string("0x22B8"));
    os.resetBuffer();
}

void testLogStreamStrings()
{
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();

    os << "Hello ";
    CHECKEQUAL(buf.toString(), string("Hello "));

    string chenshuo = "Xiemin Huang";
    os << chenshuo;
    CHECKEQUAL(buf.toString(), string("Hello Xiemin Huang"));
}

void testLogStreamFmts()
{
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();

    os << Fmt("%4d", 1); //右对齐，三个空格1
    CHECKEQUAL(buf.toString(), string("   1"));
    os.resetBuffer();

    os << Fmt("%4.2f", 1.2);
    CHECKEQUAL(buf.toString(), string("1.20"));
    os.resetBuffer();

    os << Fmt("%4.2f", 1.2) << Fmt("%4d", 43);
    CHECKEQUAL(buf.toString(), string("1.20  43"));
    os.resetBuffer();
}

void testLogStreamLong()
{
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();

    for (int i = 0; i < 399; ++i)
    {
        os << "123456789 ";
        CHECKEQUAL(buf.length(), 10 * (i + 1));
        //Buffer初始大小默认为kSmallBuffer，4000
        CHECKEQUAL(buf.avail(), 4000 - 10 * (i + 1));
    }

    os << "abcdefghi ";
    CHECKEQUAL(buf.length(), 3990);
    CHECKEQUAL(buf.avail(), 10);

    os << "abcdefghi";
    CHECKEQUAL(buf.length(), 3999);
    CHECKEQUAL(buf.avail(), 1);
}





int main()
{
    testLogStreamBooleans();
    testLogStreamIntegers();
    testLogStreamIntegerLimits();
    testLogStreamFloats();
    testLogStreamVoid();
    testLogStreamStrings();
    testLogStreamFmts();
    testLogStreamLong();
    std::cout << test_pass << "/" << test_count << " passed\n";
}