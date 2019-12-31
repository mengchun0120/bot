#include "bot_gamemap.h"

namespace bot {

GameMap::GameMap()
{

}

GameMap::~GameMap()
{
    int numRows = static_cast<int>(m_map.size());
    if(numRows == 0) {
        return;
    }

    int numCols = static_cast<int>(m_map[0].size());

    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numCols; ++j) {
            MapItem *item, *next;
            for(item = m_map[i][j]; item; item = next) {
                next = static_cast<MapItem*>(item->getNext());
                m_pool.free(item);
            }
        }
    }
}

void GameMap::init(int numRows, int numCols)
{
    m_pool.init(2 * numRows * numCols);
    
    m_map.resize(numRows);
    for(int i = 0; i < numRows; ++i) {
        m_map[i].resize(numCols);
        for(int j = 0; j < numCols; ++j) {
            m_map[i][j] = nullptr;
        }
    }
}

} // end of namespace bot
