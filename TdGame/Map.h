#pragma once

#include "Route.h"

#include <SDL.h>
#include <string>
#include <unordered_map>

/*
* TODO: 修改地图文件格式，使得Tile加载能够适配枚举值
* TODO: TileMap读取无需副本
*/
class Map
{
public:
    Map() = default;
    ~Map() = default;

    //从文件加载地图，失败则抛出异常 runtime_error
    void load(const char *path);
    
    size_t getWidth() const;
    size_t getHeight() const;

    const TileMap& getTileMap() const;
    const SDL_Point& getIdxExit() const;
    const std::unordered_map<int, Route>& getSpawnRoutePool() const;

    void placeTower(const SDL_Point& idx);
    void removeTower(const SDL_Point& idx);

private:
    // 将int数组中的指定数据转换为枚举值，如果数据缺失或非法则返回默认值
    // 此模板只在本头文件对应cpp文件中使用，无需将实现放在头文件中，减少本头文件的依赖
    template<typename T>
    inline T toTileEnum(int idx, const std::vector<int>& vec, T defaultValue, const char* dataName);

    // 将int数组中的指定数据转换为枚举值，如果数据缺失则返回默认值
    // 如果超出范围，超出最小值，返回 First，超出最大值返回Last
    // 此模板只在本头文件对应cpp文件中使用，无需将实现放在头文件中，减少本头文件的依赖
    template<typename T>
    inline T toTileEnumB(int idx, const std::vector<int>& vec, T defaultValue, 
                         const char* dataName, bool warn);

    void loadTileFromStr(const std::string& strTile, Tile& tile);
    void gererateFeature();


    TileMap tileMap;

    //地图特征数据
    SDL_Point idxExit{ 0 };                         //地图上敌人的撤离点
    std::unordered_map<int, Route> spawnRoutePool;  //刷怪点编号到路径的映射
};