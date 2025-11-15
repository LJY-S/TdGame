#include "Route.h"

#include "SDL_Point_Extend.h"

#include <spdlog/spdlog.h>
#include <unordered_set>

Route::Route(const TileMap& map, const SDL_Point& idxOrigin)
{
    size_t widthMap{ map[0].size() };
    size_t heightMap{ map.size() };
    SDL_Point idxNext{ idxOrigin };

    //用于路径点查重
    std::unordered_set<SDL_Point> idxSet;

    while (true)
    {
        if (idxNext.x >= widthMap || idxNext.y >= heightMap)
            break;

        //如果路径点有重复，则退出
        if (idxSet.find(idxNext) != idxSet.end())
        {
            spdlog::warn("路径点重复，重复坐标: {}, {}", idxNext.x, idxNext.y);
            break;
        }

        //记录上个循环找到的下个路径点
        idxSet.insert(idxNext);
        idxList.push_back(idxNext);


        const Tile& tile{ map[idxNext.y][idxNext.x] };

        //如果到达终点，则退出
        if (tile.specialFlag == Tile::SpecialFlag::Exit)
            break;

        //寻找下一个路径点
        bool hasNext{ true };

        switch (tile.direction)
        {
        case Tile::Direction::Up:
            idxNext.y--;
            break;
        case Tile::Direction::Down:
            idxNext.y++;
            break;
        case Tile::Direction::Left:
            idxNext.x--;
            break;
        case Tile::Direction::Right:
            idxNext.x++;
            break;
        default:
            hasNext = false;
            break;
        }

        //如果没有下个路径点，则退出
        if(!hasNext)
            break;

    }
}

const std::vector<SDL_Point>& Route::getPointList() const
{
    return idxList;
}

