#ifndef INCLUDE_BOT_PROGRESS_RING_PARSER
#define INCLUDE_BOT_PROGRESS_RING_PARSER

#include <rapidjson/document.h>
#include "structure/bot_named_map.h"
#include "opengl/bot_color.h"

namespace bot {

class ProgressRing;

class ProgressRingParser {
public:
    ProgressRingParser(const NamedMap<Color>& colorLib)
        : m_colorLib(colorLib)
    {}

    ~ProgressRingParser()
    {}

    ProgressRing* parse(const rapidjson::Value& elem);

private:
    const NamedMap<Color>& m_colorLib;
};

} // end of namespace bot

#endif
