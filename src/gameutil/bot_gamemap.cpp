#include <algorithm>
#include "misc/bot_log.h"
#include "misc/bot_mathutils.h"
#include "gameobj/bot_gameobject.h"
#include "gameobj/bot_player.h"
#include "gameutil/bot_collide.h"
#include "gameutil/bot_gamemap.h"

namespace bot {

const float GameMap::GRID_BREATH = 40.0f;

GameMap::GameMap()
    : m_mapWidth(0.0f)
    , m_mapHeight(0.0f)
    , m_player(nullptr)
{
}

GameMap::~GameMap()
{
	clear();
}

void GameMap::initMap(int numRows, int numCols, int poolSize)
{
	m_mapItemPool.init(poolSize);

	m_map.resize(numRows);
	for (int r = 0; r < numRows; ++r)
	{
		m_map[r].resize(numCols);
	}

    m_mapWidth = numCols * GRID_BREATH;
    m_mapHeight = numRows * GRID_BREATH;
}

void GameMap::clear()
{
	int numRows = getNumRows();
	int numCols = getNumCols();

	for (int r = 0; r < numRows; ++r)
	{
		std::vector<MapCell>& row = m_map[r];
		for (int c = 0; c < numCols; ++c)
		{
            freeMapCell(row[c]);
		}
	}
}

bool GameMap::getMapPosForGameObj(int& startRow, int& endRow, int& startCol, int& endCol, GameObject* obj) const
{
    int startRow1 = getMapCoord(obj->getCoverBottom());
    if (startRow1 >= getNumRows()) 
    {
        return false;
    }

    if (startRow1 < 0) 
    {
        startRow1 = 0;
    }

    int endRow1 = getMapCoord(obj->getCoverTop());
    if (endRow1 < 0) 
    {
        return false;
    }

    if (endRow1 >= getNumRows()) 
    {
        endRow1 = getNumRows() - 1;
    }

    int startCol1 = getMapCoord(obj->getCoverLeft());
    if (startCol1 >= getNumCols()) 
    {
        return false;
    }

    if (startCol1 < 0) 
    {
        startCol1 = 0;
    }

    int endCol1 = getMapCoord(obj->getCoverRight());
    if (endCol1 < 0) 
    {
        return false;
    }

    if (endCol1 >= getNumCols()) 
    {
        endCol1 = getNumCols() - 1;
    }

    startRow = startRow1;
    endRow = endRow1;
    startCol = startCol1;
    endCol = endCol1;

    return true;
}

void GameMap::getRectCoords(int& startRow, int& endRow, int& startCol, int& endCol,
                            float left, float bottom, float right, float top)
{
    startCol = getMapCoord(left);
    if (startCol < 0) {
        startCol = 0;
    }

    endCol = getMapCoord(right);
    if (endCol >= getNumCols()) {
        endCol = getNumCols() - 1;
    }

    startRow = getMapCoord(bottom);
    if (startRow < 0) {
        startRow = 0;
    }

    endRow = getMapCoord(top);
    if (endRow >= getNumRows()) {
        endRow = getNumRows() - 1;
    }
}


bool GameMap::addObject(GameObject* obj)
{
    int startRow, endRow, startCol, endCol;

    bool placeObjInMap = getMapPosForGameObj(startRow, endRow, startCol, endCol, obj);
    if (!placeObjInMap) 
    {
        return false;
    }

    addObjectToRect(obj, startRow, endRow, startCol, endCol);
    obj->setCoverRect(startRow, endRow, startCol, endCol);

    LOG_DEBUG("addObject %d %d %d %d", startRow, endRow, startCol, endCol);

    return true;
}

void GameMap::removeObject(GameObject* obj)
{
    removeObjectFromRect(obj, obj->getCoverStartRow(), obj->getCoverEndRow(),
                         obj->getCoverStartCol(), obj->getCoverEndCol());
}

bool GameMap::repositionObject(GameObject* obj)
{
    int newStartRow, newEndRow, newStartCol, newEndCol;

    bool placeObjInMap = getMapPosForGameObj(newStartRow, newEndRow, newStartCol, newEndCol, obj);
    if (!placeObjInMap)
    {
        removeObject(obj);
        return false;
    }
    
    int oldStartRow = obj->getCoverStartRow();
    int oldEndRow = obj->getCoverEndRow();
    int oldStartCol = obj->getCoverStartCol();
    int oldEndCol = obj->getCoverEndCol();

    bool noOverlap = newStartRow > oldEndRow ||
                      newEndRow < oldStartRow ||
                      newStartCol > oldEndCol ||
                      newEndCol < oldStartCol;
    
    if (noOverlap) 
    {
        removeObjectFromRect(obj, oldStartRow, oldEndRow, oldStartCol, oldEndCol);
        addObjectToRect(obj, newStartRow, newEndRow, newStartCol, newEndCol);
        obj->setCoverRect(newStartRow, newEndRow, newStartCol, newEndCol);
        return true;
    }
    
    if (newStartRow < oldStartRow) 
    {
        addObjectToRect(obj, newStartRow, oldStartRow - 1, newStartCol, newEndCol);
    }
    else if (newStartRow > oldStartRow) 
    {
        removeObjectFromRect(obj, oldStartRow, newStartRow - 1, oldStartCol, oldEndCol);
    }

    if (newEndRow > oldEndRow) 
    {
        addObjectToRect(obj, oldEndRow + 1, newEndRow, newStartCol, newEndCol);
    }
    else if (newEndRow < oldEndRow) 
    {
        removeObjectFromRect(obj, newEndRow + 1, oldEndRow, oldStartCol, oldEndCol);
    }

    int overlapStartRow = std::max(newStartRow, oldStartRow);
    int overlapEndRow = std::min(newEndRow, oldEndRow);

    if (newStartCol < oldStartCol) 
    {
        addObjectToRect(obj, overlapStartRow, overlapEndRow, newStartCol, oldStartCol - 1);
    }
    else if (newStartCol > oldStartCol) 
    {
        removeObjectFromRect(obj, overlapStartRow, overlapEndRow, oldStartCol, newStartCol - 1);
    }

    if (newEndCol > oldEndCol) 
    {
        addObjectToRect(obj, overlapStartRow, overlapEndRow, oldEndCol + 1, newEndCol);
    }
    else if (newEndCol < oldEndCol) 
    {
        removeObjectFromRect(obj, overlapStartRow, overlapEndRow, newEndCol + 1, oldEndCol);
    }

    obj->setCoverRect(newStartRow, newEndRow, newStartCol, newEndCol);

    return true;
}

void GameMap::addObjectToRect(GameObject* obj, int startRow, int endRow, int startCol, int endCol)
{
    for (int r = startRow; r <= endRow; ++r) 
    {
        std::vector<MapCell>& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c) 
        {
            MapItem* item = m_mapItemPool.alloc();
            item->setObj(obj);
            row[c].add(item);
        }
    }
}

bool GameMap::removeObjectAt(GameObject* obj, int row, int col)
{
    MapItem* prev = nullptr, * cur;
    MapCell& cell = m_map[row][col];

    for (cur = cell.getFirst(); cur; cur = static_cast<MapItem*>(cur->getNext()))
    {
        if (cur->getObj() == obj)
        {
            break;
        }
        prev = cur;
    }

    if (!cur)
    {
        return false;
    }

    cell.unlink(prev, cur);
    m_mapItemPool.free(cur);

    return true;
}

void GameMap::removeObjectFromRect(GameObject* obj, int startRow, int endRow, int startCol, int endCol)
{
    for (int r = startRow; r <= endRow; ++r) 
    {
        for (int c = startCol; c <= endCol; ++c) 
        {
            removeObjectAt(obj, r, c);
        }
    }
}

void GameMap::clearFlagsInRect(int startRow, int endRow, int startCol, int endCol, GameObjectFlag flag)
{
    for (int r = startRow; r <= endRow; ++r) 
    {
        std::vector<MapCell>& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c) 
        {
            for (MapItem* item = m_map[r][c].getFirst(); item; item = static_cast<MapItem*>(item->getNext())) 
            {
                item->getObj()->clearFlag(flag);
            }
        }
    }
}

void GameMap::setPlayer(Player* player)
{
    m_player = player;
    addObject(player);
}

void GameMap::getMoveToRegion(int& startRow, int& endRow, int& startCol, int& endCol, const GameObject* obj,
                              float speedX, float speedY, float delta)
{
    float left = obj->getCollideLeft();
    float right = obj->getCollideRight();
    if (speedX < 0.0f) 
    {
        left += speedX * delta;
    }
    else if (speedX > 0.0f) 
    {
        right += speedX * delta;
    }

    float bottom = obj->getCollideBottom();
    float top = obj->getCollideTop();
    if (speedY < 0.0f) 
    {
        bottom += speedY * delta;
    }
    else if (speedY > 0.0f) 
    {
        top += speedY * delta;
    }

    startRow = getMapCoord(bottom);
    startRow = clamp(startRow, 0, getNumRows() - 1);
    endRow = getMapCoord(top);
    endRow = clamp(endRow, 0, getNumRows() - 1);
    startCol = getMapCoord(left);
    startCol = clamp(startCol, 0, getNumCols() - 1);
    endCol = getMapCoord(right);
    endCol = clamp(endCol, 0, getNumCols() - 1);
}

bool GameMap::checkCollision(float& newDelta, MapCell& collideObjs, const GameObject* obj,
                             float speedX, float speedY, float delta)
{
    bool touch = checkTouchBoundary(newDelta, m_mapWidth, m_mapHeight, obj->getPosX(), obj->getPosY(),
                                     obj->getCollideBreathX(), obj->getCollideBreathY(), speedX, speedY, delta);
    bool collide = checkCollisionNonPassthroughObjs(newDelta, obj, speedX, speedY, newDelta);
    checkCollisionPassthroughObjs(collideObjs, obj, speedX, speedY, newDelta);
    return touch || collide;
}

bool GameMap::checkCollisionNonPassthroughObjs(float& newDelta, const GameObject* obj,
                                               float speedX, float speedY, float delta)
{
    int startRow, endRow, startCol, endCol;
    bool collide = false;

    newDelta = delta;
    getMoveToRegion(startRow, endRow, startCol, endCol, obj, speedX, speedY, delta);
    clearFlagsInRect(startRow, endRow, startCol, endCol, GAME_OBJ_FLAG_CHECKED);

    for (int r = startRow; r <= endRow; ++r)
    {
        std::vector<MapCell>& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c)
        {
            for (MapItem* item = row[c].getFirst(); item; item = static_cast<MapItem*>(item->getNext()))
            {
                GameObject* o = item->getObj();
                if (o == obj || o->testFlag(GAME_OBJ_FLAG_CHECKED))
                {
                    continue;
                }

                if (o->getType() != GAME_OBJ_TYPE_BOT && o->getType() != GAME_OBJ_TYPE_TILE)
                {
                    continue;
                }

                float newDelta1;
                bool collide1 = checkObjCollision(newDelta1, obj->getPosX(), obj->getPosY(),
                                                  obj->getCollideBreathX(), obj->getCollideBreathY(),
                                                  speedX, speedY, o->getPosX(), o->getPosY(),
                                                  o->getCollideBreathX(), o->getCollideBreathY(), newDelta);

                if (collide1)
                {
                    collide = true;
                    if (newDelta1 < newDelta)
                    {
                        newDelta = newDelta1;
                    }
                }

                o->setFlag(GAME_OBJ_FLAG_CHECKED);
            }
        }
    }

    return collide;
}

void GameMap::checkCollisionPassthroughObjs(MapCell& collideObjs, const GameObject* obj,
                                            float speedX, float speedY, float delta)
{
    int startRow, endRow, startCol, endCol;
    float left, bottom, right, top;
    float deltaX, deltaY;

    deltaX = speedX * delta;
    deltaY = speedY * delta;
    left = obj->getCollideLeft() + deltaX;
    right = obj->getCollideRight() + deltaX;
    bottom = obj->getCollideBottom() + deltaY;
    top = obj->getCollideTop() + deltaY;

    getMoveToRegion(startRow, endRow, startCol, endCol, obj, speedX, speedY, delta);
    clearFlagsInRect(startRow, endRow, startCol, endCol, GAME_OBJ_FLAG_CHECKED);

    for (int r = startRow; r <= endRow; ++r)
    {
        std::vector<MapCell>& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c)
        {
            for (MapItem* item = row[c].getFirst(); item; item = static_cast<MapItem*>(item->getNext()))
            {
                GameObject* o = item->getObj();
                if (o == obj || o->testFlag(GAME_OBJ_FLAG_CHECKED)) 
                {
                    continue;
                }
                
                if (o->getType() != GAME_OBJ_TYPE_ANIMATION && o->getType() != GAME_OBJ_TYPE_MISSILE)
                {
                    continue;
                }

                bool collide = checkRectOverlapp(left, bottom, right, top,
                                                  o->getCollideLeft(), o->getCollideBottom(),
                                                  o->getCollideRight(), o->getCollideTop());

                if (collide)
                {
                    MapItem* item = m_mapItemPool.alloc();
                    item->setObj(o);
                    collideObjs.add(item);
                }

                o->setFlag(GAME_OBJ_FLAG_CHECKED);
            }
        }
    }
}

void GameMap::freeMapCell(MapCell& cell)
{
    MapItem* next, * cur;
    for (cur = cell.getFirst(); cur; cur = next)
    {
        next = static_cast<MapItem*>(cur->getNext());
        m_mapItemPool.free(cur);
    }
}

} // end of namespace bot
