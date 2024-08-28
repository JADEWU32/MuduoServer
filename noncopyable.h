#pragma once
/*
noncopyable被继承后，无法进行拷贝构造和赋值操作
但是其派生类可以进行正常的构造和机构
*/

class noncopyable
{
public:
    noncopyable(const noncopyable &) = delete;
    noncopyable &operator=(const noncopyable &) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};