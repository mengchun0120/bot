#ifndef INCLUDE_BOT_GOODIE_TEMPLATE_PARSER
#define INCLUDE_BOT_GOODIE_TEMPLATE_PARSER

#include <rapidjson/document.h>
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gameobj/bot_progress_ring.h"

namespace bot {

class GoodieTemplate;

class GoodieTemplateParser {
public:
    GoodieTemplateParser(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
                         const NamedMap<ProgressRing>& ringLib)
        : m_textureLib(textureLib)
        , m_rectLib(rectLib)
        , m_ringLib(ringLib)
    {}

    ~GoodieTemplateParser()
    {}

    bool parse(GoodieTemplate& t, const rapidjson::Value& elem);

private:
    const NamedMap<Texture>& m_textureLib;
    const NamedMap<Rectangle>& m_rectLib;
    const NamedMap<ProgressRing>& m_ringLib;
};

} // end of namespace bot

#endif
