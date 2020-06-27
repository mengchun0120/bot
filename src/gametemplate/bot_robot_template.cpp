#include "gametemplate/bot_robot_template.h"

namespace bot {

RobotTemplate::RobotTemplate()
	: GameObjectTemplate(GAME_OBJ_TYPE_ROBOT)
{
	initAbilityTemplates();
}

RobotTemplate::~RobotTemplate()
{
	for (int i = 0; i < NUM_OF_ABILITIES; ++i)
	{
		if (!m_abilityTemplates[i])
		{
			delete m_abilityTemplates[i];
		}
	}
}

void RobotTemplate::setNumComponents(int numComponents)
{
	m_components.resize(numComponents);
	initComponents();
}

void RobotTemplate::initComponents()
{
	int numComponents = getNumComponents();

	for (int i = 0; i < numComponents; ++i) {
		m_components[i].m_index = i;
		m_components[i].m_texture = nullptr;
		m_components[i].m_pos[0] = 0.0f;
		m_components[i].m_pos[1] = 0.0f;
	}
}

void RobotTemplate::initAbilityTemplates()
{
	for (int i = 0; i < NUM_OF_ABILITIES; ++i) {
		m_abilityTemplates[i] = nullptr;
		m_attachComponents[i] = nullptr;
	}
}

void RobotTemplate::setMoveAbilityTemplate(float speed)
{
	MoveAbilityTemplate* moveAbility = new MoveAbilityTemplate(speed);
	m_abilityTemplates[ABILITY_MOVE] = moveAbility;
}

void RobotTemplate::setShootAbilityTemplate(float shootInterval, float shootPosX, float shootPosY,
	                                        const MissileTemplate* missileTemplate)
{
	ShootAbilityTemplate* shootAbility = new ShootAbilityTemplate(shootInterval, shootPosX, 
		                                                          shootPosY, missileTemplate);

	m_abilityTemplates[ABILITY_SHOOT] = shootAbility;
}

} // end of namespace bot
