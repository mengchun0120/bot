#ifndef INCLUDE_BOT_GAMEOBJECTMANAGER
#define INCLUDE_BOT_GAMEOBJECTMANAGER

#include <structure/bot_doublelinkedlist.h>

namespace bot {

class GameTemplateLib;
class GameObject;
class Tile;
class Robot;

class GameObjectManager {
public:
	GameObjectManager(const GameTemplateLib& gameLib);

	~GameObjectManager();

	Tile* createTile(const std::string& tileName);

	Robot* createRobot(const std::string& robotName);

	void sendObjectToDeath(GameObject* obj);

	void clearDeadObjects();

	void clearActiveObjects();

protected:
	const GameTemplateLib& m_gameLib;
	DoubleLinkedList<GameObject> m_activeObjects;
	DoubleLinkedList<GameObject> m_deadObjects;
};

} // end of namespace bot

#endif
