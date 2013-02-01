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
*/

#pragma once

#define	LIGHTS_COLOR		true		// Add color to our lights?
#define USE_SCALE			true		// Set to false to see the difference with fullscreen vs. scaled mode.
#define CANVAS_SCALE_X		4.0f		// This would be scaled by aspect ratio.  For example 4:3
#define CANVAS_SCALE_Y		3.0f

#include "LightContainer.h"

class ExampleCanvas
{
public:
	int start(const std::vector<std::string> &args);

private:
	void on_mouse_move(const clan::InputEvent &key);
	void on_keyboard_up(const clan::InputEvent &key);
	void on_mouse_up(const clan::InputEvent &key);
	void on_window_close();
	int get_random(int low, int high);

	LightContainer		lights;				// Our container for lights.
	Light				*mouse_light;		// The light that follows our mouse around.
	clan::Image			background;			// This is a simple image, so we use the simple image class.
	clan::Texture2D			light_mask;			// We require more low-level functions for our lights, so this should be a texture.
	clan::FrameBuffer		fb_lightmask;		// The FrameBuffer.
	clan::Canvas	canvas;					// Graphic Context

	bool				quit;				
};
