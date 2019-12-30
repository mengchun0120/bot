#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "bot_log.h"
#include "bot_utils.h"
#include "bot_config.h"

namespace bot {

Config Config::g_cfg;

enum ConfigValueType {
    TYPE_INT,
    TYPE_STRING,
    TYPE_DOUBLE,
    TYPE_FLOAT,
    TYPE_FLOAT_ARRAY,
    TYPE_STRING_ARRAY,
    TYPE_INT_ARRAY
};

struct ConfigItem {
    std::string m_name;
    ConfigValueType m_type;
    void* m_mem;
    bool m_required;
    bool m_set;
};

bool parseLine(const std::string &line, std::string &name, std::string &value)
{
    int eqPos = line.find('=');
    if(eqPos == std::string::npos) {
        LOG_ERROR("Invalid line. Must be of the form name = value");
        return false;
    }

    std::string tname = line.substr(0, eqPos);
    std::string tvalue = line.substr(eqPos + 1);

    name = trim(tname);
    value = trim(tvalue);

    return true;
}

int findConfigItem(const std::vector<ConfigItem> items, const std::string &name)
{
    unsigned int itemCount = items.size();
    for(unsigned i = 0; i < itemCount; ++i) {
        if(items[i].m_name == name) {
            return i;
        }
    }

    return -1;
}

void parseFloatArray(float *arr, const char *value)
{
    std::istringstream iss(value);

    for(float *ptr = arr; !iss.eof(); ++ptr) {
        iss >> *ptr;
    }
}

template <typename T>
void parseArray(std::vector<T> *strArr, const std::string &value)
{
    std::istringstream iss(value);

    while(!iss.eof()) {
        strArr->emplace_back();
        iss >> strArr->back();
    }
}

void setValue(ConfigItem& item, const std::string &value)
{
    switch(item.m_type) {
    case TYPE_INT:
        *((int *)(item.m_mem)) = stoi(value);
        item.m_set = true;
        break;
    case TYPE_DOUBLE:
        *((double *)(item.m_mem)) = stod(value);
        item.m_set = true;
        break;
    case TYPE_STRING:
        *((std::string *)item.m_mem) = value;
        item.m_set = true;
        break;
    case TYPE_FLOAT:
        *((float *)(item.m_mem)) = stof(value);
        item.m_set = true;
        break;
    case TYPE_FLOAT_ARRAY:
        parseArray<float>((std::vector<float>*)(item.m_mem), value);
        item.m_set = true;
        break;
    case TYPE_STRING_ARRAY:
        parseArray<std::string>((std::vector<std::string>*)(item.m_mem), value);
        item.m_set = true;
        break;
    case TYPE_INT_ARRAY:
        parseArray<int>((std::vector<int>*)(item.m_mem), value);
        item.m_set = true;
        break;
    }
}

bool validateConfigItems(const std::vector<ConfigItem> items)
{
    unsigned itemCount = items.size();
    for(unsigned int i = 0; i < itemCount; ++i) {
        if(items[i].m_required && !items[i].m_set) {
            LOG_ERROR("Parameter %s is required, but not set", items[i].m_name);
            return false;
        }
    }

    return true;
}

Config::Config()
: m_width(0)
, m_height(0)
{
}

Config::~Config()
{
}

bool Config::load(const char* fileName)
{
    std::vector<ConfigItem> items = {
        { "width", TYPE_INT, &m_width, true, false },
        { "height", TYPE_INT, &m_height, true, false },
        { "title", TYPE_STRING, &m_title, true, false },
        { "timeDeltaHistoryLen", TYPE_INT, &m_timeDeltaHistoryLen, true, false },
        { "eventQueueSize", TYPE_INT, &m_eventQueueSize, true, false }
    };

    std::ifstream is(fileName);

    if(!is.is_open()) {
        LOG_ERROR("Failed to open file %s", fileName);
        return false;
    }

    while(is.good()) {
        std::string line;
        std::getline(is, line);

        std::string name, value;
        if(!parseLine(line, name, value) ) {
            return false;
        }

        LOG_INFO("Reading %s = %s", name.c_str(), value.c_str());

        int i = findConfigItem(items, name);
        if(i == -1) {
            LOG_WARN("Unknown parameter name %s", name.c_str());
            continue;
        }

        setValue(items[i], value);
    }

    if(is.bad()) {
        LOG_ERROR("Failed to read file %s", fileName);
        return false;
    }

    return true;
}

} // end of namespace bot

