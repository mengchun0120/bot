#ifndef INCLUDE_UTILS
#define INCLUDE_UTILS

#include <vector>
#include <string>
#include <chrono>
#include <initializer_list>
#include <rapidjson/document.h>
#include "bot_log.h"
#include "bot_csvreader.h"

namespace bot {

using Clock = std::chrono::high_resolution_clock;

template <typename T>
T clamp(const T& t, const T& min, const T& max)
{
    if (t < min) {
        return min;
    }
    else if (t > max) {
        return max;
    }

    return t;
}

bool readText(std::string& text, const std::string& fileName);

std::string trim(const std::string &s);

std::string getFileSeparator();

std::string constructPath(std::initializer_list<std::string> s);

void fillColor(float *color, int r, int g, int b, int alpha);

bool isBlank(const std::string &s);

void tokenize(std::vector<std::string> &tokens, const std::string &s, 
              char delimiter, bool trimSpace=true);

bool readJson(rapidjson::Document& doc, const char* fileName);

void rotate(float& x, float& y, float directionX, float directionY);

void getDirection(float& directionX, float& directionY, float srcX, float srcY, float dstX, float dstY);

template <typename PROCESSOR>
bool readAndProcessCSVFile(const std::string &file,
                           PROCESSOR processor)
{
    CSVReader reader;

    if(!reader.load(file)) {
        LOG_ERROR("Failed to load %s", file.c_str());
        return false;
    }

    int count = reader.numRecords();
    if(count == 0) {
        LOG_WARN("No records read from %s", file.c_str());
        return true;
    }

    processor.init(count);
    
    reader.start();
    while(reader.hasNext()) {
        if(!processor(reader)) {
            return false;
        }
        reader.moveNext();
    }

    return true;
}

} // end of namespace bot

#endif
