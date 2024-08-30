#pragma once

#include <arpa/inet.h>      //使用inet相关函数
#include <netinet/in.h>     //使用sockeaddr
#include <string>


// 封装socket地址类型
// 用于封装 IPV4 地址，将 IPV4 
class InetAddress
{
public:
    explicit InetAddress(uint16_t port = 0, std::string ip = "127.0.0.1");
    explicit InetAddress(const sockaddr_in &addr)
        : addr_(addr) {}

    std::string toIp() const;
    std::string toIpPort() const;
    uint16_t toPort() const;

    const sockaddr_in *getSockAddr() const { return &addr_; }
    void setSockAddr(const sockaddr_in &addr) { addr_ = addr; }

private:
    sockaddr_in addr_;
};