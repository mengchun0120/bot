#ifndef INCLUDE_BOT_SCREEN_MANAGER
#define INCLUDE_BOT_SCREEN_MANAGER

namespace bot {

class Screen;
class App;
struct InputEvent;

class ScreenManager {
public:
    enum ScreenType {
        SCREEN_START,
        SCREEN_GAME,
        SCREEN_NONE
    };

    ScreenManager();

    ~ScreenManager();

    void init();

    int update(float delta);

    void present();

    int processInput(const InputEvent& e);

    void switchScreen(ScreenType type);

private:
    ScreenType m_curScreenType;
    Screen* m_prevScreen;
    Screen* m_curScreen;
};

} // end of namespace bot

#endif

