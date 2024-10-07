#pragma once

#include "copyable.h"

#include "HttpRequest.h"

#include <string>

class Buffer;

class HttpContext : public copyable
{
public:
    enum HttpRequestParseState // 解析http请求的不同阶段枚举
    {
        kExpectRequestLine, // 正在解析http请求 行
        kExpectHeaders,     // 正在解析http请求 头部字段
        kExpectBody,        // 正在解析http请求 主体部分
        kGotAll,            // 解析完成
    };

    HttpContext()
        : state_(kExpectRequestLine)
    {
    }

    bool parseRequest(Buffer *buf, Timestamp receiveTime);

    bool gotAll() const
    {
        return state_ == kGotAll;
    }

    void reset()
    {
        state_ = kExpectRequestLine;
        HttpRequest dummy;
        request_.swap(dummy);
    }

    const HttpRequest &request() const
    {
        return request_;
    }

    HttpRequest &request()
    {
        return request_;
    }

private:
    bool processRequestLine(const char *begin, const char *end);

    HttpRequestParseState state_;
    HttpRequest request_;
};