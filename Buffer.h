#pragma once
#include <vector>
#include <string>
#include <algorithm>
// 底层缓冲区类型
class Buffer
{
public:
    // 小量数据常量，允许进来的数据非常小时，在数据的开头进行缓存，一般是8字节
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize)
        : buffer_(kCheapPrepend + initialSize), readerIndex_(kCheapPrepend), writerIndex_(kCheapPrepend)
    {
    }
    // readable 是指已经被读取到缓冲区中，但还没有被应用程序处理的数据。
    size_t readableBytes() const { return writerIndex_ - readerIndex_; }
    // writable 是指可以写入的剩余空间
    size_t writableBytes() const { return buffer_.size() - writerIndex_; }
    // 是指那些在可读数据前面，可以用来插入新数据的空间。
    size_t prependableBytes() const { return readerIndex_; }

    // 返回缓冲区的可读数据起始地址
    const char *peek() const { return begin() + readerIndex_; }
    // 更新已读空间位置
    void retrieve(size_t len)
    {
        if (len < readableBytes())
        {
            readerIndex_ += len; // 只读取的可读缓冲区的一部分（len）
        }
        else
        {
            retrieveAll();
        }
    }
    // 可读剩余空间为0，因此全部初始化为最开始的样子
    void retrieveAll()
    {
        readerIndex_ = writerIndex_ = kCheapPrepend;
    }
    // 数据转成string类型
    std::string retrieveAllAsString()
    {
        // 可读取的数据长度
        return retrieveAsString(readableBytes());
    }
    std::string retrieveAsString(size_t len)
    {
        std::string result(peek(), len);
        retrieve(len); // 对缓冲区进行复位操作
        return result;
    }
    // 确保缓冲区中有足够的可写空间
    void ensureWriteableBytes(size_t len)
    {
        if (writableBytes() < len)
        {
            makeSpace(len); // 扩容
        }
    }

    // 把[data， data+len]数据添加到writeable中
    void append(const char *data, size_t len)
    {
        ensureWriteableBytes(len);
        std::copy(data, data + len, beginWrite());
        writerIndex_ += len;
    }
    char *beginWrite()
    {
        return begin() + writerIndex_;
    }
    const char *beginWrite() const
    {
        return begin() + writerIndex_;
    }

    // 从fd上读取数据
    ssize_t readFd(int fd, int *saveErrno);
    // 通过fd发送数据
    ssize_t writeFd(int fd, int *saveErrno);

private:
    char *begin()
    {
        // 调用了*的重载函数+&运算法的重载函数 begin()代表容器的初始位置的迭代器，*代表容器的初始位置迭代器的引用，&代表取该引用的地址，最后得到的就是第一个元素的指针
        return &*buffer_.begin();
    }

    const char *begin() const
    {
        return &*buffer_.begin();
    }

    void makeSpace(size_t len)
    {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend)
        {
            buffer_.resize(writerIndex_ + len);
        }
        else
        {
            // 如果剩余空间足够，则将未读的数据移动至8字节位置，给数据腾出空间
            size_t readable = readableBytes();
            std::copy(begin() + readerIndex_, begin() + writerIndex_, begin() + kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writerIndex_ = readerIndex_ + readable;
        }
    }

    std::vector<char> buffer_;
    size_t readerIndex_; // 可读数据的起始位置
    size_t writerIndex_; // 可写数据的起始位置
};