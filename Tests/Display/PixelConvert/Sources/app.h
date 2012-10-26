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
*/

#pragma once

class App
{
public:
	int start(const std::vector<CL_String> &args);

private:
	void on_input_up(const CL_InputEvent &key);
	void on_window_close();
	void update_images();

private:
	bool quit;
	CL_TextureFormat current_format;
	CL_GraphicContext *display_gc;
	CL_Image tux_original;
	CL_Image tux_convert;
	CL_Image tux_unconvert;
	CL_PixelBuffer tux;
	bool gl_supported;
};
