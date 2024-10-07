// // #include <muduo/net/EventLoop.h>
// // #include <muduo/net/http/HttpServer.h>

// #include <MuduoServer/TcpServer.h>
// #include <MuduoServer/Logger.h>

// using namespace muduo;
// using namespace muduo::net;

// void handleRequest(const HttpRequest& req, HttpResponse* resp) {
//     // 处理请求并生成响应
//     resp->setStatusCode(HttpResponse::k200Ok);
//     resp->setContentType("application/json");
//     resp->setBody(R"({"message": "Hello from Mymuduo!"})");
// }

// int main() {
//     EventLoop loop;
//     HttpServer server(&loop, InetAddress(8080), "MymuduoServer");
    
//     server.setHttpCallback(handleRequest);
//     server.start();
//     loop.loop();
//     return 0;
// }
