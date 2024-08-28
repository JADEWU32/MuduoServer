#include "Logger.h"
#include "Timestamp.h"

#include <iostream>

// 获取日志唯一的实例对象
Logger &Logger::instance()
{
    // 单例模式，声明一个静态局部变量，并将该地址传递出去 后续都是同一个 logger 被调用
    static Logger logger;
    return logger;
}
// 设置日志级别
void Logger::setLogLevel(int level)
{
    logLevel_ = level;
}
// 写日志 [级别信息] time : msg
void Logger::log(std::string msg)
{
    // 通过日志级别对进来的信息进行打印
    switch (logLevel_)
    {
    case INFO:
        std::cout << "[INFO]";
        break;
    case ERROR:
        std::cout << "[ERROR]";
        break;
    case FATAL:
        std::cout << "[FATAL]";
        break;
    case DEBUG:
        std::cout << "[DEBUG]";
        break;
    default:
        break;
    }
    // 打印时间和msg
    std::cout << Timestamp::now().toString() << ":" << msg << std::endl;
}