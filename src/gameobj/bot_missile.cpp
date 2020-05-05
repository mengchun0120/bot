#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gameutil/bot_gamemap.h"
#include "screen/bot_gamescreen.h"
#include "gameobj/bot_missile.h"

namespace bot {

Missile::Missile()
	: GameObject(nullptr)
{
	m_direction[0] = 0.0f;
	m_direction[1] = 0.0f;
}

Missile::Missile(const MissileTemplate* t)
	: GameObject(t)
{
	m_direction[0] = 0.0f;
	m_direction[1] = 0.0f;
}

Missile::~Missile()
{
}

void Missile::present(SimpleShaderProgram& program)
{
	const MissileTemplate* t = getTemplate();
	t->getRect()->draw(program, m_pos, m_direction, nullptr, nullptr, t->getTexture()->textureId(), t->getColor());
}

bool Missile::update(float delta, GameScreen& screen)
{
	const MissileTemplate* t = getTemplate();
	GameMap& map = screen.getMap();
	GameObjectManager& gameObjManager = screen.getGameObjManager();

	float speedX = t->getSpeed() * m_direction[0];
	float speedY = t->getSpeed() * m_direction[1];
	float deltaX = speedX * delta;
	float deltaY = speedY * delta;

	shiftPos(deltaX, deltaY);

	LinkedList<GameObjectItem> collideObjs;
	bool outOfSight = map.checkCollision(collideObjs, this);

	if (outOfSight)
	{
		setFlag(GAME_OBJ_FLAG_DEAD);
		gameObjManager.sendObjectToDeath(this);
		return false;
	}

	if (!collideObjs.isEmpty())
	{
		// explode
		map.freeGameObjList(collideObjs);
		setFlag(GAME_OBJ_FLAG_DEAD);
		gameObjManager.sendObjectToDeath(this);
		return false;
	}

	map.repositionObject(this);

	return true;
}

void Missile::shiftPos(float deltaX, float deltaY)
{
	m_pos[0] += deltaX;
	m_pos[1] += deltaY;
}

void Missile::setPos(float x, float y)
{
	m_pos[0] = x;
	m_pos[1] = y;
}

void Missile::setDirection(float directionX, float directionY)
{
	m_direction[0] = directionX;
	m_direction[1] = directionY;
}

} // end of namespace bot
