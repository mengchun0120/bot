#include "gametemplate/bot_player_template.h"
#include "parser/bot_player_template_parser.h"

namespace bot {

PlayerTemplateParser::PlayerTemplateParser(const NamedMap<Texture>& textureLib,
										   const NamedMap<Rectangle>& rectLib,
	                                       const NamedMap<Color>& colorLib,
	                                       const NamedMap<MissileTemplate>& missileTemplateLib)
	: RobotTemplateParser(textureLib, rectLib, colorLib, missileTemplateLib)
{
}

PlayerTemplate* PlayerTemplateParser::parse(const rapidjson::Value& elem)
{
    PlayerTemplate* playerTemplate = new PlayerTemplate();

    if (!RobotTemplateParser::parse(playerTemplate, elem))
    {
        delete playerTemplate;
        return nullptr;
    }

    return playerTemplate;
}

} // end of namespace bot
