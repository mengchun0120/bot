#ifndef INCLUDE_BOT_GAMESCREEN
#define INCLUDE_BOT_GAMESCREEN

#include "screen/bot_screen.h"
#include "gameutil/bot_gamemap.h"
#include "gameutil/bot_gameobjectmanager.h"

namespace bot {

class MouseMoveEvent;
class MouseButtonEvent;
class KeyEvent;
class App;

class GameScreen: public Screen {
public:
    GameScreen(App* app);

    virtual ~GameScreen();

    virtual bool init();

    bool loadMap(const std::string& fileName);

    virtual int update(float delta);

    virtual void present();

    virtual int processInput(const InputEvent &e);

    GameMap& getMap()
    {
        return m_map;
    }

    const GameMap& getMap() const
    {
        return m_map;
    }

    /*
    bool checkOutsideMap(float x, float y);

    bool testCollision(float collideLeft, float collideBottom, float collideRight, float collideTop,
                       int excludeSide, GameObjectType excludeType);

    bool checkMoveThroughObjects(float& newDelta, GameObject* obj, float speedX, float speedY, 
                                 int excludeSide, GameObjectType excludeType, float delta);
*/
private:
    void updateViewport();

    int handleMouseMove(const MouseMoveEvent& e);

    int handleMouseButton(const MouseButtonEvent& e);

    int handleKey(const KeyEvent& e);

    float getWorldX(float x) const
    {
        return x + m_viewportWorldX;
    }

    float getWorldY(float y) const
    {
        return y + m_viewportWorldY;
    }

    /*
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



    void getDisplayRegion(int& startRow, int& endRow, int& startCol, int& endCol);

    void clearFlagsInRect(int startRow, int endRow, int startCol, int endCol, GameObjectFlag flag);



    int handleFireKey(int action);



    int updatePlayer(float delta);

    void updateObject(GameObject* obj, float delta);

    int movePlayerToDest(float delta);

    bool checkMoveWithinBoundary(float& newDelta, GameObject* obj, float speedX, float speedY, float delta);

    bool checkMoveToDest(float& newDelta, GameObject* obj, float delta);

    void getCollisionCheckRegion(int& startRow, int& endRow, int& startCol, int& endCol, const GameObject* obj,
                                 float speedX, float speedY, float delta);
    
    */
private:
    App* m_app;
    GameMap m_map;
    GameObjectManager m_gameObjManager;
    float m_minViewportX, m_minViewportY, m_maxViewportX, m_maxViewportY;
    float m_viewportWorldX, m_viewportWorldY;
    float m_viewportPos[Constants::NUM_FLOATS_PER_POSITION];
    /*

    GameObject* m_player;
    GameObject* m_firstObj;
    

    
    int m_maxRowIdx, m_maxColIdx;*/
};

} // end of namespace bot

#endif

