#pragma once

enum class EnemyType
{
    Slim,           //普通史莱姆
    KingSlim,       //史莱姆王
    Skeleton,       //骷髅
    Goblin,         //哥布林
    GoblinPriest,   //哥布林祭司

    Size,
    First = Slim,
    Last = GoblinPriest
};