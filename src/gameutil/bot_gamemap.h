#ifndef INCLUDE_BOT_GAMEMAP
#define INCLUDE_BOT_GAMEMAP

#include <vector>
#include "structure/bot_objectpool.h"
#include "structure/bot_linkedlist.h"
#include "gameutil/bot_mapitem.h"
#include "gameobj/bot_gameobjectflag.h"

namespace bot {

class Player;

class GameMap {
public:
	typedef LinkedList<MapItem> MapCell;
	
	static int getMapCoord(float z)
	{
		return static_cast<int>(z / GRID_BREATH);
	}

	GameMap();

	virtual ~GameMap();

	void initMap(int numRows, int numCols, int poolSize);

	void clear();

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

	MapCell& getMapCell(int row, int col)
	{
		return m_map[row][col];
	}

	const MapCell& getMapCell(int row, int col) const
	{
		return m_map[row][col];
	}

	bool getMapPosForGameObj(int& startRow, int& endRow, int& startCol, int& endCol, GameObject* obj) const;

	void getRectCoords(int& startRow, int& endRow, int& startCol, int& endCol,
		               float left, float bottom, float right, float top);

	bool addObject(GameObject* obj);

	void removeObject(GameObject* obj);

	bool repositionObject(GameObject* obj);

	void clearFlagsInRect(int startRow, int endRow, int startCol, int endCol, GameObjectFlag flag);

	void setPlayer(Player* player);

	Player* getPlayer()
	{
		return m_player;
	}

	const Player* getPlayer() const
	{
		return m_player;
	}

	void getMoveToRegion(int& startRow, int& endRow, int& startCol, int& endCol, const GameObject* obj,
						 float speedX, float speedY, float delta);

	bool checkCollision(float& newDelta, MapCell& collideObjs, const GameObject* obj,
					    float speedX, float speedY, float delta);

	void freeMapCell(MapCell& cell);

public:
	static const float GRID_BREATH;

protected:
	void addObjectToRect(GameObject* obj, int startRow, int endRow, int startCol, int endCol);

	bool removeObjectAt(GameObject* obj, int row, int col);

	void removeObjectFromRect(GameObject* obj, int startRow, int endRow, int startCol, int endCol);

	bool checkCollisionNonPassthroughObjs(float& newDelta, const GameObject* obj, 
		                                  float speedX, float speedY, float delta);

	void checkCollisionPassthroughObjs(MapCell& collideObjs, const GameObject* obj,
									   float speedX, float speedY, float delta);

protected:
	ObjectPool<MapItem> m_mapItemPool;
	std::vector<std::vector<MapCell>> m_map;
	float m_mapWidth, m_mapHeight;
	Player* m_player;
};

} // end of namespace bot

#endif
