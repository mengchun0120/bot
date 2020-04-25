#ifndef INCLUDE_BOT_ROBOTTEMPLATELIBPARSER
#define INCLUDE_BOT_ROBOTTEMPLATELIBPARSER

#include <rapidjson/document.h>
#include "structure/bot_namedlib.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "gametemplate/bot_missiletemplate.h"

namespace bot {

class RobotTemplate;

class RobotTemplateParser {
public:
	RobotTemplateParser(const NamedLib<Texture>& textureLib,
						const NamedLib<Rectangle>& rectLib,
					    const NamedLib<Color>& colorLib,
						const NamedLib<MissileTemplate>& missileTemplateLib)
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
	const NamedLib<Texture>& m_textureLib;
	const NamedLib<Rectangle>& m_rectLib;
	const NamedLib<Color>& m_colorLib;
	const NamedLib<MissileTemplate>& m_missileTemplateLib;
};

} // end of namespace bot

#endif
