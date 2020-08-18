#ifndef INCLUDE_BOT_ISLAND_MAP_GENERATOR
#define INCLUDE_BOT_ISLAND_MAP_GENERATOR

#include "gameutil/bot_map_generator.h"

namespace bot {

class TileTemplate;

class IslandMapGenerator: public MapGenerator {
public:
    IslandMapGenerator();

    virtual ~IslandMapGenerator()
    {}

    virtual bool init(const rapidjson::Value& json);

    virtual bool generate(const char* fileName);

private:
    void generateTiles(Map& map);

    void generateIsland(Map& map, const std::string* tileName, const TileTemplate* t,
                        int islandSlotX, int islandSlotY, int rows, int cols);

    bool writeMap(const char* fileName, const Map& map);

private:
    int m_minIslandLen, m_maxIslandLen;
    int m_minIslandDist, m_maxIslandDist;
    std::vector<std::string> m_islandTiles;
    std::vector<const TileTemplate*> m_islandTileTemplates;
};

} // end of namespace bot

#endif
