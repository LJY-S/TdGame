#pragma once

#include <vector>
#include <memory>

constexpr int SIZE_TILE{ 48 };  //每个地图块的尺寸，单位：像素
//constexpr int MAP_WIDTH{ 20 };
//constexpr int MAP_HEIGHT{ 15 };

struct Tile
{
    enum class SpecialFlag
    {
        None    = -1,
        Exit,           //终点
        Spawn,          //刷怪点

        First   = None,
        Last    = Spawn
    };

    enum class Direction
    {
        None,
        Up,
        Down,
        Left,
        Right,

        First = None,
        Last = Right
    };

    //TODO: 可以考虑用enum class替代int
    int terrian{ 0 };       //地形层
    int decoration{ -1 };   //装饰层
    SpecialFlag specialFlag{ SpecialFlag::None };   //特殊层
    int spawnPointId{ -1 };                         //刷怪点编号，仅当 specialFlag == Spawn 时有效
    Direction direction{ Direction::None };         //怪物行进方向

    bool hasTower{ false }; //是否有防御塔
};

//地图数据，二维数组表示
using TileMap = std::vector<std::vector<Tile>>;
