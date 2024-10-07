#include <MuduoServer/TcpServer.h>
#include <MuduoServer/HttpServer.h>
#include <MuduoServer/HttpRequest.h>
#include <MuduoServer/HttpResponse.h>
#include <MuduoServer/Logger.h>
#include <string.h>

#include <json/json.h>  // 需要安装 jsoncpp 库

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class EchoServer
{
public:
    EchoServer(EventLoop *loop,            // 事件循环
               const InetAddress &tcpAddr, // 服务器需要连接的IP地址和端口
               const InetAddress &httpAddr,
               const std::string &tcpname,
               const std::string &httpname) // 服务器名称
        : _server(loop, tcpAddr, tcpname), _loop(loop), _httpserver(loop, httpAddr, httpname)
    {
        // 给服务器注册用户连接的创建和断开回调函数
        _server.setConnectionCallback(bind(&EchoServer::onConnection, this, std::placeholders::_1));

        // 给服务器注册用户读写事件回调函数
        _server.setMessageCallback(bind(&EchoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        // 设置服务器端的线程数量 1个I/O线程，3个worker线程
        _server.setThreadNum(4);

        _httpserver.setHttpCallback(bind(&EchoServer::handleRequest, this, std::placeholders::_1, std::placeholders::_2));
    }

    void start()
    {
        _server.start();
        _httpserver.start();
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
        // _message = "这是一条消息";
        conn->send(msg); // 收到什么数据发回去什么数据
        int fd_= conn->getsocketfd();
        string _message = time.toString() + " : " + msg.substr(0, msg.size() - 2);
        _sendMessage[to_string(fd_)].emplace_back(_message);
        // _loop->addOutString(msg);
        // conn->shutdown();
    }

    void handleRequest(const HttpRequest &req, HttpResponse *resp)
    {
        if (req.path() == "/getdata"){
            // 处理请求并生成响应
            resp->setStatusCode(HttpResponse::k200Ok);
            resp->setContentType("application/json");
            resp->addHeader("Access-Control-Allow-Origin", "*"); // 允许所有来源
            // 创建JSON对象并填充数据
            Json::Value jsonData;
            jsonData["data"] = Json::arrayValue;  // 初始化为数组
            for(auto& temp : _sendMessage){
                // 添加数据到JSON数组
                Json::Value item;
                item["category"] = temp.first;
                for(auto& a : temp.second){
                    item["message"] = a;
                    jsonData["data"].append(item);
                }
            }
            
            // 将JSON数据转为字符串
            Json::StreamWriterBuilder writer;
            std::string jsonResponse = Json::writeString(writer, jsonData);
            resp->setBody(jsonResponse);
        }
        else if(req.path() == "/cleardata"){
            resp->setStatusCode(HttpResponse::k200Ok);
            resp->setContentType("text/plain");
            _sendMessage.clear(); // 清除服务端的缓存内容
        }
        else
        {
            // 处理未知请求
            resp->setStatusCode(HttpResponse::k404NotFound);
            resp->setContentType("text/plain");
        }
    }

    EventLoop *_loop;
    TcpServer _server;
    HttpServer _httpserver;
    unordered_map<string, vector<string>> _sendMessage;
};

int main()
{
    EventLoop loop;

    EchoServer server(&loop, InetAddress(7777), InetAddress(8080), "TcpMuduoServer", "HttpMymuduoServer");
    server.start();
    loop.loop();
    return 0;
}