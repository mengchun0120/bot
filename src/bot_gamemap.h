#ifndef INCLUDE_BOT_GAMEMAP
#define INCLUDE_BOT_GAMEMAP

#include <vector>
#include "bot_objectpool.h"

namespace bot {

class GameObject;

class GameMap {
public:
    class MapItem: public LinkedItem {
    public:
        MapItem()
        : LinkedItem()
        , m_obj(nullptr)
        {}

        virtual ~MapItem()
        {}

        GameObject *getObj() const
        {
            return m_obj;
        }

        void setObj(GameObject *obj)
        {
            m_obj = obj;
        }

    protected:
        GameObject *m_obj;
    };

    GameMap();

    virtual ~GameMap();

    void init(int numRows, int numCols);

private:
    ObjectPool<MapItem> m_pool;
    std::vector<std::vector<MapItem*>> m_map;
};

} // end of namespace bot

#endif
