#include "Buffer.h"

#include <unistd.h>
#include <errno.h>
#include <sys/uio.h>

const char Buffer::kCRLF[] = "\r\n";

// 从fd上读取数据
ssize_t Buffer::readFd(int fd, int *saveErrno)
{
    char extrabuf[65536] = {0}; // 栈空间
    struct iovec vec[2];

    const size_t writeable = writableBytes();
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writeable;

    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    // 如果剩余可写空间大于临时栈空间，则全部由可写空间进行缓存接收
    const int iovcnt = (writeable < sizeof(extrabuf)) ? 2 : 1;
    // readv 支持多个缓存区进行数据读取，这里可能将buffer内的剩余可写空间填满，然后剩余数据写入临时栈空间内
    const ssize_t n = ::readv(fd, vec, iovcnt);
    if (n < 0)
    {
        *saveErrno = errno;
    }
    else if (n <= writeable) // 缓冲区大小>数据
    {
        writerIndex_ += n;
    }
    else
    {
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writeable);
    }
    return n;
}
// 通过fd发送数据
ssize_t Buffer::writeFd(int fd, int *saveErrno)
{
    ssize_t n = ::write(fd, peek(), readableBytes());
    if (n < 0)
    {
        *saveErrno = errno;
    }
    return n;
}