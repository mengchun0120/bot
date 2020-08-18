#include <cmath>
#include <utility>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <fstream>
#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gametemplate/bot_tile_template.h"
#include "gameutil/bot_island_map_generator.h"
#include "gameutil/bot_game_map.h"
#include "app/bot_app.h"

namespace bot {

IslandMapGenerator::IslandMapGenerator()
    : MapGenerator()
    , m_minIslandLen(0)
    , m_maxIslandLen(0)
    , m_minIslandDist(0.0f)
{
}

bool IslandMapGenerator::init(const rapidjson::Value& json)
{
    if (!MapGenerator::init(json))
    {
        return false;
    }

    std::vector<JsonParseParam> params = {
        {&m_minIslandLen,      "minIslandLen",      JSONTYPE_INT},
        {&m_maxIslandLen,      "maxIslandLen",      JSONTYPE_INT},
        {&m_minIslandDist,     "minIslandDist",     JSONTYPE_INT},
        {&m_maxIslandDist,     "maxIslandDist",     JSONTYPE_INT},
        {&m_islandTiles,       "islandTiles",       JSONTYPE_STRING_ARRAY},
    };

    if (!parseJson(params, json))
    {
        return false;
    }

    const GameLib& lib = App::getInstance().getGameLib();
    int islandTileCount = static_cast<int>(m_islandTiles.size());

    m_islandTileTemplates.resize(islandTileCount);
    for (int i = 0; i < islandTileCount; ++i)
    {
        const TileTemplate* t = lib.getTileTemplate(m_islandTiles[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find tile template %s", m_islandTiles[i].c_str());
            return false;
        }
        m_islandTileTemplates[i] = t;
    }

    return true;
}

bool IslandMapGenerator::generate(const char* fileName)
{
    Map map;

    initMap(map);
    generateTiles(map);
    map.deployRobots(m_rand, m_maxRobotCount, m_robotNames, m_robotTemplates);

    return writeMap(fileName, map);
}

void IslandMapGenerator::generateTiles(Map& map)
{
    int islandTileIdx = m_rand.get(0, m_islandTiles.size());
    const std::string* tileName = &m_islandTiles[islandTileIdx];
    const TileTemplate* t = m_islandTileTemplates[islandTileIdx];
    float tileHeight = t->getCoverBreathY() * 2.0f;
    float tileWidth = t->getCoverBreathX() * 2.0f;
    int minIslandLenY = static_cast<int>(ceil(m_minIslandLen * tileHeight / m_robotSlotSize));
    int maxIslandLenY = static_cast<int>(ceil(m_maxIslandLen * tileHeight / m_robotSlotSize));
    int minIslandLenX = static_cast<int>(ceil(m_minIslandLen * tileWidth / m_robotSlotSize));
    int maxIslandLenX = static_cast<int>(ceil(m_maxIslandLen * tileWidth / m_robotSlotSize));
    int totalSlotsY = static_cast<int>(map.m_robotSlots.size());
    int totalSlotsX = static_cast<int>(map.m_robotSlots[0].size());
    int islandSlotY = m_rand.get(m_minIslandDist, m_maxIslandDist + 1);

    while (true)
    {
        int maxRowSlots = totalSlotsY - islandSlotY - m_minIslandDist;
        if (maxRowSlots < minIslandLenY)
        {
            break;
        }

        if (maxRowSlots > maxIslandLenY)
        {
            maxRowSlots = maxIslandLenY;
        }

        int maxRowTiles = static_cast<int>(floor(maxRowSlots * m_robotSlotSize / tileHeight));
        if (maxRowTiles > m_maxIslandLen)
        {
            maxRowTiles = m_maxIslandLen;
        }

        int islandSlotX = m_rand.get(m_minIslandDist, m_maxIslandDist + 1);
        while (true)
        {
            int maxColSlots = totalSlotsX - islandSlotX - m_minIslandDist;
            if (maxColSlots < minIslandLenX)
            {
                break;
            }

            if (maxColSlots > maxIslandLenX)
            {
                maxColSlots = maxIslandLenX;
            }

            int maxColTiles = static_cast<int>(floor(maxColSlots * m_robotSlotSize / tileWidth));
            if (maxColTiles > m_maxIslandLen)
            {
                maxColTiles = m_maxIslandLen;
            }

            int rows = m_rand.get(m_minIslandLen, maxRowTiles);
            int cols = m_rand.get(m_minIslandLen, maxColTiles);

            generateIsland(map, tileName, t, islandSlotX, islandSlotY, rows, cols);

            int colSlots = static_cast<int>(ceil(cols * tileWidth / m_robotSlotSize));
            islandSlotX += colSlots + m_rand.get(m_minIslandDist, m_maxIslandDist + 1);
        }

        islandSlotY += maxRowSlots + m_rand.get(m_minIslandDist, m_maxIslandDist + 1);
    }
}

void IslandMapGenerator::generateIsland(Map& map, const std::string* tileName, const TileTemplate* t,
                                        int islandSlotX, int islandSlotY, int rows, int cols)
{
    float tileHeight = t->getCoverBreathY() * 2.0f;
    float tileWidth = t->getCoverBreathX() * 2.0f;
    float startX = islandSlotX * m_robotSlotSize;
    float y = islandSlotY * m_robotSlotSize;

    for (int r = 0; r < rows; ++r)
    {
        float x = startX;
        for (int c = 0; c < cols; ++c)
        {
            map.m_tiles.emplace_back();
            TileItem& tile = map.m_tiles.back();
            tile.m_name = tileName;
            tile.m_template = t;
            tile.m_x = x;
            tile.m_y = y;
            x += tileWidth;
        }
        
        y += tileHeight;
    }

    int rowSlots = static_cast<int>(ceil(rows * tileHeight / m_robotSlotSize));
    int colSlots = static_cast<int>(ceil(cols * tileWidth / m_robotSlotSize));

    int row = islandSlotY;
    for (int i = 0; i < rowSlots; ++i, ++row)
    {
        int col = islandSlotX;
        for (int j = 0; j < colSlots; ++j, ++col)
        {
            map.m_robotSlots[i][j].m_occupied = true;
        }
    }
}

bool IslandMapGenerator::writeMap(const char* fileName, const Map& map)
{
    using namespace rapidjson;

    Document doc;
    map.createJson(doc);

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

} // end of namespace bot
