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
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

// This is the Application class (That is instantiated by the Program Class)
class Alpha
{
public:
	int start(const std::vector<std::string> &args);

private:
	void on_input_up(const InputEvent &key);
	void on_window_close();
	Image create_block(Canvas &canvas, const Colorf &colour);
	void draw_section(Canvas &canvas, Font &font, int yoffset, const Colorf &background, const Colorf &vertex_colour, const Colorf &image_colour);
private:
	bool quit;
	BlendState blend_disabled;
	BlendState blend_enabled;
};
