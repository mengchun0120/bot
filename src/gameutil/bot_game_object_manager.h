#ifndef INCLUDE_BOT_GAME_OBJECT_MANAGER
#define INCLUDE_BOT_GAME_OBJECT_MANAGER

#include <string>
#include "structure/bot_double_linked_list.h"
#include "structure/bot_object_pool.h"
#include "gameobj/bot_missile.h"
#include "gameobj/bot_particle_effect.h"
#include "gameobj/bot_goodie.h"
#include "gameutil/bot_goodie_generator.h"
#include "gameobj/bot_side.h"

namespace bot {

class GameLib;
class GameObject;
class Tile;
class Player;
class TileTemplate;
class MissileTemplate;
class AIRobotTemplate;
class AIRobot;
class GameMap;

class GameObjectManager {
public:
	GameObjectManager(const GameLib& gameLib, GameMap& map, int missilePoolSize);

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

	AIRobot* createRobot(const std::string& robotName, float x, float y, 
		                 float directionX, float directionY, Side side);

	AIRobot* createRobot(const AIRobotTemplate* aiRobotTemplate, float x, float y, 
		                 float directionX, float directionY, Side side);

	Missile* createMissile(const std::string& missileName, Robot* shooter, float x, float y,
		                   float directionX, float directionY, Side side, float damageMultiplier);

	Missile* createMissile(const MissileTemplate* missileTemplate, Robot* shooter, float x, float y,
		                   float directionX, float directionY, Side side, float damageMultiplier);

    Goodie* createGoodie(float prob, float x, float y);
    
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

    Goodie* getFirstActiveGoodie()
    {
        return m_activeGoodies.getFirst();
    }

    const Goodie* getFirstActiveGoodie() const
    {
        return m_activeGoodies.getFirst();
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

    const Player* getPlayer() const
    {
        return m_player;
    }

    Player* getPlayer()
    {
        return m_player;
    }

	void sendToDeathQueue(GameObject* obj);

	void clearDeadObjects();

	void clearActiveObjects();

private:
    void onRobotDeath(Robot* robot);

protected:
	const GameLib& m_gameLib;
    GameMap& m_map;
    GoodieGenerator m_goodieGenerator;
	ObjectPool<Missile> m_missilePool;
	ObjectPool<ParticleEffect> m_particleEffectPool;
	DoubleLinkedList<Robot> m_activeRobots;
	DoubleLinkedList<Tile> m_activeTiles;
	DoubleLinkedList<Missile> m_activeMissiles;
	DoubleLinkedList<ParticleEffect> m_activeParticleEffect;
    DoubleLinkedList<Goodie> m_activeGoodies;
	DoubleLinkedList<GameObject> m_deadObjects;
    Player* m_player;
};

} // end of namespace bot

#endif
