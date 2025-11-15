#include "GameManager.h"


#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

int GameManager::run(int argc, char* argv[])
{
    //帧率控制
    int fps{ 144 }; //目标帧率
    double timeConsumptionExpected{ 1000.0 / fps };             //每帧预计消耗时间，单位：毫秒
    Uint64 lastCounter{ SDL_GetPerformanceCounter() };          //上次计时器数值
    const Uint64 counterFrep{ SDL_GetPerformanceFrequency() };  //计时器频率

    //主事件循环
    while(!isQuit)
    {
        while (SDL_PollEvent(&event))
        {
            //处理所有事件，直到没有事件排队
            onInput();
        }
        
        //帧率控制
        Uint64 currentCounter{ SDL_GetPerformanceCounter() };   //当前计时器数值
        //计算上一帧消耗时间，单位：毫秒
        double lastTimeConsumption{
            (double)(currentCounter - lastCounter) / counterFrep * 1000
        };
        lastCounter = currentCounter;
        //如果上一帧消耗时间小于预计时间，则延时
        if (lastTimeConsumption < timeConsumptionExpected)
            SDL_Delay((Uint32)(timeConsumptionExpected - lastTimeConsumption));

        onUpdate(lastTimeConsumption);

        //清空渲染窗口
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //游戏渲染和更新逻辑
        onRender();

        //显示渲染结果
        SDL_RenderPresent(renderer);
    }

    return 0;
}

GameManager::GameManager()
{
    initAssert(!SDL_Init(SDL_INIT_EVERYTHING), "SDL 初始化失败");
    initAssert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG), "SDL_image 初始化失败");
    initAssert(!TTF_Init(), "SDL_ttf 初始化失败");

    initAssert(Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3, "SDL_mixer 初始化失败");
    initAssert(!Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048), "SDL_mixer 打开音频设备失败");

    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1"); //启用输入法候选词界面

    window = SDL_CreateWindow(
        "喵喵塔防", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    initAssert(window, "创建游戏窗口失败");

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    initAssert(renderer, "创建渲染器失败");
}

GameManager::~GameManager()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

inline void GameManager::initAssert(bool flag, const char* errMsg)
{
    if (flag)
        return;

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "错误", errMsg, window);

    SDL_Quit();
    exit(-1);
}

void GameManager::onInput()
{
    //退出事件，将结束主循环
    if (event.type == SDL_QUIT)
        isQuit = true;
}

void GameManager::onUpdate(double lastTimeConsumption)
{
}

void GameManager::onRender()
{
}

