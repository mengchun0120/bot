#ifndef INCLUDE_BOT_GAME_OBJECT_MANAGER
#define INCLUDE_BOT_GAME_OBJECT_MANAGER

#include "structure/bot_double_linked_list.h"
#include "structure/bot_object_pool.h"
#include "gameobj/bot_missile.h"
#include "gameobj/bot_particle_effect.h"
#include "gameobj/bot_side.h"

namespace bot {

class GameTemplateLib;
class GameObject;
class Tile;
class Robot;
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

	Robot* createRobot(const std::string& robotName, float x, float y, 
		               float directionX, float directionY, Side side);

	Robot* createRobot(const RobotTemplate* robotTemplate, float x, float y, 
		               float directionX, float directionY, Side side);

	Robot* getFirstActiveRobot()
	{
		return m_activeRobots.getFirst();
	}

	const Robot* getFirstActiveRobot() const
	{
		return m_activeRobots.getFirst();
	}

	Missile* createMissile(const std::string& missileName, Robot* shooter, float x, float y,
		                   float directionX, float directionY, Side side);

	Missile* createMissile(const MissileTemplate* missileTemplate, Robot* shooter, float x, float y,
		                   float directionX, float directionY, Side side);

	Missile* getFirstActiveMissile()
	{
		return m_activeMissiles.getFirst();
	}

	const Missile* getFirstActiveMissile() const
	{
		return m_activeMissiles.getFirst();
	}

	ParticleEffect* createParticleEffect(const ParticleEffectTemplate* t, float x, float y);

	ParticleEffect* getFirstParticleEffect()
	{
		return m_activeParticleEffect.getFirst();
	}

	const ParticleEffect* getFirstParticleEffect() const
	{
		return m_activeParticleEffect.getFirst();
	}

	GameObject* getFirstDeadObject()
	{
		return m_deadObjects.getFirst();
	}

	const GameObject* getFirstDeadObject() const
	{
		return m_deadObjects.getFirst();
	}

	Player* createPlayer(float x, float y, float directionX, float directionY);

	void sendToDeathQueue(GameObject* obj);

	void clearDeadObjects();

	void clearActiveObjects();

protected:
	const GameTemplateLib& m_gameLib;
	ObjectPool<Missile> m_missilePool;
	ObjectPool<ParticleEffect> m_particleEffectPool;
	DoubleLinkedList<Robot> m_activeRobots;
	DoubleLinkedList<Tile> m_activeTiles;
	DoubleLinkedList<Missile> m_activeMissiles;
	DoubleLinkedList<ParticleEffect> m_activeParticleEffect;
	DoubleLinkedList<GameObject> m_deadObjects;
};

} // end of namespace bot

#endif
