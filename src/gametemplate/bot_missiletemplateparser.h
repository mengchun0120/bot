#ifndef INCLUDE_BOT_MISSILETEMPLATELIBPARSER
#define INCLUDE_BOT_MISSILETEMPLATELIBPARSER

#include <rapidjson/document.h>
#include "structure/bot_namedlib.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "gametemplate/bot_animationtemplate.h"

namespace bot {

class MissileTemplate;

class MissileTemplateParser {
public:
	MissileTemplateParser(const NamedLib<Texture>& textureLib, 
					      const NamedLib<Rectangle>& rectLib,
						  const NamedLib<Color>& colorLib,
		                  const NamedLib<AnimationTemplate>& animationTemplateLib)
		: m_textureLib(textureLib)
		, m_rectLib(rectLib)
		, m_colorLib(colorLib)
		, m_animationTemplateLib(animationTemplateLib)
	{}

	~MissileTemplateParser()
	{}

	bool parse(MissileTemplate* missileTemplate, const rapidjson::Value& elem);

private:
	const NamedLib<Texture>& m_textureLib;
	const NamedLib<Rectangle>& m_rectLib;
	const NamedLib<Color>& m_colorLib;
	const NamedLib<AnimationTemplate>& m_animationTemplateLib;
};

} // end of namespace bot

#endif
