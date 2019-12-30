#include <cstdlib>
#include <cstring>
#include <iostream>
#include "bot_log.h"
#include "bot_config.h"
#include "bot_app.h"

void showUsageAndExit() 
{
    std::cerr << "Usage: bot -l logFile -c configFile -r resourceFolder" << std::endl;
    exit(1);
}

int main(int argc, char* argv[])
{
    const char *logFile = nullptr;
    const char *resDir = nullptr;
    const char* cfgFile = nullptr;

    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            logFile = argv[i + 1];
            i += 2;
        } 
        else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            cfgFile = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            resDir = argv[i + 1];
            i += 2;
        }
        else {
            showUsageAndExit();
        }
    }

    if (!logFile || !resDir || !cfgFile) {
        showUsageAndExit();
    }

#ifdef ENABLE_LOG
    if (!bot::Logger::g_logger.init(logFile, bot::Logger::LEVEL_INFO)) {
        exit(1);
    }
#endif

    if (!bot::Config::g_cfg.load(cfgFile)) {
        exit(1);
    }

    if (!bot::App::g_app.init(resDir)) {
        exit(1);
    }

    bot::App::g_app.run();
}