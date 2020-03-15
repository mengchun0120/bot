#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <utility>
#include <cstdarg>
#include <rapidjson/filereadstream.h>
#include "misc/bot_log.h"
#include "misc/bot_utils.h"

namespace bot {

bool readText(std::string& text, const std::string& fileName)
{
    std::ifstream in;

    in.open(fileName);
    if(in.fail()) {
        LOG_ERROR("Failed to open file %s", fileName.c_str());
        return false;
    }

    std::stringstream ss;
    ss << in.rdbuf();
    text = ss.str();

    if(in.bad()) {
        LOG_ERROR("Error happened while reading file %s", fileName.c_str());
        return false;
    }

    return true;
}

std::string trim(const std::string &s)
{
    int len = static_cast<int>(s.length());
    int start;
    
    for (start = 0; start < len; ++start) {
        if (!isspace(s[start])) {
            break;
        }
    }

    if (start >= len) {
        return "";
    }

    int end;

    for (end = len - 1; end >= 0; --end) {
        if (!isspace(s[end])) {
            break;
        }
    }

    return s.substr(start, end - start + 1);
}

std::string getFileSeparator()
{
#ifdef _WIN32
    return "\\";
#elif __linux__
    return "/";
#endif
}

std::string constructPath(std::initializer_list<std::string> s)
{
    if (s.size() == 0) {
        return "";
    }

    std::ostringstream oss;
    std::string separator = getFileSeparator();
    auto it = s.begin();

    oss << *it;
    for (++it; it != s.end(); ++it) {
        oss << separator << *it;
    }

    return oss.str();
}

void fillColor(float *color, int r, int g, int b, int alpha)
{
    color[0] = static_cast<float>(r) / 255.0f;
    color[1] = static_cast<float>(g) / 255.0f;
    color[2] = static_cast<float>(b) / 255.0f;
    color[3] = static_cast<float>(alpha) / 255.0f;
}

bool isBlank(const std::string &s)
{
    int sz = static_cast<int>(s.size());
    for(int i  = 0; i < sz; ++i) {
        if(!isspace(s[i])) {
            return false;
        }
    }

    return true;
}

void tokenize(std::vector<std::string> &tokens, const std::string &s,
              char delimiter, bool trimSpace)
{
    std::list<std::string> tokenList;
    int sz = static_cast<int>(s.size());
    int lastDelim = -1;

    while(true) {
        int j = lastDelim + 1;
        while(j < sz && s[j] != delimiter) {
            ++j;
        }
       
        std::string token = std::move(s.substr(lastDelim + 1, j - lastDelim - 1));
        if(trimSpace) {
            token = std::move(trim(token));
        }

        tokenList.push_back(std::move(token));

        if(j >= sz) {
            break;
        }

        lastDelim = j;
    }

    if(tokenList.size() == 0) {
        return;
    }

    int count = static_cast<int>(tokenList.size());
    tokens.resize(count);
    auto it = tokenList.begin();

    for(int i = 0; i < count; ++i, ++it) {
        tokens[i] = std::move(*it);
    }
}

bool readJson(rapidjson::Document& doc, const char* fileName)
{
    FILE* fp = fopen(fileName, "rb");
    if (!fp) {
        LOG_ERROR("Cannot open %s", fileName);
        return false;
    }

    char readBuffer[1000];
    rapidjson::FileReadStream stream(fp, readBuffer, sizeof(readBuffer));
    doc.ParseStream(stream);
    fclose(fp);

    if (doc.HasParseError()) {
        LOG_ERROR("Failed to parse %s", fileName);
        return false;
    }

    return true;
}

void rotate(float& x, float& y, float directionX, float directionY)
{
    float x1 = x * directionX - y * directionY;
    float y1 = x * directionY + y * directionX;
    x = x1;
    y = y1;
}

void getDirection(float& directionX, float& directionY, float srcX, float srcY, float dstX, float dstY)
{
    float deltaX = dstX - srcX;
    float deltaY = dstY - srcY;
    float dist = sqrt(deltaX * deltaX + deltaY * deltaY);
    directionX = deltaX / dist;
    directionY = deltaY / dist;
}

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

} // end of namespace bot
