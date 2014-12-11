/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "sprite_component.h"

SpriteComponent::SpriteComponent(clan::GUIComponent *parent)
: clan::GUIComponent(parent, "spritecomponent"),
  sprite(0), image(0)
{
	button_startstop = new clan::PushButton(this);
	button_startstop->set_text("Start");
	button_startstop->func_clicked().set(this, &SpriteComponent::on_button_startstop_clicked);
	button_prevframe = new clan::PushButton(this);
	button_prevframe->set_text("<");
	button_prevframe->func_clicked().set(this, &SpriteComponent::on_button_prevframe_clicked);
	button_nextframe = new clan::PushButton(this);
	button_nextframe->set_text(">");
	button_nextframe->func_clicked().set(this, &SpriteComponent::on_button_nextframe_clicked);
	label_frame = new clan::Label(this);

	update_buttons_enabled_state();

	func_render().set(this, &SpriteComponent::on_render);
	func_resized().set(this, &SpriteComponent::on_resized);
}

void SpriteComponent::on_render(clan::Canvas &canvas, const clan::Rect &update_rect)
{
	push_cliprect(canvas, get_size());

	canvas.fill_rect( get_size(), clan::Colorf::cadetblue);

	if(sprite)
	{
		if(is_playing)
		{
			game_time.update();
			sprite->update(game_time.get_time_elapsed_ms());
		}

		sprite->draw(canvas, 0, 0);

		if(sprite->get_current_frame() != sprite_current_frame)
		{
			label_frame->set_text(clan::string_format("Frame %1 / %2", sprite->get_current_frame() + 1, sprite->get_frame_count()));
			sprite_current_frame = sprite->get_current_frame();
		}
	}

	if(image)
	{
		image->draw(canvas, 0, 0);
	}
	pop_cliprect(canvas);
}

void SpriteComponent::on_resized()
{
	int width = get_width();

	button_startstop->set_geometry(clan::Rect(clan::Point(width - 200,0), clan::Size(50,20)));
	button_prevframe->set_geometry(clan::Rect(clan::Point(width - 140,0), clan::Size(20,20)));
	button_nextframe->set_geometry(clan::Rect(clan::Point(width - 120,0), clan::Size(20,20)));
	label_frame->set_geometry(clan::Rect(clan::Point(width - 80,0), clan::Size(80,20)));
}

void SpriteComponent::on_button_startstop_clicked()
{
	is_playing = !is_playing;

	update_buttons_enabled_state();
}

void SpriteComponent::on_button_prevframe_clicked()
{
	sprite->set_frame(sprite->get_current_frame() - 1);
}

void SpriteComponent::on_button_nextframe_clicked()
{
	sprite->set_frame(sprite->get_current_frame() + 1);
}

void SpriteComponent::clear_sprite()
{
	sprite = 0;
	image = 0;

	this->set_constant_repaint(false);

	is_playing = false;
	sprite_current_frame = -1;

	update_buttons_enabled_state();
}

void SpriteComponent::set_image(clan::Image *image)
{
	this->image = image;

	is_playing = true;
	sprite_current_frame = -1;

	update_buttons_enabled_state();

	request_repaint();
}

void SpriteComponent::set_sprite(clan::Sprite *sprite)
{
	this->sprite = sprite;
	this->set_constant_repaint(true);

	is_playing = true;
	sprite_current_frame = -1;

	update_buttons_enabled_state();
}

void SpriteComponent::update_buttons_enabled_state()
{
	if(sprite)
	{
		button_prevframe->set_enabled(!is_playing);
		button_nextframe->set_enabled(!is_playing);

		button_startstop->set_enabled();

		if(is_playing)
			button_startstop->set_text("Stop");
		else
			button_startstop->set_text("Start");
	}
	else
	{
		button_prevframe->set_enabled(false);
		button_nextframe->set_enabled(false);
		button_startstop->set_enabled(false);
		label_frame->set_text("");
	}
}
