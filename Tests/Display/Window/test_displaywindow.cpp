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
**    (if your name is missing here, please add it)
*/

#include "test.h"

void TestApp::test_display_window(void)
{
	Console::write_line(" Header: display_window.h");
	Console::write_line("  Class: DisplayWindow");

	DisplayWindowDescription desc_1;
	DisplayWindowDescription desc_2;

	int window_1_xpos = 73;
	int window_1_ypos = 63;
	int window_1_width = 223;
	int window_1_height = 91;
	int window_2_xpos = 55;
	int window_2_ypos = 573;
	int window_2_width = 345;
	int window_2_height = 329;

	Rect border_size = get_window_frame_size();

	desc_1.show_caption(true);
	desc_1.set_visible(true);
	desc_1.set_topmost(false);
	desc_1.set_drop_shadow(false);
	desc_1.set_title("Window 1");
	Rect window_1_rect = Rect(window_1_xpos, window_1_ypos, window_1_width + window_1_xpos, window_1_height + window_1_ypos);
	desc_1.set_position(window_1_rect, false);
	Rect window_1_viewport_rect = Rect(0, 0, window_1_width - border_size.right + border_size.left, window_1_height - border_size.bottom + border_size.top);
	desc_2.show_caption(false);
	desc_2.set_visible(true);
	desc_2.set_topmost(false);
	desc_2.set_drop_shadow(false);
	desc_2.set_title("Window 2");
	Rect window_2_rect = Rect(window_2_xpos, window_2_ypos, window_2_width + window_2_xpos, window_2_height + window_2_ypos);
	desc_2.set_position(window_2_rect, false);
	Rect window_2_viewport_rect = Rect(0, 0, window_2_width, window_2_height);

	Console::write_line(" [Testing using a normal window and a window without a caption]");
	Console::write_line("   Function: DisplayWindow(DisplayWindowDescription)");
	DisplayWindow window_1(desc_1);
	DisplayWindow window_2(desc_2);

	Console::write_line("   Function: Rect get_geometry()");

	Rect geometry = window_1.get_geometry();
	if (geometry != window_1_rect) fail();

	geometry = window_2.get_geometry();
	if (geometry != window_2_rect) fail();

	Console::write_line("   Function: Rect get_viewport()");

	Rect viewport = window_1.get_viewport();
	if (viewport != window_1_viewport_rect) fail();

	viewport = window_2.get_viewport();
	if (viewport != window_2_viewport_rect) fail();

	Console::write_line("   Function: bool has_focus()");
	if (window_1.has_focus()) fail();
	if (!window_2.has_focus()) fail();

	Console::write_line("   Function: Point client_to_screen(const Point &client)");

	Pointf point;
	Pointf new_point;
	point.x = 423;
	point.y = 527;

	new_point = window_1.client_to_screen(point);
	if (new_point.x != (point.x + window_1_xpos - border_size.left)) fail();
	if (new_point.y != (point.y + window_1_ypos - border_size.top)) fail();

	new_point = window_2.client_to_screen(point);
	if (new_point.x != (point.x + window_2_xpos)) fail();
	if (new_point.y != (point.y + window_2_ypos )) fail();

	Console::write_line("   Function: Point screen_to_client(const Point &client)");

	point.x = 423;
	point.y = 527;

	new_point = window_1.screen_to_client(point);
	if (new_point.x != (point.x - window_1_xpos + border_size.left)) fail();
	if (new_point.y != (point.y - window_1_ypos + border_size.top)) fail();

	new_point = window_2.screen_to_client(point);
	if (new_point.x != (point.x - window_2_xpos)) fail();
	if (new_point.y != (point.y - window_2_ypos)) fail();

	Console::write_line("   Function: set_size(int width, int height, bool client_area)");

	const int new_width = 311;
	const int new_height = 133;

	window_1.set_size(new_width, new_height, false);
	window_1_rect = Rect(window_1_xpos, window_1_ypos, new_width + window_1_xpos, new_height + window_1_ypos);
	geometry = window_1.get_geometry();
	if (geometry != window_1_rect) fail();

#ifdef WIN32
	window_2.set_size(new_width, new_height, false);
	window_2_rect = Rect(window_2_xpos, window_2_ypos, new_width + window_2_xpos, new_height + window_2_ypos);
	geometry = window_2.get_geometry();
	if (geometry != window_2_rect) fail();
#else
	Console::write_line(" * Test partially failed on Linux. Cannot resize windows without a caption *");	
#endif

	window_1.set_size(new_width, new_height, true);
	window_1_viewport_rect = Rect(0, 0, new_width, new_height);
	viewport = window_1.get_viewport();
	if (viewport != window_1_viewport_rect) fail();

#ifdef WIN32
	window_2.set_size(new_width, new_height, true);
	window_2_viewport_rect = Rect(0, 0, new_width, new_height);
	viewport = window_2.get_viewport();
	if (viewport != window_2_viewport_rect) fail();
#endif
	Console::write_line("   Function: set_position(const Rect &pos, bool client_area)");

	window_1_xpos += 1;
	window_1_ypos += 2;
	window_1_width += 3;
	window_1_height += 4;
	window_2_xpos += 1;
	window_2_ypos += 2;
	window_2_width += 3;
	window_2_height += 4;

	window_1_rect = Rect(window_1_xpos, window_1_ypos, window_1_width + window_1_xpos, window_1_height + window_1_ypos);
	window_2_rect = Rect(window_2_xpos, window_2_ypos, window_2_width + window_2_xpos, window_2_height + window_2_ypos);

	window_1.set_position(window_1_rect, false);
	geometry = window_1.get_geometry();
	if (geometry != window_1_rect) fail();

#ifdef WIN32
	window_2.set_position(window_2_rect, false);
	geometry = window_2.get_geometry();
	if (geometry != window_2_rect) fail();
#else
	Console::write_line(" * Test partially failed on Linux. Cannot resize windows without a caption *");	
#endif

	window_1_viewport_rect = Rect(0, 0, window_1_width, window_1_height);
	window_2_viewport_rect = Rect(0, 0, window_2_width, window_2_height);

	window_1.set_position(window_1_rect, true);
	viewport = window_1.get_viewport();
	if (viewport != window_1_viewport_rect) fail();
	geometry = window_1.get_geometry();
	window_1_viewport_rect = Rect(0, 0, window_1_width - border_size.right + border_size.left, window_1_height - border_size.bottom + border_size.top);
	window_1_rect = Rect(window_1_xpos + border_size.left,
				window_1_ypos + border_size.top,
				window_1_xpos + window_1_width + border_size.right,
				window_1_ypos + window_1_height + border_size.bottom);
	if (geometry != window_1_rect) fail();

#ifdef WIN32
	window_2.set_position(window_2_rect, true);
	viewport = window_2.get_viewport();
	if (viewport != window_2_viewport_rect) fail();
	geometry = window_2.get_geometry();
	window_2_rect = Rect(window_2_xpos,
				window_2_ypos ,
				window_2_xpos + window_2_width,
				window_2_ypos + window_2_height);
	if (geometry != window_2_rect) fail();
#endif

	// Test these three together
	Console::write_line("   Function: void set_clipboard_text(const std::string &text);");
	Console::write_line("   Function: bool is_clipboard_text_available() const");
	Console::write_line("   Function: std::string get_clipboard_text() const");
	window_1.set_clipboard_text("Hello World");

//NOTE: This test will not work on linux, as the RunLoop::process() needs operating during is_clipboard_text_available()
	RunLoop::process();
	if (!window_2.is_clipboard_text_available()) fail();
	std::string text = window_2.get_clipboard_text();

	if (text != "Hello World") fail();
	Console::write_line(" * Ensure \"Hello World\" is in the clipboard *");
}
