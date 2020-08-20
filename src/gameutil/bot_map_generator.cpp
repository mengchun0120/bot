#include <algorithm>
#include <cmath>
#include "misc/bot_json_utils.h"
#include "gameutil/bot_map_generator.h"
#include "gameutil/bot_game_map.h"
#include "app/bot_app.h"

namespace bot {

void randomDirection(Random& rand, float& directionX, float& directionY)
{
    static double TWO_PI = 2.0 * 3.14159265358979323846;
    double theta = rand.get(0.0, TWO_PI);
    directionX = static_cast<float>(cos(theta));
    directionY = static_cast<float>(sin(theta));
}

void MapGenerator::Map::deployRobots(Random& rand, int maxRobotCount, const std::vector<std::string>& robotNames,
                                     const std::vector<const AIRobotTemplate*>& robotTemplates)
{
    std::vector<const RobotSlot*> freeSlots;
    collectFreeSlots(freeSlots);
    
    int freeSlotCount = static_cast<int>(freeSlots.size());
    int lastSlot = freeSlotCount - 1;

    int maxPlayerSlot = std::min(20, freeSlotCount);
    int playerSlot = rand.get(0, maxPlayerSlot);

    m_playerX = freeSlots[playerSlot]->m_x;
    m_playerY = freeSlots[playerSlot]->m_y;
    randomDirection(rand, m_playerDirectionX, m_playerDirectionY);
    if (lastSlot != playerSlot)
    {
        std::swap(freeSlots[lastSlot], freeSlots[playerSlot]);
    }
    --freeSlotCount;
    --lastSlot;

    if (maxRobotCount > freeSlotCount)
    {
        maxRobotCount = freeSlotCount;
    }

    int robotTypeCount = static_cast<int>(robotNames.size());
    for (int i = 0; i < maxRobotCount; ++i, --freeSlotCount, --lastSlot)
    {
        int robotSlot = rand.get(0, freeSlotCount);
        int robotIdx = rand.get(0, robotTypeCount);

        m_robots.emplace_back();
        RobotItem& robot = m_robots.back();
        robot.m_name = &robotNames[robotIdx];
        robot.m_template = robotTemplates[robotIdx];
        robot.m_x = freeSlots[robotSlot]->m_x;
        robot.m_y = freeSlots[robotSlot]->m_y;
        randomDirection(rand, robot.m_directionX, robot.m_directionY);

        if (robotSlot != lastSlot)
        {
            std::swap(freeSlots[robotSlot], freeSlots[lastSlot]);
        }
    }
}

MapGenerator::MapGenerator()
    : m_minRowCount(0)
    , m_maxRowCount(0)
    , m_minColCount(0)
    , m_maxColCount(0)
    , m_maxRobotCount(0)
    , m_playerTemplate(nullptr)
    , m_robotSlotSize(0.0f)
{
}

bool MapGenerator::init(const rapidjson::Value& json)
{
    std::vector<JsonParseParam> params = {
        {&m_minRowCount,   "minRowCount",   JSONTYPE_INT},
        {&m_maxRowCount,   "maxRowCount",   JSONTYPE_INT},
        {&m_minColCount,   "minColCount",   JSONTYPE_INT},
        {&m_maxColCount,   "maxColCount",   JSONTYPE_INT},
        {&m_maxRobotCount, "maxRobotCount", JSONTYPE_INT},
        {&m_robotNames,    "robots",        JSONTYPE_STRING_ARRAY},
    };

    if (!parseJson(params, json))
    {
        return false;
    }

    const GameLib& lib = App::getInstance().getGameLib();
    int count = static_cast<int>(m_robotNames.size());

    m_playerTemplate = &lib.getPlayerTemplate();
    m_robotSlotSize = 2.0f * std::max(m_playerTemplate->getCoverBreathX(), m_playerTemplate->getCoverBreathY());

    m_robotTemplates.resize(count);
    for (int i = 0; i < count; ++i)
    {
        const AIRobotTemplate* t = lib.getAIRobotTemplate(m_robotNames[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find robot template %s", m_robotNames[i].c_str());
            return false;
        }
        m_robotTemplates[i] = t;

        float slotSize = 2.0f * std::max(t->getCoverBreathX(), t->getCoverBreathY());
        if (m_robotSlotSize < slotSize)
        {
            m_robotSlotSize = slotSize;
        }
    }

    return true;
}

} // end of namespace bot
