#include "gametemplate/bot_player_template.h"
#include "gametemplate/bot_player_template_parser.h"

namespace bot {

PlayerTemplateParser::PlayerTemplateParser(const NamedLib<Texture>& textureLib,
										   const NamedLib<Rectangle>& rectLib,
	                                       const NamedLib<Color>& colorLib,
	                                       const NamedLib<MissileTemplate>& missileTemplateLib)
	: RobotTemplateParser(textureLib, rectLib, colorLib, missileTemplateLib)
{
}

bool PlayerTemplateParser::parse(PlayerTemplate* playerTemplate, const rapidjson::Value& elem)
{
	RobotTemplate* robotTemplate = static_cast<RobotTemplate*>(playerTemplate);
	return RobotTemplateParser::parse(robotTemplate, elem);
}

} // end of namespace bot
