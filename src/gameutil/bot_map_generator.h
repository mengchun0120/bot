#ifndef INCLUDE_BOT_MAP_GENERATOR
#define INCLUDE_BOT_MAP_GENERATOR

#include <string>
#include <rapidjson/document.h>
#include "misc/bot_random.h"

namespace bot {

class AIRobotTemplate;
class PlayerTemplate;
class TileTemplate;
class GeneratedMap;

class MapGenerator {
public:
    MapGenerator();

    virtual ~MapGenerator()
    {}

    virtual bool init(const rapidjson::Value& json);

    virtual bool generate(const char* fileName) = 0;

protected:
    virtual int deployRobots(GeneratedMap& map);

protected:
    int m_minRowCount, m_maxRowCount;
    int m_minColCount, m_maxColCount;
    int m_maxRobotCount;
    std::vector<std::string> m_robotNames;
    std::vector<const AIRobotTemplate*> m_robotTemplates;
    const PlayerTemplate* m_playerTemplate;
    float m_robotSlotSize;
    Random m_rand;
};

} // end of namespace bot

#endif
