#ifndef HEADER_SPRITE_MANAGER_H
#define HEADER_SPRITE_MANAGER_H

#ifdef WIN32
#pragma once
#endif

#include <vector>
#include <ClanLib/core.h>

class CL_ResourceManager;
class CL_Sprite;

class SpriteManager
{
public:
	explicit SpriteManager(CL_ResourceManager & resource);
	SpriteManager(SpriteManager const & copy);
	~SpriteManager();
public:
	enum Sprites
	{
		HANDLE_ON,
		HANDLE_OFF,
		HANDLE_ON_HINT,
		HANDLE_OFF_HINT,
		HANDLE_ON_HIGHLIGHT,
		HANDLE_OFF_HIGHLIGHT,
		TOTAL_NUMBER_OF_SPRITES
	};
public:
	CL_Size HandleSize() const;
	CL_Sprite & operator()(Sprites number) const;
	SpriteManager & operator=(SpriteManager const & copy);
private:
	void CleanUp();
private:
	std::vector<CL_Sprite*> sprites;
};

#endif // HEADER_SPRITE_MANAGER_H
