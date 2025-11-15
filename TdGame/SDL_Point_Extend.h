#pragma once

#include "LJYTools/HashCombine.h"

#include <SDL_rect.h>

//SDL_Point == 运算符重载
[[nodiscard]] bool operator==(const SDL_Point& lhs, const SDL_Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

//SDL_Point 哈希函数
template <>
struct std::hash<SDL_Point>
{
    std::size_t operator()(const SDL_Point& s) const
    {
        auto intHasher{ std::hash<int>{} };

        std::size_t seed = 0;
        hashCombine(seed, intHasher(s.x));
        hashCombine(seed, intHasher(s.y));

        return seed;
    }
};