/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "precomp.h"
#include "sprite_component.h"

SpriteComponent::SpriteComponent(CL_GUIComponent *parent)
: CL_GUIComponent(parent),
  sprite_description(0),
  sprite(0)
{
	set_type_name("spritecomponent");

	func_render().set(this, &SpriteComponent::on_render);
	func_style_changed().set(this, &SpriteComponent::on_style_changed);
	func_resized().set(this, &SpriteComponent::on_resized);
}

void SpriteComponent::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	if(sprite)
	{
		sprite->update();
		sprite->draw(gc, 0, 0);
	}

/*	if(sprite_description)
	{
		const std::vector<CL_SpriteDescriptionFrame> &frames = sprite_description->get_frames();
		std::vector<CL_SpriteDescriptionFrame>::const_iterator it;
		for(it = frames.begin(); it != frames.end(); ++it)
		{
			CL_SpriteDescriptionFrame frame = (*it);

			gc.set_texture(0, frame.texture);
			CL_Draw::texture(gc, update_rect);
			gc.reset_texture(0);

			break;
		}
	}
*/}

void SpriteComponent::on_style_changed()
{
}

void SpriteComponent::on_resized()
{
}

void SpriteComponent::set_sprite_description(CL_SpriteDescription *sprite_description)
{
	this->sprite_description = sprite_description;
}

void SpriteComponent::set_sprite(CL_Sprite *sprite)
{
	this->sprite = sprite;
	this->set_constant_repaint(true);
}
