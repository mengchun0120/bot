#ifndef INCLUDE_BOT_MISSILE_TEMPLATE
#define INCLUDE_BOT_MISSILE_TEMPLATE

#include "gametemplate/bot_game_object_template.h"

namespace bot {

class Texture;
class Rectangle;
class AnimationTemplate;
class Color;

class MissileTemplate : public GameObjectTemplate {
public:
	MissileTemplate();

	virtual ~MissileTemplate()
	{}

	float getSpeed() const
	{
		return m_speed;
	}

	void setSpeed(float speed) 
	{
		m_speed = speed;
	}

	int getExplosionPower() const
	{
		return m_explosionPower;
	}

	void setExplosionPower(int explosionPower)
	{
		m_explosionPower = explosionPower;
	}

	float getExplosionBreath() const
	{
		return m_explosionBreath;
	}

	void setExplosionBreath(float explosionBreath)
	{
		m_explosionBreath = explosionBreath;
	}

	const AnimationTemplate* getExplosionTemplate() const
	{
		return m_explosionTemplate;
	}

	void setExplosionTemplate(const AnimationTemplate* explosionTemplate)
	{
		m_explosionTemplate = explosionTemplate;
	}

	const Texture* getTexture() const
	{
		return m_texture;
	}

	void setTexture(const Texture* texture)
	{
		m_texture = texture;
	}

	const Rectangle* getRect() const
	{
		return m_rect;
	}

	void setRect(const Rectangle* rect)
	{
		m_rect = rect;
	}

	const Color* getColor() const
	{
		return m_color;
	}

	void setColor(const Color* color)
	{
		m_color = color;
	}

protected:
	float m_speed;
	int m_explosionPower;
	float m_explosionBreath;
	const Texture* m_texture;
	const Rectangle* m_rect;
	const Color* m_color;
	const AnimationTemplate* m_explosionTemplate;
};

} // end of namespace bot

#endif
