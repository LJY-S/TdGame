#pragma once

#include "EnemyType.h"

#include <vector>

struct Wave
{
    struct SpawnEvent
    {
        double interval{ 0 };                   //距离上次刷怪的时间间隔，单位：秒
        int spawnPointId{ 0 };                  //刷怪点编号
        EnemyType enemyType{ EnemyType::Slim }; //刷出的怪物类型
    };

    double rawards{ 0 };                    //通关奖励
    double interval{ 0 };                   //本波怪物全部刷出后，距离下一波开始的时间间隔，单位：秒
    std::vector<SpawnEvent> spawnEventList; //本波刷怪事件列表
};