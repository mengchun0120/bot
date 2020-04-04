#ifndef INCLUDE_BOT_ANIMATIONTEMPLATELIBPARSER
#define INCLUDE_BOT_ANIMATIONTEMPLATELIBPARSER

#include <string>
#include <rapidjson/document.h>
#include "geometry/bot_rectangle.h"
#include "structure/bot_namedlib.h"

namespace bot {

class AnimationTemplate;

class AnimationTemplateParser {
public:
	AnimationTemplateParser(const std::string& animationFolder, const NamedLib<Rectangle>& rectLib)
		: m_animationFolder(animationFolder)
		, m_rectLib(rectLib)
	{}

	~AnimationTemplateParser()
	{}

	bool parse(AnimationTemplate* animationTemplate, const rapidjson::Value& elem);

private:
	std::string m_animationFolder;
	const NamedLib<Rectangle>& m_rectLib;
};

} // end of namespace bot

#endif
