#pragma once

#include "noncopyable.h"
#include "EventLoop.h"

#include <vector>
#include <unordered_map>

class Channel;
class Timestamp;

// muduo库中多路事件分发器的核心IP复用模块
class Poller : noncopyable
{
public:
    using ChannelList = std::vector<Channel *>;

    Poller(EventLoop *loop);
    virtual ~Poller() = default;

    // 给所有IO复用保留统一的接口
    virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels) = 0;
    virtual void updateChannel(Channel *channel) = 0;
    virtual void removeChannel(Channel *channel) = 0;

    // 判断参数channel是否在当前的poller中
    bool hasChannel(Channel *channel) const;

    // EventLoop可以通过该接口获取默认的IO复用具体实现
    static Poller *newDefaultPoller(EventLoop *loop);
    /*
    为什么上面这个没有写实现，那是因为这个函数本身需要创建一个新的poller
    而这样会include其他的派生类头文件，这是不好的，实现需要进行派生类去写
    */
protected:
    // 这个map的key: sockfd, value：sockfd所属的channel通道类型
    using ChannelMap = std::unordered_map<int, Channel *>;
    ChannelMap channels_;

private:
    EventLoop *ownerLoop_;
};