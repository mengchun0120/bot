#include <algorithm>
#include "bot_gameobject.h"
#include "bot_collisionrect.h"
#include "bot_gamemap.h"

namespace bot {

const float GameMap::GRID_BREATH = 40.0f;

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

bool GameMap::addGameObj(GameObject* obj)
{
    int startRow, endRow, startCol, endCol;
    if (!getMapPosForGameObj(startRow, endRow, startCol, endCol, obj)) {
        return false;
    }

    for (int row = startRow; row <= endRow; ++row) {
        for (int col = startCol; col <= endCol; ++col) {
            addGameObjAt(obj, row, col);
        }
    }

    obj->setCoverStartRow(startRow);
    obj->setCoverEndRow(endRow);
    obj->setCoverStartCol(startCol);
    obj->setCoverEndCol(endCol);

    return true;
}

bool GameMap::repositionGameObj(GameObject* obj)
{
    int newStartRow, newEndRow, newStartCol, newEndCol;

    if (!getMapPosForGameObj(newStartRow, newEndRow, newStartCol, newEndCol, obj)) {
        removeGameObj(obj);
        return false;
    }

    int oldStartRow = obj->getCoverStartRow();
    int oldEndRow = obj->getCoverEndRow();
    int oldStartCol = obj->getCoverStartCol();
    int oldEndCol = obj->getCoverEndCol();

    if (newStartRow > oldEndRow || newEndRow < oldStartRow || newStartCol > oldEndCol || newEndCol < oldStartCol) {
        removeGameObjFromRect(obj, oldStartRow, oldEndRow, oldStartCol, oldEndCol);
        addGameObjToRect(obj, newStartRow, newEndRow, newStartCol, newEndCol);
        return true;
    }

    if (newStartRow < oldStartRow) {
        addGameObjToRect(obj, newStartRow, oldStartRow - 1, newStartCol, newEndCol);
    }
    else if (newStartRow > oldStartRow) {
        removeGameObjFromRect(obj, oldStartRow, newStartRow - 1, oldStartCol, oldEndCol);
    }

    if (newEndRow > oldEndRow) {
        addGameObjToRect(obj, oldEndRow + 1, newEndRow, newStartCol, newEndCol);
    }
    else if (newEndRow < oldEndRow) {
        removeGameObjFromRect(obj, newEndRow + 1, oldEndRow, oldStartCol, oldEndCol);
    }

    int overlapStartRow = std::max(newStartRow, oldStartRow);
    int overlapEndRow = std::min(newEndRow, oldEndRow);

    if (newStartCol < oldStartCol) {
        addGameObjToRect(obj, overlapStartRow, overlapEndRow, newStartCol, oldStartCol - 1);
    }
    else if (newStartCol > oldStartCol) {
        removeGameObjFromRect(obj, overlapStartRow, overlapEndRow, oldStartCol, newStartCol - 1);
    }

    if (newEndCol > oldEndCol) {
        addGameObjToRect(obj, overlapStartRow, overlapEndRow, oldEndCol + 1, newEndCol);
    }
    else if (newEndCol < oldEndCol) {
        removeGameObjFromRect(obj, overlapStartRow, overlapEndRow, newEndCol + 1, oldEndCol);
    }

    return true;
}

void GameMap::removeGameObj(GameObject* obj)
{
    int endRow = obj->getCoverEndRow();
    int endCol = obj->getCoverEndCol();

    for (int row = obj->getCoverStartRow(); row <= endRow; ++row) {
        for (int col = obj->getCoverStartCol(); col <= endCol; ++col) {
            removeGameObjAt(obj, row, col);
        }
    }
}

bool GameMap::getMapPosForGameObj(int& startRow, int& endRow, int& startCol, int& endCol, GameObject* obj)
{
    int startRow1 = getMapCoord(obj->getCoverBottom());
    if (startRow1 >= getNumRows()) {
        return false;
    }

    if (startRow1 < 0) {
        startRow1 = 0;
    }

    int endRow1 = getMapCoord(obj->getCoverTop());
    if (endRow1 < 0) {
        return false;
    }

    if (endRow1 >= getNumRows()) {
        endRow1 = getNumRows() - 1;
    }

    int startCol1 = getMapCoord(obj->getCoverLeft());

    if (startCol1 >= getNumCols()) {
        return false;
    }

    if (startCol1 < 0) {
        startCol1 = 0;
    }

    int endCol1 = getMapCoord(obj->getCoverRight());

    if (endCol1 < 0) {
        return false;
    }

    if (endCol1 >= getNumCols()) {
        endCol1 = getNumCols() - 1;
    }

    startRow = startRow1;
    endRow = endRow1;
    startCol = startCol1;
    endCol = endCol1;

    return true;
}

void GameMap::addGameObjAt(GameObject* obj, int row, int col)
{
    MapItem* newItem = m_pool.alloc();
    newItem->setObj(obj);
    newItem->setNext(m_map[row][col]);
    m_map[row][col] = newItem;
}

MapItem* GameMap::findGameObjectAt(GameObject* obj, int row, int col)
{
    MapItem* item = m_map[row][col];
    for (; item; item = static_cast<MapItem*>(item->getNext())) {
        if (item->getObj() == obj) {
            break;
        }
    }

    return item;
}

void GameMap::removeGameObjAt(GameObject* obj, int row, int col)
{
    MapItem* prev = nullptr;
    MapItem* item = m_map[row][col];

    for (; item; item = static_cast<MapItem*>(item->getNext())) {
        if (item->getObj() == obj) {
            break;
        }
        prev = item;
    }

    if (!item) {
        return;
    }

    if (prev) {
        prev->setNext(item->getNext());
    }
    else {
        m_map[row][col] = static_cast<MapItem*>(item->getNext());
    }

    m_pool.free(item);
}

void GameMap::removeGameObjFromRect(GameObject* obj, int startRow, int endRow, int startCol, int endCol)
{
    for (int r = startRow; r <= endRow; ++r) {
        for (int c = startCol; c <= endCol; ++c) {
            removeGameObjAt(obj, r, c);
        }
    }
}

void GameMap::addGameObjToRect(GameObject* obj, int startRow, int endRow, int startCol, int endCol)
{
    for (int r = startRow; r <= endRow; ++r) {
        for (int c = startCol; c <= endCol; ++c) {
            addGameObjAt(obj, r, c);
        }
    }
}

} // end of namespace bot
