#include "ConfigManager.h"

#include <fstream>
#include <sstream>

using namespace std;

void ConfigManager::loadLevelConfig(const char* path)
{
    ifstream file(path);

    if (!file.good())
        throw runtime_error("地图文件打开失败");

    
}
