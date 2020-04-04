#ifndef INCLUDE_BOT_GAMETEMPLATELIB
#define INCLUDE_BOT_GAMETEMPLATELIB

#include "structure/bot_namedlib.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_tiletemplate.h"
#include "gametemplate/bot_missiletemplate.h"
#include "gametemplate/bot_robottemplate.h"
#include "gametemplate/bot_animationtemplate.h"

namespace bot {

class GameTemplateLib {
public:
	GameTemplateLib();

	~GameTemplateLib();

	bool load(const std::string& textureFolder, const std::string& textureLibFile, 
		      const std::string& rectLibFile, const std::string& colorLibFile,
		      const std::string& tileTemplateLibFile, const std::string& animationFolder,
			  const std::string& animationTemplateLibFile, const std::string& missileTemplateLibFile, 
		      const std::string& robotTemplateLibFile);

	const NamedLib<Texture>& getTextureLib() const
	{
		return m_textureLib;
	}

	int getTextureLibSize() const
	{
		return m_textureLib.getLibSize();
	}

	const Texture* getTextureByIdx(int idx) const
	{
		return m_textureLib.getObjectByIdx(idx);
	}

	const Texture* getTextureByName(const std::string& name) const
	{
		return m_textureLib.getObjectByName(name.c_str());
	}

	const NamedLib<Rectangle>& getRectLib() const
	{
		return m_rectLib;
	}

	int getRectLibSize() const
	{
		return m_rectLib.getLibSize();
	}

	const Rectangle* getRectByIdx(int idx) const
	{
		return m_rectLib.getObjectByIdx(idx);
	}

	const Rectangle* getRectByName(const std::string& name) const
	{
		return m_rectLib.getObjectByName(name.c_str());
	}

	const NamedLib<Color>& getColorLib() const
	{
		return m_colorLib;
	}

	int getColorLibSize() const
	{
		return m_colorLib.getLibSize();
	}

	const Color* getColorByIdx(int idx) const
	{
		return m_colorLib.getObjectByIdx(idx);
	}

	const Color* getColorByName(const std::string& name) const
	{
		return m_colorLib.getObjectByName(name.c_str());
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

	const TileTemplate* getTileTemplateByIdx(int idx) const
	{
		return m_tileTemplateLib.getObjectByIdx(idx);
	}

	const TileTemplate* getTileTemplateByName(const std::string& name) const
	{
		return m_tileTemplateLib.getObjectByName(name.c_str());
	}

	NamedLib<MissileTemplate>& getMissileTemplate()
	{
		return m_missileTemplateLib;
	}

	const NamedLib<MissileTemplate>& getMissileTemplate() const
	{
		return m_missileTemplateLib;
	}

	int getMissileTemplateLibSize() const
	{
		return m_missileTemplateLib.getLibSize();
	}

	const MissileTemplate* getMissileTemplateByIdx(int idx) const
	{
		return m_missileTemplateLib.getObjectByIdx(idx);
	}

	const MissileTemplate* getMissileTemplateByName(const std::string& name) const
	{
		return m_missileTemplateLib.getObjectByName(name.c_str());
	}

	const NamedLib<RobotTemplate>& getRobotTemplateLib() const
	{
		return m_robotTemplateLib;
	}

	int getRobotTemplateLibSize() const
	{
		return m_robotTemplateLib.getLibSize();
	}

	const RobotTemplate* getRobotTemplateByIdx(int idx) const
	{
		return m_robotTemplateLib.getObjectByIdx(idx);
	}

	const RobotTemplate* getRobotTemplateByName(const std::string& name) const
	{
		return m_robotTemplateLib.getObjectByName(name.c_str());
	}

	const NamedLib<AnimationTemplate>& getAnimationTemplateLib() const
	{
		return m_animationTemplateLib;
	}

	int getAnimationTemplateLibSize() const
	{
		return m_animationTemplateLib.getLibSize();
	}

	const AnimationTemplate* getAnimationTemplateByIdx(int idx) const
	{
		return m_animationTemplateLib.getObjectByIdx(idx);
	}

	const AnimationTemplate* getAnimatinTemplateByName(const std::string& name) const
	{
		return m_animationTemplateLib.getObjectByName(name.c_str());
	}

private:
	NamedLib<Texture> m_textureLib;
	NamedLib<Rectangle> m_rectLib;
	NamedLib<Color> m_colorLib;
	NamedLib<TileTemplate> m_tileTemplateLib;
	NamedLib<MissileTemplate> m_missileTemplateLib;
	NamedLib<RobotTemplate> m_robotTemplateLib;
	NamedLib<AnimationTemplate> m_animationTemplateLib;
};

} // end of namespace bot

#endif
