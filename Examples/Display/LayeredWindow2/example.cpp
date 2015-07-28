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

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <cmath>
#include <cstdlib>

class TuxBall
{
public:
	TuxBall(const clan::Sprite &image, const clan::Rect &boundary);

	void move(std::vector<TuxBall> &tuxballs, float time_diff);
	void draw(clan::Canvas &canvas);

private:
	float xpos;
	float ypos;
	float xdir;
	float ydir;
	float scale;
	clan::Sprite sprite;
	clan::Rect boundary;
};

class App : public clan::Application
{
public:
	App();
	bool update() override;

private:
	void on_mouse_down(const clan::InputEvent &key);
	void on_window_close();
	void on_lost_focus();
	void on_input_up(const clan::InputEvent &key);
private:
	clan::DisplayWindow window_center;
	clan::DisplayWindow window_top;
	clan::DisplayWindow window_bottom;
	clan::DisplayWindow window_left;
	clan::DisplayWindow window_right;

	clan::Canvas canvas_center;
	clan::Canvas canvas_top;
	clan::Canvas canvas_right;
	clan::Canvas canvas_bottom;
	clan::Canvas canvas_left;
	clan::Font font;
	clan::SlotContainer sc;
	clan::GameTime game_time;

	clan::Sprite tux;
	clan::Image rock;

	const int num_tuxballs = 256;
	std::vector<TuxBall> tuxballs;

	bool quit = false;
};

clan::ApplicationInstance<App> clanapp;

App::App()
{

	// We support all display targets, in order listed here
	clan::OpenGLTarget::enable();

	// (See README.TXT for more documentation)

	// Define the whole size
	clan::Size entire_window_size(650, 650);
	const int window_inner_offset = 128;
	clan::Point window_initial_position(100, 100);

	// Set the window description
	clan::DisplayWindowDescription desc_window;
	desc_window.set_popup_window();
	desc_window.set_title("Layered Example");
	desc_window.set_allow_resize(false);
	desc_window.show_caption(false);

	// Open the main window
	desc_window.set_layered(false);
	desc_window.set_position(clan::Rect(window_initial_position.x + window_inner_offset, window_initial_position.y + window_inner_offset, clan::Size(entire_window_size.width - (window_inner_offset*2), entire_window_size.height - (window_inner_offset*2))), false);
	window_center = clan::DisplayWindow(desc_window);

	// Create the border layered windows
	desc_window.set_layered(true);
	desc_window.set_title("");
	desc_window.set_owner_window(window_center);
	desc_window.set_position(clan::Rect(window_initial_position.x, window_initial_position.y, clan::Size(entire_window_size.width, window_inner_offset)), false);
	window_top = clan::DisplayWindow(desc_window);
	desc_window.set_position(clan::Rect(window_initial_position.x, window_initial_position.y + entire_window_size.height - window_inner_offset, clan::Size(entire_window_size.width, window_inner_offset)), false);
	window_bottom = clan::DisplayWindow(desc_window);
	desc_window.set_position(clan::Rect(window_initial_position.x, window_initial_position.y + window_inner_offset , clan::Size(window_inner_offset, entire_window_size.height - (window_inner_offset*2))), false);
	window_left = clan::DisplayWindow(desc_window);
	desc_window.set_position(clan::Rect(window_initial_position.x + entire_window_size.width - window_inner_offset, window_initial_position.y + window_inner_offset , clan::Size(window_inner_offset, entire_window_size.height - (window_inner_offset*2))), false);
	window_right = clan::DisplayWindow(desc_window);

	// Setup the slots
	sc.connect(window_center.sig_window_close(), clan::bind_member(this, &App::on_window_close));
	sc.connect(window_center.get_ic().get_mouse().sig_key_down(), clan::bind_member(this, &App::on_mouse_down));
	sc.connect(window_center.get_ic().get_mouse().sig_key_dblclk(), clan::bind_member(this, &App::on_mouse_down));
	sc.connect(window_center.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));

	// Get the canvas
	canvas_center = clan::Canvas(window_center);
	canvas_top = clan::Canvas(window_top);
	canvas_right = clan::Canvas(window_right);
	canvas_bottom = clan::Canvas(window_bottom);
	canvas_left = clan::Canvas(window_left);

	// Get the graphics
	clan::FontDescription font_desc;
	font_desc.set_height(24);
	font = clan::Font("tahoma", font_desc);

	tux = clan::Sprite(canvas_center, "../LayeredWindow/round_tux.png");
	rock = clan::Image(canvas_center, "../LayeredWindow/rock.png");

	// Translate and scale the window matrix to position the graphics at the correct position
	clan::Mat4f scale_matrix = clan::Mat4f::scale((float)entire_window_size.width / rock.get_width(), (float)entire_window_size.height / rock.get_height(), 1.0f);
	canvas_center.set_transform(clan::Mat4f::translate(-window_inner_offset, -window_inner_offset, 0.0f) * scale_matrix);
	canvas_top.set_transform(clan::Mat4f::translate(0.0f, 0.0f, 0.0f) * scale_matrix);
	canvas_left.set_transform(clan::Mat4f::translate(0, -window_inner_offset, 0.0f) * scale_matrix);
	canvas_right.set_transform(clan::Mat4f::translate(-entire_window_size.width + window_inner_offset, -window_inner_offset, 0.0f) * scale_matrix);
	canvas_bottom.set_transform(clan::Mat4f::translate(0, -entire_window_size.height + window_inner_offset, 0.0f) * scale_matrix);

	// Prepare the static image in the layered window
	canvas_top.clear(clan::Colorf(0.0f,0.0f,0.0f, 0.0f));
	canvas_right.clear(clan::Colorf(0.0f,0.0f,0.0f, 0.0f));
	canvas_bottom.clear(clan::Colorf(0.0f,0.0f,0.0f, 0.0f));
	canvas_left.clear(clan::Colorf(0.0f,0.0f,0.0f, 0.0f));
	rock.draw(canvas_top, 0.0f, 0.0f);
	rock.draw(canvas_right, 0.0f, 0.0f);
	rock.draw(canvas_bottom, 0.0f, 0.0f);
	rock.draw(canvas_left, 0.0f, 0.0f);

	// Draw the layered windows
	window_top.flip();
	window_right.flip();
	window_bottom.flip();
	window_left.flip();

	// Setup the demo
	clan::Rect boundary(120, 120, clan::Size(360, 360));
	tuxballs.reserve(num_tuxballs);
	for (int cnt=0; cnt<num_tuxballs; cnt++)
	{
		tuxballs.push_back(TuxBall(tux, boundary));
	}

	game_time.reset();
}

bool App::update()
{
	game_time.update();
	
	canvas_center.clear(clan::Colorf(0.0f,0.0f,0.0f, 1.0f));
	rock.draw(canvas_center, 0.0f, 0.0f);

	// Move tux
	for (int cnt=0; cnt<num_tuxballs; cnt++)
	{
		tuxballs[cnt].move(tuxballs, game_time.get_time_elapsed() * 1000.0f);
	}

	// Draw tux
	for (int cnt=0; cnt<num_tuxballs; cnt++)
	{
		tuxballs[cnt].draw(canvas_center);
	}

	std::string fps = clan::string_format("%1 fps", clan::StringHelp::float_to_text(game_time.get_updates_per_second(), 1));
	font.draw_text(canvas_center, 150-2, 150-2, fps, clan::Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	font.draw_text(canvas_center, 150, 150, fps, clan::Colorf(1.0f, 1.0f, 1.0f, 1.0f));

	window_center.flip(0);

	return !quit;
}

void App::on_mouse_down(const clan::InputEvent &key)
{
	quit = true;
}

void App::on_window_close()
{
	quit = true;
}

void App::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

TuxBall::TuxBall(const clan::Sprite &image, const clan::Rect &new_boundary) : boundary(new_boundary), sprite(image)
{
	scale =  ((float) ((rand() % 512)) + 256) / 4096.0f;
	float scaled_width = scale * (float) sprite.get_width();
	float scaled_height = scale * (float) sprite.get_height();

	boundary.right -= scaled_width;
	boundary.bottom -= scaled_height;

	xpos = (float) ((rand() % (boundary.get_width())) + boundary.left);
	ypos = (float) ((rand() % (boundary.get_height())) + boundary.top );

	xdir =  ((float) ((rand() % 512)) + 256) / 256.0f;
	ydir =  ((float) ((rand() % 512)) + 256) / 256.0f;


}

void TuxBall::move(std::vector<TuxBall> &tuxballs, float time_diff)
{
	xpos += xdir*(time_diff/50.0f);
	ypos += ydir*(time_diff/50.0f);

	if ((int) xpos < boundary.left)
	{
		xdir = -xdir;
		xpos = (float) boundary.left;
	}

	if ((int) ypos < boundary.top)
	{
		ydir = -ydir;
		ypos = (float) boundary.top;
	}

	if ((int) xpos > boundary.right)
	{
		xdir = -xdir;
		xpos = (float) boundary.right;
	}

	if ((int) ypos > boundary.bottom)
	{
		ydir = -ydir;
		ypos = (float) boundary.bottom;
	}
}

void TuxBall::draw(clan::Canvas &canvas)
{
	sprite.set_scale(scale, scale);
	sprite.set_alpha(0.5f);
	sprite.draw(canvas, xpos, ypos);
}



