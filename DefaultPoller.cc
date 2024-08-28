#pragma once
// 该文件作为公共源文件进行包括和引用，这样可以避免出现派生类和基类互相引用的情况

#include "Poller.h"

#include <stdlib.h>
Poller *Poller::newDefaultPoller(EventLoop *loop)
{
    // getenv 是访问环境变量的函数 C语言的代码
    if (::getenv("MUDUO_USE_POLL"))
    {
        return nullptr; // 生成poll的实例
    }
    else
    {
        return nullptr; // 生成epoll的实例
    }
}