#pragma once

#include "noncopyable.h"
#include "Timestamp.h"

#include <functional>
#include <memory>

class EventLoop; // 使用前向声明可以减少头文件暴露过多，以及互相包含头文件的情况

/*
需要理解清楚 EventLoop、Channel、Poller之间的关系 <= 对应于 Reactor 模型上多路事件分发器
Channel 理解为通道，用于封装sockfd和其感兴趣的event，例如EPOLLIN,EPOLLOUT事件
还绑定了poller返回的具体事件
*/
class Channel : noncopyable
{
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop *loop, int fd);
    ~Channel();

    // fd 得到 poller 通知以后，处理事件的，调用相应的回调方法
    void handleEvent(Timestamp receiveTime);

    // 设置回调函数对象
    void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    // 防止Channel被手动remove掉后，channel还在执行回调操作
    void tie(const std::shared_ptr<void> &);

    int fd() const { return fd_; }
    int events() const { return events_; }
    void set_revents(int revt) { revents_ = revt; }

    // 设置fd相应的事件状态
    void enableReading() { events_ |= kReadEvent; update(); }       // 感兴趣事件设置为读
    void disableReading() { events_ &= ~kReadEvent; update(); }     // 清除读感兴趣事件
    void enableWriting() { events_ |= kWriteEvent; update(); }      // 感兴趣事件设置为写
    void disableWriting() { events_ &= ~kWriteEvent; update(); }    // 清除写感兴趣事件
    void disableAll() { events_ = kNoneEvent; update(); }           // 清除所有感兴趣事件

    // 返回fd当前的事件状态
    bool isNoneEvent() const { return events_ == kNoneEvent; }
    bool isReading() const { return events_ & kReadEvent; }
    bool isWriting() const { return events_ & kWriteEvent; }

    // index用于区分channel状态
    int index() { return index_; }
    void set_index(int idx) { index_ = idx; }

    // one loop per thread
    EventLoop *ownerLoop() { return loop_; }
    void remove();

private:
    void update();
    void handleEventWithGuard(Timestamp receiveTime);

    // 状态描述符
    static const int kNoneEvent;  // 对任何事件不感兴趣
    static const int kReadEvent;  // 对读事件感兴趣
    static const int kWriteEvent; // 对写事件感兴趣

    EventLoop *loop_; // 事件循环
    const int fd_;    // fd,Poller 的监听对象
    int events_;      // 注册fd感兴趣的事件
    int revents_;     // poller 返回的具体发生的事件
    int index_;       // poller 的id

    std::weak_ptr<void> tie_;
    bool tied_;

    // 这四个函数用于装载感兴趣事件的回调函数
    // 因为Channel通道内可以获得fd最终发生的具体事件revents，所以它负责调用具体事件的回调操作
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};