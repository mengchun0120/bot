#include "misc/bot_log.h"
#include "gametemplate/bot_gametemplatelib.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_gameobjectmanager.h"

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
