/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

#include <cmath>

// This is the Application class (That is instantiated by the Program Class)
class App : public clan::Application
{
public:
	App();
	bool update() override;

private:
	void on_input_up(const clan::InputEvent &key);
	void show(int &inout_ypos, const std::string &prefix, const std::string &text);
	void show(int &inout_ypos, const std::string &prefix, const Sizef &size);
	void show(int &inout_ypos, const std::string &prefix, const Rectf &rect);
	void show(int &inout_ypos, const std::string &prefix, bool value);
	void show(int &inout_ypos, const std::string &prefix, int value);
	void show(int &inout_ypos, const std::string &prefix, const DisplayWindowHandle &handle);
	void show(int &inout_ypos, const std::string &prefix, const DisplayWindow &window);
		
	void build_test_window();
private:
	bool quit = false;
	clan::SlotContainer sc;
	clan::SlotContainer test_sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::Font font;
	clan::DisplayWindowDescription test_desc;
	clan::DisplayWindow test_window;
	clan::Canvas test_canvas;
	const int font_increment = 20;
};

clan::ApplicationInstance<App> clanapp;

App::App()
{
	clan::OpenGLTarget::set_current();

	quit = false;

	clan::DisplayWindowDescription desc;
	desc.set_title("Main Window");
	desc.set_size(clan::Sizef(400.0f, 800.0f), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);

	test_desc.set_title("Test");
	test_desc.set_position(clan::Rectf(32.0f, 32.0f, clan::Sizef(256.0f, 256.0f)), true);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), [&]() {quit = true; });

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_down(), clan::bind_member(this, &App::on_input_up));

	font = clan::Font("tahoma", 18);

	build_test_window();

}
bool App::update()
{
	canvas.clear(clan::Colorf(0.0f, 0.0f, 0.2f));

	font.draw_text(canvas, 0, 40, "Use keyboard to modify window", clan::Colorf::white);

	//[15:26] <Judas-> 1. popup window line states that this is a 'popup' window as seen by the OS.This basically means it shouldn't appear in any taskbars and so on. Exactly what it means to be 'popup' is probably the highest guarded secret of the Win32 API :)
	//[15:26] <Judas-> 2. the no activate line states that clicking on the window shouldn't move the focus away from the parent. This is needed to avoid having the titlebar switch to inactive.
	//[15:28] <Judas->forgot to write about #1 that currently this also means it shouldn't decorate the window
	//[15:29] <Judas->ideally there would be a separate show_window_frame function for this

	int ypos = 100;
	show(ypos, "0) is_fullscreen()", test_desc.is_fullscreen());
	show(ypos, "1) get_allow_resize()", test_desc.get_allow_resize());
	show(ypos, "2) is_layered()", test_desc.is_layered());
	show(ypos, "3) has_caption()", test_desc.has_caption());
	show(ypos, "4) has_sysmenu()", test_desc.has_sysmenu());
	show(ypos, "5) has_minimize_button()", test_desc.has_minimize_button());
	show(ypos, "6) has_maximize_button()", test_desc.has_maximize_button());
	show(ypos, "7) is_visible()", test_desc.is_visible());
	show(ypos, "8) is_topmost()", test_desc.is_topmost());
	show(ypos, "9) has_no_activate()", test_desc.has_no_activate());
	show(ypos, "a) has_drop_shadow()", test_desc.has_drop_shadow());
	show(ypos, "c) get_allow_screensaver()", test_desc.get_allow_screensaver());
	show(ypos, "d) is_update_supported()", test_desc.is_update_supported());
	show(ypos, "e) is_main()", test_desc.is_main());
	show(ypos, "f) is_dialog()", test_desc.is_dialog());
	show(ypos, "g) is_popup()", test_desc.is_popup());
	show(ypos, "h) get_owner()", test_desc.get_owner());
	show(ypos, "*) get_title()", test_desc.get_title());
	show(ypos, "*) get_size()", test_desc.get_size());
	show(ypos, "*) get_position()", test_desc.get_position());
	show(ypos, "*) get_position_client_area()", test_desc.get_position_client_area());
	show(ypos, "*) get_flipping_buffers()", test_desc.get_flipping_buffers());
	show(ypos, "*) get_swap_interval()", test_desc.get_swap_interval());
	show(ypos, "*) get_bpp()", test_desc.get_bpp());
	show(ypos, "*) get_refresh_rate()", test_desc.get_refresh_rate());
	show(ypos, "*) get_handle()", test_desc.get_handle());
	show(ypos, "*) get_depth_size()", test_desc.get_depth_size());
	show(ypos, "*) get_stencil_size()", test_desc.get_stencil_size());
	show(ypos, "*) get_fullscreen_monitor()", test_desc.get_fullscreen_monitor());
	show(ypos, "*) get_multisampling()", test_desc.get_multisampling());

	test_canvas.clear(clan::Colorf(0.0f, 0.0f, 0.2f));
	font.draw_text(test_canvas, 0, 40, string_format("%1,%2", test_canvas.get_width(), test_canvas.get_height()), clan::Colorf::white);
	test_window.flip(0);

	window.flip(1);

	return !quit;
}

void App::build_test_window()
{
	test_sc = clan::SlotContainer();
	test_window = clan::DisplayWindow(test_desc);
	test_canvas = clan::Canvas(test_window);
	test_sc.connect(test_window.sig_window_close(), [&]() {quit = true; });
	test_sc.connect(test_window.get_keyboard().sig_key_down(), clan::bind_member(this, &App::on_input_up));
}

void App::on_input_up(const clan::InputEvent &key)
{
	if (key.id == clan::keycode_0) test_desc.set_fullscreen(!test_desc.is_fullscreen());
	if (key.id == clan::keycode_1) test_desc.set_allow_resize(!test_desc.get_allow_resize());
	if (key.id == clan::keycode_2) test_desc.set_layered(!test_desc.is_layered());
	if (key.id == clan::keycode_3) test_desc.show_caption(!test_desc.has_caption());
	if (key.id == clan::keycode_4) test_desc.show_sysmenu(!test_desc.has_sysmenu());
	if (key.id == clan::keycode_5) test_desc.show_minimize_button(!test_desc.has_minimize_button());
	if (key.id == clan::keycode_6) test_desc.show_maximize_button(!test_desc.has_maximize_button());
	if (key.id == clan::keycode_7) test_desc.set_visible(!test_desc.is_visible());
	if (key.id == clan::keycode_8) test_desc.set_topmost(!test_desc.is_topmost());
	if (key.id == clan::keycode_9) test_desc.set_no_activate(!test_desc.has_no_activate());
	if (key.id == clan::keycode_a) test_desc.set_drop_shadow(!test_desc.has_drop_shadow());
	if (key.id == clan::keycode_c) test_desc.set_allow_screensaver(!test_desc.get_allow_screensaver());
	if (key.id == clan::keycode_d) test_desc.set_update_supported(!test_desc.is_update_supported());
	if (key.id == clan::keycode_e) test_desc.set_main_window();
	if (key.id == clan::keycode_f) test_desc.set_dialog_window();
	if (key.id == clan::keycode_g) test_desc.set_popup_window();
	if (key.id == clan::keycode_h) test_desc.set_owner_window(test_desc.get_owner().is_null() ? window : DisplayWindow());

	build_test_window();

	if (key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

void App::show(int &inout_ypos, const std::string &prefix, const std::string &text)
{
	font.draw_text(canvas, 0, inout_ypos, string_format("%1 = %2", prefix, text));
	inout_ypos += font_increment;
}

void App::show(int &inout_ypos, const std::string &prefix, const Sizef &size)
{
	font.draw_text(canvas, 0, inout_ypos, string_format("%1 = %2,%3", prefix, size.width, size.height));
	inout_ypos += font_increment;
}

void App::show(int &inout_ypos, const std::string &prefix, const Rectf &size)
{
	font.draw_text(canvas, 0, inout_ypos, string_format("%1 = %2,%3,%4,%5", prefix, size.left, size.top, size.right, size.bottom));
	inout_ypos += font_increment;
}

void App::show(int &inout_ypos, const std::string &prefix, bool value)
{
	font.draw_text(canvas, 0, inout_ypos, string_format("%1 = %2", prefix, value? "true" : "false"));
	inout_ypos += font_increment;
}

void App::show(int &inout_ypos, const std::string &prefix, int value)
{
	font.draw_text(canvas, 0, inout_ypos, string_format("%1 = %2", prefix, value));
	inout_ypos += font_increment;
}

void App::show(int &inout_ypos, const std::string &prefix, const DisplayWindowHandle &handle)
{
#ifdef WIN32
	font.draw_text(canvas, 0, inout_ypos, string_format("%1 = %2", prefix, (int) handle.hwnd));
#else
	font.draw_text(canvas, 0, inout_ypos, string_format("%1 = %2", prefix, (int) handle.window));
#endif
	inout_ypos += font_increment;
}
void App::show(int &inout_ypos, const std::string &prefix, const DisplayWindow &window)
{
	font.draw_text(canvas, 0, inout_ypos, string_format("%1 = %2", prefix, window.is_null() ? "null" : "valid"));
	inout_ypos += font_increment;
}
