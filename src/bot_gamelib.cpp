#include <utility>
#include <sstream>
#include "bot_log.h"
#include "bot_config.h"
#include "bot_utils.h"
#include "bot_abilitytemplate.h"
#include "bot_app.h"
#include "bot_gamelib.h"

using namespace std;

namespace bot {

struct TextureProcessor {
    vector<Texture> &m_textureLib;
    NameIdMap &m_textureMap;
    
    TextureProcessor(vector<Texture> &textureLib, NameIdMap &textureMap)
    : m_textureLib(textureLib)
    , m_textureMap(textureMap)
    {}

    void init(int count)
    {
        m_textureLib.resize(count);
    }

    bool operator()(const CSVReader &reader)
    {
        const string &name = reader.read("name");
        if(m_textureMap.search(name) != -1) {
            LOG_ERROR("Texture %s already existed", name.c_str());
            return false;
        }

        int idx = reader.curRecordIdx();
        const string &file = reader.read("file");
        string textureFile = constructPath({App::g_app.getResourceDir(), "ui", file});
        if(!m_textureLib[idx].load(textureFile)) {
            LOG_ERROR("Failed to load texture %s", textureFile.c_str());
            return false;
        }

        m_textureMap.add(name, idx);

        return true;
    }
};

struct RectProcessor {
    vector<Rectangle> &m_rectLib;
    NameIdMap &m_rectMap;

    RectProcessor(vector<Rectangle> &rectLib, NameIdMap &rectMap)
    : m_rectLib(rectLib)
    , m_rectMap(rectMap)
    {}

    void init(int count)
    {
        m_rectLib.resize(count);
    }

    bool operator()(const CSVReader &reader)
    {
        const string &name = reader.read("name");
        if(m_rectMap.search(name) != -1) {
            LOG_ERROR("Rect %s already existed", name.c_str());
            return false;
        }

        int idx = reader.curRecordIdx();
        float width = stof(reader.read("width"));
        float height = stof(reader.read("height"));

        if(!m_rectLib[idx].init(width, height, true)) {
            LOG_ERROR("Cannot initialize rectangle width=%f height=%f", width, height);
            return false;
        }

        m_rectMap.add(name, idx);

        return true;
    }
};

struct ColorProcessor {
    vector<Color> &m_colorLib;
    NameIdMap &m_colorMap;

    ColorProcessor(vector<Color> &colorLib, NameIdMap &colorMap)
    : m_colorLib(colorLib)
    , m_colorMap(colorMap)
    {}

    void init(int count)
    {
        m_colorLib.resize(count);
    }

    bool operator()(const CSVReader &reader)
    {
        const string &name = reader.read("name");
        if(m_colorMap.search(name) != -1) {
            LOG_ERROR("Color %s already existed", name.c_str());
            return false;
        }

        int red = stoi(reader.read("red"));
        if(red < 0 || red > 255) {
            LOG_ERROR("Invalid red value %d", red);
            return false;
        }

        int green = stoi(reader.read("green"));
        if(green < 0 || green > 255) {
            LOG_ERROR("Invalid green value %d", green);
            return false;
        }

        int blue = stoi(reader.read("blue"));
        if(blue < 0 || blue > 255) {
            LOG_ERROR("Invalid blue value %d", blue);
            return false;
        }

        int alpha = stoi(reader.read("alpha"));
        if(alpha < 0 || alpha > 255) {
            LOG_ERROR("Invalid alpha value %d", alpha);
            return false;
        }

        int idx = reader.curRecordIdx();
        m_colorLib[idx].set(red, green, blue, alpha);
        m_colorMap.add(name, idx);

        return true;
    }
};

struct ComponentTemplateProcessor {
    vector<Texture> &m_textureLib;
    NameIdMap &m_textureMap;
    vector<Rectangle> &m_rectLib;
    NameIdMap &m_rectMap;
    vector<ComponentTemplate> &m_componentTemplateLib;
    NameIdMap &m_componentTemplateMap;

    ComponentTemplateProcessor(vector<Texture> &textureLib, NameIdMap &textureMap,
                            vector<Rectangle> &rectLib, NameIdMap &rectMap,
                            vector<ComponentTemplate> &componentTemplateLib,
                            NameIdMap &componentTemplateMap)
    : m_textureLib(textureLib)
    , m_textureMap(textureMap)
    , m_rectLib(rectLib)
    , m_rectMap(rectMap)
    , m_componentTemplateLib(componentTemplateLib)
    , m_componentTemplateMap(componentTemplateMap)
    {}

    void init(int count)
    {
        m_componentTemplateLib.resize(count);
    }

    bool operator()(const CSVReader &reader)
    {
        int idx = reader.curRecordIdx();
        ComponentTemplate &t = m_componentTemplateLib[idx];

        const string &name = reader.read("name");
        if(m_componentTemplateMap.search(name) != -1) {
            LOG_ERROR("Component template %s already existed", name.c_str());
            return false;
        }

        const string &texture = reader.read("texture");
        int textureIdx = m_textureMap.search(texture);
        if(textureIdx == -1) {
            LOG_ERROR("Cannot find texture %s", texture.c_str());
            return false;
        }
        t.setTexture(&m_textureLib[textureIdx]);

        const string &rect = reader.read("rect");
        int rectIdx = m_rectMap.search(rect);
        if(rectIdx == -1) {
            LOG_ERROR("Cannot find rect %s", rect.c_str());
        }
        t.setRect(&m_rectLib[rectIdx]);

        t.setHP(stoi(reader.read("hp")));

        float collideBreathX = stof(reader.read("collide_breath_x"));
        t.setCollideBreathX(collideBreathX);
        float collideBreathY = stof(reader.read("collide_breath_y"));
        t.setCollideBreathY(collideBreathY);

        bool canMove = stoi(reader.read("can_move")) != 0;
        if(canMove) {
            float speed = stof(reader.read("speed"));
            MoveAbilityTemplate *ability = new MoveAbilityTemplate(speed);
            t.addAbility(ability);
        }

        bool canFire = stoi(reader.read("can_fire")) != 0;
        if(canFire) {
            float speed = stof(reader.read("attack_speed"));
            float firePointX = stof(reader.read("fire_point_x"));
            float firePointY = stof(reader.read("fire_point_y"));
            float fireDirectionX = stof(reader.read("fire_direction_x"));
            float fireDirectionY = stof(reader.read("fire_direction_y"));
            const string &bulletName = reader.read("bullet");
            FireAbilityTemplate *ability = new FireAbilityTemplate(speed, 
                                                    firePointX, firePointY, fireDirectionX,
                                                    fireDirectionY, bulletName);
            t.addAbility(ability);
        }

        bool canExplode = stoi(reader.read("can_explode")) != 0;
        if(canExplode) {
            int explodPower = stoi(reader.read("explode_power"));
            ExplodeAbilityTemplate *ability = new ExplodeAbilityTemplate(explodPower);
            t.addAbility(ability);
        }

        m_componentTemplateMap.add(name, idx);

        return true;
    }
};

struct GameObjectTemplateProcessor {
    vector<ComponentTemplate> &m_componentTemplateLib;
    NameIdMap &m_componentTemplateMap;
    vector<GameObjectTemplate> &m_gameObjectTemplateLib;
    NameIdMap &m_gameObjectTemplateMap;


    GameObjectTemplateProcessor(vector<ComponentTemplate> &componentTemplateLib,
                                NameIdMap &componentTemplateMap,
                                vector<GameObjectTemplate> &gameObjectTemplateLib,
                                NameIdMap &gameObjectTemplateMap)
    : m_componentTemplateLib(componentTemplateLib)
    , m_componentTemplateMap(componentTemplateMap)
    , m_gameObjectTemplateLib(gameObjectTemplateLib)
    , m_gameObjectTemplateMap(gameObjectTemplateMap)
    {}

    void init(int count)
    {
        m_gameObjectTemplateLib.resize(count);
    }

    bool operator()(const CSVReader &reader)
    {
        int idx = reader.curRecordIdx();
        GameObjectTemplate &obj = m_gameObjectTemplateLib[idx];

        const string &name = reader.read("name");
        if(m_gameObjectTemplateMap.search(name) != -1) {
            LOG_ERROR("GameObjectTemplate %s already existed", name.c_str());
            return false;
        }

        const string &t = reader.read("type");
        GameObjectType type = getType(t);
        if(type == GAMEOBJ_UNKNONWN) {
            LOG_ERROR("Invalid GameObjectTemplate type %s", t.c_str());
        }
        obj.setType(type);

        float coverBreathX = stof(reader.read("cover_breath_x"));
        obj.setCoverBreathX(coverBreathX);
        float coverBreathY = stof(reader.read("cover_breath_y"));
        obj.setCoverBreathY(coverBreathY);

        const string &baseComponent = reader.read("base_component");
        int baseComponentIdx = m_componentTemplateMap.search(baseComponent);
        if(baseComponentIdx == -1) {
            LOG_ERROR("Cannot find ComponentTemplate %s", baseComponent.c_str());
            return false;
        }
        obj.setBaseComponent(&m_componentTemplateLib[baseComponentIdx]);

        list<string> partNames;
        list<float> xPos;
        list<float> yPos;
        for(int j = 1; j <= GameObjectTemplate::MAX_PARTS; ++j) {
            string c = "component" + to_string(j);
            const string &partName = reader.read(c);
            if(partName.empty()) {
                continue;
            }

            partNames.emplace_back(partName);
            
            string xs = "x" + to_string(j);
            xPos.push_back(stof(reader.read(xs)));
            string ys = "y" + to_string(j);
            yPos.push_back(stof(reader.read(ys)));
        }

        int numParts = static_cast<int>(partNames.size());
        obj.resizeParts(numParts);
       
        auto partNameIt = partNames.begin();
        auto xPosIt = xPos.begin();
        auto yPosIt = yPos.begin();

        for(int i = 0; i < numParts; ++i, ++partNameIt, ++xPosIt, ++yPosIt) {
            int componentIdx = m_componentTemplateMap.search(*partNameIt);
            if(componentIdx == -1) {
                LOG_ERROR("Cannot find component %s", partNameIt->c_str());
                return false;
            }

            obj.setPart(i, &m_componentTemplateLib[componentIdx], *xPosIt, *yPosIt);
        }

        m_gameObjectTemplateMap.add(name, idx);

        return true;
    }

    GameObjectType getType(const string &t)
    {
        if(t == "bot") {
            return GAMEOBJ_BOT;
        } else if(t == "tile") {
            return GAMEOBJ_TILE;
        } else if(t == "bullet") {
            return GAMEOBJ_BULLET;
        } else {
            return GAMEOBJ_UNKNONWN;
        }
    }
};

GameLib::GameLib()
{

}

GameLib::~GameLib()
{

}

bool GameLib::init()
{
    if(!initTextureLib()) {
        LOG_ERROR("Failed to initialize texture lib");
        return false;
    }

    if(!initRectLib()) {
        LOG_ERROR("Failed to initialize rect lib");
        return false;
    }

    if(!initColorLib()) {
        LOG_ERROR("Failed to initialize color lib");
        return false;
    }

    if(!initComponentTemplateLib()) {
        LOG_ERROR("Failed to initialize component lib");
        return false;
    }

    if(!initGameObjectTemplateLib()) {
        LOG_ERROR("Failed to initialize GameObjectTemplateLib");
        return false;
    }

    if(!fillComponentBullet()) {
        LOG_ERROR("Failed to set bullet");
        return false;
    }

    organizeGameObjectTemplate();

    return true;
}

bool GameLib::initTextureLib()
{
    Config &cfg = Config::g_cfg;
    string fileName = constructPath({App::g_app.getResourceDir(), "lib", "texture_lib.csv"});
    TextureProcessor processor(m_textureLib, m_textureMap);
    return readAndProcessCSVFile(fileName, processor);
}

bool GameLib::initRectLib()
{
    Config &cfg = Config::g_cfg;
    string fileName = constructPath({ App::g_app.getResourceDir(), "lib", "rect_lib.csv" }); 
    RectProcessor processor(m_rectLib, m_rectMap);
    return readAndProcessCSVFile(fileName, processor);
}

bool GameLib::initColorLib()
{
    Config &cfg = Config::g_cfg;
    string fileName = constructPath({ App::g_app.getResourceDir(), "lib", "color_lib.csv" });
    ColorProcessor processor(m_colorLib, m_colorMap);
    return readAndProcessCSVFile(fileName, processor);
}

bool GameLib::initComponentTemplateLib()
{
    Config &cfg = Config::g_cfg;
    string fileName = constructPath({ App::g_app.getResourceDir(), "lib", "component_template_lib.csv" });
    ComponentTemplateProcessor processor(m_textureLib, m_textureMap, m_rectLib,
                                         m_rectMap, m_componentTemplateLib,
                                         m_componentTemplateMap);
    return readAndProcessCSVFile(fileName, processor);
}

bool GameLib::initGameObjectTemplateLib()
{
    Config &cfg = Config::g_cfg;
    string fileName = constructPath({ App::g_app.getResourceDir(), "lib", "game_obj_template_lib.csv" });
    GameObjectTemplateProcessor processor(m_componentTemplateLib, m_componentTemplateMap,
                                          m_gameObjectTemplateLib, m_gameObjectTemplateMap);
    return readAndProcessCSVFile(fileName, processor);
}

void GameLib::organizeGameObjectTemplate()
{
    int tileCount = 0, botCount = 0, bulletCount = 0;
    int count = static_cast<int>(m_gameObjectTemplateLib.size());

    for(int i = 0; i < count; ++i) {
        switch(m_gameObjectTemplateLib[i].getType()) {
        case GAMEOBJ_BOT:
            botCount++;
            break;
        case GAMEOBJ_TILE:
            tileCount++;
            break;
        case GAMEOBJ_BULLET:
            bulletCount++;
            break;
        }
    }

    if(botCount > 0) {
        m_botTemplateLib.reserve(botCount);
    }

    if(tileCount > 0) {
        m_tileTemplateLib.reserve(tileCount);
    }

    if(bulletCount > 0) {
        m_bulletTemplateLib.reserve(bulletCount);
    }

    for(int i = 0; i < count; ++i) {
        GameObjectTemplate &t = m_gameObjectTemplateLib[i];
        switch(t.getType()) {
        case GAMEOBJ_BOT:
            m_botTemplateLib.push_back(&t);
            break;
        case GAMEOBJ_TILE:
            m_tileTemplateLib.push_back(&t);
            break;
        case GAMEOBJ_BULLET:
            m_bulletTemplateLib.push_back(&t);
            break;
        }
    }
}

bool GameLib::fillComponentBullet()
{
    int count = static_cast<int>(m_componentTemplateLib.size());

    for(int i = 0; i < count; ++i) {
        ComponentTemplate &t = m_componentTemplateLib[i];
        AbilityTemplate *a;
        for(a = t.firstAbility(); a; a = static_cast<AbilityTemplate*>(a->getNext())) {
            if(a->getType() != ABILITY_FIRE) {
                continue;
            }

            FireAbilityTemplate *fa = static_cast<FireAbilityTemplate*>(a);
            const string &bulletName = fa->getBulletName();
            int bulletIdx = m_gameObjectTemplateMap.search(bulletName);
            if(bulletIdx == -1) {
                LOG_ERROR("Cannot find bullet %s", bulletName.c_str());
                return false;
            }

            GameObjectTemplate &bullet = m_gameObjectTemplateLib[bulletIdx];
            if(bullet.getType() != GAMEOBJ_BULLET) {
                LOG_ERROR("GameObjectTemplate %s is not a bullet", bulletName.c_str());
                return false;
            }

            fa->setBullet(&m_gameObjectTemplateLib[bulletIdx]);
        }
    }

    return true;
}

const Texture* GameLib::getTexture(const char* name) const
{
    int idx = m_textureMap.search(name);
    if (idx == -1) {
        LOG_ERROR("Cannot find texture %s", name);
        return nullptr;
    }

    return &m_textureLib[idx];
}

const Rectangle* GameLib::getRect(const char* name) const
{
    int idx = m_rectMap.search(name);
    if (idx == -1) {
        LOG_ERROR("Cannot find rect %s", name);
        return nullptr;
    }

    return &m_rectLib[idx];
}

const Color* GameLib::getColor(const char* name) const
{
    int idx = m_colorMap.search(name);
    if (idx == -1) {
        LOG_ERROR("Cannot find color %s", name);
        return nullptr;
    }

    return &m_colorLib[idx];
}

const ComponentTemplate* GameLib::getComponentTemplate(const char* name) const
{
    int idx = m_componentTemplateMap.search(name);
    if (idx == -1) {
        LOG_ERROR("Cannot find ComponentTemplate %s", name);
        return nullptr;
    }

    return &m_componentTemplateLib[idx];
}

const GameObjectTemplate* GameLib::getGameObjectTemplate(const char* name) const
{
    int idx = m_gameObjectTemplateMap.search(name);
    if (idx == -1) {
        LOG_ERROR("Cannot find GameObjectTemplate %s", name);
        return nullptr;
    }

    return &m_gameObjectTemplateLib[idx];
}

} // end of namespace bot
