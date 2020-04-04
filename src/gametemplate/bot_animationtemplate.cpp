#include "gametemplate/bot_animationtemplate.h"

namespace bot {

AnimationTemplate::AnimationTemplate()
	: GameObjectTemplate()
	, m_frameInterval(0.0f)
	, m_rect(nullptr)
{
}

AnimationTemplate::AnimationTemplate(const Rectangle* rect, float coverBreathX, float coverBreathY,
                                     float collideBreathX, float collideBreathY,
	                                 int flags, int numFrames, float frameInterval)
	: GameObjectTemplate(coverBreathX, coverBreathY, collideBreathX, collideBreathY, 0, flags)
	, m_frameInterval(frameInterval)
	, m_rect(rect)
{
	setNumFrames(numFrames);
}

AnimationTemplate::~AnimationTemplate()
{}

void AnimationTemplate::setNumFrames(int numFrames)
{
	m_frames.resize(numFrames);
}

} // end of namespace bot
