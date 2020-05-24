#ifndef INCLUDE_BOT_AI_PARSER
#define INCLUDE_BOT_AI_PARSER

#include <rapidjson/document.h>

namespace bot {

class AI;
class ChaseShootAI;

class AIParser {
public:
    AIParser()
    {}

    virtual ~AIParser()
    {}

    AI* parse(const rapidjson::Value& elem);

protected:
    ChaseShootAI* parseChaseShootAI(const rapidjson::Value& elem);
};

} // end of namespace bot

#endif
