#ifndef INCLUDE_BOT_PLAYER_TEMPLATE_PARSER
#define INCLUDE_BOT_PLAYER_TEMPLATE_PARSER

#include "gametemplate/bot_robot_template_parser.h"

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
