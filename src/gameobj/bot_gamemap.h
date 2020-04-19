#ifndef INCLUDE_BOT_GAMEMAP
#define INCLUDE_BOT_GAMEMAP

#include <vector>
#include "structure/bot_objectpool.h"
#include "structure/bot_linkedlist.h"
#include "gameobj/bot_mapitem.h"

namespace bot {

class GameMap {
	
	typedef LinkedList<MapItem> MapCell;

public:
	static int getMapCoord(float z)
	{
		return z / GRID_BREATH;
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

	bool getMapPosForGameObj(int& startRow, int& endRow, int& startCol, int& endCol, GameObject* obj) const;

	bool addObject(GameObject* obj);

	void removeObject(GameObject* obj);

	bool repositionObject(GameObject* obj);

public:
	static const float GRID_BREATH;

protected:
	void addObjectToRect(GameObject* obj, int startRow, int endRow, int startCol, int endCol);

	bool removeObjectAt(GameObject* obj, int row, int col);

	void removeObjectFromRect(GameObject* obj, int startRow, int endRow, int startCol, int endCol);

protected:
	ObjectPool<MapItem> m_mapItemPool;
	std::vector<std::vector<MapCell>> m_map;
};

} // end of namespace bot

#endif
