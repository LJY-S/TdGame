#pragma once

#include "Map.h"
#include "Enemy/wave.h"
#include "LJYTools/Singleton.h"

#include <array>
#include "Defender/DefenderType.h"

class ConfigManager : public Singleton<ConfigManager>
{
    friend class Singleton<ConfigManager>; // 允许 Singleton 访问私有构造函数
public:
    struct BaseData
    {
        int windowWidth{ 1280 };   //窗口宽度，单位：像素
        int windowHeight{ 720 };   //窗口高度，单位：像素
    };

    struct PlayerData
    {
        double speed{ 0 };                  //玩家移动速度，单位：单元格/秒
        double normalAttackInterval{ 0 };   //普通攻击 间隔，单位：秒
        double normalAttackDamage{ 0 };     //普通攻击伤害
        double skillInterval{ 0 };          //技能间隔，单位：秒
        double skillDamage{ 0 };            //技能伤害
    };

    struct DefenderData
    {
        constexpr static int MAX_LEVEL{ 10 };                   //防御塔最大等级
        std::array<double, MAX_LEVEL> interval{ 0 };            //防御塔攻击间隔，单位：秒
        std::array<double, MAX_LEVEL> damage{ 0 };              //防御塔攻击伤害
        std::array<double, MAX_LEVEL> viewRange{ 0 };           //防御塔攻击范围，单位：单元格/秒
        std::array<double, MAX_LEVEL> cost{ 0 };                //防御塔费用
        std::array<double, MAX_LEVEL - 1> upgrade_cost{ 0 };    //防御塔升级费用
    };

    struct EnemyData
    {
        double hp{ 0 };
        double speed{ 0 };              //移动速度，单位：单元格/秒
        double damage{ 0 };             //对玩家的伤害
        double damageToExit{ 0 };       //对终点建筑的伤害
        double recoverInterval{ 0 };    //回血间隔，单位：秒
        double recoverRange{ 0 };       //回血范围，单位：单元格
        double recoverIntensity{ 0 };   //每次回血量
    };

public:
    void loadLevelConfig(const char* path); //加载关卡配置，失败则抛出异常 runtime_error
    bool loadGameConfig(const char* path);  //加载游戏配置，失败则抛出异常 runtime_error

public:
    Map map;                        //地图数据
    std::vector<Wave> waveList;     //刷怪波次列表

    double initExitHp{ 0 };         //初始终点建筑生命值
    double initMoney{ 0 };          //初始金钱
    double initCoinPropMoney{ 0 };  //初始金币道具金钱

    int levelArcher{ 0 };           //弓箭塔等级
    int levelAxeMan{ 0 };           //战斧塔等级
    int levelGunner{ 0 };           //枪手塔等级

    bool gameWin{ false };          //游戏胜利标志
    bool gameFinish{ false };       //游戏结束标志

    SDL_Rect rectMapRender{ 0 };    //地图渲染区域

    BaseData baseData;              //基础数据
    PlayerData playerData;          //玩家数据

    std::array<DefenderData, (int)DefenderType::Size> defenderDatalAry; //各类防御塔数据
    std::array<int, (int)DefenderType::Size> defenderLevelAry;          //各类防御塔等级

    std::array<EnemyData, (int)EnemyType::Size> enemyDataAry;       //各类敌人数据
};
