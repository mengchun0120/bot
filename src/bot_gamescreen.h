#ifndef INCLUDE_BOT_GAMESCREEN
#define INCLUDE_BOT_GAMESCREEN

#include <vector>
#include "bot_objectpool.h"
#include "bot_gameobject.h"
#include "bot_screen.h"

namespace bot {

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

class GameScreen: public Screen {
    static const float GRID_BREATH;

    static int getMapCoord(float z)
    {
        return static_cast<int>(z / GRID_BREATH);
    }

public:
    GameScreen();

    virtual ~GameScreen();

    virtual bool init();

    virtual int update(float delta);

    virtual void present();

    virtual int processInput(const InputEvent &e);

private:
    void initMap(int numRows, int numCols);

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
    GameObject m_player;
};

} // end of namespace bot

#endif

