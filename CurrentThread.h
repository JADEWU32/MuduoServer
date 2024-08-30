#pragma once

#include <unistd.h>
#include <sys/syscall.h>

namespace CurrentThread
{
    extern __thread int t_cachedTid;

    void cacheTid();

    inline int tid()
    {
        /*
        __builtin_expect 分支预测，可接受两个参数，第二个参数为0则代表发生可能性小
        若不为0则是发生else的可能性小
        简单来说，因为代码的连续性，在汇编代码的内部，if和else中有一个代码需要JMP指令才能转移过去
        通过这个函数可以将更有可能执行的代码部分放在连续的部分，而不是使用JMP跳过去执行
        */ 
        if (__builtin_expect(t_cachedTid == 0, 0))
        {
            cacheTid();
        }
        return t_cachedTid;
    }
}