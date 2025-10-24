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
#include "app.h"
#include "options.h"
#include <cstdlib>

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Blend Example");
	win_desc.set_size(CL_Size( 900, 570 ), false);

	CL_DisplayWindow window(win_desc);
	CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	CL_String theme;
	if (CL_FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (CL_FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw CL_Exception("No themes found");

	CL_GUIWindowManagerTexture wm(window);
	CL_GUIManager gui(wm, theme);
	
	CL_GraphicContext gc = window.get_gc();

	// Deleted automatically by the GUI
	Options *options = new Options(gui, CL_Rect(0, 0, gc.get_size()));

	CL_Image image_grid(gc, "Resources/grid.png");
	CL_Image image_ball(gc, "Resources/ball.png");
	CL_ImageImportDescription desc;
	desc.set_premultiply_alpha(true);
	CL_Image image_ball_premultiply_alpha(gc, "Resources/ball.png", desc);
	grid_space = (float) (image_grid.get_width() - image_ball.get_width());

	setup_balls();

	CL_BlendMode blend_mode;
	CL_BlendMode default_blend_mode;
	CL_BufferControl buffer_control;
	CL_BufferControl default_buffer_control;

	options->request_repaint();

	CL_Font font(gc, "Tahoma", 20);

	unsigned int time_last = CL_System::get_time();

	while (!quit)
	{
		unsigned int time_now = CL_System::get_time();
		float time_diff = (float) (time_now - time_last);
		time_last = time_now;

		wm.process();
		wm.draw_windows(gc);

		const float grid_xpos = 10.0f;
		const float grid_ypos = 10.0f;
		image_grid.draw(gc, grid_xpos, grid_ypos);

		image_ball.set_color(options->primary_color);
		image_ball_premultiply_alpha.set_color(options->primary_color);

		int num_balls = options->num_balls;
		if (num_balls > max_balls)
			num_balls = max_balls;

		if (options->is_moveballs_set)
			move_balls(time_diff, num_balls);

		blend_mode.set_blend_function(options->blendfunc[0],options->blendfunc[1],options->blendfunc[2],options->blendfunc[3]);
		blend_mode.set_blend_equation(options->blendequation[0], options->blendequation[1]);
		blend_mode.set_blend_color(options->blend_color);
		blend_mode.enable_blending(options->is_blending_set);

		buffer_control.set_logic_op(options->logic_operation);
		buffer_control.enable_logic_op(options->logic_operation_enabled);

		gc.set_blend_mode(blend_mode);
		gc.set_buffer_control(buffer_control);

		for (int cnt=0; cnt<num_balls; cnt++)
		{
			if (options->is_premult_alpha_set)
			{
				image_ball_premultiply_alpha.draw(gc, grid_xpos + balls[cnt].xpos, grid_ypos + balls[cnt].ypos);
			}
			else
			{
				image_ball.draw(gc, grid_xpos + balls[cnt].xpos, grid_ypos + balls[cnt].ypos);
			}
		}

		gc.set_blend_mode(default_blend_mode);
		gc.set_buffer_control(default_buffer_control);

		draw_equation(gc, font, options);

		window.flip(1);

		CL_KeepAlive::process();
	}

	return 0;
}

// A key was pressed
void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
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
		float xdisp = (balls[cnt].xspeed * time_diff) / 5000.0f;
		float ydisp = (balls[cnt].xspeed * time_diff) / 5000.0f;

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

void App::draw_equation(CL_GraphicContext &gc, CL_Font &font, Options *options)
{
	CL_Rect equation_rect(10, gc.get_height() - 70, CL_Size(gc.get_width() - 20, 60));
	CL_Draw::fill(gc, equation_rect, CL_Colorf::black);
	CL_Draw::box(gc, equation_rect, CL_Colorf::white);

	if (options->logic_operation_enabled)
	{
		// OpenGL spec says blending is disabled with Logic Operation is enabled.
		CL_String text = get_logic_operation(options->logic_operation, "Source", "Dest");
		font.draw_text(gc, equation_rect.left + 8, equation_rect.bottom - 38, "RGBA = " + text );
		if (options->is_blending_set)
			font.draw_text(gc, equation_rect.left + 8, equation_rect.bottom - 8, "(OpenGL disables blending when logic operation is used)");
	}
	else
	{
		if (options->is_blending_set)
		{
			CL_String blendfunc_src_color = get_blendfunc(options->blendfunc[0], "Cs");
			CL_String blendfunc_dest_color = get_blendfunc(options->blendfunc[1], "Cd");
			CL_String blendfunc_src_alpha = get_blendfunc(options->blendfunc[2], "As");
			CL_String blendfunc_dest_alpha = get_blendfunc(options->blendfunc[3], "Ad");

			CL_String blendfunc_combined_color = get_blendequation(options->blendequation[0], blendfunc_src_color, blendfunc_dest_color, "Cs", "Cd");
			CL_String blendfunc_combined_alpha = get_blendequation(options->blendequation[1], blendfunc_src_alpha, blendfunc_dest_alpha, "As", "Ad");
			font.draw_text(gc, equation_rect.left + 8, equation_rect.bottom - 38, "Color = " + blendfunc_combined_color );
			font.draw_text(gc, equation_rect.left + 8, equation_rect.bottom - 8, "Alpha = " + blendfunc_combined_alpha );

			font.draw_text(gc, equation_rect.right - 250, equation_rect.bottom - 38, "(C=RGB Value, A=Alpha Value)", CL_Colorf::yellow);
			font.draw_text(gc, equation_rect.right - 250, equation_rect.bottom - 8, "(s=Source, d=Dest, c=Constant)", CL_Colorf::yellow);
		}

	}

}

CL_String App::get_blendfunc(CL_BlendFunc blendfunc, const CL_String &fragment_colour)
{
	CL_String text;
	CL_String source_colour = "Cs";
	CL_String source_alpha = "As";
	CL_String dest_colour = "Cd";
	CL_String dest_alpha = "Ad";
	CL_String constant_colour = "Cc";
	CL_String constant_alpha = "Ac";

	switch (blendfunc)
	{
		case cl_blend_zero:
			break;
		case cl_blend_one:
			text = fragment_colour + " ";
			break;
		case cl_blend_dest_color:
			text = dest_colour + " * " + fragment_colour + " ";
			break;
		case cl_blend_src_color:
			text = source_colour + " * " + fragment_colour + " ";
			break;
		case cl_blend_one_minus_dest_color:
			text = "(1 - " + dest_colour + ") * " + fragment_colour + " ";
			break;
		case cl_blend_one_minus_src_color:
			text = "(1 - " + source_colour + ") * " + fragment_colour + " ";
			break;
		case cl_blend_src_alpha:
			text = source_alpha + " * " + fragment_colour + " ";
			break;
		case cl_blend_one_minus_src_alpha:
			text = "(1 - " + source_alpha + ") * " + fragment_colour + " ";
			break;
		case cl_blend_dest_alpha:
			text = dest_alpha + " * " + fragment_colour + " ";
			break;
		case cl_blend_one_minus_dest_alpha:
			text = "(1 - " + dest_alpha + ") * " + fragment_colour + " ";
			break;
		case cl_blend_src_alpha_saturate:
			text = "min(" + source_alpha + ", 1 - " + dest_alpha + ") * " + fragment_colour + " ";
			break;
		case cl_blend_constant_color:
			text = constant_colour + " * " + fragment_colour + " ";
			break;
		case cl_blend_one_minus_constant_color:
			text = "(1 - " + constant_colour + ") * " + fragment_colour + " ";
			break;
		case cl_blend_constant_alpha:
			text = constant_alpha + " * " + fragment_colour + " ";
			break;
		case cl_blend_one_minus_constant_alpha:
			text = "(1 - " + constant_alpha + ") * " + fragment_colour + " ";
			break;
		default:
			text = "ERROR";
	}
	return text;
}

CL_String App::get_blendequation(CL_BlendEquation equation, const CL_String &source, const CL_String &dest, const CL_String &source_fragment, const CL_String &dest_fragment)
{
	CL_String text;
	switch (equation)
	{
		case cl_blend_equation_add:
			text = source + "+ " + dest;
			break;

		case cl_blend_equation_subtract:
			text = source + "- " + dest;
			break;

		case cl_blend_equation_reverse_subtract:
			text = dest + "- " + source;
			break;

		case cl_blend_equation_min:
			text = "min(" + source_fragment + ", " + dest_fragment + ")";
			break;

		case cl_blend_equation_max:
			text = "max(" + source_fragment + ", " + dest_fragment + ")";
			break;
		default:
			text = "ERROR";

	}
	return text;
}

CL_String App::get_logic_operation(CL_LogicOp logic_operation, const CL_String &source_fragment, const CL_String &dest_fragment)
{
	CL_String text;

	switch (logic_operation)
	{
		case cl_logic_op_clear:
			text = "0";
			break;

		case cl_logic_op_and:
			text = source_fragment + " & " + dest_fragment;
			break;

		case cl_logic_op_and_reverse:
			text = source_fragment + " & ( ! " + dest_fragment + ")";
			break;

		case cl_logic_op_copy:
			text = source_fragment;
			break;

		case cl_logic_op_and_inverted:
			text = "( ! " + source_fragment + " ) & " + dest_fragment;
			break;

		case cl_logic_op_noop:
			text = dest_fragment;
			break;

		case cl_logic_op_xor:
			text = source_fragment + " ^ " + dest_fragment;
			break;

		case cl_logic_op_or:
			text = source_fragment + " | " + dest_fragment;
			break;

		case cl_logic_op_nor:
			text = "! (" + source_fragment + " | " + dest_fragment + ")";
			break;

		case cl_logic_op_equiv:
			text = "! (" + source_fragment + " ^ " + dest_fragment + ")";
			break;

		case cl_logic_op_invert:
			text = "! " + dest_fragment;
			break;

		case cl_logic_op_or_reverse:
			text = source_fragment + " | ( ! " + dest_fragment + " )";
			break;

		case cl_logic_op_copy_inverted:
			text = "! " + source_fragment;
			break;

		case cl_logic_op_or_inverted:
			text = "( ! " + source_fragment + ") | " + dest_fragment;
			break;

		case cl_logic_op_nand:
			text = "! ( " + source_fragment + " & " + dest_fragment + ")";
			break;

		case cl_logic_op_set:
			text = "1";
			break;

		default:
			text = "ERROR";
	}
	return text;
}

