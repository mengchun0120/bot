#include "misc/bot_log.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_ai_robot.h"
#include "gameobj/bot_player.h"
#include "gameutil/bot_game_map.h"
#include "gameutil/bot_game_object_manager.h"
#include "app/bot_app.h"

namespace bot {

GameObjectManager::GameObjectManager(GameMap& map)
	: m_map(map)
    , m_player(nullptr)
    , m_goodieGenerator()
{
    const AppConfig& cfg = App::getInstance().getConfig();
    m_missilePool.init(cfg.getMissilePoolSize());
}

GameObjectManager::~GameObjectManager()
{
	clearActiveObjects();
	clearDeadObjects();
    delete m_player;
}

Tile* GameObjectManager::createTile(const std::string& tileName)
{
    const GameLib& lib = App::getInstance().getGameLib();
	const TileTemplate* tileTemplate = lib.getTileTemplate(tileName);
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
	LOG_INFO("add tile %p, %d", static_cast<GameObject*>(tile), tile->getFlags());
	m_activeTiles.add(tile);

	return tile;
}

AIRobot* GameObjectManager::createRobot(const std::string& robotName, float x, float y,
						                float directionX, float directionY, Side side)
{
    const GameLib& lib = App::getInstance().getGameLib();
	const AIRobotTemplate* aiRobotTemplate = lib.getAIRobotTemplate(robotName);
	if (!aiRobotTemplate)
	{
		LOG_ERROR("Failed to find ai-robot template %s", robotName.c_str());
		return nullptr;
	}

	return createRobot(aiRobotTemplate, x, y, directionX, directionY, side);
}

AIRobot* GameObjectManager::createRobot(const AIRobotTemplate* aiRobotTemplate, float x, float y,
								        float directionX, float directionY, Side side)
{
	AIRobot* robot = new AIRobot(aiRobotTemplate);
	robot->setPos(x, y);
	robot->setDirection(directionX, directionY);
	robot->setSide(side);

	m_activeRobots.add(robot);

	return robot;
}

Missile* GameObjectManager::createMissile(const std::string& missileName, Robot* shooter, float x, float y,
								          float directionX, float directionY, Side side, float damageMultiplier)
{
    const GameLib& lib = App::getInstance().getGameLib();
	const MissileTemplate* missileTemplate = lib.getMissileTemplate(missileName);
	if (!missileTemplate)
	{
		LOG_ERROR("Failed to find missile template %s", missileName.c_str());
		return nullptr;
	}

	return createMissile(missileTemplate, shooter, x, y, directionX, directionY, side, damageMultiplier);
}

Missile* GameObjectManager::createMissile(const MissileTemplate* missileTemplate, Robot* shooter, float x, float y,
								          float directionX, float directionY, Side side, float damageMultiplier)
{
	Missile* missile = m_missilePool.alloc();
    missile->clearAllFlags();
	missile->setTemplate(missileTemplate);
	missile->setShooter(shooter);
	missile->setPos(x, y);
	missile->setDirection(directionX, directionY);
	missile->setSide(side);
    missile->setDamageMultiplier(damageMultiplier);

	m_activeMissiles.add(missile);

	return missile;
}

ParticleEffect* GameObjectManager::createParticleEffect(const ParticleEffectTemplate* t, float x, float y)
{
	ParticleEffect* effect = m_particleEffectPool.alloc();
    effect->clearAllFlags();
	effect->init(t, x, y);
	m_activeParticleEffect.add(effect);
	return effect;
}

Player* GameObjectManager::createPlayer(float x, float y, float directionX, float directionY)
{
    const GameLib& lib = App::getInstance().getGameLib();
	m_player = new Player(&lib.getPlayerTemplate());
	m_player->setPos(x, y);
	m_player->setDirection(directionX, directionY);
	m_player->setSide(SIDE_PLAYER);
	return m_player;
}

Goodie* GameObjectManager::createGoodie(float prob, float x, float y)
{
    int goodieIdx = m_goodieGenerator.generate(prob);
    if (goodieIdx < 0)
    {
        return nullptr;
    }

    const GameLib& lib = App::getInstance().getGameLib();
    const GoodieTemplate* t = lib.getGoodieTemplate(goodieIdx);
    Goodie* goodie = new Goodie(t, x, y);
    m_activeGoodies.add(goodie);

    return goodie;
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
            m_deadObjects.add(obj);
			break;
		}
		case GAME_OBJ_TYPE_ROBOT:
		{
			Robot* robot = static_cast<Robot*>(obj);
            if (robot->getSide() == SIDE_AI) {
                m_activeRobots.unlink(robot);
                onRobotDeath(robot);
                m_deadObjects.add(obj);
            }
			break;
		}
		case GAME_OBJ_TYPE_MISSILE:
		{
			Missile* missile = static_cast<Missile*>(obj);
			m_activeMissiles.unlink(missile);
            m_deadObjects.add(obj);
			break;
		}
		case GAME_OBJ_TYPE_PARTICLE_EFFECT:
		{
			ParticleEffect* effect = static_cast<ParticleEffect*>(obj);
			m_activeParticleEffect.unlink(effect);
            m_deadObjects.add(obj);
			break;
		}
		case GAME_OBJ_TYPE_ANIMATION:
		{
			break;
		}
        case GAME_OBJ_TYPE_GOODIE:
        {
            Goodie* goodie = static_cast<Goodie*>(obj);
            m_activeGoodies.unlink(goodie);
            m_deadObjects.add(obj);
            break;
        }
		default: 
		{
			LOG_ERROR("Invalid game obj type %d", static_cast<int>(obj->getType()));
			return;
		}
	}
}

void GameObjectManager::clearDeadObjects()
{
	auto deallocator = [this](GameObject* obj)
	{
		if (obj->getType() == GAME_OBJ_TYPE_MISSILE)
		{
			m_missilePool.free(static_cast<Missile*>(obj));
		}
		else if (obj->getType() == GAME_OBJ_TYPE_PARTICLE_EFFECT)
		{
			m_particleEffectPool.free(static_cast<ParticleEffect*>(obj));
		}
		else
		{
			delete obj;
		}
	};

	m_deadObjects.clear(deallocator);
}

void GameObjectManager::clearActiveObjects()
{
	m_activeTiles.clear();
	m_activeRobots.clear();
    m_activeGoodies.clear();
	
	auto missileDeallocator = [this](Missile* missile)
	{
		m_missilePool.free(missile);
	};

	m_activeMissiles.clear(missileDeallocator);

	auto particleEffectDeallocator = [this](ParticleEffect* effect)
	{
		m_particleEffectPool.free(effect);
	};

	m_activeParticleEffect.clear(particleEffectDeallocator);
}

void GameObjectManager::onRobotDeath(Robot* robot)
{
    Goodie* goodie = createGoodie(robot->getGoodieSpawnProb(), robot->getPosX(), robot->getPosY());
    if (goodie)
    {
        m_map.addObject(goodie);
    }
}

} // end of namespace bot
