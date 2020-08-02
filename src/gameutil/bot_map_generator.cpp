#include <algorithm>
#include "misc/bot_json_utils.h"
#include "gameutil/bot_map_generator.h"
#include "gameutil/bot_game_map.h"
#include "app/bot_app.h"

namespace bot {

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
    m_robotSlotSize = std::max(m_playerTemplate->getCoverBreathX(), m_playerTemplate->getCoverBreathY());

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

        float slotSize = std::max(t->getCoverBreathX(), t->getCoverBreathY());
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

    int slotRowCount = static_cast<int>(map.m_mapHeight / m_robotSlotSize);
    int slotColCount = static_cast<int>(map.m_mapWidth / m_robotSlotSize);
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
