#include "misc/bot_log.h"
#include "gametemplate/bot_gametemplatelib.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_player.h"
#include "gameutil/bot_gameobjectmanager.h"

namespace bot {

GameObjectManager::GameObjectManager(const GameTemplateLib& gameLib)
	: m_gameLib(gameLib)
{
}

GameObjectManager::~GameObjectManager()
{
	clearActiveObjects();
	clearDeadObjects();
}

Tile* GameObjectManager::createTile(const std::string& tileName)
{
	const TileTemplate* tileTemplate = m_gameLib.getTileTemplateByName(tileName);
	if (!tileTemplate)
	{
		LOG_ERROR("Failed to find tile template %s", tileName.c_str());
		return nullptr;
	}

	Tile* tile = new Tile(tileTemplate);
	m_activeObjects.add(tile);

	return tile;
}

Robot* GameObjectManager::createRobot(const std::string& robotName)
{
	const RobotTemplate* robotTemplate = m_gameLib.getRobotTemplateByName(robotName);
	if (!robotTemplate)
	{
		LOG_ERROR("Failed to find robot template %s", robotName.c_str());
		return nullptr;
	}

	Robot* robot = new Robot(robotTemplate);
	m_activeObjects.add(robot);

	return robot;
}

Player* GameObjectManager::createPlayer()
{
	Player* player = new Player(m_gameLib.getPlayerTemplate());
	m_activeObjects.add(player);
	return player;
}

void GameObjectManager::sendObjectToDeath(GameObject* obj)
{
	m_activeObjects.unlink(obj);
	m_deadObjects.add(obj);
}

void GameObjectManager::clearDeadObjects()
{
	m_deadObjects.clear();
}

void GameObjectManager::clearActiveObjects()
{
	m_activeObjects.clear();
}

} // end of namespace bot
