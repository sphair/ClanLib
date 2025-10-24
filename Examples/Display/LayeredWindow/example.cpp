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

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl1.h>

#include <cmath>

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<CL_String> &args);

private:
	void on_mouse_down(const CL_InputEvent &key, const CL_InputState &state);
	void on_mouse_up(const CL_InputEvent &key, const CL_InputState &state);
	void on_mouse_move(const CL_InputEvent &key, const CL_InputState &state, CL_DisplayWindow *window);
	void on_window_close(CL_DisplayWindow *window);
	void on_lost_focus();
	void on_input_up(const CL_InputEvent &key, const CL_InputState &state);

private:
	int tux_radius;
	CL_Pointf tux_position;
	CL_Point last_mouse_pos;
	bool drag_start;

	bool quit;
};

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		// Initialize ClanLib base components
		CL_SetupCore setup_core;

		// Initialize the ClanLib display component
		CL_SetupDisplay setup_display;

		// Initilize the OpenGL drivers
		CL_SetupGL1 setup_gl1;

		// Start the Application
		App app;
		int retval = app.start(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	quit = false;
	drag_start = false;

	try
	{
		// Set the window description
		CL_DisplayWindowDescription desc_window;
		desc_window.set_title("Layered Window Example");
		desc_window.set_allow_resize(false);
#ifdef WIN32
		desc_window.set_layered(true);
#endif
		desc_window.set_decorations(false);
		desc_window.set_size(CL_Size(600, 600), false);

		// Open the windows
		CL_DisplayWindow window(desc_window);
		CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close, &window);
		CL_Slot slot_mouse_down = (window.get_ic().get_mouse()).sig_key_down().connect(this, &App::on_mouse_down);
		CL_Slot slot_mouse_up = (window.get_ic().get_mouse()).sig_key_up().connect(this, &App::on_mouse_up);
		CL_Slot slot_mouse_move = (window.get_ic().get_mouse()).sig_pointer_move().connect(this, &App::on_mouse_move, &window);
		CL_Slot slot_lost_focus = window.sig_lost_focus().connect(this, &App::on_lost_focus);
		CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		// Get the graphic context
		CL_GraphicContext gc = window.get_gc();

		// Get the graphics
		CL_FontDescription font_desc;
		font_desc.set_typeface_name("tahoma");
		font_desc.set_height(64);
		font_desc.set_subpixel(false);
		CL_Font font_large(gc, font_desc);

		font_desc.set_height(30);
		CL_Font font_small(gc, font_desc);
		CL_Sprite tux(gc, "round_tux.png");
		tux_radius = tux.get_width()/2;

		CL_Image rock(gc, "rock.png");

		float rotation = 0.0f;
		unsigned int time_last = CL_System::get_time();

		CL_BlendMode blend_mode;
		gc.set_blend_mode(blend_mode);

		// Run until someone presses escape
		while (!quit)
		{
			unsigned time_now = CL_System::get_time();
			float time_diff = (float) (time_now - time_last);
			time_last = time_now;

			gc.clear(CL_Colorf(0.0f,0.0f,0.0f, 0.0f));
			rock.set_color(CL_Colorf(1.0f, 1.0f, 1.0f, 0.8f));
			rock.draw(gc, 0.0f, 0.0f);

			// Rotate tux
			rotation += time_diff / 10.0f;
			CL_Angle angle;
			angle.set_degrees(rotation);
			tux.set_angle(angle);

			// Caculate tux position
			CL_Pointf circle_center(  (float) (gc.get_width()/2), (float) (gc.get_height()/2) );
			const float radius = 210.0f;
			int tux_circle = 12;
			tux_position.x = -(radius - tux_radius - tux_circle)  * cos( angle.to_radians() / 2.0f );
			tux_position.y = (radius - tux_radius - tux_circle) * sin( angle.to_radians()/ 2.0f );
			tux_position += circle_center;
			tux_position.x -= tux.get_width()/2;
			tux_position.y -= tux.get_height()/2;

			// Give tux circle blue outer outline, because it looks nice
			CL_Draw::circle(gc, tux_position.x + tux_radius, tux_position.y + tux_radius, tux_radius+tux_circle, CL_Colorf(0.0f, 0.0f, 1.0f, 1.0f));

			// Make see through border
			blend_mode.enable_blending(false);
			gc.set_blend_mode(blend_mode);
			CL_Draw::circle(gc, tux_position.x + tux_radius, tux_position.y + tux_radius, tux_radius+tux_circle-2, CL_Colorf(0.0f, 0.0f, 0.0f, 0.0f));
			blend_mode.enable_blending(true);
			gc.set_blend_mode(blend_mode);

			// Give tux circle blue outline, to mask the alpha channel
			CL_Draw::circle(gc, tux_position.x + tux_radius, tux_position.y + tux_radius, tux_radius+2, CL_Colorf(0.0f, 0.0f, 1.0f, 1.0f));

			// Draw tux
			tux.draw(gc, tux_position.x, tux_position.y);

			// Draw text
			font_large.draw_text(gc, 10-2, 50-2, "ClanLib Layered Window", CL_Colorf(0.1f, 0.1f, 0.1f, 1.0f));
			font_large.draw_text(gc, 10, 50, "ClanLib Layered Window", CL_Colorf::green);
			font_small.draw_text(gc, 90-2, 80-2, "Click mouse on the penguin to exit", CL_Colorf(0.1f, 0.1f, 0.1f, 1.0f));
			font_small.draw_text(gc, 90, 80, "Click mouse on the penguin to exit", CL_Colorf::green);
			font_small.draw_text(gc, 140-2, 110-2, "Drag rock to move window", CL_Colorf(0.1f, 0.1f, 0.1f, 1.0f));
			font_small.draw_text(gc, 140, 110, "Drag rock to move window", CL_Colorf::green);

			window.flip(1);

			// This call processes user input and other events
			CL_KeepAlive::process();
		}
	}
	catch(CL_Exception &exception)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console", 80, 160);
		CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();

		return -1;
	}

	return 0;
}

void App::on_mouse_down(const CL_InputEvent &key, const CL_InputState &state)
{
	// Check for click on penguin
	int centre_x = key.mouse_pos.x - ( tux_position.x + tux_radius);
	int centre_y = key.mouse_pos.y - ( tux_position.y + tux_radius);
	int distance = sqrt( (float) ( centre_x * centre_x + centre_y * centre_y ) );
	if (distance < tux_radius)
		quit = true;

	last_mouse_pos = key.mouse_pos;
	drag_start = true;
}

void App::on_mouse_up(const CL_InputEvent &key, const CL_InputState &state)
{
	drag_start = false;
}

void App::on_lost_focus()
{
	drag_start = false;
}

void App::on_mouse_move(const CL_InputEvent &key, const CL_InputState &state, CL_DisplayWindow *window)
{
	if (drag_start)
	{
			CL_Rect geometry = window->get_geometry();
			geometry.translate(key.mouse_pos.x - last_mouse_pos.x, key.mouse_pos.y - last_mouse_pos.y);
			window->set_position(geometry.left, geometry.top);
	}
}

void App::on_window_close(CL_DisplayWindow *window)
{
	quit = true;
}

void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}
