#pragma once

#include "Tile.h"

#include <SDL.h>

#include <memory>

class Route
{
public:
    Route() = default;
    Route(const TileMap& map, const SDL_Point& idxOrigin);
    ~Route() = default;

    const std::vector<SDL_Point>& getPointList() const;
private:
    //TODO: 如果无需关注路径顺序，可以用std::unordered_set替代std::vector，构造函数中就无需多创建一个set用于查重
    std::vector<SDL_Point> idxList;
};
