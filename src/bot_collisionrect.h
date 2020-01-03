#ifndef INCLUDE_BOT_COLLISIONRECT
#define INCLUDE_BOT_COLLISIONRECT

namespace bot {

class CollisionRect {
public:
	CollisionRect()
		: m_x(0.0f)
		, m_y(0.0f)
		, m_width(0.0f)
		, m_height(0.0f)
	{}

	CollisionRect(float x, float y, float width, float height)
		: m_x(x)
		, m_y(y)
		, m_width(width)
		, m_height(height)
	{}

	virtual ~CollisionRect()
	{}

	void init(float x, float y, float width, float height)
	{
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;
	}

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

	void move(float deltaX, float deltaY)
	{
		m_x += deltaX;
		m_y += deltaY;
	}

	float getWidth() const
	{
		return m_width;
	}

	float getHeight() const
	{
		return m_height;
	}

	float getLeft() const
	{
		return m_x - m_width / 2.0f;
	}

	float getRight() const
	{
		return m_x + m_width / 2.0f;
	}

	float getTop() const
	{
		return m_y + m_height / 2.0f;
	}

	float getBottom() const
	{
		return m_y - m_height / 2.0f;
	}

protected:
	float m_x, m_y;
	float m_width, m_height;
};

} // end of namespace bot

#endif
