#define SDL_MAIN_HANDLED

#include "GameManager.h"

#include "LJYTools/IntToEnumSafe.h"

#include <spdlog/spdlog.h>

enum class Weekday
{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
    First = Monday,
    Last = Sunday
};

int main(int argc, char* argv[])
{
    // 设置控制台为 UTF-8 编码，防止中文输出乱码
    system("chcp 65001 > nul");

    Weekday day = intToEnumSB<Weekday>(4, []() {
        spdlog::info("out of range");
    });
    spdlog::info("day: {}, equal: {}", (int)day, day == Weekday::Friday);

    return GameManager::getInstance().run(argc, argv);
}
