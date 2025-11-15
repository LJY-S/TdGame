#include "Map.h"

#include <fstream>
#include <sstream>
#include "LJYTools/IntToEnumSafe.h"
#include <spdlog/spdlog.h>

using namespace std;

//去除字符串首尾空白字符
string trim(const string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos)
        return {};
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

//去除字符串所有空格
string trimAll(const string& str)
{
    string result;
    for (char ch : str)
    {
        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r')
            result += ch;
    }
    return result;
}

void Map::load(const char* path)
{
    ifstream file{ path };
    if(!file.good())
        throw runtime_error("地图文件打开失败");

    std::ifstream in("file.txt");


    TileMap newTileMap;
    int x{ -1 }, y{ -1 };
    string strLine;
    while (getline(file, strLine))
    {
        //读取一行
        y++, x = -1;
        newTileMap.emplace_back();

        strLine = trimAll(strLine);
        if (strLine.empty()) //跳过空行
            continue;

        stringstream ssLine{ strLine };
        string strTile;
        while (getline(ssLine, strTile, ','))
        {
            //读取一格，即读取一格瓦片的属性
            x++;
            newTileMap[y].emplace_back();

            if (strTile.empty()) //跳过空格
                continue;

            Tile& newTile = newTileMap[y].back();
            loadTileFromStr(strTile, newTile);
        }
    }

    file.close();

    if(newTileMap.empty() || newTileMap[0].empty())
        throw runtime_error("地图读取结果为空");

    tileMap = move(newTileMap);

    gererateFeature();
}

const TileMap& Map::getTileMap() const
{
    return tileMap;
}

const SDL_Point& Map::getIdxExit() const
{ 
    return idxExit;
}

inline const std::unordered_map<int, Route>& Map::getSpawnRoutePool() const 
{ 
    return spawnRoutePool;
}

void Map::placeTower(const SDL_Point& idx)
{
    if (idx.y < 0 || idx.y >= static_cast<int>(getHeight()) ||
        idx.x < 0 || idx.x >= static_cast<int>(getWidth()))
    {
        spdlog::warn("尝试在地图外放置防御塔，坐标: {}, {}", idx.x, idx.y);
        return;
    }
    Tile& tile = tileMap[idx.y][idx.x];
    if (tile.hasTower)
    {
        spdlog::warn("尝试在已有防御塔的瓦片上放置防御塔，坐标: {}, {}", idx.x, idx.y);
        return;
    }
    tile.hasTower = true;
}

void Map::removeTower(const SDL_Point& idx)
{
    if (idx.y < 0 || idx.y >= static_cast<int>(getHeight()) ||
        idx.x < 0 || idx.x >= static_cast<int>(getWidth()))
    {
        spdlog::warn("尝试在地图外移除防御塔，坐标: {}, {}", idx.x, idx.y);
        return;
    }
    Tile& tile = tileMap[idx.y][idx.x];
    if (!tile.hasTower)
    {
        spdlog::warn("尝试在无防御塔的瓦片上移除防御塔，坐标: {}, {}", idx.x, idx.y);
        return;
    }
    tile.hasTower = false;
}

size_t Map::getWidth() const
{
    return tileMap.empty() ? 0 : tileMap[0].size();
}

size_t Map::getHeight() const
{
    return tileMap.size();
}

template<typename T>
T Map::toTileEnum(int idx, const std::vector<int>& vec, T defaultValue, const char* dataName)
{
    if (vec.size() < idx + 1)
    {
        spdlog::warn("地图文件中 {} 数据缺失，使用默认值", dataName);
        return defaultValue;
    }

    return intToEnumS(vec[idx], defaultValue, [dataName]() {
        spdlog::warn("地图文件中 {} 数据非法，使用默认值", dataName);
    });
}

template<typename T>
T Map::toTileEnumB(int idx, const std::vector<int>& vec, T defaultValue, const char* dataName, bool warn)
{
    if (vec.size() < idx + 1)
    {
        spdlog::warn("地图文件中 {} 数据缺失，使用默认值", dataName);
        return defaultValue;
    }

    return intToEnumSB<T>(vec[idx], [dataName, warn]() {
        if(warn)
            spdlog::warn("地图文件中 {} 数据非法，", dataName);
    });
}

void Map::loadTileFromStr(const std::string& strTile, Tile& tile)
{
    //将字符串解析为 Tile 结构
    //先将字符串转换为int数组
    stringstream ssTile{ strTile };
    vector<int> values;
    string strValue;
    while (getline(ssTile, strValue, '\\'))
    {
        int value;
        try
        {
            value = stoi(strValue);
        }
        catch (const std::invalid_argument&)
        {
            value = -1;
        }
        values.push_back(value);
    }

    //根据int数组设置 Tile 结构
    tile.terrian = (values.size() < 1 || values[0] < 0) ? 0 : values[0];
    tile.decoration = (values.size() < 2) ? -1 : values[1];
    //tile.direction = toTileEnum(2, values, Tile::Direction::None);
    tile.direction = toTileEnum(2, values, Tile::Direction::None, "direction");
    //tile.specialFlag = toTileEnum(3, values, Tile::SpecialFlag::None);
    tile.specialFlag = toTileEnumB(3, values, Tile::SpecialFlag::None, "specialFlag", false);
    if (tile.specialFlag == Tile::SpecialFlag::Spawn)
    {
        tile.spawnPointId = (values.size() < 4 || values[3] < 0) ? 0 : values[3];
    }
}

void Map::gererateFeature()
{
    for (int y{ 0 }; y < getHeight(); y++)
    {
        for(int x{ 0 }; x < getWidth(); x++)
        {
            Tile& tile = tileMap[y][x];
            //跳过无特殊标记的瓦片
            if(tile.specialFlag <= Tile::SpecialFlag::None)
                continue;

            //记录刷怪点
            if (tile.specialFlag == Tile::SpecialFlag::Spawn)
            {
                int spawnId = tile.spawnPointId;
                if (spawnRoutePool.find(spawnId) != spawnRoutePool.end())
                {
                    spdlog::warn("地图中存在重复的刷怪点编号: {}，旧路径可能会被覆盖", spawnId);
                }
                Route route{ tileMap, { x, y } };
                if (route.getPointList().empty())
                {
                    spdlog::warn("地图中刷怪点编号 {} 的路径无效", spawnId);
                    continue;
                }
                spawnRoutePool.emplace(spawnId, move(route));
                continue;
            }

            //记录出口
            if (tile.specialFlag == Tile::SpecialFlag::Exit)
            {
                idxExit.x = x;
                idxExit.y = y;
                continue;
            }
        }
    }
}

