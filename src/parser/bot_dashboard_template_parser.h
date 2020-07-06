#ifndef INCLUDE_BOT_DASHBOARD_TEMPLATE_PARSER
#define INCLUDE_BOT_DASHBOARD_TEMPLATE_PARSER

#include <string>

namespace bot {

class DashboardTemplate;

class DashboardTemplateParser {
public:
    DashboardTemplateParser()
    {}

    ~DashboardTemplateParser()
    {}

    bool parse(DashboardTemplate& t, const std::string& fileName);
};

} // end of namespace bot

#endif
