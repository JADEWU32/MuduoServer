#include <MuduoServer/TcpServer.h>
#include <MuduoServer/Logger.h>

#include <string>

class EchoServer
{
public:
    EchoServer(EventLoop *loop,         // 事件循环
               const InetAddress &Addr, // 服务器需要连接的IP地址和端口
               const std::string &name)      // 服务器名称
        : _server(loop, Addr, name), _loop(loop)
    {
        // 给服务器注册用户连接的创建和断开回调函数
        _server.setConnectionCallback(bind(&EchoServer::onConnection, this, std::placeholders::_1));

        // 给服务器注册用户读写事件回调函数
        _server.setMessageCallback(bind(&EchoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        // 设置服务器端的线程数量 1个I/O线程，3个worker线程
        _server.setThreadNum(4);
    }

    void start()
    {
        _server.start();
    }

private:
    // 专门处理用户的连接创建和断开 epoll listenfd accept
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            LOG_INFO("Connection UP : %s", conn->peerAddress().toIpPort().c_str());
        }
        else
        {
            LOG_INFO("Connection DOWN : %s", conn->peerAddress().toIpPort().c_str());
        }
    }

    // 专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区
                   Timestamp time)               // 接收到数据的时间信息
    {
        std::string msg = buffer->retrieveAllAsString();
        conn->send(msg); // 收到什么数据发回去什么数据
        // conn->shutdown();
    }
    EventLoop *_loop;
    TcpServer _server;
};

int main()
{
    EventLoop loop;
    InetAddress addr(8080);                          // 默认是127.0.0.1
    EchoServer server(&loop, addr, "EchoServer-01"); // Acceptor non-blocking listenfd create bind
    server.start();                                  // listen loopthread listenfd ->acceptorChannel ->mainLoop
    loop.loop();                                     // 启动mainloop的底层Poller
    return 0;
}