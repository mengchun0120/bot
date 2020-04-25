#ifndef INCLUDE_BOT_TILE
#define INCLUDE_BOT_TILE

#include "misc/bot_constants.h"
#include "gameobj/bot_gameobject.h"

namespace bot {

class TileTemplate;

class Tile : public GameObject {
public:
    Tile(const TileTemplate* tileTemplate);

    virtual ~Tile();

    virtual void present(SimpleShaderProgram& program);

    virtual bool update(float delta, GameScreen& screen);

    void setPos(float x, float y);

    int getHP() const
    {
        return m_hp;
    }

    bool descreaseHP(int deltaHP);

protected:
    int m_hp;
};

} // end of namespace bot

#endif
