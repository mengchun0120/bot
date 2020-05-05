#include "misc/bot_log.h"
#include "gametemplate/bot_gametemplatelib.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_missile.h"
#include "gameobj/bot_player.h"
#include "gameutil/bot_gameobjectmanager.h"

namespace bot {

GameObjectManager::GameObjectManager(const GameTemplateLib& gameLib, int missilePoolSize)
	: m_gameLib(gameLib)
	, m_missilePool(missilePoolSize)
{
}

GameObjectManager::~GameObjectManager()
{
	clearActiveObjects();
	clearDeadObjects();
}

Tile* GameObjectManager::createTile(const std::string& tileName)
{
	const TileTemplate* tileTemplate = m_gameLib.getTileTemplate(tileName);
	if (!tileTemplate)
	{
		LOG_ERROR("Failed to find tile template %s", tileName.c_str());
		return nullptr;
	}

	return createTile(tileTemplate);
}

Tile* GameObjectManager::createTile(const TileTemplate* tileTemplate)
{
	Tile* tile = new Tile(tileTemplate);
	m_activeTiles.add(tile);

	return tile;
}

Robot* GameObjectManager::createRobot(const std::string& robotName)
{
	const RobotTemplate* robotTemplate = m_gameLib.getRobotTemplate(robotName);
	if (!robotTemplate)
	{
		LOG_ERROR("Failed to find robot template %s", robotName.c_str());
		return nullptr;
	}

	return createRobot(robotTemplate);
}

Robot* GameObjectManager::createRobot(const RobotTemplate* robotTemplate)
{
	Robot* robot = new Robot(robotTemplate);
	m_activeRobots.add(robot);

	return robot;
}

Missile* GameObjectManager::createMissile(const std::string& missileName)
{
	const MissileTemplate* missileTemplate = m_gameLib.getMissileTemplate(missileName);
	if (!missileTemplate)
	{
		LOG_ERROR("Failed to find missile template %s", missileName.c_str());
		return nullptr;
	}

	return createMissile(missileTemplate);
}

Missile* GameObjectManager::createMissile(const MissileTemplate* missileTemplate)
{
	Missile* missile = m_missilePool.alloc();
	missile->setTemplate(missileTemplate);
	m_activeMissiles.add(missile);

	return missile;
}

Player* GameObjectManager::createPlayer()
{
	Player* player = new Player(m_gameLib.getPlayerTemplate());
	m_activeRobots.add(player);
	return player;
}

void GameObjectManager::free(Missile* missile)
{
	m_activeMissiles.unlink(missile);
	m_missilePool.free(missile);
}

void GameObjectManager::sendToDeathQueue(GameObject* obj)
{
	obj->setFlag(GAME_OBJ_FLAG_DEAD);
	switch (obj->getType())
	{
		case GAME_OBJ_TYPE_TILE: 
		{
			Tile* tile = static_cast<Tile*>(obj);
			m_activeTiles.unlink(tile);
			break;
		}
		case GAME_OBJ_TYPE_ROBOT: 
		{
			Robot* robot = static_cast<Robot*>(obj);
			m_activeRobots.unlink(robot);
			break;
		}
		case GAME_OBJ_TYPE_MISSILE: 
		{
			Missile* missile = static_cast<Missile*>(obj);
			m_activeMissiles.unlink(missile);
			break;
		}
		case GAME_OBJ_TYPE_ANIMATION: 
		{
			break;
		}
		default: 
		{
			LOG_ERROR("Invalid game obj type %d", static_cast<int>(obj->getType()));
			break;
		}
	}
	m_deadObjects.add(obj);
}

void GameObjectManager::clearDeadObjects()
{
	auto deallocator = [this](GameObject* obj)
	{
		if (obj->getType() != GAME_OBJ_TYPE_MISSILE)
		{
			delete obj;
		}
		else
		{
			m_missilePool.free(static_cast<Missile*>(obj));
		}
	};

	m_deadObjects.clear(deallocator);
}

void GameObjectManager::clearActiveObjects()
{
	m_activeTiles.clear();
	m_activeRobots.clear();
	
	auto deallocator = [this](Missile* missile)
	{
		m_missilePool.free(missile);
	};

	m_activeMissiles.clear(deallocator);
}

} // end of namespace bot
