#ifndef INCLUDE_BOT_CONFIG
#define INCLUDE_BOT_CONFIG

#include <string>

namespace bot {

class Config {
public:
    static Config g_cfg;

    Config();

    ~Config();

    bool load(const char* fileName);

public:
    int m_width;
    int m_height;
    std::string m_title;
    int m_timeDeltaHistoryLen;
    int m_eventQueueSize;
};

} // end of namespace bot

#endif
