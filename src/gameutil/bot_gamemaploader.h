#ifndef INCLUDE_BOT_GAMEMAPLOADER
#define INCLUDE_BOT_GAMEMAPLOADER

#include <string>
#include <rapidjson/document.h>
#include "gameutil/bot_gamemap.h"
#include "gameutil/bot_gameobjectmanager.h"

namespace bot {

class GameMapLoader {
public:
	GameMapLoader(GameMap& map, GameObjectManager& gameObjManager, float mapPoolFactor)
		: m_map(map)
		, m_gameObjManager(gameObjManager)
		, m_mapPoolFactor(mapPoolFactor)
	{}

	~GameMapLoader()
	{}

	bool load(const std::string& file);

private:
	bool initMap(const rapidjson::Value& mapJson);

	bool loadTiles(const rapidjson::Value& mapJson);

	bool addTile(const std::string& name, float x, float y);

	bool loadRobots(const rapidjson::Value& mapJson);

	bool addRobot(const std::string& name, float x, float y, float directionX, float directionY);

	bool loadPlayer(const rapidjson::Value& mapJson);

private:
	GameMap& m_map;
	GameObjectManager& m_gameObjManager;
	float m_mapPoolFactor;
};

} // end of namespace bot

#endif
