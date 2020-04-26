#include "misc/bot_log.h"
#include "misc/bot_jsonutils.h"
#include "misc/bot_fileutils.h"
#include "opengl/bot_opengl.h"
#include "app/bot_app.h"

namespace bot {

App::App()
    : m_window(nullptr)
    , m_viewportWidth(0.0f)
    , m_viewportHeight(0.0f)
    , m_mapPoolFactor(1.0f)
{
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
    rapidjson::Document doc;

    if (!readJson(doc, cfgFile.c_str()))
    {
        LOG_ERROR("Failed to read config from %s", cfgFile.c_str());
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("%s has wrong format", cfgFile.c_str());
        return false;
    }

    rapidjson::Value cfg = doc.GetObject();
    std::string resRelativeDir;

    if (!parseJson(resRelativeDir, cfg, "resDir"))
    {
        return false;
    }

    m_appDir = appDir;
    m_resDir = constructPath({ m_appDir, resRelativeDir });

    if (!initWindow(cfg)) 
    {
        LOG_ERROR("Failed to initialize window");
        return false;
    }

    if (!initOpenGL(cfg)) 
    {
        LOG_ERROR("Failed to initialize OpenGL");
        return false;
    }

    if (!initInputManager(cfg))
    {
        LOG_ERROR("Failed to intialize input manager");
        return false;
    }

    if (!initGame(cfg)) 
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

bool App::initWindow(const rapidjson::Value& cfg)
{
    LOG_INFO("Initializing window");
       
    int width = 0, height = 0;
    std::string title;

    std::vector<JsonParseParam> params =
    {
        {&width,  "width",  JSONTYPE_INT},
        {&height, "height", JSONTYPE_INT},
        {&title,  "title",  JSONTYPE_STRING}
    };

    if (!parseJson(params, cfg))
    {
        return false;
    }

    LOG_INFO("width=%d height=%d title=%s", width, height, title.c_str());

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

    m_window = glfwCreateWindow(width, height, title.c_str(),
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

bool App::initInputManager(const rapidjson::Value& cfg)
{
    LOG_INFO("Initializing input manager");

    int eventQueueSize = 0;

    if (!parseJson(eventQueueSize, cfg, "eventQueueSize"))
    {
        return false;
    }

    m_inputMgr.init(m_window, eventQueueSize, m_viewportHeight);

    LOG_INFO("Done initializing input manager");

    return true;
}

bool App::initOpenGL(const rapidjson::Value& cfg)
{
    LOG_INFO("Initializing OpenGL");

    std::string vertexShaderFile, fragShaderFile;
    std::string glslDir;

    std::vector<JsonParseParam> params =
    {
        {&vertexShaderFile, "vertexShaderFile", JSONTYPE_STRING},
        {&fragShaderFile,   "fragShaderFile",   JSONTYPE_STRING},
        {&glslDir,          "glslDir",          JSONTYPE_STRING}
    };

    if (!parseJson(params, cfg))
    {
        return false;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::string vertexShaderPath = constructPath({ m_resDir, glslDir, vertexShaderFile });
    std::string fragShaderPath = constructPath({ m_resDir, glslDir, fragShaderFile });

    if (!m_program.init(vertexShaderPath, fragShaderPath)) 
    {
        LOG_ERROR("Failed to init shader program");
        return false;
    }

    m_program.use();

    updateViewport();

    LOG_INFO("Done initializing OpenGL");

    return true;
}

void App::updateViewport()
{
    int width, height;

    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    m_viewportWidth = static_cast<float>(width);
    m_viewportHeight = static_cast<float>(height);

    float viewportSize[] = { m_viewportWidth, m_viewportHeight };

    m_program.setViewportSize(viewportSize);

    LOG_INFO("viewportWidth=%f viewportHeight=%f", m_viewportWidth, m_viewportHeight);
}

bool App::initGame(const rapidjson::Value& cfg)
{
    LOG_INFO("Initializing game");

    if (!initTimeDeltaSmoother(cfg))
    {
        LOG_ERROR("Failed to initialize time-delta smoother");
        return false;
    }

    if (!initTextSystem(cfg))
    {
        LOG_ERROR("Failed to initialize text system");
        return false;
    }
    
    if (!initGameTemplateLib(cfg))
    {
        LOG_ERROR("Failed to initialize game template lib");
        return false;
    }

    if (!initMapConfig(cfg))
    {
        LOG_ERROR("Failed to initialize map config");
        return false;
    }

    LOG_INFO("Done initializing game");

    return true;
}

bool App::initTimeDeltaSmoother(const rapidjson::Value& cfg)
{
    LOG_INFO("Initializing time-delta smoother");

    int timeDeltaHistoryLen = 0;

    if (!parseJson(timeDeltaHistoryLen, cfg, "timeDeltaHistoryLen"))
    {
        return false;
    }

    m_timeDeltaSmoother.init(timeDeltaHistoryLen);

    LOG_INFO("Done initializing time-delta smoother");

    return true;
}

bool App::initTextSystem(const rapidjson::Value& cfg)
{
    LOG_INFO("Initializing text system");

    std::string fontRelativeDir;

    if (!parseJson(fontRelativeDir, cfg, "fontDir"))
    {
        return false;
    }

    std::string fontDir = constructPath({ m_resDir, fontRelativeDir });

    if (!m_textSystem.init(fontDir))
    {
        return false;
    }

    LOG_INFO("Done initializing text system");

    return true;
}

bool App::initGameTemplateLib(const rapidjson::Value& cfg)
{
    LOG_INFO("Initializing game template libraries");

    std::string textureRelativeDir;
    std::string libRelativeDir;
    std::string animationRelativeDir;
    std::string textureLibFile;
    std::string colorLibFile;
    std::string rectLibFile;
    std::string tileTemplateLibFile;
    std::string animationTemplateLibFile;
    std::string missileTemplateLibFile;
    std::string robotTemplateLibFile;
    std::string playerTemplateLibFile;

    std::vector<JsonParseParam> params =
    {
        {&textureRelativeDir,       "textureDir",           JSONTYPE_STRING},
        {&libRelativeDir,           "libDir",               JSONTYPE_STRING},
        {&animationRelativeDir,     "animationDir",         JSONTYPE_STRING},
        {&textureLibFile,           "textureLib",           JSONTYPE_STRING},
        {&colorLibFile,             "colorLib",             JSONTYPE_STRING},
        {&rectLibFile,              "rectLib",              JSONTYPE_STRING},
        {&tileTemplateLibFile,      "tileTemplateLib",      JSONTYPE_STRING},
        {&animationTemplateLibFile, "animationTemplateLib", JSONTYPE_STRING},
        {&missileTemplateLibFile,   "missileTemplateLib",   JSONTYPE_STRING},
        {&robotTemplateLibFile,     "robotTemplateLib",     JSONTYPE_STRING},
        {&playerTemplateLibFile,    "playerTemplateLib",    JSONTYPE_STRING}
    };

    if (!parseJson(params, cfg))
    {
        return false;
    }

    std::string textureDir = constructPath({ m_resDir, textureRelativeDir });
    std::string libDir = constructPath({ m_resDir, libRelativeDir });
    std::string animationDir = constructPath({ m_resDir, animationRelativeDir });
    std::string textureLibPath = constructPath({ libDir, textureLibFile });
    std::string colorLibPath = constructPath({ libDir, colorLibFile });
    std::string rectLibPath = constructPath({ libDir, rectLibFile });
    std::string tileTemplateLibPath = constructPath({ libDir, tileTemplateLibFile });
    std::string animationTemplateLibPath = constructPath({ libDir, animationTemplateLibFile });
    std::string missileTemplateLibPath = constructPath({ libDir, missileTemplateLibFile });
    std::string robotTemplateLibPath = constructPath({ libDir, robotTemplateLibFile });
    std::string playerTemplateLibPath = constructPath({ libDir, playerTemplateLibFile });

    bool success = m_gameTemplateLib.load(textureDir, textureLibPath, rectLibPath, colorLibPath,
                                          tileTemplateLibPath, animationDir, animationTemplateLibPath,
                                          missileTemplateLibPath, robotTemplateLibPath, playerTemplateLibPath);

    if (!success)
    {
        return false;
    }

    m_screenMgr.init(this);

    LOG_INFO("Done initializing game template libraries");

    return true;
}

bool App::initMapConfig(const rapidjson::Value& cfg)
{
    LOG_INFO("Initialize map config");

    std::string mapRelativeDir, mapBaseFile;

    if (!parseJson(mapRelativeDir, cfg, "mapDir"))
    {
        return false;
    }

    if (!parseJson(mapBaseFile, cfg, "mapFile"))
    {
        return false;
    }

    m_mapDir = constructPath({ m_resDir, mapRelativeDir });
    m_mapFile = constructPath({ m_mapDir, mapBaseFile });

    if (!parseJson(m_mapPoolFactor, cfg, "mapPoolFactor"))
    {
        return false;
    }

    LOG_INFO("Done initializing map config");

    return true;
}

} // end of namespace bot
