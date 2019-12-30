#include <cstdlib>
#include "bot_log.h"
#include "bot_test.h"

int main(int argc, char *argv[])
{

#ifdef ENABLE_LOG
    if(!bot::Logger::g_logger.init("test_log.txt", bot::Logger::LEVEL_INFO)) {
        exit(1);
    }
#endif

    bot::testNameIdMap();

}
