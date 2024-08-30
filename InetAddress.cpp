#include "InetAddress.h"

#include <iostream>
#include <string.h>

InetAddress::InetAddress(uint16_t port, std::string ip)
{
    bzero(&addr_, sizeof(addr_));
    addr_.sin_family = AF_INET;
    // 将一个 16 位的主机字节序的整数转换为网络字节序
    addr_.sin_port = htons(port);
    // 将 IPv4 地址的点分十进制字符串形式转换为网络字节序的 32 位二进制形式
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

std::string InetAddress::toIp() const
{
    // addr_
    char buf[64] = {0};
    // inet_ntop将网络地址从二进制形式转化为可读字符串形式
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
    return buf;
}
std::string InetAddress::toIpPort() const
{
    // ip:port
    char buf[64] = {0};
    // inet_ntop将网络地址从二进制形式转化为可读字符串形式
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
    size_t end = strlen(buf);
    // 将网络字节序的 16 位整数转换为主机字节序的函数
    uint16_t port = ntohs(addr_.sin_port);
    sprintf(buf + end, ":%u", port);
    return buf;
}
uint16_t InetAddress::toPort() const
{
    return ntohs(addr_.sin_port);
}