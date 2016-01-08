/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Mark Page
*/

#pragma once

class ExampleCanvas : public clan::Application
{
public:
	ExampleCanvas();
	bool update() override;

private:
	void on_keyboard_up(const InputEvent &key);
	void on_mouse_move(const InputEvent &key);
	void on_mouse_down(const InputEvent &key);
	void on_window_close();

	DisplayWindow window;
	clan::SlotContainer sc;

	Image image_ground;
	Image image_ground_back;
	Image image_cutter;

	FrameBuffer framebuffer_ground;

	Canvas canvas_window;
	Canvas canvas_framebuffer;

	Pointf mouse_pos;

	float scale = 1.0f;
	float back_pos = 0;

	bool mouse_down;

	bool quit = false;
};

