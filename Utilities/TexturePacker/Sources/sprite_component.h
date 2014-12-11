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

#pragma once

class SpriteComponent : public clan::GUIComponent
{
public:
	SpriteComponent(clan::GUIComponent *parent);
	~SpriteComponent() {};

// Attributes:
public:

// Operations:
public:
	void clear_sprite();

	void set_sprite(clan::Sprite *sprite);
	void set_image(clan::Image *image);

// Implementation:
private:
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	void on_resized();
	void on_button_startstop_clicked();

	void update_buttons_enabled_state();

	void on_button_prevframe_clicked();
	void on_button_nextframe_clicked();

	clan::PushButton *button_startstop;
	clan::PushButton *button_nextframe;
	clan::PushButton *button_prevframe;
	clan::Label *label_frame;

	clan::Sprite *sprite;
	clan::Image *image;

	bool is_playing;

	int sprite_current_frame;
	clan::GameTime game_time;
};
