#ifndef INCLUDE_BOT_SCREENMANAGER
#define INCLUDE_BOT_SCREENMANAGER

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

    void init(App* app);

    int update(float delta);

    void present();

    int processInput(const InputEvent& e);

    void switchScreen(ScreenType type);

private:
    App* m_app;
    ScreenType m_curScreenType;
    Screen* m_prevScreen;
    Screen* m_curScreen;
};

} // end of namespace bot

#endif

