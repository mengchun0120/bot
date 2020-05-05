#ifndef INCLUDE_BOT_GAMEOBJECTMANAGER
#define INCLUDE_BOT_GAMEOBJECTMANAGER

#include <structure/bot_doublelinkedlist.h>
#include <structure/bot_objectpool.h>

namespace bot {

class GameTemplateLib;
class GameObject;
class Tile;
class Robot;
class Missile;
class Player;
class TileTemplate;
class MissileTemplate;
class RobotTemplate;

class GameObjectManager {
public:
	GameObjectManager(const GameTemplateLib& gameLib, int missilePoolSize);

	~GameObjectManager();

	Tile* createTile(const std::string& tileName);

	Tile* createTile(const TileTemplate* tileTemplate);

	Tile* getFirstActiveTile()
	{
		return m_activeTiles.getFirst();
	}

	const Tile* getFirstActiveTile() const
	{
		return m_activeTiles.getFirst();
	}

	Robot* createRobot(const std::string& robotName);

	Robot* createRobot(const RobotTemplate* robotTemplate);

	Robot* getFirstActiveRobot()
	{
		return m_activeRobots.getFirst();
	}

	const Robot* getFirstActiveRobot() const
	{
		return m_activeRobots.getFirst();
	}

	Missile* createMissile(const std::string& missileName);

	Missile* createMissile(const MissileTemplate* missileTemplate);

	Missile* getFirstActiveMissile()
	{
		return m_activeMissiles.getFirst();
	}

	const Missile* getFirstActiveMissile() const
	{
		return m_activeMissiles.getFirst();
	}

	GameObject* getFirstDeadObject()
	{
		return m_deadObjects.getFirst();
	}

	const GameObject* getFirstDeadObject() const
	{
		return m_deadObjects.getFirst();
	}

	Player* createPlayer();

	void free(Missile* missile);

	void sendToDeathQueue(GameObject* obj);

	void clearDeadObjects();

	void clearActiveObjects();

protected:
	const GameTemplateLib& m_gameLib;
	ObjectPool<Missile> m_missilePool;
	DoubleLinkedList<Robot> m_activeRobots;
	DoubleLinkedList<Tile> m_activeTiles;
	DoubleLinkedList<Missile> m_activeMissiles;
	DoubleLinkedList<GameObject> m_deadObjects;
};

} // end of namespace bot

#endif
