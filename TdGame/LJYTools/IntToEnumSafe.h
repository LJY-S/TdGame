#pragma once

#include <stdexcept>
#include <type_traits>

/*
* int 转 enum 安全转换
* 参考 https://stackoverflow.com/a/58725346/24491719
* 根据 https://stackoverflow.com/questions/1005476/how-to-detect-whether-there-is-a-specific-member-variable-in-class/16000226#16000226
* 对回答做了改进，将模板参数默认类型由 int 改为 void，以及 decltype 中省去逗号运算符
* 使得其更简洁
* 
* 参考 https://stackoverflow.com/a/16111317/24491719
* 增加了可以指定错误处理函数的重载版本
* 
* 使用方法：

enum class Weekday
{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
    First   = Monday,
    Last    = Sunday
};

Weekday day = intToEnumS<Weekday>(3, Weekday::Monday);


* 或者使用指定错误处理函数的重载版本
Weekday day = intToEnumS<Weekday>(-1, Weekday::Monday, []() {
    spdlog::info("out of range");
});


* 还有超出范围时，超出最小值，返回 First，超出最大值返回Last的版本,
* 枚举之间对比，只会对比实际值，所以 无需担心 First 与第一个枚举值进行 == 运算后为 false
Weekday day = intToEnumSB<Weekday>(-1, []() {
    spdlog::info("out of range");
});
spdlog::info("day: {}, equal: {}", (int)day, day == Weekday::Monday);

*/


template <typename T, typename = void>
struct HasFirst : std::false_type {};

template <typename T>
struct HasFirst <T, decltype((void)T::First)> : std::true_type {};

template <typename T, typename = void>
struct HasLast : std::false_type {};

template <typename T>
struct HasLast <T, decltype((void)T::Last)> : std::true_type {};

/*
* 检查枚举类型是否含有 First 和 Last 枚举常量
* 以及 int 值是否在枚举类型范围外
* 返回 true 表示在范围外
*/
template <typename T>
inline bool enumCheck(int value)
{
    static_assert(HasFirst<T>::value,   "enum missing First");
    static_assert(HasLast<T>::value,    "enum missing Last");
    return  value < static_cast<int>(T::First) ||
            value >  static_cast<int>(T::Last);
}

/*
* int 转 enum 安全转换,带异常抛出
* SE 为 Safe with Exception 的缩写
*/
template <typename T>
T intToEnumSE(int value)
{
    if (enumCheck<T>(value))
    {
        throw std::out_of_range("Inv value");
    }

    return static_cast<T>(value);
}

/*
* int 转 enum 安全转换，不带异常抛出，需指定默认值
* S 为 Safe 的缩写
*/
template <typename T>
T intToEnumS(int value, T defaultValue)
{
    if(enumCheck<T>(value))
    {
        return defaultValue;
    }

    return static_cast<T>(value);
}

/*
* int 转 enum 安全转换,不带异常抛出，需指定默认值
* 可以指定错误处理lambda
* S 为 Safe 的缩写
*/
template <typename T, typename F>
T intToEnumS(int value, T defaultValue, F&& errorHandle)
{
    if (enumCheck<T>(value))
    {
        // 这里用std::forward包一层，是针对传入的是仿函数，且仿函数的operator()有&&尾部修饰词
        std::forward<F>(errorHandle)();
        return defaultValue;
    }

    return static_cast<T>(value);
}

/*
* int 转 enum 安全转换，不带异常抛出
* 超出范围时，超出最小值，返回 First，超出最大值返回Last
* 枚举之间对比，只会对比实际值，所以 无需担心 First 与第一个枚举值进行 == 运算后为 false
* S 为 Safe 的缩写，B 为 Bound的缩写
*/
template <typename T>
T intToEnumSB(int value)
{
    static_assert(HasFirst<T>::value,   "enum missing First");
    static_assert(HasLast<T>::value,    "enum missing Last");

    if (value < static_cast<int>(T::First))
    {
        return T::First;
    }
    else if (value > static_cast<int>(T::Last))
    {
        return T::Last;
    }
    else
    {
        return static_cast<T>(value);
    }
}

/*
* int 转 enum 安全转换，不带异常抛出
* 可以指定错误处理lambda
* 超出范围时，超出最小值，返回 First，超出最大值返回Last
* 枚举之间对比，只会对比实际值，所以 无需担心 First 与第一个枚举值进行 == 运算后为 false
* S 为 Safe 的缩写，B 为 Bound的缩写
*/
template <typename T, typename F>
T intToEnumSB(int value, F&& errorHandle)
{
    static_assert(HasFirst<T>::value,   "enum missing First");
    static_assert(HasLast<T>::value,    "enum missing Last");

    if (value < static_cast<int>(T::First))
    {
        // 这里用std::forward包一层，是针对传入的是仿函数，且仿函数的operator()有&&尾部修饰词
        std::forward<F>(errorHandle)();
        return T::First;
    }
    else if (value > static_cast<int>(T::Last))
    {
        // 与上同理
        std::forward<F>(errorHandle)();
        return T::Last;
    }
    else
    {
        return static_cast<T>(value);
    }
}