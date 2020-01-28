#ifndef INCLUDE_BOT_GAMESCREEN
#define INCLUDE_BOT_GAMESCREEN

#include <vector>
#include <cmath>
#include <rapidjson/document.h>
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
public:
    GameScreen();

    virtual ~GameScreen();

    virtual bool init();

    bool loadMap(const char* fileName);

    virtual int update(float delta);

    virtual void present();

    virtual int processInput(const InputEvent &e);

private:
    static int getMapCoord(float z)
    {
        return static_cast<int>(ceil(z / GRID_BREATH - 1.0f));
    }

    bool initMap(const rapidjson::Document& doc);

    bool loadObjects(const rapidjson::Document& doc);

    bool loadPlayer(const rapidjson::Document& doc);

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

    void clearMap();

    void updateViewport();

    void getDisplayRegion(int& startRow, int& endRow, int& startCol, int& endCol);

    void clearFlagsInRect(int startRow, int endRow, int startCol, int endCol, GameObjectFlag flag);

public:
    static const float GRID_BREATH;
    static const int MIN_NUM_ROWS;
    static const int MAX_NUM_ROWS;
    static const int MIN_NUM_COLS;
    static const int MAX_NUM_COLS;

private:
    ObjectPool<MapItem> m_pool;
    std::vector<std::vector<MapItem*>> m_map;
    float m_mapWidth, m_mapHeight;
    GameObject* m_player;
    float m_minViewportX, m_minViewportY, m_maxViewportX, m_maxViewportY;
    float m_viewportPos[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif

