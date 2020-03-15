#ifndef INCLUDE_BOT_GAMESCREEN
#define INCLUDE_BOT_GAMESCREEN

#include <vector>
#include <cmath>
#include <rapidjson/document.h>
#include "structure/bot_objectpool.h"
#include "gameobj/bot_gameobject.h"
#include "screen/bot_screen.h"

namespace bot {

class MouseMoveEvent;
class MouseButtonEvent;
class KeyEvent;

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

    // Add a GameObject to the map. Returns true if the GameObject is added in the map; or false if
    // the GameObject lies outside the map.
    bool addGameObj(GameObject* obj);

    bool checkOutsideMap(float x, float y);

    bool testCollision(float collideLeft, float collideBottom, float collideRight, float collideTop,
                       int excludeSide, GameObjectType excludeType);

    static int getMapCoord(float z)
    {
        return z / GRID_BREATH;
    }

    int getRow(float y) const
    {
        return clamp(getMapCoord(y), 0, m_maxRowIdx);
    }

    int getCol(float x) const
    {
        return clamp(getMapCoord(x), 0, m_maxColIdx);
    }

    int getNumRows() const
    {
        return static_cast<int>(m_map.size());
    }

    int getNumCols() const
    {
        return static_cast<int>(m_map[0].size());
    }

    float getMapWidth() const
    {
        return m_mapWidth;
    }

    float getMapHeight() const
    {
        return m_mapHeight;
    }

    bool checkMoveThroughObjects(float& newDelta, GameObject* obj, float speedX, float speedY, 
                                 int excludeSide, GameObjectType excludeType, float delta);

private:
    bool initMap(const rapidjson::Document& doc);

    bool loadObjects(const rapidjson::Document& doc);

    bool loadPlayer(const rapidjson::Document& doc);

    bool repositionGameObj(GameObject* obj);

    void removeGameObj(GameObject* obj);


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

    int handleMouseMove(const MouseMoveEvent& e);

    int handleMouseButton(const MouseButtonEvent& e);

    int handleKey(const KeyEvent& e);

    int handleFireKey(int action);

    float getWorldX(float x) const
    {
        return x + m_viewportWorldX;
    }

    float getWorldY(float y) const
    {
        return y + m_viewportWorldY;
    }

    int updatePlayer(float delta);

    void updateObject(GameObject* obj, float delta);

    int movePlayerToDest(float delta);

    bool checkMoveWithinBoundary(float& newDelta, GameObject* obj, float speedX, float speedY, float delta);

    bool checkMoveToDest(float& newDelta, GameObject* obj, float delta);

    void getCollisionCheckRegion(int& startRow, int& endRow, int& startCol, int& endCol, const GameObject* obj,
                                 float speedX, float speedY, float delta);

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
    GameObject* m_firstObj;
    float m_minViewportX, m_minViewportY, m_maxViewportX, m_maxViewportY;
    float m_viewportPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_viewportWorldX, m_viewportWorldY;
    int m_maxRowIdx, m_maxColIdx;
};

} // end of namespace bot

#endif

