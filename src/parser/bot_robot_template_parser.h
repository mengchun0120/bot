#ifndef INCLUDE_BOT_ROBOT_TEMPLATE_PARSER
#define INCLUDE_BOT_ROBOT_TEMPLATE_PARSER

#include <rapidjson/document.h>
#include "structure/bot_named_map.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "gametemplate/bot_missile_template.h"

namespace bot {

class RobotTemplate;

class RobotTemplateParser {
public:
	RobotTemplateParser(const NamedMap<Texture>& textureLib,
						const NamedMap<Rectangle>& rectLib,
					    const NamedMap<Color>& colorLib,
						const NamedMap<MissileTemplate>& missileTemplateLib)
		: m_textureLib(textureLib)
		, m_rectLib(rectLib)
		, m_colorLib(colorLib)
		, m_missileTemplateLib(missileTemplateLib)
	{}

	virtual ~RobotTemplateParser()
	{}

	bool parse(RobotTemplate* robotTemplate, const rapidjson::Value& elem);

private:
	bool parseBaseAttributes(RobotTemplate* robotTemplate, const rapidjson::Value& elem);

	bool parseComponents(RobotTemplate* robotTemplate, const rapidjson::Value& elem);

	bool parseMoveAbility(RobotTemplate* robotTemplate, const rapidjson::Value& elem);

	bool parseShootAbility(RobotTemplate* robotTemplate, const rapidjson::Value& elem);

protected:
	const NamedMap<Texture>& m_textureLib;
	const NamedMap<Rectangle>& m_rectLib;
	const NamedMap<Color>& m_colorLib;
	const NamedMap<MissileTemplate>& m_missileTemplateLib;
};

} // end of namespace bot

#endif
