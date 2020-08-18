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

void MapGenerator::Map::collectFreeSlots(std::vector<const RobotSlot*>& slots) const
{
    int freeSlotCount = 0;
    int rowCount = static_cast<int>(m_robotSlots.size());
    int colCount = static_cast<int>(m_robotSlots[0].size());

    for (int r = 0; r < rowCount; ++r)
    {
        const std::vector<RobotSlot>& row = m_robotSlots[r];
        for (int c = 0; c < colCount; ++c)
        {
            if (!row[c].m_occupied)
            {
                ++freeSlotCount;
            }
        }
    }

    slots.resize(freeSlotCount);
    for (int r = 0, i = 0; r < rowCount; ++r)
    {
        const std::vector<RobotSlot>& row = m_robotSlots[r];
        for (int c = 0; c < colCount; ++c, ++i)
        {
            if (!row[c].m_occupied)
            {
                slots[i] = &row[c];
            }
        }
    }
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

void MapGenerator::Map::createJson(rapidjson::Document& doc) const
{
    using namespace rapidjson;

    doc.SetObject();
    Document::AllocatorType& allocator = doc.GetAllocator();

    doc.AddMember("numRows", m_rowCount, allocator);
    doc.AddMember("numCols", m_colCount, allocator);

    Value player(kObjectType);
    player.AddMember("x", m_playerX, allocator);
    player.AddMember("y", m_playerY, allocator);
    player.AddMember("directionX", m_playerDirectionX, allocator);
    player.AddMember("directionY", m_playerDirectionY, allocator);
    doc.AddMember("player", player, allocator);

    Value tiles(kArrayType);
    tiles.Reserve(m_tiles.size(), allocator);
    for (auto& t : m_tiles)
    {
        Value tile(kObjectType);
        tile.AddMember("name", StringRef(t.m_name->c_str()), allocator);
        tile.AddMember("x", t.m_x, allocator);
        tile.AddMember("y", t.m_y, allocator);
        tiles.PushBack(tile, allocator);
    }
    doc.AddMember("tiles", tiles, allocator);

    Value robots(kArrayType);
    robots.Reserve(m_robots.size(), allocator);
    for (auto& t : m_robots)
    {
        Value robot(kObjectType);
        robot.AddMember("name", StringRef(t.m_name->c_str()), allocator);
        robot.AddMember("x", t.m_x, allocator);
        robot.AddMember("y", t.m_y, allocator);
        robot.AddMember("directionX", t.m_directionX, allocator);
        robot.AddMember("directionY", t.m_directionY, allocator);
        robots.PushBack(robot, allocator);
    }
    doc.AddMember("robots", robots, allocator);
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

void MapGenerator::initMap(Map& map)
{
    map.m_rowCount = m_rand.get(m_minRowCount, m_maxRowCount);
    map.m_colCount = m_rand.get(m_minColCount, m_maxColCount);
    map.m_mapHeight = map.m_rowCount * GameMap::GRID_BREATH;
    map.m_mapWidth = map.m_colCount * GameMap::GRID_BREATH;

    int slotRowCount = static_cast<int>(floor(map.m_mapHeight / m_robotSlotSize));
    int slotColCount = static_cast<int>(floor(map.m_mapWidth / m_robotSlotSize));
    float y = m_robotSlotSize / 2.0f;

    map.m_robotSlots.resize(slotRowCount);
    for (int r = 0; r < slotRowCount; ++r)
    {
        std::vector<RobotSlot>& row = map.m_robotSlots[r];
        row.resize(slotColCount);

        float x = m_robotSlotSize / 2.0f;
        for (int c = 0; c < slotColCount; ++c)
        {
            row[c].m_x = x;
            row[c].m_y = y;
            x += m_robotSlotSize;
        }

        y += m_robotSlotSize;
    }
}

} // end of namespace bot
