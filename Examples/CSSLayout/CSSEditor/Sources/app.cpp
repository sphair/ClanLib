/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "app.h"
#include "options.h"
#include <cstdlib>

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("CSS Editor");
	win_desc.set_size(clan::Size( 800, 520 ), false);

	clan::DisplayWindow window(win_desc);
	clan::Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	clan::Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	std::string theme;
	if (clan::FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (clan::FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw clan::Exception("No themes found");

	clan::Canvas canvas(window);

	clan::GUIWindowManagerTexture wm(window);
	clan::GUIManager gui(wm, theme);

	const int css_window_border_size = 10;
	clan::Rect css_window_rect(400, css_window_border_size, clan::Size(0,0));

	// Deleted automatically by the GUI
	Options *options = new Options(gui, clan::Rect( css_window_border_size, css_window_border_size, clan::Size(css_window_rect.left - css_window_border_size*2, 400)));

	clan::Image image_grid(canvas, "../../Display_Render/Blend/Resources/grid.png");
	image_grid.set_color(clan::Colorf(0.4f, 0.4f, 1.0f, 1.0f));


	while (!quit)
	{
		canvas.clear(clan::Colorf::blue);
		wm.process();
		wm.draw_windows(canvas);

		clan::Size css_window_size( clan::max(canvas.get_width() - css_window_rect.left - css_window_border_size, 0),  clan::max(canvas.get_height() - css_window_border_size*2, 0) );
		if (css_window_size != css_window_rect.get_size())
		{
			css_window_rect.right = css_window_rect.left + css_window_size.width;
			css_window_rect.bottom = css_window_rect.top + css_window_size.height;
		}

		std::string created_css =
			".custom\n"
			"{\n"
			"	background: #ff0000;\n"
			"margin: 1em 0; font: 13px \"Segoe UI\"; "
			"}\n"
			"body { background: white; }"

			//"custom\n"
			//"{\n"
			//"	border-color: #ffffff;\n"
			//"   	width: 300px;\n"
			//"	height: 100px;\n"
			//"	background-color: #ff0000;\n"
			//"	text-align: center;\n"
			//"}\n"
			;
		clan::DataBuffer created_css_buffer(created_css.c_str(), created_css.size());
		clan::IODevice_Memory iodevice_memory(created_css_buffer);
		clan::CSSDocument css;
		css.add_default_html_sheet();
		css.add_sheet(clan::author_sheet_origin, iodevice_memory, "");

		clan::CSSLayout layout;

		clan::CSSLayoutElement element_html = layout.create_element("html");
		clan::CSSLayoutElement element_body = element_html.create_element("body");
		clan::CSSLayoutElement element_div = element_body.create_element("div");
		element_div.set_attribute("class", "custom");
		element_div.create_text("Hello World");
		layout.set_document_element(element_html);
		layout.set_css_document(css);
		layout.layout(canvas, css_window_rect);
		image_grid.draw(canvas, css_window_rect);
		layout.render(canvas);


		window.flip(1);

		clan::KeepAlive::process();
	}
	return 0;
}

// A key was pressed
void App::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}

}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

