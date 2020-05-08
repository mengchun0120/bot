#ifndef INCLUDE_BOT_GAME_OBJECT_FLAG
#define INCLUDE_BOT_GAME_OBJECT_FLAG

namespace bot {

enum class GameObjectFlag {
    DRAWN             = 0x00000001,
    UPDATED           = 0x00000002,
    CHECKED           = 0x00000004,
    DEAD              = 0x00000008,
    COLLIDABLE        = 0x00000010,
    INDESTRUCTABLE    = 0x00000020,
    INVISIBLE         = 0x00000040,
    PASSTHROUGH       = 0x00000080,
    EXPLODE_CHECKED   = 0x00000100
};

} // end of namespace bot

#endif
