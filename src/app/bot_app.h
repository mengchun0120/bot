#ifndef INCLUDE_BOT_APP
#define INCLUDE_BOT_APP

#include <string>
#include <rapidjson/document.h>
#include "input/bot_inputmanager.h"
#include "opengl/bot_simpleshaderprogram.h"
#include "opengl/bot_textsystem.h"
#include "gameutil/bot_timedeltasmoother.h"
#include "gametemplate/bot_gametemplatelib.h"
#include "screen/bot_screenmanager.h"

struct GLFWwindow;

namespace bot {

class App {
public:
    App();

    ~App();

    bool init(const std::string& appDir, const std::string& cfgFile);

    bool run();

    float getViewportWidth() const
    {
        return m_viewportWidth;
    }

    float getViewportHeight() const
    {
        return m_viewportHeight;
    }

    const std::string& getAppDir() const
    {
        return m_appDir;
    }

    const std::string& getResDir() const
    {
        return m_resDir;
    }

    const std::string& getMapDir() const
    {
        return m_mapDir;
    }

    const std::string& getMapFile() const
    {
        return m_mapFile;
    }

    SimpleShaderProgram& getSimpleShaderProgram()
    {
        return m_program;
    }

    const GameTemplateLib& getGameTemplateLib() const
    {
        return m_gameTemplateLib;
    }

    const TextSystem& getTextSystem() const
    {
        return m_textSystem;
    }

    ScreenManager& getScreenManager()
    {
        return m_screenMgr;
    }

    float getMapPoolFactor() const
    {
        return m_mapPoolFactor;
    }

private:
    bool initWindow(const rapidjson::Value& cfg);

    bool initInputManager(const rapidjson::Value& cfg);

    bool initOpenGL(const rapidjson::Value& cfg);

    void updateViewport();

    bool initGame(const rapidjson::Value& cfg);

    bool initTimeDeltaSmoother(const rapidjson::Value& cfg);

    bool initTextSystem(const rapidjson::Value& cfg);

    bool initGameTemplateLib(const rapidjson::Value& cfg);

    bool initMapConfig(const rapidjson::Value& cfg);

private:
    GLFWwindow* m_window;
    std::string m_appDir;
    std::string m_resDir;
    std::string m_mapDir;
    std::string m_mapFile;
    float m_viewportWidth;
    float m_viewportHeight;
    InputManager m_inputMgr;
    SimpleShaderProgram m_program;
    TimeDeltaSmoother m_timeDeltaSmoother;
    TextSystem m_textSystem;
    GameTemplateLib m_gameTemplateLib;
    ScreenManager m_screenMgr;
    float m_mapPoolFactor;
};

} // end of namespace bot

#endif

