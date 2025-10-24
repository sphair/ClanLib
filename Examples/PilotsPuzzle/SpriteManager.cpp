#include "SpriteManager.h"

#include <ClanLib/display.h>

//--------------------------------------------------------------------------
SpriteManager::SpriteManager(CL_ResourceManager & resource)
	: sprites(TOTAL_NUMBER_OF_SPRITES, 0)
{
	sprites[HANDLE_ON] = new CL_Sprite("handle_on", &resource);
	sprites[HANDLE_OFF] = new CL_Sprite("handle_off", &resource);
	sprites[HANDLE_ON_HINT] = new CL_Sprite("handle_on_hint", &resource);
	sprites[HANDLE_OFF_HINT] = new CL_Sprite("handle_off_hint", &resource);
	sprites[HANDLE_ON_HIGHLIGHT] = new CL_Sprite("handle_on_highlight", &resource);
	sprites[HANDLE_OFF_HIGHLIGHT] = new CL_Sprite("handle_off_highlight", &resource);
}

//--------------------------------------------------------------------------
SpriteManager::SpriteManager(SpriteManager const & copy)
	: sprites(copy.sprites.size(), 0)
{
	for(size_t i = 0; i < copy.sprites.size(); ++i)
	{
		if (copy.sprites[i])
			sprites[i] = new CL_Sprite(*copy.sprites[i]);
	}
}

//--------------------------------------------------------------------------
SpriteManager::~SpriteManager()
{
	CleanUp();
}

//--------------------------------------------------------------------------
CL_Size SpriteManager::HandleSize() const
{
	// FIXME: add bounds and Null checking
	return sprites[HANDLE_ON]->get_frame_size(0);
}

//--------------------------------------------------------------------------
CL_Sprite & SpriteManager::operator()(Sprites number) const
{
	// FIXME: add bounds and Null checking
	return *sprites[number];
}

//--------------------------------------------------------------------------
void SpriteManager::CleanUp()
{
	for(size_t i = 0; i < sprites.size(); ++i)
	{
		delete sprites[i];
		sprites[i] = 0;
	}
}

//--------------------------------------------------------------------------
SpriteManager & SpriteManager::operator=(SpriteManager const & copy)
{
	if (this == &copy)
		return *this;

	CleanUp();

	sprites.resize(copy.sprites.size(), 0);
	for(size_t i = 0; i < copy.sprites.size(); ++i)
	{
		if (copy.sprites[i])
			sprites[i] = new CL_Sprite(*copy.sprites[i]);
	}

	return *this;
}
