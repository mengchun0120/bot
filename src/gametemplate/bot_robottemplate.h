#ifndef INCLUDE_BOT_ROBOTTEMPLATE
#define INCLUDE_BOT_ROBOTTEMPLATE

#include <vector>
#include "misc/bot_constants.h"
#include "gametemplate/bot_moveabilitytemplate.h"
#include "gametemplate/bot_shootabilitytemplate.h"
#include "gametemplate/bot_gameobjecttemplate.h"

namespace bot {

class Texture;
class Rectangle;
class Color;

class RobotTemplate: public GameObjectTemplate {
public:
	struct Component {
		const Texture* m_texture;
		const Rectangle* m_rect;
		const Color* m_color;
		float m_pos[Constants::NUM_FLOATS_PER_POSITION];
	};

	static const int MAX_COMPONENTS = 4;

	RobotTemplate();

	RobotTemplate(float coverBreathX, float coverBreathY, float collideBreathX, float collideBreathY,
		          int hp, int flags, int numComponents);

	virtual ~RobotTemplate();

	int getNumComponents() const
	{
		return static_cast<int>(m_components.size());
	}

	void setNumComponents(int numComponents);

	const Component& getComponent(int componentIdx) const
	{
		return m_components[componentIdx];
	}

	const Texture* getComponentTexture(int componentIdx) const
	{
		return m_components[componentIdx].m_texture;
	}

	void setComponentTexture(int componentIdx, const Texture* texture)
	{
		m_components[componentIdx].m_texture = texture;
	}

	const Rectangle* getComponentRect(int componentIdx) const
	{
		return m_components[componentIdx].m_rect;
	}

	void setComponentRect(int componentIdx, const Rectangle* rect)
	{
		m_components[componentIdx].m_rect = rect;
	}

	const Color* getComponentColor(int componentIdx) const
	{
		return m_components[componentIdx].m_color;
	}

	void setComponentColor(int componentIdx, const Color* color)
	{
		m_components[componentIdx].m_color = color;
	}

	const float* getComponentPos(int componentIdx) const
	{
		return m_components[componentIdx].m_pos;
	}

	float getComponentPosX(int componentIdx) const
	{
		return m_components[componentIdx].m_pos[0];
	}

	void setComponentPosX(int componentIdx, float x)
	{
		m_components[componentIdx].m_pos[0] = x;
	}

	float getComponentPosY(int componentIdx) const
	{
		return m_components[componentIdx].m_pos[1];
	}

	void setComponentPosY(int componentIdx, float y)
	{
		m_components[componentIdx].m_pos[1] = y;
	}

	const MoveAbilityTemplate* getMoveAbilityTemplate() const
	{
		return static_cast<MoveAbilityTemplate*>(m_abilityTemplates[ABILITY_MOVE]);
	}

	void setMoveAbilityTemplate(float speed);

	const ShootAbilityTemplate* getShootAbilityTemplate() const
	{
		return static_cast<ShootAbilityTemplate*>(m_abilityTemplates[ABILITY_SHOOT]);
	}

	void setShootAbilityTemplate(float shootInterval, float shootPosX, float shootPosY, 
		                         const MissileTemplate* missileTemplate);

	const Component* getComponentForMoveAbility() const
	{
		return m_attachComponents[ABILITY_MOVE];
	}

	void setComponentForMoveAbility(int componentIdx)
	{
		m_attachComponents[ABILITY_MOVE] = &m_components[componentIdx];
	}

	const Component* getComponentForShootAbility() const
	{
		return m_attachComponents[ABILITY_SHOOT];
	}

	void setComponentForShootAbility(int componentIdx)
	{
		m_attachComponents[ABILITY_SHOOT] = &m_components[componentIdx];
	}

	int getHP() const
	{
		return m_hp;
	}

	void setHP(int hp)
	{
		m_hp = hp;
	}

protected:
	void initComponents();

	void initAbilityTemplates();

protected:
	std::vector<Component> m_components;
	int m_hp;
	AbilityTemplate* m_abilityTemplates[NUM_ABILITY_TYPES];
	Component* m_attachComponents[NUM_ABILITY_TYPES];
};

} // end of namespace bot

#endif
