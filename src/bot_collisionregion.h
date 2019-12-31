#ifndef INCLUDE_BOT_COLLISIONREGION
#define INCLUDE_BOT_COLLISIONREGION

#include "bot_linkeditem.h"

namespace bot {

class CollisionRect: public LinkedItem {
public:
	CollisionRect()
		: LinkedItem()
	{}

	CollisionRect(float x, float y, float width, float height)
		: LinkedItem()
		, m_x(x)
		, m_y(y)
		, m_width(width)
		, m_height(height)
	{}

	virtual ~CollisionRect()
	{}

	float getX() const
	{
		return m_x;
	}

	void setX(float x)
	{
		m_x = x;
	}

	float getY() const
	{
		return m_y;
	}

	void setY(float y)
	{
		m_y = y;
	}

	float getWidth() const
	{
		return m_width;
	}

	float getHeight() const
	{
		return m_height;
	}

protected:
	float m_x, m_y;
	float m_width, m_height;
};

class CollisionRegion {
public:
	CollisionRegion()
		: m_first(nullptr)
	{}

	virtual ~CollisionRegion();

	CollisionRect* getFirst() const
	{
		return m_first;
	}

	void add(CollisionRect* r);

protected:
	CollisionRect* m_first;
};

} // end of namespace bot

#endif
