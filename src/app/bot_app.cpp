#include "misc/bot_log.h"
#include "opengl/bot_opengl.h"
#include "app/bot_app.h"

namespace bot {

std::shared_ptr<App> App::k_app;

bool App::initInstance(const std::string& appDir, const std::string& cfgFile)
{
    k_app.reset(new App());
    return k_app->init(appDir, cfgFile);
}

App::App()
    : m_window(nullptr)
{
    m_viewportSize[0] = 0.0f;
    m_viewportSize[1] = 0.0f;
}

App::~App()
{
    if(!m_window) 
    {
        glfwTerminate();
    }
}

bool App::init(const std::string& appDir, const std::string& cfgFile)
{
    if (!m_config.load(appDir, cfgFile))
    {
        return false;
    }

    if (!initWindow()) 
    {
        LOG_ERROR("Failed to initialize window");
        return false;
    }

    if (!initOpenGL()) 
    {
        LOG_ERROR("Failed to initialize OpenGL");
        return false;
    }

    if (!initInputManager())
    {
        LOG_ERROR("Failed to intialize input manager");
        return false;
    }

    if (!initGame()) 
    {
        LOG_ERROR("Failed to initialize game");
        return false;
    }

    return true;
}

bool App::run()
{
    float delta;
    int ret;
    InputProcessor processor = std::bind(&ScreenManager::processInput,
                                         &m_screenMgr,
                                         std::placeholders::_1);

    m_timeDeltaSmoother.start();    
    m_inputMgr.start();

    while(glfwWindowShouldClose(m_window) == 0) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (!m_inputMgr.processInput(processor)) {
            break;
        }

        delta = m_timeDeltaSmoother.getTimeDelta();
        ret = m_screenMgr.update(delta);
        if (ret == 2) {
            // the app should exit
            break;
        }
        else if (ret == 1) {
            // switched to another screen, clear input
            m_inputMgr.clear();
        }

        m_screenMgr.present();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    return true;
}

bool App::initWindow()
{
    LOG_INFO("Initializing window");

    if (!glfwInit()) 
    {
        LOG_ERROR("Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_config.getWidth(), m_config.getHeight(), m_config.getTitle().c_str(),
                                NULL, NULL);
    if (!m_window)
    {
        LOG_ERROR("Failed to open GLFW window");
        return false;
    }

    glfwMakeContextCurrent(m_window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) 
    {
        LOG_ERROR("Failed to initialize GLEW");
        return false;
    }

    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
    
    LOG_INFO("Done initializing window");

    return true;
}

bool App::initInputManager()
{
    m_inputMgr.init(m_window, m_config.getEventQueueSize(), getViewportHeight());

    LOG_INFO("Done initializing input manager");

    return true;
}

bool App::initOpenGL()
{
    LOG_INFO("Initializing OpenGL");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_BLEND);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!m_simpleShaderProgram.init(m_config.getSimpleVertexShaderFile(), m_config.getSimpleFragShaderFile())) 
    {
        LOG_ERROR("Failed to init simple shader program");
        return false;
    }

    if (!m_particleShaderProgram.init(m_config.getParticleVertexShaderFile(), m_config.getParticleFragShaderFile()))
    {
        LOG_ERROR("Failed to init particle shader program");
        return false;
    }

    updateViewport();

    LOG_INFO("Done initializing OpenGL");

    return true;
}

void App::updateViewport()
{
    int width, height;

    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    m_viewportSize[0] = static_cast<float>(width);
    m_viewportSize[1] = static_cast<float>(height);

    LOG_INFO("viewportWidth=%f viewportHeight=%f", getViewportWidth(), getViewportHeight());
}

bool App::initGame()
{
    LOG_INFO("Initializing game");

    if (!initTimeDeltaSmoother())
    {
        LOG_ERROR("Failed to initialize time-delta smoother");
        return false;
    }

    if (!initTextSystem())
    {
        LOG_ERROR("Failed to initialize text system");
        return false;
    }
    
    if (!initGameLib())
    {
        LOG_ERROR("Failed to initialize game template lib");
        return false;
    }

    LOG_INFO("Done initializing game");

    return true;
}

bool App::initTimeDeltaSmoother()
{
    LOG_INFO("Initializing time-delta smoother");

    m_timeDeltaSmoother.init(m_config.getTimeDeltaHistoryLen());

    LOG_INFO("Done initializing time-delta smoother");

    return true;
}

bool App::initTextSystem()
{
    LOG_INFO("Initializing text system");

    if (!m_textSystem.init(m_config.getFontDir()))
    {
        return false;
    }

    LOG_INFO("Done initializing text system");

    return true;
}

bool App::initGameLib()
{
    LOG_INFO("Initializing game template libraries");

    bool success = m_gameLib.load();

    if (!success)
    {
        return false;
    }

    m_screenMgr.init();

    LOG_INFO("Done initializing game template libraries");

    return true;
}

} // end of namespace bot
