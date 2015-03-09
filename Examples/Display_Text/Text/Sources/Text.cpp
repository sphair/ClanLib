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
"Copyright (c) 1997-2015 The ClanLib Team",
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

clan::ApplicationInstance<ExampleText> clanapp;

ExampleText::ExampleText()
{
	// We support all display targets, in order listed here
#ifdef WIN32
	clan::D3DTarget::enable();
#endif
	clan::OpenGLTarget::enable();

int ExampleText::start(const std::vector<std::string> &args)
{ 
	quit = false;

	// Set a videomode
	clan::DisplayWindowDescription desc;
	desc.set_allow_resize(false);
	desc.set_title("ClanLib SpanLayout Example");
	desc.set_size(clan::Size(800, 600), true);
	clan::DisplayWindow window(desc);
    clan::SlotContainer cc;

	// Connect the Window close event
	cc.connect(window.sig_window_close(), clan::bind_member(this, &ExampleText::on_window_close));

	// Connect a keyboard handler to on_key_up()
	cc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &ExampleText::on_keyboard_up));

	clan::Canvas canvas(window);

	clan::FontDescription font_desc;
	font_desc.set_height(32);
	clan::Font font_normal("Tahoma", font_desc);

	font_desc.set_weight(clan::FontWeight::bold);
	font_desc.set_height(40);
	clan::Font font_bold("Tahoma", font_desc);

	clan::Texture2D texture_text(canvas, text_window_size, text_window_size);
	texture_text.set_pixel_ratio(1.0f);
	texture_text.set_wrap_mode(clan::wrap_repeat, clan::wrap_repeat);
	texture_text.set_min_filter(clan::filter_linear);
	texture_text.set_mag_filter(clan::filter_linear);

	// Create the framebuffer, and attach the texture text into its color buffer
	clan::FrameBuffer fb_text;
	fb_text = clan::FrameBuffer(canvas);
	fb_text.attach_color(0, texture_text);

	clan::Canvas canvas_fb(canvas, fb_text);

	float angle = 0.0f;

	std::vector<clan::SpanLayout> layout;

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
		layout[line_count] = clan::SpanLayout();

		layout[line_count].set_position(clan::Point(20, ypos));

		layout[line_count].set_align(clan::span_justify);

		const char *text_ptr = TextToShow[line_count];
		if (text_ptr[0] == '-')
		{
			layout[line_count].add_text(text_ptr, font_normal, clan::Colorf::red);
		}
		else if (strstr(text_ptr, "Clan"))
		{
			layout[line_count].add_text(text_ptr, font_bold, clan::Colorf::yellow);
		}
		else
		{
			layout[line_count].add_text(text_ptr, font_normal, clan::Colorf::yellow);
		}

		layout[line_count].layout(canvas_fb, texture_text.get_width() - 60);

		ypos += layout[line_count].get_size().height;

	}
	game_time.reset();
}

bool ExampleText::update()
{
	game_time.update();

	canvas.set_map_mode(clan::map_2d_upper_left);

	// Draw a nice blue gradient in the background
	canvas.fill_rect(window.get_viewport(), clan::Gradient(clan::Colorf::lightblue, clan::Colorf::lightblue, clan::Colorf::darkblue, clan::Colorf::darkblue));

	// Draw the text into the frame buffer
	update_text(canvas_fb, fb_text, font_normal, layout);

	angle += 0.5f;
	if (angle >= 360.0f)
		angle -= 360.0f;

	// Draw the text
	draw_text(canvas, texture_text, clan::Angle(angle, clan::angle_degrees));

	last_fps = game_time.get_updates_per_second();
	// Flip the display, showing on the screen what we have drawn
	window.flip(1);

	return !quit;
}

void ExampleText::draw_text(clan::Canvas &canvas, clan::Texture2D &texture, clan::Angle angle)
{
	canvas.set_transform(clan::Mat4f::translate(canvas.get_width() / 2.0f, canvas.get_height() / 2.0f, 0.0f) * clan::Mat4f::rotate(angle, 0.0f, 0.0f, 1.0f));

	clan::Image image(texture, texture.get_size());
	image.set_alpha(0.7f);
	image.draw(canvas, clan::Rectf(-300.0f, -300.0f, 300.0f, 300.0f));

	canvas.set_transform(clan::Mat4f::identity());
}

void ExampleText::update_text(clan::Canvas &canvas_fb, clan::FrameBuffer &fb_text, clan::Font &font, std::vector<clan::SpanLayout> &layout)
{
	canvas_fb.fill_rect( 0.0f, 0.0f, (float)text_window_size, (float)text_window_size, clan::Colorf(0.0f, 0.0f, 0.0f, 1.0f));

	std::string text(clan::string_format("Frames per second = %1", last_fps));
	font.draw_text(canvas_fb, 20, 20, text, clan::Colorf::white);

	for (unsigned int line_count = 0; line_count < layout.size(); line_count++)
	{
		layout[line_count].draw_layout(canvas_fb);
	}

}

void ExampleText::on_keyboard_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
		quit = true;
}
	
void ExampleText::on_window_close()
{
	quit = true;
}

