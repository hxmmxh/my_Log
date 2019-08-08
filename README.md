## 日志基础
### 定义
* 诊断日志，文本的，供人阅读的日志，用于故障诊断和追踪，也用于性能分析
* Log Everything All The Time
* 还有一种交易日志，用记录状态变更，通过回放日志可以逐步恢复每一个修改之后的状态

### 记录的内容
* 收到的每条内部信息的id
* 收到的每条外部信息的全文
* 发出的每条消息的全文，每条消息具有全局唯一的id
* 关键内部状态的变更

### 组成
* 前端：提供供应用程序使用的接口，生成日志消息，Logging.{h.cpp}
* 后端：把日志消息写到目的地,Logging.{h.cpp}
* 多线程异步模型，AsyncLogging.{h.cpp}
* 输入的stream模型，LogStream.{h.cpp}

### 功能
* 日志消息可以有多个级别
* 日志消息可以有多个目的地
* 日志消息的格式可配置
* 可以设置运行时过滤器，控制不同组件的日志消息的级别和目的地

### 滚动(rolling)
* 满足一定条件时新建一个日志文件
* 当文件大小达到一定值时(默认是1GB)
* 每隔一定时间(每天新建一个)


## 多线程日志
### 多生产者-单消费者
* 每个线程都有自己的前端，整个程序共用一个后端，难点在于将日志数据从多个前端高效地传输到后端
* 前端:尽量低延迟，低CPU开销，无阻塞
* 后端：足够大地吞吐量，占用较少资源


## LogStream，日志内容的输入
### 前端API风格
* 使用C++的stream<<风格
* 使用自然，不用保持格式字符串与参数类型的一致性，随用随写，类型安全
* 当输出日志高于语句的日志级别时，打印日志是个空操作，运行时开销接近于零
* 还有种可选方案是printf函数的风格

### StringPiece类
* 为了实现了高效的字符串传递，节省string的拷贝开销，内建了一个StringPiece类
* 有两个数据成员 const char *ptr_; int length_;
* const char*和string能隐式转化成StringPiece，所以涉及这两个类的函数可以合并成使用StringPiece的函数
* 不具有字符串的控制权，无法修改字符，添加和删除操作只是移动ptr和改变length
* 不保证ptr字符串会以'\0'结尾
* 详细见StringPiece.h

### FixedBuffer
* LogStream的内部具有FixedBuffer实现类，用于缓冲输入，输入到LogStrem其实就是输入到FixedBuffer中
* 实现类似StringPiece类，具有char data_[]和char* cur_


### LogStream类
* 内含FixedBuffer成员buffer_，日志的写入都是先写入buffer_中
* 对字符串，整型、double类型（整型和double要先将之转换成字符型，再放到buffer里面)重载了<<操作符
* bool类型，转换成'1'或'0'以字符串的形式输入
* 提供convert和convertHex函数将整形转换成字符串(保持原样)，例如2048000->"2048000"
* 提供FormatSI和FormatIEC将整形转换成固定长度的字符串(数字+单位)(K,M，G,T...),例如2048000->"2.00Mi"或"2.04M"
* (unsigned) short转化成(unsigned)int后，再转换成字符串输入到buffer中
* (unsigned)int/long/long long直接转换成字符串输入到buffer中
* 对于指针，输出”0x1232"的十六进制字符串
* float先转换成double，double再以"%.12g"的格式输入

### 通用的格式化输出类Fmt
* 构造函数为Fmt(const char *fmt, T val)，含有成员char buf_[32];int length_;
* 以fmt的格式把val输入到buf_中，长度为length
* 重载了<<操作符，把Fmt对象输入到LogStream中

## Logging

### 日志级别
* ALL 最低等级的，用于打开所有日志记录。
* TRACE  designates finer-grained informational events than the DEBUG.Since:1.2.12，很低的日志级别，一般不会使用。
* DEBUG 指出细粒度信息事件对调试应用程序是非常有帮助的，主要用于开发过程中打印一些运行信息。
* INFO 消息在粗粒度级别上突出强调应用程序的运行过程。打印一些你感兴趣的或者重要的信息，这个可以用于生产环境中输出程序运行的一些重要信息，但是不能滥用，避免打印过多的日志。
* WARN 表明会出现潜在错误的情形，有些信息不是错误信息，但是也要给程序员的一些提示。
* ERROR 指出虽然发生错误事件，但仍然不影响系统的继续运行。打印错误和异常信息，如果不想输出太多的日志，可以使用这个级别。
* FATAL 指出每个严重的错误事件将会导致应用程序的退出。这个级别比较高了。重大错误，这种级别你可以直接停止程序了。
* OFF 最高等级的，用于关闭所有日志记录。  
如果将log level设置在某一个级别上，那么比此级别优先级高的log都能打印出来

### Logger类设计
* 内置SourceFile类封装写入的日志文件名以及路径
* 内置Impl类实现大部分功能，Logger只有一个Impl类型的数据成员
* Impl类记录


### 输出宏
* __FILE__指示当前文件名
* __func__指示当前函数名
* __LINE__指示运行当前文件的行数。


## LogFile
* 与日志存储文件相关的类和函数
* 在FileUtile命名空间中定义了两个功能类，ReadSmallFile和ApendFile类，LogFile的大部分功能都转交给它们完成

### 新学习函数

#### CLOEXEC,close-on-exec
* 作用：当调用exec()函数成功后，文件描述符会自动关闭
* 父进程执行fork()后，子进程以写时复制（COW，Copy-On-Write）方式获得父进程的数据空间、堆和栈副本，这其中也包括文件描述符。刚刚fork成功时，父子进程中相同的文件描述符指向系统文件表中的同一项（这也意味着他们共享同一文件偏移量）。
* 之后一般我们会调用exec执行另一个程序，此时会用全新的程序替换子进程的正文，数据，堆和栈等。默认子进程的文件描述符复制到exec后的进程中。这会导致如果父进程关闭之后，也无法调用驱动程序中的release函数来释放文件struct file 结构，只有当子进程也结束时，才会调用realse函数释放open打开的文件
* 因此我们需要会fork子进程后在子进程中直接执行close手动关掉对子进程无用的文件描述符，然后再执行exec。
* 但是在复杂系统中，有时我们fork子进程时已经不知道打开了多少个文件描述符（包括socket句柄等），这此时进行逐一清理确实有很大难度
* CLOEXEC的功能就是在fork子进程前打开某个文件句柄时就指定好：“这个句柄我在fork子进程后执行exec时就关闭”。
* 总结来说，设置CLOEXEC后，当fork子进程后，仍然可以使用fd。但子进程执行exec后系统就主动关闭子进程中的fd了，如果不设置该close_on_exec标志位，子进程的fd将复制到exec之后的进程中。

#### setvbuf,流缓存区操作 
* 系统中有三种可用的buffer类型，分别为无Buffer(Unbuffered)，块Buffer(Block Bufferd)和行Buffer(Line Buffered)。当一个流是Unbufferd的时候，往这个Stream中写入的信息会立刻写入目标文件。当Buffer类型是Block Buffered时，往该Stream中写入的信息会先暂存在Buffer中，每次实际的写入都是以块(Block)大小为单位写入目标文件。而当Buffer类型是Line Buffered时，往该Stream中输入的信息都会先暂存在Buffer中直到遇到新的一行，或是从一个终端设备读入例如stdin。fflush()可以强制将Buffer提前清空。
* 通常所有的文件和设备都是Block Buffered，如果任何一个流涉及到终端（例如stdin）那它将是Line Buffer类型。而标准错误流stderr的默认Buffer类型永远是Unbuffered类型。
* setvbuf() 函数可以用于任何已经打开的流以更改它的缓存区
* 设置块buffer时必须保证 buf 指定的空间一直到 stream 关闭时都有效
* [参考来源](https://my.oschina.net/u/2501393/blog/524386)
```cpp
#include <stdio.h>
//成功时返回 0。失败时它返回非零值(mode无效或请求无法完成)，同时设置 errno。 
int  setvbuf(FILE *stream, char *buf, int mode, size_t size);
//mode的取值是下面三个
//_IONBF 不缓存 
//_IOLBF 行缓存 
//_IOFBF 全缓存 
//setbuf类似于setvbuf(stream, buf, buf?_IOFBF:_IONBF,BUFSIZ);
void setbuf(FILE *stream, char *buf); 
void setbuffer(FILE *stream, char *buf, size_t size); 
//setlinebuf类似于setvbuf(stream, NULL, _IOLBF, 0);
void setlinebuf(FILE *stream) 
```

#### 流式文件操作函数
* 通过缓冲区来完成对文件的处理
* 围绕FILE指针来进行
* [参考1](https://blog.csdn.net/qq_15821725/article/details/78929344)
* [参考2](https://blog.csdn.net/no_wa/article/details/77802721)
```cpp
typedef struct {
int level;
unsigned flags;
char fd;
unsigned char hold;
int bsize;
unsigned char _FAR *buffer;
unsigned char _FAR *curp;
unsigned istemp;
short token;
}FILE;
```
```cpp
//参数filename指向要打开的文件名，mode表示打开状态的字符串
FILE *fopen(const char *filename,const char *mode)
//关闭用fopen()打开的文件,如果成功，返回0,失败返回EOF。
int fclose(FILE *fp)
//向流写一个字符,成功返回这个字符,失败返回EOF。
int fputc(int c, FILE *stream);
// 从流中读一个字符,成功返回这个字符,失败返回EOF。
int fputc(FILE *stream);
//写一个字符串到流中，
int fputs(const char *s, FILE *stream);
//从流中读一行或指定个字符,从流中读取n-1个字符，除非读完一行，参数s是来接收字符串，如果成功则返回s的指针，否则返回NULL。
char *fgets(char *s, int n, FILE *stream)
//按格式输入到流
int fprintf(FILE *stream, const char *format[, argument, ...])
//从流中按格式读取
int fscanf(FILE *stream, const char *format[, address, ...])
//从流中读取n个数据，每个数据有size个字节，并将数据放到buf的缓冲区中,如果成功，返回实际读取的块数(不是字节数)
size_t fread(void *buf, size_t size, size_t n, FILE *stream)
//把buf指向的nr个元素写到stream中，每个元素长为size;成功时，返回写入元素个数，小于nr表面错误。
size_t fwrite(void *buf, size_t size, size_t nr, FILE *stream);
//定位到流中指定的位置，如果成功返回0，参数offset是移动的字符数，whence是移动的基准，取值如下
//SEEK_SET 0 文件开头，SEEK_CUR 1 当前读写的位置，SEEK_END 2 文件尾部
int fseek(FILE *stream, long offset, int whence)
//把当前的读写位置回到文件开始，相当于fseek(fp,0L,SEEK_SET);
void rewind(FILE *stream)
//检测是否已到文件尾，是返回真，否则返回0
int feof(FILE *stream);
//返回流最近的错误代码
int ferror(FILE *stream)
//清除流最近的错误代码
void clearerr(FILE *stream);
//删除文件，成功返回0
int remove(const char *filename)
```
#### fopen mode



#### 直接I/O文件操作
* 通过直接存/取文件来完成对文件的处理
* 由文件描述符（file descriptors）称作fds的非负整数进行索引
* 文件描述符用int类型表示,默认从0开始；按照惯例进程至少会有三个文件描述符0是标准输入（stdin），1是标准输出（stdout），2是标准错误输出（stderr）
* [参考](https://blog.csdn.net/no_wa/article/details/77777451)
*[fopen和open对比](https://www.cnblogs.com/hnrainll/archive/2011/09/16/2178692.html)

#### strerror函数

```cpp
//根据参数errnum的错误代码来查询其错误原因的描述字符串, 然后将该字符串指针返回.
#include <string.h>
#include <errno.h>
//strerror()不是线程安全的，strerror_r()是线程安全的。
char *strerror(int errnum);
//第二个参数buf是用户提供的存储错误描述的缓存，第三个参数n是缓存的大小。
int strerror_r(int errnum, char *buf, size_t n);
```

#### fstat函数
* 由文件描述词取得文件状态
* [参考](https://blog.csdn.net/qq_44884619/article/details/89232269)
```cpp
struct stat
{
  dev_t st_dev;         /* 文件所在设备的标识  */
  ino_t st_ino;         /* 文件结点号  */
  mode_t st_mode;       /* 文件保护模式  */
  nlink_t st_nlink;     /* 硬连接数  */
  uid_t st_uid;         /* 文件用户标识  */
  gid_t st_gid;         /* 文件用户组标识  */
  dev_t st_rdev;        /* 文件所表示的特殊设备文件的设备标识  */
  off_t st_size;        /* 总大小，字节为单位  */
  blksize_t st_blksize; /* 文件系统的块大小  */
  blkcnt_t st_blocks;   /* 分配给文件的块的数量，512字节为单元  */
  time_t st_atime;      /* 最后访问时间  */
  time_t st_mtime;      /* 最后修改时间  */
  time_t st_ctime;      /* 最后状态改变时间  */
};
/*st_mode这个域不像其他域那么容易使用，需要一些宏予以配合。实际上这些宏就是一些特定位置为1的二进制数的绰号，我们使用它们和st_mode进行”&”操作，从而就可以得到某些特定的信息。
S_IFBLK：文件是一个特殊的块设备
S_IFDIR：文件是一个目录
S_IFCHR：文件是一个特殊的字符设备
S_IFIFO：文件是一个FIFO设备
S_IFREG：文件是一个普通文件
S_IFLNK：文件是一个符号链接、
S_ISUID：文件设置了SUID位
S_ISGID：文件设置了SGID位
S_ISVTX：文件设置了sticky位

S_IFMT：文件类型S_IRWXU：属主的读/写/执行权限，可以分成S_IXUSR, S_IRUSR, S_IWUSRS_IRWXG：属组的读/写/执行权限，可以分成S_IXGRP, S_IRGRP, S_IWGRPS_IRWXO：其他用户的读/写/执行权限，可以分为S_IXOTH, S_IROTH, S_IWOTH
*/

/*
带有参数的宏，类似与函数的使用方法：
S_ISBLK：测试是否是特殊的块设备文件
S_ISCHR：测试是否是特殊的字符设备文件
S_ISDIR：测试是否是目录
S_ISFIFO：测试是否是FIFO设备
S_ISREG：测试是否是普通文件
S_ISLNK：测试是否是符号链接
S_ISSOCK：测试是否是socket
/*

```
```cpp
//将参数fildes所指的文件状态，复制到参数buf所指的结构中
//fstat系统调用接受的是 一个“文件描述符”，而另外两个则直接接受“文件全路径”
//当文件是一个符号链接时，lstat返回的是该符号链接本身的信息；而stat返回的是该链接指向的文件的
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
int stat(const char *path, struct stat *buf); 
int fstat(int filedes, struct stat *buf); 
int lstat(const char *path, struct stat *buf); 
/*
成功执行时，返回0。失败返回-1，errno被设为以下的某个值
   EBADF：  文件描述词无效 
   EFAULT： 地址空间不可访问 
   ELOOP：  遍历路径时遇到太多的符号连接 
   ENAMETOOLONG：文件路径名太长 
   ENOENT：路径名的部分组件不存在，或路径名是空字串 
   ENOMEM：内存不足 
   ENOTDIR：路径名的部分组件不是目录 
*/
```

#### pread
* 用于多线程程序中进行读写
* pread函数相当于先后调用了lseek和read函数，但是还是有区别的，有以下两点区别：1. pread函数是原子操作，而先后调用两个函数不是原子操作;2. pread函数是不会改变当前文件偏移量的，而read和write函数会改变当前文件偏移量
```cpp
#include <unistd.h>
//在指定偏移offset位置开始读取count个字节
ssize_t pread(int fd, void *buf, size_t count, off_t offset);
ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);
```




####  strftime
strftime()函数是用来格式化一个日期、日期时间和时间的函数，支持date、datetime、time等类，把这些日期、日期时间或时间通过格式字符要求格式为字符串表示。相反strptime()函数就是从字符串表示的日期时间按格式化字符串要求转换为相应的日期时间。
[格式字符](https://blog.csdn.net/caimouse/article/details/50198695)
### ReadSmallFile
* 用于读取文件内容进入缓冲区
### AppendFile
* 用于往文件中写入内容



#### gethostname
```cpp
//如果函数成功，则返回0。如果发生错误则返回-1。错误号存放在外部变量errno中
#include <unistd.h>
int gethostname(char *name, size_t len);
```






## AsyncLogging
### 异步模型1：双缓冲技术
* 准备两块buffer：A和B,前端负责往buffer A填数据(日志消息)，后端负责将buffer B的数据写入文件
* buffer A写满后，交换A和B,让后端将buffer A的数据写入文件，前端则往buffer B种填入新的日志消息
* 为了及时将日志消息写入文件，即便buffer A未满，也会每3秒执行一次叫交换写入操作

### 异步模型2：ring环状


### 异步模型3：blocking queue
[blocking queue](https://www.cnblogs.com/WangHaiMing/p/8798709.html)


### 异步模型4 :BoundedBlockingQueue
[BoundedBlockingQueue](https://blog.csdn.net/kangroger/article/details/47129139)


