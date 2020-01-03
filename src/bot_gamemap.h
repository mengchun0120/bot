#ifndef INCLUDE_BOT_GAMEMAP
#define INCLUDE_BOT_GAMEMAP

#include <vector>
#include "bot_objectpool.h"

namespace bot {

class GameObject;

class MapItem : public LinkedItem {
public:
    MapItem()
        : LinkedItem()
        , m_obj(nullptr)
    {}

    virtual ~MapItem()
    {}

    GameObject* getObj() const
    {
        return m_obj;
    }

    void setObj(GameObject* obj)
    {
        m_obj = obj;
    }

protected:
    GameObject* m_obj;
};

class GameMap {
public:
    static const float GRID_BREATH;

    static int getMapCoord(float z)
    {
        return static_cast<int>(z / GRID_BREATH);
    }

    GameMap();

    virtual ~GameMap();

    void init(int numRows, int numCols);

    // Add a GameObject to the map. Returns true if the GameObject is added in the map; or false if
    // the GameObject lies outside the map.
    bool addGameObj(GameObject* obj);

    bool repositionGameObj(GameObject* obj);

    void removeGameObj(GameObject* obj);

    int getNumRows() const
    {
        return static_cast<int>(m_map.size());
    }

    int getNumCols() const
    {
        return static_cast<int>(m_map[0].size());
    }

private:
    // Get the position in map for a GameObject. Returns true, if the GameObject lies in the map; or
    // false otherwise.
    bool getMapPosForGameObj(int& startRow, int& endRow, int& startCol, int& endCol, GameObject* obj);

    void addGameObjAt(GameObject* obj, int row, int col);

    MapItem* findGameObjectAt(GameObject* obj, int row, int col);

    void removeGameObjAt(GameObject* obj, int row, int col);

    void removeGameObjFromRect(GameObject* obj, int startRow, int endRow, int startCol, int endCol);

    void addGameObjToRect(GameObject* obj, int startRow, int endRow, int startCol, int endCol);

private:
    ObjectPool<MapItem> m_pool;
    std::vector<std::vector<MapItem*>> m_map;
};

} // end of namespace bot

#endif
