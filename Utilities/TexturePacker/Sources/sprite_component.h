/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

class SpriteComponent : public GUIComponent
{
public:
	SpriteComponent(GUIComponent *parent);
	~SpriteComponent() {};

// Attributes:
public:

// Operations:
public:
	void clear_sprite();

	void set_sprite(Sprite *sprite);
	void set_sprite_description(SpriteDescription *sprite_description);

// Implementation:
private:
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_resized();
	void on_button_startstop_clicked();

	void update_buttons_enabled_state();

	void on_button_prevframe_clicked();
	void on_button_nextframe_clicked();

	PushButton *button_startstop;
	PushButton *button_nextframe;
	PushButton *button_prevframe;
	Label *label_frame;

	Sprite *sprite;
	SpriteDescription *sprite_description;

	bool is_playing;

	int sprite_current_frame;
};
