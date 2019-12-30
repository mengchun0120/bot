#ifndef INCLUDE_BOT_CONSTANTS
#define INCLUDE_BOT_CONSTANTS

namespace bot {

class Constants {
public:
    static const unsigned int NUM_FLOATS_PER_POSITION = 2;
    static const unsigned int NUM_FLOATS_PER_TEXCOORD = 2;
    static const unsigned int POSITION_SIZE = NUM_FLOATS_PER_POSITION * sizeof(float);
    static const unsigned int TEXCOORD_SIZE = NUM_FLOATS_PER_TEXCOORD * sizeof(float);
    static const unsigned int NUM_FLOATS_COLOR = 4;
};

} // end of namespace bot

#endif
