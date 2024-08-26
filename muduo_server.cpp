/*
muduo 网络库给用户提供了两个主要的类
TcpServer ：用于编写服务器程序的
TcpClient : 用于编写客户端程序的

epoll + 线程池
好处：能够把网络I/O的代码和业务代码区分开来
用户的断开和连接   用户的可读写事件
*/
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
using namespace std;
using namespace muduo;
using namespace muduo::net;
/*
基于muduo网络库开发服务器程序
1.组合TcpServer对象
2.创建EventLoop事件循环对象的指针
3.明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
*/
class ChatServer
{
public:
private:
    TcpServer _server;
    EventLoop *_loop;
};