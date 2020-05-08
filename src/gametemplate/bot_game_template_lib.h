#ifndef INCLUDE_BOT_GAME_TEMPLATE_LIB
#define INCLUDE_BOT_GAME_TEMPLATE_LIB

#include "structure/bot_named_lib.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_tile_template.h"
#include "gametemplate/bot_missile_template.h"
#include "gametemplate/bot_robot_template.h"
#include "gametemplate/bot_animation_template.h"
#include "gametemplate/bot_player_template.h"

namespace bot {

class GameTemplateLib {
public:
	GameTemplateLib();

	~GameTemplateLib();

	bool load(const std::string& textureFolder, const std::string& textureLibFile, 
		      const std::string& rectLibFile, const std::string& colorLibFile,
		      const std::string& tileTemplateLibFile, const std::string& animationFolder,
			  const std::string& animationTemplateLibFile, const std::string& missileTemplateLibFile, 
		      const std::string& robotTemplateLibFile, const std::string& playerTemplateFile);

	const NamedLib<Texture>& getTextureLib() const
	{
		return m_textureLib;
	}

	int getTextureLibSize() const
	{
		return m_textureLib.getLibSize();
	}

	const Texture* getTexture(int idx) const
	{
		return m_textureLib.getObject(idx);
	}

	const Texture* getTexture(const std::string& name) const
	{
		return m_textureLib.getObject(name.c_str());
	}

	const NamedLib<Rectangle>& getRectLib() const
	{
		return m_rectLib;
	}

	int getRectLibSize() const
	{
		return m_rectLib.getLibSize();
	}

	const Rectangle* getRect(int idx) const
	{
		return m_rectLib.getObject(idx);
	}

	const Rectangle* getRect(const std::string& name) const
	{
		return m_rectLib.getObject(name.c_str());
	}

	const NamedLib<Color>& getColorLib() const
	{
		return m_colorLib;
	}

	int getColorLibSize() const
	{
		return m_colorLib.getLibSize();
	}

	const Color* getColor(int idx) const
	{
		return m_colorLib.getObject(idx);
	}

	const Color* getColor(const std::string& name) const
	{
		return m_colorLib.getObject(name.c_str());
	}

	NamedLib<TileTemplate>& getTileTemplateLib()
	{
		return m_tileTemplateLib;
	}

	const NamedLib<TileTemplate>& getTileTemplateLib() const
	{
		return m_tileTemplateLib;
	}

	int getTileTemplateLibSize() const
	{
		return m_tileTemplateLib.getLibSize();
	}

	const TileTemplate* getTileTemplate(int idx) const
	{
		return m_tileTemplateLib.getObject(idx);
	}

	const TileTemplate* getTileTemplate(const std::string& name) const
	{
		return m_tileTemplateLib.getObject(name.c_str());
	}

	NamedLib<MissileTemplate>& getMissileTemplateLib()
	{
		return m_missileTemplateLib;
	}

	const NamedLib<MissileTemplate>& getMissileTemplateLib() const
	{
		return m_missileTemplateLib;
	}

	int getMissileTemplateLibSize() const
	{
		return m_missileTemplateLib.getLibSize();
	}

	const MissileTemplate* getMissileTemplate(int idx) const
	{
		return m_missileTemplateLib.getObject(idx);
	}

	const MissileTemplate* getMissileTemplate(const std::string& name) const
	{
		return m_missileTemplateLib.getObject(name.c_str());
	}

	const NamedLib<RobotTemplate>& getRobotTemplateLib() const
	{
		return m_robotTemplateLib;
	}

	int getRobotTemplateLibSize() const
	{
		return m_robotTemplateLib.getLibSize();
	}

	const RobotTemplate* getRobotTemplate(int idx) const
	{
		return m_robotTemplateLib.getObject(idx);
	}

	const RobotTemplate* getRobotTemplate(const std::string& name) const
	{
		return m_robotTemplateLib.getObject(name.c_str());
	}

	const NamedLib<AnimationTemplate>& getAnimationTemplateLib() const
	{
		return m_animationTemplateLib;
	}

	int getAnimationTemplateLibSize() const
	{
		return m_animationTemplateLib.getLibSize();
	}

	const AnimationTemplate* getAnimationTemplate(int idx) const
	{
		return m_animationTemplateLib.getObject(idx);
	}

	const AnimationTemplate* getAnimatinTemplate(const std::string& name) const
	{
		return m_animationTemplateLib.getObject(name.c_str());
	}

	const PlayerTemplate* getPlayerTemplate() const
	{
		return &m_playerTemplate;
	}

	PlayerTemplate* getPlayerTemplate()
	{
		return &m_playerTemplate;
	}

private:
	bool loadPlayerTemplate(const std::string& playerTemplateFile);

private:
	NamedLib<Texture> m_textureLib;
	NamedLib<Rectangle> m_rectLib;
	NamedLib<Color> m_colorLib;
	NamedLib<TileTemplate> m_tileTemplateLib;
	NamedLib<MissileTemplate> m_missileTemplateLib;
	NamedLib<RobotTemplate> m_robotTemplateLib;
	NamedLib<AnimationTemplate> m_animationTemplateLib;
	PlayerTemplate m_playerTemplate;
};

} // end of namespace bot

#endif
