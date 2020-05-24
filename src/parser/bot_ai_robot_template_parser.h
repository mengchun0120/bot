#ifndef INCLUDE_BOT_AI_ROBOT_TEMPLATE_PARSER
#define INCLUDE_BOT_AI_ROBOT_TEMPLATE_PARSER

#include <rapidjson/document.h>
#include "structure/bot_named_map.h"
#include "ai/bot_ai.h"
#include "parser/bot_robot_template_parser.h"

namespace bot {

class AIRobotTemplate;

class AIRobotTemplateParser : public RobotTemplateParser {
public:
    AIRobotTemplateParser(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
                          const NamedMap<Color>& colorLib, const NamedMap<MissileTemplate>& missileTemplateLib,
                          const NamedMap<AI>& aiLib)
        : RobotTemplateParser(textureLib, rectLib, colorLib, missileTemplateLib)
        , m_aiLib(aiLib)
    {}

    ~AIRobotTemplateParser()
    {}

    AIRobotTemplate* parse(const rapidjson::Value& elem);

protected:
    const NamedMap<AI>& m_aiLib;
};

}

#endif
