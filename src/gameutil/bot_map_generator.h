#ifndef INCLUDE_BOT_MAP_GENERATOR
#define INCLUDE_BOT_MAP_GENERATOR

#include <string>
#include <vector>
#include <list>
#include <rapidjson/document.h>
#include "misc/bot_random.h"

namespace bot {

class AIRobotTemplate;
class PlayerTemplate;
class TileTemplate;

class MapGenerator {
public:




    struct Map {


        void collectFreeSlots(std::vector<const RobotSlot*>& slots) const;

        void deployRobots(Random& rand, int maxRobotCount, const std::vector<std::string>& robotNames,
                          const std::vector<const AIRobotTemplate*>& robotTemplates);

        void createJson(rapidjson::Document& doc) const;
    };

public:
    MapGenerator();

    virtual ~MapGenerator()
    {}

    virtual bool init(const rapidjson::Value& json);

    virtual bool generate(const char* fileName)= 0;

protected:
    void initMap(Map& map);

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
