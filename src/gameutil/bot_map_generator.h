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
    struct TileItem {
        TileItem(const std::string* name, const TileTemplate* t, float x, float y)
            : m_name(name)
            , m_template(t)
            , m_x(x)
            , m_y(y)
        {}

        const std::string* m_name;
        const TileTemplate* m_template;
        float m_x, m_y;
    };

    struct RobotItem {
        RobotItem(const std::string* name, const AIRobotTemplate* t, float x, float y,
            float directionX, float directionY)
            : m_name(name)
            , m_template(t)
            , m_x(x)
            , m_y(y)
            , m_directionX(directionX)
            , m_directionY(directionY)
        {}

        const std::string* m_name;
        const AIRobotTemplate* m_template;
        float m_x, m_y;
        float m_directionX, m_directionY;
    };

    struct RobotSlot {
        RobotSlot()
            : m_occupied(false)
            , m_x(0.0f)
            , m_y(0.0f)
        {}

        bool m_occupied;
        float m_x, m_y;
    };

    struct Map {
        std::list<TileItem> m_tiles;
        std::list<RobotItem> m_robots;
        std::vector<std::vector<RobotSlot>> m_robotSlots;
        int m_rowCount, m_colCount;
        float m_mapWidth, m_mapHeight;
        float m_playerX, m_playerY;
        float m_playerDirectionX, m_playerDirectionY;
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
