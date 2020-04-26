#include "misc/bot_mathutils.h"
#include "gameobj/bot_moveability.h"

namespace bot {

void MoveAbility::checkDest(float x, float y)
{
	if (!m_hasDest)
	{
		return;
	}

	if (dist(x, y, m_destX, m_destY) < 0.1f)
	{
		m_moving = false;
		m_hasDest = false;
	}
}

} // end of namespace bot
