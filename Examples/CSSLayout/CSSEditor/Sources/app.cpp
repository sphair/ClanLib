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
	win_desc.set_size(clan::Size( 1000, 800 ), false);

    clan::SlotContainer cc;
	clan::DisplayWindow window(win_desc);
	cc.connect(window.sig_window_close(), std::function<void()>(this, &App::on_window_close));
	cc.connect(window.get_ic().get_keyboard().sig_key_up(), std::function<void(const clan::InputEvent&)>(this, &App::on_input_up));

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
	clan::Rect css_window_rect(460, css_window_border_size, clan::Size(0,0));

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
			"{\n";

		if (options->value_margin.css_enabled)
			created_css+= clan::string_format("margin: %1%2;\n", options->value_margin.css_value, options->value_margin.css_type_length);
		if (options->value_padding.css_enabled)
			created_css+= clan::string_format("padding: %1%2;\n", options->value_padding.css_value, options->value_padding.css_type_length);
		if (options->list_border_style.css_enabled)
			created_css+= clan::string_format("border-style: %1;\n", options->list_border_style.css_item);
		if (options->value_border_width.css_enabled)
			created_css+= clan::string_format("border-width: %1%2;\n", options->value_border_width.css_value, options->value_border_width.css_type_length);
		if (options->value_width.css_enabled)
			created_css+= clan::string_format("width: %1%2;\n", options->value_width.css_value, options->value_width.css_type_length);
		if (options->value_height.css_enabled)
			created_css+= clan::string_format("height: %1%2;\n", options->value_height.css_value, options->value_height.css_type_length);
		if (options->rgb_background.css_enabled)
			created_css+= clan::string_format("background:rgb(%1,%2,%3);\n", options->rgb_background.css_red, options->rgb_background.css_green, options->rgb_background.css_blue);
		if (options->rgb_border_color.css_enabled)
			created_css+= clan::string_format("border-color:rgb(%1,%2,%3);\n", options->rgb_border_color.css_red, options->rgb_border_color.css_green, options->rgb_border_color.css_blue);
		if (options->rgb_color.css_enabled)
			created_css+= clan::string_format("color:rgb(%1,%2,%3);\n", options->rgb_color.css_red, options->rgb_color.css_green, options->rgb_color.css_blue);
		if (options->list_background_image.css_enabled)
			created_css+= clan::string_format("background-image:url('%1');\n", options->list_background_image.css_item);

		created_css+= 
		 "font: 13px \"Segoe UI\"; "
			"}\n"
			;
		clan::DataBuffer created_css_buffer(created_css.c_str(), created_css.size());
		clan::IODevice_Memory iodevice_memory(created_css_buffer);
		clan::CSSDocument css;
		css.add_default_html_sheet();
		css.add_sheet(clan::author_sheet_origin, iodevice_memory, "");

		clan::CSSLayout layout;
		layout.func_get_image() = bind_member(this, &App::get_image);
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

		if (options->export_selected)
		{
			options->export_selected = false;
			clan::File file("test.html", clan::File::create_always, clan::File::access_write);
			file.write_string_text("<!DOCTYPE html>\n<html>\n<head>\n<style>\n");
			file.write_string_text(created_css);
			file.write_string_text("</style>\n</head>\n<body>\n<div class=\"custom\">Hello World</div>\n</body>\n</html>\n");
			file.close();
#ifdef WIN32
			ShellExecute(window.get_hwnd(), "open", "test.html", NULL, NULL, SW_SHOWNORMAL);
#endif
		}

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

clan::Image App::get_image(clan::Canvas &canvas, const std::string &url)
{
	return clan::Image(canvas, url);
}
