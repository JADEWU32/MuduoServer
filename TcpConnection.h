#pragma once

#include "noncopyable.h"
#include "Callbacks.h"
#include "Buffer.h"
#include "InetAddress.h"
#include "Timestamp.h"
#include "Socket.h"

#include <memory>
#include <atomic>
#include <string>
#include <any>

class Channel;
class EventLoop;
// class Socket;

class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop *loop,
                  const std::string &name,
                  int sockfd,
                  const InetAddress &localAddr,
                  const InetAddress &peerAddr);
    ~TcpConnection();

    EventLoop *getLoop() const { return loop_; }
    const std::string &name() const { return name_; }
    const InetAddress &localAddress() const { return localAddr_; }
    const InetAddress &peerAddress() const { return peerAddr_; }

    bool connected() const { return state_ == kConnected; }
    // 发送数据

    void send(const std::string &buf);
    void send(Buffer *buf);

    void setContext(const std::any &context) { context_ = context; };
    const std::any& getContext() const { return context_; }

    std::any* getMutableContext() { return &context_; }
    // 关闭数据
    void shutdown();

    void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
    void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }
    void setWriteCompleteCallback(const WriteCompleteCallback &cb) { writeCompleteCallback_ = cb; }
    void setHighWaterMarkCallback(const HighWaterMarkCallback &cb, size_t highWaterMark)
    {
        highWaterMarkCallback_ = cb;
        highWaterMArk_ = highWaterMark;
    }
    void setCloseCallback(const CloseCallback &cb) { closeCallback_ = cb; }

    // 连接建立
    void connectionEstablished();
    // 销毁链接
    void connectionDestroyed();

    int getsocketfd(){
        return socket_->fd();
    }

private:
    enum StateE
    {
        kDisconnected, // 连接已断开，或未连接
        kConnecting,   // 连接中，连接未完全建立
        kConnected,    // 已连接，可进行数据传输
        kDisconnecting // 连接正在断开
    };

    void setState(StateE state) { state_ = state; }

    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();

    void sendInLoop(const void *message, size_t len);

    void shutdownInLoop();

    EventLoop *loop_;
    const std::string name_;
    bool reading_;
    std::atomic_int state_;

    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;

    const InetAddress localAddr_;
    const InetAddress peerAddr_;

    ConnectionCallback      connectionCallback_;    // 建立连接时的回调函数
    MessageCallback         messageCallback_;       // 接收信息时的回调函数
    WriteCompleteCallback   writeCompleteCallback_; // 写入完成时的回调函数
    HighWaterMarkCallback   highWaterMarkCallback_;
    CloseCallback           closeCallback_;         // 关闭连接时的回调函数

    size_t highWaterMArk_;

    Buffer inputBuffer_;
    Buffer outputBuffer_;

    std::any context_;
};