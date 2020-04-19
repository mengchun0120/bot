#ifndef INCLUDE_BOT_SCREEN
#define INCLUDE_BOT_SCREEN

#include <string>

namespace bot {

struct InputEvent;
class App;

class Screen {
public:
    Screen()
    {}

    virtual ~Screen()
    {}

    virtual bool init(App* app) = 0;

    // Returns 0 when the screen has been updated;
    // 1 when switched to another screen;
    // 2 when the app should exit
    virtual int update(float delta) = 0;

    virtual void present() = 0;

    // Returns 0 when the screen is ready to process next input;
    // 1 when switched to another screen;
    // 2 when the app should exit
    virtual int processInput(const InputEvent &e) = 0;
};

} // end of namespace bot

#endif

