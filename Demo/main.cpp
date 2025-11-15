#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>

int main()
{
    //初始化SDL相关库
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);
    TTF_Init();

    //打开音频通道
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    //初始化窗口
    SDL_Window* window{ SDL_CreateWindow(
        u8"你好世界", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN) };

    //创建渲染器
    SDL_Renderer* renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) };

    //图片渲染
    SDL_Surface* sufImg{ IMG_Load("104001022_p0.jpg") };
    SDL_Texture* texImg{ SDL_CreateTextureFromSurface(renderer, sufImg) };
    SDL_Rect rectImg{0, 0, sufImg->w, sufImg->h};

    //字体渲染
    TTF_Font* font{ TTF_OpenFont("ipix.ttf", 32) };
    SDL_Color fontColor{ 255, 255, 255, 255 };
    SDL_Surface* sufText{ TTF_RenderUTF8_Blended(font, u8"你好世界", fontColor) };
    SDL_Texture* texText{ SDL_CreateTextureFromSurface(renderer, sufText) };
    SDL_Rect rectText{ 0, 0, sufText->w, sufText->h };

    //加载音乐
    Mix_Music* music{ Mix_LoadMUS("music.mp3") };
    Mix_FadeInMusic(music, -1, 1500);

    //鼠标位置
    SDL_Point posCursor{ 0, 0 };

    //帧率控制
    int fps{ 144 };
    double timeConsumptionExpected{ 1000.0 / fps };
    Uint64 lastCounter{ SDL_GetPerformanceCounter() };
    const Uint64 counterFrep{ SDL_GetPerformanceFrequency() };

    //主事件循环
    SDL_Event event;
    bool isQuit{ false };
    while (!isQuit)
    {
        while (SDL_PollEvent(&event))
        {
            //处理所有事件，直到没有事件排队

            //退出事件，将结束主循环
            if (event.type == SDL_QUIT)
            {
                isQuit = true;
                break;
            }

            //记录鼠标位置
            if (event.type == SDL_MOUSEMOTION)
            {
                posCursor.x = event.motion.x;
                posCursor.y = event.motion.y;
                break;
            }
        }

        //帧率控制
        Uint64 currentCounter{ SDL_GetPerformanceCounter() };
        double lastTimeConsumption{
            (double)(currentCounter - lastCounter) / counterFrep * 1000
        };
        if (lastTimeConsumption < timeConsumptionExpected)
            SDL_Delay((Uint32)(timeConsumptionExpected - lastTimeConsumption));
        lastCounter = currentCounter;

        //更新游戏状态，绘制当前帧
        
        //清空渲染窗口
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //渲染图片
        rectImg.x = posCursor.x;
        rectImg.y = posCursor.y;
        SDL_RenderCopy(renderer, texImg, nullptr, &rectImg);

        //画圆
        filledCircleRGBA(renderer, posCursor.x, posCursor.y, 50, 255, 0, 0, 125);

        //渲染字体
        rectText.x = posCursor.x;
        rectText.y = posCursor.y;
        SDL_RenderCopy(renderer, texText, nullptr, &rectText);

        //更新渲染
        SDL_RenderPresent(renderer);
    }

    return 0;
}
