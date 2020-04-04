#ifndef INCLUDE_UTILS
#define INCLUDE_UTILS

#include <vector>
#include <string>
#include <chrono>
#include <initializer_list>
#include <rapidjson/document.h>
#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "misc/bot_csvreader.h"

namespace bot {

using Clock = std::chrono::high_resolution_clock;

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
