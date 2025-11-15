#pragma once

/*
* 模板化单例模式
* 使用方法：
* class MyClass : public Singleton<MyClass> 
{
    friend class Singleton<MyClass>; // 允许 Singleton 访问私有构造函数
private:
    MyClass() {} // 私有构造和析构
    ~MyClass() {}
};
*/

template <typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        static T instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

protected:
    Singleton() = default;
    ~Singleton() = default;
};

