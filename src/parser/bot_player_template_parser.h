#ifndef INCLUDE_BOT_PLAYER_TEMPLATE_PARSER
#define INCLUDE_BOT_PLAYER_TEMPLATE_PARSER

#include "parser/bot_robot_template_parser.h"

namespace bot {

class PlayerTemplate;

class PlayerTemplateParser: public RobotTemplateParser {
public:
	PlayerTemplateParser(const NamedMap<Texture>& textureLib,
						 const NamedMap<Rectangle>& rectLib,
						 const NamedMap<Color>& colorLib,
						 const NamedMap<MissileTemplate>& missileTemplateLib);

	virtual ~PlayerTemplateParser()
	{}

    PlayerTemplate* parse(const rapidjson::Value& elem);
};

} // end of namespace bot

#endif
