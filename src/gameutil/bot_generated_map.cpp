#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <fstream>
#include "gameutil/bot_generated_map.h"
#include "gameutil/bot_game_map.h"

namespace bot {

GeneratedMap::GeneratedMap(int rowCount, int colCount, float slotSize)
    : m_rowCount(rowCount)
    , m_colCount(colCount)
    , m_mapWidth(m_rowCount* GameMap::GRID_BREATH)
    , m_mapHeight(m_colCount* GameMap::GRID_BREATH)
    , m_playerX(0.0f)
    , m_playerY(0.0f)
    , m_playerDirectionX(0.0f)
    , m_playerDirectionY(0.0f)
    , m_slotSize(slotSize)
{
    initSlots();
}

void GeneratedMap::initSlots()
{
    int slotRowCount = static_cast<int>(floor(m_mapHeight / m_slotSize));
    int slotColCount = static_cast<int>(floor(m_mapWidth / m_slotSize));
    float y = m_slotSize / 2.0f;

    m_slots.resize(slotRowCount);
    for (int r = 0; r < slotRowCount; ++r)
    {
        std::vector<Slot>& row = m_slots[r];
        row.resize(slotColCount);

        float x = m_slotSize / 2.0f;
        for (int c = 0; c < slotColCount; ++c)
        {
            row[c].m_x = x;
            row[c].m_y = y;
            x += m_slotSize;
        }

        y += m_slotSize;
    }
}

void GeneratedMap::init(int rowCount, int colCount, float slotSize)
{
    m_rowCount = rowCount;
    m_colCount = colCount;
    m_mapHeight = m_rowCount * GameMap::GRID_BREATH;
    m_mapWidth = m_colCount* GameMap::GRID_BREATH;
}

void GeneratedMap::setPlayer(float x, float y, float directionX, float directionY)
{
    m_playerX = x;
    m_playerY = y;
    m_playerDirectionX = directionX;
    m_playerDirectionY = directionY;
}

void GeneratedMap::addTile(const std::string* name, const TileTemplate* t, float x, float y)
{
    m_tiles.emplace_back(name, t, x, y);
}

void GeneratedMap::addRobot(const std::string* name, const AIRobotTemplate* t, float x, float y,
                            float directionX, float directionY)
{
    m_robots.emplace_back(name, t, x, y, directionX, directionY);
}

bool GeneratedMap::write(const char* fileName)
{
    using namespace rapidjson;

    Document doc;
    toJson(doc);

    std::ofstream ofs(fileName);
    if (!ofs.good())
    {
        return false;
    }

    OStreamWrapper osw(ofs);

    Writer<OStreamWrapper> writer(osw);
    doc.Accept(writer);

    ofs.close();

    return true;
}

void GeneratedMap::toJson(rapidjson::Document& doc)
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

void GeneratedMap::getFreeSlots(std::vector<Slot*> freeSlots)
{
    int freeSlotCount = 0;
    int rowCount = static_cast<int>(m_slots.size());
    int colCount = static_cast<int>(m_slots[0].size());

    for (auto& row: m_slots)
    {
        for (auto& slot: row)
        {
            if (!slot.m_occupied)
            {
                ++freeSlotCount;
            }
        }
    }

    int i = 0;

    freeSlots.resize(freeSlotCount);
    for (auto& row: m_slots)
    {
        for (auto& slot: row)
        {
            if (!slot.m_occupied)
            {
                freeSlots[i++] = &slot;
            }
        }
    }
}

} // end of namespace bot
