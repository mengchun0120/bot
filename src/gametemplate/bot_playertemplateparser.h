#ifndef INCLUDE_BOT_PLAYERTEMPLATEPARSER
#define INCLUDE_BOT_PLAYERTEMPLATEPARSER

#include "gametemplate/bot_robottemplateparser.h"

namespace bot {

class PlayerTemplate;

class PlayerTemplateParser: public RobotTemplateParser {
public:
	PlayerTemplateParser(const NamedLib<Texture>& textureLib,
						 const NamedLib<Rectangle>& rectLib,
						 const NamedLib<Color>& colorLib,
						 const NamedLib<MissileTemplate>& missileTemplateLib);

	virtual ~PlayerTemplateParser()
	{}

	bool parse(PlayerTemplate* playerTemplate, const rapidjson::Value& elem);
};

} // end of namespace bot

#endif
