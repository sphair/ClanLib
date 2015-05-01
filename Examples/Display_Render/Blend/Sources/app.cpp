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
#include "app.h"
#include "options.h"
#include <cstdlib>

clan::ApplicationInstance<App> clanapp;

App::App()
{
	// We support all display targets, in order listed here
#ifdef WIN32
	//clan::D3DTarget::enable();
#endif
	clan::OpenGLTarget::enable();

	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Blend Example");
	win_desc.set_size(clan::Size( 1400, 730 ), false);

	window = clan::DisplayWindow(win_desc);
	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::on_window_close));
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));

	canvas = clan::Canvas(window);

	clan::FileResourceDocument doc(clan::FileSystem("../../ThemeAero"));
	clan::ResourceManager resources = clan::FileResourceManager::create(doc);
	ui_thread = clan::UIThread(resources);

	options = std::make_shared<Options>(canvas);
	options->set_event_window(window);
	options->set_cursor_window(window);

	image_grid = clan::Image(canvas, "Resources/grid.png");
	image_ball = clan::Image(canvas, "Resources/ball.png");
	clan::ImageImportDescription desc;
	desc.set_premultiply_alpha(true);
	image_ball_premultiply_alpha = clan::Image(canvas, "Resources/ball.png", desc);
	grid_space = (float) (image_grid.get_width() - image_ball.get_width());

	setup_balls();

	font = clan::Font("Tahoma", 16);

	game_time.reset();
}

bool App::update()
{
	game_time.update();
	//canvas.clear();

	options->set_needs_layout();	//TODO: Remove this line when completed optimising the GUI
	options->set_needs_render();
	options->set_viewport(canvas.get_size());
	options->set_background_color(clan::Colorf(0.6f, 0.6f, 0.2f, 1.0f));
	options->update();

	const float grid_xpos = 10.0f;
	const float grid_ypos = 10.0f;
	image_grid.draw(canvas, grid_xpos, grid_ypos);

	image_ball.set_color(options->primary_color);
	image_ball_premultiply_alpha.set_color(options->primary_color);

	int num_balls = options->num_balls;
	if (num_balls > max_balls)
		num_balls = max_balls;

	if (options->is_moveballs_set)
		move_balls(game_time.get_time_elapsed(), num_balls);

	clan::BlendStateDescription blend_desc;
	blend_desc.set_blend_function(options->blendfunc[0],options->blendfunc[1],options->blendfunc[2],options->blendfunc[3]);
	blend_desc.set_blend_equation(options->blendequation[0], options->blendequation[1]);
	blend_desc.enable_blending(options->is_blending_set);
	blend_desc.set_logic_op(options->logic_operation);
	blend_desc.enable_logic_op(options->logic_operation_enabled);

	clan::BlendState blend_state(canvas, blend_desc);
	canvas.set_blend_state(blend_state, options->blend_color);

	for (int cnt=0; cnt<num_balls; cnt++)
	{
		if (options->is_premult_alpha_set)
		{
			image_ball_premultiply_alpha.draw(canvas, grid_xpos + balls[cnt].xpos, grid_ypos + balls[cnt].ypos);
		}
		else
		{
			image_ball.draw(canvas, grid_xpos + balls[cnt].xpos, grid_ypos + balls[cnt].ypos);
		}
	}

	canvas.reset_blend_state();
	
	draw_equation(canvas, font, options);

	font.draw_text(canvas, canvas.get_width() - 100.0f, 20.0f, clan::string_format("FPS = %1", clan::StringHelp::float_to_text(game_time.get_updates_per_second(), 1)));

	window.flip(0);

	return !quit;
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

void App::setup_balls()
{
	for (int cnt=0; cnt<max_balls; cnt++)
	{
		balls[cnt].xpos = (float) (rand() % (int) grid_space);
		balls[cnt].ypos = (float) (rand() % (int) grid_space);
		balls[cnt].xspeed = (float) ((rand() & 0xFF)+0x7F);
		balls[cnt].yspeed = (float) ((rand() & 0xFF)+0x7F);
		balls[cnt].xdir = ((rand() & 1) == 0);
		balls[cnt].ydir = ((rand() & 1) == 0);
	}
}

void App::move_balls(float time_diff, int num_balls)
{
	for (int cnt=0; cnt<num_balls; cnt++)
	{
		float xdisp = (balls[cnt].xspeed * time_diff) * 0.5f;
		float ydisp = (balls[cnt].xspeed * time_diff) * 0.5f;

		if (balls[cnt].xdir)
		{
			balls[cnt].xpos += xdisp;
			if (balls[cnt].xpos >= (float) grid_space)
			{
				balls[cnt].xpos = grid_space;
				balls[cnt].xdir = false;
			}
		}
		else
		{
			balls[cnt].xpos -= xdisp;
			if (balls[cnt].xpos <= 0.0f)
			{
				balls[cnt].xpos = 0.0f;
				balls[cnt].xdir = true;
			}
		}

		if (balls[cnt].ydir)
		{
			balls[cnt].ypos += ydisp;
			if (balls[cnt].ypos >= (float) grid_space)
			{
				balls[cnt].ypos = grid_space;
				balls[cnt].ydir = false;
			}
		}
		else
		{
			balls[cnt].ypos -= ydisp;
			if (balls[cnt].ypos <= 0.0f)
			{
				balls[cnt].ypos = 0.0f;
				balls[cnt].ydir = true;
			}
		}

	}
}

void App::draw_equation(clan::Canvas &canvas, clan::Font &font, std::shared_ptr<Options> &options)
{
	clan::Rect equation_rect(10, canvas.get_height() - 70, clan::Size(canvas.get_width() - 20, 60));
	canvas.fill_rect(equation_rect, clan::Colorf::black);
	canvas.draw_box(equation_rect, clan::Colorf::white);

	if (options->logic_operation_enabled)
	{
		// OpenGL spec says blending is disabled with Logic Operation is enabled.
		std::string text = get_logic_operation(options->logic_operation, "Source", "Dest");
		font.draw_text(canvas, equation_rect.left + 8, equation_rect.bottom - 38, "RGBA = " + text );
		if (options->is_blending_set)
			font.draw_text(canvas, equation_rect.left + 8, equation_rect.bottom - 8, "(OpenGL disables blending when logic operation is used)");
	}
	else
	{
		if (options->is_blending_set)
		{
			std::string blendfunc_src_color = get_blendfunc(options->blendfunc[0], "Cs");
			std::string blendfunc_dest_color = get_blendfunc(options->blendfunc[1], "Cd");
			std::string blendfunc_src_alpha = get_blendfunc(options->blendfunc[2], "As");
			std::string blendfunc_dest_alpha = get_blendfunc(options->blendfunc[3], "Ad");

			std::string blendfunc_combined_color = get_blendequation(options->blendequation[0], blendfunc_src_color, blendfunc_dest_color, "Cs", "Cd");
			std::string blendfunc_combined_alpha = get_blendequation(options->blendequation[1], blendfunc_src_alpha, blendfunc_dest_alpha, "As", "Ad");
			font.draw_text(canvas, equation_rect.left + 8, equation_rect.bottom - 38, "Color = " + blendfunc_combined_color );
			font.draw_text(canvas, equation_rect.left + 8, equation_rect.bottom - 8, "Alpha = " + blendfunc_combined_alpha );

			font.draw_text(canvas, equation_rect.right - 250, equation_rect.bottom - 38, "(C=RGB Value, A=Alpha Value)", clan::Colorf::yellow);
			font.draw_text(canvas, equation_rect.right - 250, equation_rect.bottom - 8, "(s=Source, d=Dest, c=Constant)", clan::Colorf::yellow);
		}

	}

}

std::string App::get_blendfunc(clan::BlendFunc blendfunc, const std::string &fragment_colour)
{
	std::string text;
	std::string source_colour = "Cs";
	std::string source_alpha = "As";
	std::string dest_colour = "Cd";
	std::string dest_alpha = "Ad";
	std::string constant_colour = "Cc";
	std::string constant_alpha = "Ac";

	switch (blendfunc)
	{
		case clan::blend_zero:
			break;
		case clan::blend_one:
			text = fragment_colour + " ";
			break;
		case clan::blend_dest_color:
			text = dest_colour + " * " + fragment_colour + " ";
			break;
		case clan::blend_src_color:
			text = source_colour + " * " + fragment_colour + " ";
			break;
		case clan::blend_one_minus_dest_color:
			text = "(1 - " + dest_colour + ") * " + fragment_colour + " ";
			break;
		case clan::blend_one_minus_src_color:
			text = "(1 - " + source_colour + ") * " + fragment_colour + " ";
			break;
		case clan::blend_src_alpha:
			text = source_alpha + " * " + fragment_colour + " ";
			break;
		case clan::blend_one_minus_src_alpha:
			text = "(1 - " + source_alpha + ") * " + fragment_colour + " ";
			break;
		case clan::blend_dest_alpha:
			text = dest_alpha + " * " + fragment_colour + " ";
			break;
		case clan::blend_one_minus_dest_alpha:
			text = "(1 - " + dest_alpha + ") * " + fragment_colour + " ";
			break;
		case clan::blend_src_alpha_saturate:
			text = "min(" + source_alpha + ", 1 - " + dest_alpha + ") * " + fragment_colour + " ";
			break;
		case clan::blend_constant_color:
			text = constant_colour + " * " + fragment_colour + " ";
			break;
		case clan::blend_one_minus_constant_color:
			text = "(1 - " + constant_colour + ") * " + fragment_colour + " ";
			break;
		case clan::blend_constant_alpha:
			text = constant_alpha + " * " + fragment_colour + " ";
			break;
		case clan::blend_one_minus_constant_alpha:
			text = "(1 - " + constant_alpha + ") * " + fragment_colour + " ";
			break;
		default:
			text = "ERROR";
	}
	return text;
}

std::string App::get_blendequation(clan::BlendEquation equation, const std::string &source, const std::string &dest, const std::string &source_fragment, const std::string &dest_fragment)
{
	std::string text;
	switch (equation)
	{
		case clan::equation_add:
			text = source + "+ " + dest;
			break;

		case clan::equation_subtract:
			text = source + "- " + dest;
			break;

		case clan::equation_reverse_subtract:
			text = dest + "- " + source;
			break;

		case clan::equation_min:
			text = "min(" + source_fragment + ", " + dest_fragment + ")";
			break;

		case clan::equation_max:
			text = "max(" + source_fragment + ", " + dest_fragment + ")";
			break;
		default:
			text = "ERROR";

	}
	return text;
}

std::string App::get_logic_operation(clan::LogicOp logic_operation, const std::string &source_fragment, const std::string &dest_fragment)
{
	std::string text;

	switch (logic_operation)
	{
		case clan::logic_clear:
			text = "0";
			break;

		case clan::logic_and:
			text = source_fragment + " & " + dest_fragment;
			break;

		case clan::logic_and_reverse:
			text = source_fragment + " & ( ! " + dest_fragment + ")";
			break;

		case clan::logic_copy:
			text = source_fragment;
			break;

		case clan::logic_and_inverted:
			text = "( ! " + source_fragment + " ) & " + dest_fragment;
			break;

		case clan::logic_noop:
			text = dest_fragment;
			break;

		case clan::logic_xor:
			text = source_fragment + " ^ " + dest_fragment;
			break;

		case clan::logic_or:
			text = source_fragment + " | " + dest_fragment;
			break;

		case clan::logic_nor:
			text = "! (" + source_fragment + " | " + dest_fragment + ")";
			break;

		case clan::logic_equiv:
			text = "! (" + source_fragment + " ^ " + dest_fragment + ")";
			break;

		case clan::logic_invert:
			text = "! " + dest_fragment;
			break;

		case clan::logic_or_reverse:
			text = source_fragment + " | ( ! " + dest_fragment + " )";
			break;

		case clan::logic_copy_inverted:
			text = "! " + source_fragment;
			break;

		case clan::logic_or_inverted:
			text = "( ! " + source_fragment + ") | " + dest_fragment;
			break;

		case clan::logic_nand:
			text = "! ( " + source_fragment + " & " + dest_fragment + ")";
			break;

		case clan::logic_set:
			text = "1";
			break;

		default:
			text = "ERROR";
	}
	return text;
}

