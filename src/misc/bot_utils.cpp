#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <utility>
#include <cstdarg>
#include "misc/bot_log.h"
#include "misc/bot_utils.h"

namespace bot {




void fillColor(float *color, int r, int g, int b, int alpha)
{
    color[0] = static_cast<float>(r) / 255.0f;
    color[1] = static_cast<float>(g) / 255.0f;
    color[2] = static_cast<float>(b) / 255.0f;
    color[3] = static_cast<float>(alpha) / 255.0f;
}

/*


bool readSheet(std::vector<std::string> &fields,
               std::list<std::vector<std::string>> &lines, 
               const std::string &file,
               char delimiter,
               bool trimSpace)
{
    std::ifstream is(file);

    LOG_INFO("read sheet %s", file.c_str());
    if(!is.is_open()) {
        LOG_ERROR("Failed to open %s", file.c_str());
        return false;
    }

    std::string header;
    std::getline(is, header);
    tokenize(fields, header, delimiter, trimSpace);

    int lineNum = 2;

    for(; is.good(); ++lineNum) {
        std::string line;
        std::getline(is, line);

        if(isBlank(line)) {
            continue;
        }

        lines.emplace_back();
        std::vector<std::string> &tokens = lines.back();
        tokenize(tokens, line, delimiter, trimSpace);
        
        if(tokens.size() == 0) {
            continue;
        }

        if(tokens.size() != fields.size()) {
            LOG_ERROR("line %d is invalid %d %d", lineNum, tokens.size(), fields.size());
            return false;
        }
    }

    if(is.bad()) {
        LOG_ERROR("Error happened when reading from %s", file.c_str());
        return false;
    }

    return true;
}
*/
} // end of namespace bot
