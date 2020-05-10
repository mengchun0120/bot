#ifndef INCLUDE_BOT_APP
#define INCLUDE_BOT_APP

#include <string>
#include <rapidjson/document.h>
#include "input/bot_input_manager.h"
#include "opengl/bot_simple_shader_program.h"
#include "opengl/bot_particle_shader_program.h"
#include "opengl/bot_text_system.h"
#include "gameutil/bot_time_delta_smoother.h"
#include "gametemplate/bot_game_template_lib.h"
#include "screen/bot_screen_manager.h"

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
        return m_viewportSize[0];
    }

    float getViewportHeight() const
    {
        return m_viewportSize[1];
    }
    
    const float* getViewportSize() const
    {
        return m_viewportSize;
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
        return m_simpleShaderProgram;
    }

    ParticleShaderProgram& getParticleShaderProgram()
    {
        return m_particleShaderProgram;
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

    int getMissilePoolSize() const
    {
        return m_missilePoolSize;
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
    float m_viewportSize[Constants::NUM_FLOATS_PER_POSITION];
    InputManager m_inputMgr;
    SimpleShaderProgram m_simpleShaderProgram;
    ParticleShaderProgram m_particleShaderProgram;
    TimeDeltaSmoother m_timeDeltaSmoother;
    TextSystem m_textSystem;
    GameTemplateLib m_gameTemplateLib;
    ScreenManager m_screenMgr;
    float m_mapPoolFactor;
    int m_missilePoolSize;
};

} // end of namespace bot

#endif

