#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_progress_ring.h"
#include "parser/bot_progress_ring_parser.h"

namespace bot {

ProgressRing* ProgressRingParser::parse(const rapidjson::Value& elem)
{
    std::string backColorName, frontColorName;
    float lineWidth, radius;
    int numEdges;

    std::vector<JsonParseParam> params = {
        {&backColorName,  "backColor",  JSONTYPE_STRING},
        {&frontColorName, "frontColor", JSONTYPE_STRING},
        {&radius,         "radius",     JSONTYPE_FLOAT},
        {&numEdges,       "numEdges",   JSONTYPE_INT}
    };

    if (!parseJson(params, elem))
    {
        return nullptr;
    }

    const Color* backColor = m_colorLib.search(backColorName);
    if (!backColor)
    {
        LOG_ERROR("Failed to find backColor %s", backColorName.c_str());
        return nullptr;
    }

    const Color* frontColor = m_colorLib.search(frontColorName);
    if (!frontColor)
    {
        LOG_ERROR("Failed to find frontColor %s", frontColorName.c_str());
        return nullptr;
    }

    ProgressRing* ring = new ProgressRing();
    if (!ring->init(frontColor, backColor, radius, numEdges))
    {
        LOG_ERROR("Failed to initialize progress ring");
        delete ring;
        return nullptr;
    }

    return ring;
}

} // end of namespace bot
