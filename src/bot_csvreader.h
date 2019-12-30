#ifndef INCLUDE_BOT_CSVREADER
#define INCLUDE_BOT_CSVREADER

#include <string>
#include <list>
#include <vector>
#include <unordered_map>

namespace bot {

class CSVReader {
public:
    typedef std::list<std::vector<std::string>> CSVRecords;

    CSVReader()
    {}

    virtual ~CSVReader()
    {}

    bool load(const std::string &fileName);

    int numRecords() const
    {
        return static_cast<int>(m_records.size());
    }

    int numColumns() const
    {
        return static_cast<int>(m_nameIdxMap.size());
    }

    void start();

    bool hasNext() const
    {
        return m_it != m_records.end();
    }

    void moveNext()
    {
        ++m_it;
        ++m_recordIdx;
    }

    const std::string &read(const std::string& columnName) const;

    const std::string &read(int idx) const
    {
        return (*m_it)[idx];
    }

    int curRecordIdx() const
    {
        return m_recordIdx;
    }

private:
    bool readHeader(std::ifstream &is);

private:
    CSVRecords::iterator m_it;
    int m_recordIdx;
    CSVRecords m_records;
    std::unordered_map<std::string,int> m_nameIdxMap;
};

} // end of namespace bot

#endif