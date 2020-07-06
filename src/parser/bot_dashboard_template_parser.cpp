#include <rapidjson/document.h>
#include "misc/bot_json_utils.h"
#include "gametemplate/bot_dashboard_template.h"
#include "parser/bot_dashboard_template_parser.h"

namespace bot {

bool DashboardTemplateParser::parse(DashboardTemplate& t, const std::string& fileName)
{
    rapidjson::Document doc;

    if (!readJson(doc, fileName.c_str()))
    {
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Invalid file format %s", fileName.c_str());
        return false;
    }

    const rapidjson::Value& cfg = doc.GetObject();
    float spacingX, marginBottom, ringRadius;

    std::vector<JsonParseParam> params = {
        {&spacingX,     "spacingX",     JSONTYPE_FLOAT},
        {&marginBottom, "marginBottom", JSONTYPE_FLOAT},
        {&ringRadius,   "ringRadius",   JSONTYPE_FLOAT}
    };

    if (!parseJson(params, cfg))
    {
        return false;
    }

    t.setSpacingX(spacingX);
    t.setMarginBottom(marginBottom);
    t.setRingRadius(ringRadius);

    return true;
}

} // end of namespace bot
