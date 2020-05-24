#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "ai/bot_chase_shoot_ai.h"
#include "parser/bot_ai_parser.h"

namespace bot {

AI* AIParser::parse(const rapidjson::Value& elem)
{
    std::string algorithm;
    if (!parseJson(algorithm, elem, "algorithm"))
    {
        return nullptr;
    }

    if (algorithm == "chase_shoot")
    {
        return parseChaseShootAI(elem);
    }

    LOG_ERROR("Unsupported AI algorithm");

    return nullptr;
}

ChaseShootAI* AIParser::parseChaseShootAI(const rapidjson::Value& elem)
{
    float chaseDurationMs, directionChangeIntervalMs, shootDurationMs, chaseProb, stopChaseDist;

    std::vector<JsonParseParam> params =
    {
        {&chaseDurationMs,           "chaseDuration",           JSONTYPE_FLOAT},
        {&directionChangeIntervalMs, "directionChangeInterval", JSONTYPE_FLOAT},
        {&shootDurationMs,           "shootDuration",           JSONTYPE_FLOAT},
        {&chaseProb,                 "chaseProb",               JSONTYPE_FLOAT},
        {&stopChaseDist,             "stopChaseDist",           JSONTYPE_FLOAT}
    };

    if (!parseJson(params, elem))
    {
        return nullptr;
    }

    ChaseShootAI* ai = new ChaseShootAI(chaseDurationMs, shootDurationMs, directionChangeIntervalMs, 
                                        chaseProb, stopChaseDist);

    return ai;
}

} // end of namespace bot