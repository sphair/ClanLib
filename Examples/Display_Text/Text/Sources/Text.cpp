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
**    Mark Page
*/

#include "precomp.h"
#include "Text.h"

static const char *TextToShow[] = {
"(All text is redrawn each frame)",
"",
"This is the ClanLib Licence:",
"",
"-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-",
"ClanLib SDK",
"Copyright (c) 1997-2010 The ClanLib Team",
"",
"This software is provided 'as-is', without any express or implied "
"warranty.  In no event will the authors be held liable for any damages "
"arising from the use of this software."
"",
"Permission is granted to anyone to use this software for any purpose, "
"including commercial applications, and to alter it and redistribute it "
"freely, subject to the following restrictions:",
"",
"1. The origin of this software must not be misrepresented; you must not "
"   claim that you wrote the original software. If you use this software "
"   in a product, an acknowledgment in the product documentation would be "
"   appreciated but is not required.",
"2. Altered source versions must be plainly marked as such, and must not be ",
"   misrepresented as being the original software.",
"3. This notice may not be removed or altered from any source distribution.",
"Note: Some of the libraries ClanLib may link to may have additional "
"requirements or restrictions.",
"",
"-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-",
NULL
};

int ExampleText::start(const std::vector<CL_String> &args)
{ 
	// Create a console window for text-output if not available
	CL_ConsoleWindow console("Console");
	
	quit = false;

	// Set a videomode
	CL_DisplayWindowDescription desc;
	desc.set_allow_resize(false);
	desc.set_title("ClanLib CL_SpanLayout Example");
	desc.set_size(CL_Size(800, 600), true);
	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &ExampleText::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_keyboard_up = window.get_ic().get_keyboard().sig_key_up().connect(this, &ExampleText::on_keyboard_up);

	CL_GraphicContext gc = window.get_gc();

	CL_TextureGroup texture_group(CL_Size(512, 512));

	CL_FontDescription font_desc;
	font_desc.set_typeface_name("Tahoma");
	font_desc.set_anti_alias(true);
	font_desc.set_height(32);
	CL_Font_System font_normal(gc, font_desc);

	font_desc.set_weight(800);
	font_desc.set_height(40);
	CL_Font_System font_bold(gc, font_desc);

	// Share the font texture
	font_normal.set_texture_group(texture_group);
	font_bold.set_texture_group(texture_group);

	CL_Texture texture_text(gc, text_window_size, text_window_size);
	texture_text.set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat, cl_wrap_repeat);
	texture_text.set_min_filter(cl_filter_linear);
	texture_text.set_mag_filter(cl_filter_linear);

	// Create the framebuffer, and attach the texture text into its color buffer
	CL_FrameBuffer fb_text;
	fb_text = CL_FrameBuffer(gc);
	fb_text.attach_color_buffer(0, texture_text);

	float angle = 0.0f;

	std::vector<CL_SpanLayout> layout;

	// Count number of lines
	int num_lines = 0;
	for (const char **text_ptr = TextToShow; *text_ptr; text_ptr++)
	{
		num_lines++;
	}

	// Extend layout vector
	layout.resize(num_lines);

	int ypos = 60;
	for (int line_count=0; line_count < num_lines; line_count++, ypos += 8)
	{
		layout[line_count] = CL_SpanLayout();

		layout[line_count].set_position(CL_Point(20, ypos));

		layout[line_count].set_align(cl_justify);

		const char *text_ptr = TextToShow[line_count];
		if (text_ptr[0] == '-')
		{
			layout[line_count].add_text(text_ptr, font_normal, CL_Colorf::red);
		}
		else if (strstr(text_ptr, "Clan"))
		{
			layout[line_count].add_text(text_ptr, font_bold, CL_Colorf::yellow);
		}
		else
		{
			layout[line_count].add_text(text_ptr, font_normal, CL_Colorf::yellow);
		}

		layout[line_count].layout(gc, texture_text.get_width() - 60);

		ypos += layout[line_count].get_size().height;

	}

	// Run until someone presses escape
	while (!quit)
	{
		int start_time = CL_System::get_time();

		gc.set_map_mode(CL_MapMode(cl_map_2d_upper_left));

		// Draw a nice blue gradient in the background
		CL_Draw::gradient_fill(gc, window.get_viewport(), CL_Gradient(CL_Colorf::lightblue, CL_Colorf::lightblue, CL_Colorf::darkblue, CL_Colorf::darkblue));

		// Draw the text into the frame buffer
		update_text(gc, fb_text, font_normal, layout);

		angle += 0.5f;
		if (angle >= 360.0f)
			angle -= 360.0f;

		// Draw the text
		draw_text(gc, texture_text, CL_Angle(angle, cl_degrees));

		last_fps = 1000.0f / (CL_System::get_time()-start_time);
		// Flip the display, showing on the screen what we have drawn
		window.flip(1);

		// This call updates input and performs other "housekeeping" call this each frame
		CL_KeepAlive::process();
	}

	return 0;
}

void ExampleText::draw_text(CL_GraphicContext &gc, CL_Texture &texture, CL_Angle angle)
{
	gc.set_texture(0, texture);

	gc.push_modelview();

	gc.mult_translate(gc.get_width()/2.0f, gc.get_height()/2.0f);
	gc.mult_rotate(angle, 0.0f, 0.0f, 1.0f);

	CL_Draw::texture(gc, CL_Rectf(-300.0f, -300.0f, 300.0f, 300.0f), CL_Colorf(1.0f, 1.0f, 1.0f, 0.7f));

	gc.pop_modelview();

	gc.reset_texture(0);
}

void ExampleText::update_text(CL_GraphicContext &gc, CL_FrameBuffer &fb_text, CL_Font &font, std::vector<CL_SpanLayout> &layout)
{
	gc.set_frame_buffer(fb_text);

	CL_Draw::fill(gc, 0.0f, 0.0f, (float)text_window_size, (float)text_window_size, CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f));

	CL_String text(cl_format("Frames per second = %1", last_fps));
	font.draw_text(gc, 20, 20, text, CL_Colorf::white);

	for (unsigned int line_count = 0; line_count < layout.size(); line_count++)
	{
		layout[line_count].draw_layout(gc);
	}

	gc.reset_frame_buffer();
}

void ExampleText::on_keyboard_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
		quit = true;
}
	
void ExampleText::on_window_close()
{
	quit = true;
}

