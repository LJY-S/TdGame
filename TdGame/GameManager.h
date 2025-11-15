#pragma once

#include "LJYTools/Singleton.h"

#include <SDL.h>

/*
* TODO: 增加肉鸽元素，比如每关结束得到随机卡牌，可以升级金币道具的金币数量等
*/
class GameManager : public Singleton<GameManager> 
{
    friend class Singleton<GameManager>; // 允许 Singleton 访问私有构造函数

public:
    int run(int argc, char* argv[]);

private:
    GameManager();
    ~GameManager();

    void initAssert(bool flag, const char* errMsg);

    void onInput();
    void onUpdate(double lastTimeConsumption);
    void onRender();

private:
    SDL_Window* window{ nullptr };
    SDL_Renderer* renderer{ nullptr };

    SDL_Event event;
    bool isQuit{ false };
};