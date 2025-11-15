#pragma once
#include <functional>

template <typename T>
void hashCombine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;

    //参考Boost库的 hash_combine 实现
    // https://www.boost.org/doc/libs/1_55_0/doc/html/hash/reference.html#boost.hash_combine
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
