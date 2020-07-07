#ifndef INCLUDE_BOT_DASHBOARD_TEMPLATE_PARSER
#define INCLUDE_BOT_DASHBOARD_TEMPLATE_PARSER

#include <string>
#include "structure/bot_named_map.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"

namespace bot {

class DashboardTemplate;

class DashboardTemplateParser {
public:
    DashboardTemplateParser(const NamedMap<Rectangle>& rectLib, const NamedMap<Texture>& textureLib, 
                            const NamedMap<Color>& colorLib)
        : m_rectLib(rectLib)
        , m_textureLib(textureLib)
        , m_colorLib(colorLib)
    {}

    ~DashboardTemplateParser()
    {}

    bool parse(DashboardTemplate& t, const std::string& fileName);

private:
    const NamedMap<Rectangle>& m_rectLib;
    const NamedMap<Texture>& m_textureLib;
    const NamedMap<Color>& m_colorLib;
};

} // end of namespace bot

#endif
