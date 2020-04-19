#include "gametemplate/bot_animationtemplate.h"

namespace bot {

AnimationTemplate::AnimationTemplate()
	: GameObjectTemplate()
	, m_frameInterval(0.0f)
	, m_rect(nullptr)
{
}

void AnimationTemplate::setNumFrames(int numFrames)
{
	m_frames.resize(numFrames);
}

} // end of namespace bot
