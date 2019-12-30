#include <stdexcept>
#include <fstream>
#include "bot_log.h"
#include "bot_utils.h"
#include "bot_csvreader.h"

namespace bot {

bool CSVReader::readHeader(std::ifstream &is)
{
    std::string header;
    std::getline(is, header);
    if(is.bad()) {
        LOG_ERROR("Failed to read header");
        return false;
    }
    
    if(isBlank(header)) {
        LOG_ERROR("Header is blank");
        return false;
    }

    std::vector<std::string> columnNames;

    tokenize(columnNames, header, ',', true);
    for(unsigned int i = 0; i < columnNames.size(); ++i) {
        m_nameIdxMap[columnNames[i]] = i;
    }

    return true;
}

bool CSVReader::load(const std::string &fileName)
{
    std::ifstream is(fileName);

    if(!is.is_open()) {
        LOG_ERROR("Failed to open %s", fileName.c_str());
        return false;
    }

    if(!readHeader(is)) {
        return false;
    }

    int lineNum = 2;

    for(; is.good(); ++lineNum) {
        std::string line;
        std::getline(is, line);

        if(isBlank(line)) {
            continue;
        }

        m_records.emplace_back();
        std::vector<std::string> &tokens = m_records.back();
        tokenize(tokens, line, ',', true);
        
        if(tokens.size() == 0) {
            continue;
        }

        if(tokens.size() != m_nameIdxMap.size()) {
            LOG_ERROR("line %d is invalid %d %d", lineNum, tokens.size(), 
                      m_nameIdxMap.size());
            return false;
        }
    }

    if(is.bad()) {
        LOG_ERROR("Error happened when reading from %s", fileName.c_str());
        return false;
    }

    return true;
}

void CSVReader::start()
{
    m_it = m_records.begin();
    m_recordIdx = 0;
}

const std::string &CSVReader::read(const std::string& columnName) const
{
    auto it = m_nameIdxMap.find(columnName);
    if(it == m_nameIdxMap.end()) {
        LOG_ERROR("Failed to find column %s", columnName.c_str());
        std::string msg = "Failed to find column " + columnName;
        throw std::runtime_error(msg);
    }

    return (*m_it)[it->second];
}

} // end of namespace bot
