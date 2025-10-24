
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

int blend_table_src[] =
{
	blend_zero,                     // source or destination (0, 0, 0, 0)
	blend_one,                      // source or destination (1, 1, 1, 1)
	blend_dest_color,               // source                (Rd, Gd, Bd, Ad)
	blend_one_minus_dest_color,     // source                (1, 1, 1, 1) - (Rd, Gd, Bd, Ad)
	blend_src_alpha,                // source or destination (As, As, As, As)
	blend_one_minus_src_alpha,      // source or destination (1, 1, 1, 1) - (As, As, As, As)
	blend_dst_alpha,                // source or destination (Ad, Ad, Ad, Ad)
	blend_one_minus_dest_alpha,     // source or destination (1, 1, 1, 1) - (Ad, Ad, Ad, Ad)
	blend_src_alpha_saturate,       // source                (f, f, f, 1) - f = min(As, 1 - Ad)
	blend_constant_color,           // source or destination (Rc, Gc, Bc, Ac)
	blend_one_minus_constant_color, // source or destination (1, 1, 1, 1) - (Rc, Gc, Bc, Ac)
	blend_constant_alpha,           // source or destination (Ac, Ac, Ac, Ac)
	blend_one_minus_constant_alpha  // source or destination (1, 1, 1, 1) - (Ac, Ac, Ac, Ac)
	-1
};

char *blend_strings_src[] =
{
	"zero",
	"one",
	"dest_color",
	"one_minus_dest_color",
	"src_alpha",
	"one_minus_src_alpha",
	"dst_alpha",
	"one_minus_dest_alpha",
	"src_alpha_saturate",
	"constant_color",
	"one_minus_constant_color",
	"constant_alpha",
	"one_minus_constant_alpha",
	0
};

int blend_table_dst[] =
{
	blend_zero,                     // source or destination (0, 0, 0, 0)
	blend_one,                      // source or destination (1, 1, 1, 1)
	blend_src_color,                // destination           (Rs, Gs, Bs, As)
	blend_one_minus_src_color,      // destination           (1, 1, 1, 1) - (Rs, Gs, Bs, As)
	blend_src_alpha,                // source or destination (As, As, As, As)
	blend_one_minus_src_alpha,      // source or destination (1, 1, 1, 1) - (As, As, As, As)
	blend_dst_alpha,                // source or destination (Ad, Ad, Ad, Ad)
	blend_one_minus_dest_alpha,     // source or destination (1, 1, 1, 1) - (Ad, Ad, Ad, Ad)
	blend_constant_color,           // source or destination (Rc, Gc, Bc, Ac)
	blend_one_minus_constant_color, // source or destination (1, 1, 1, 1) - (Rc, Gc, Bc, Ac)
	blend_constant_alpha,           // source or destination (Ac, Ac, Ac, Ac)
	blend_one_minus_constant_alpha  // source or destination (1, 1, 1, 1) - (Ac, Ac, Ac, Ac)
	-1
};

char *blend_strings_dst[] =
{
	"zero",
	"one",
	"src_color",
	"one_minus_src_color",
	"src_alpha",
	"one_minus_src_alpha",
	"dst_alpha",
	"one_minus_dest_alpha",
	"constant_color",
	"one_minus_constant_color",
	"constant_alpha",
	"one_minus_constant_alpha",
	0
};

class ExampleBlendFunctions : public CL_ClanApplication
{
public:
	int selected_src, selected_dst;

	virtual int main(int argc, char **argv)
	{
		int index_src, index_dst;

		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();
		
		quit = false;

		try
		{
			// Initialize ClanLib base components
			CL_SetupCore setup_core;

			// Initialize the ClanLib display component
			CL_SetupDisplay setup_display;

			// Initilize the OpenGL drivers
			CL_SetupGL setup_gl;

			// Set a videomode - 640x480
			CL_DisplayWindow window("ClanLib BlendFunctions Example", 1024, 768);

			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &ExampleBlendFunctions::on_window_close);

			// Connect a keyboard handler to on_key_up()
			CL_Slot slot_input_down = (window.get_ic()->get_keyboard()).sig_key_down().connect(this, &ExampleBlendFunctions::on_input_down);

			// Load resources
			CL_ResourceManager resources("resources.xml", false);
			CL_Font font_text("font", &resources);
			CL_Surface sur_logo("background", &resources);

			selected_src = 0;
			selected_dst = 0;

			// Select blend_src_alpha per default (ClanLib default source blend)
			for (index_src = 0; blend_strings_src[index_src] != 0; index_src++)
			{
				if (blend_table_src[index_src] == blend_src_alpha)
				{
					selected_src = index_src;
					break;
				}
			}

			// Select blend_one_minus_src_alpha per default (ClanLib default dest blend)
			for (index_dst = 0; blend_strings_dst[index_dst] != 0; index_dst++)
			{
				if (blend_table_dst[index_dst] == blend_one_minus_src_alpha)
				{
					selected_dst = index_dst;
					break;
				}
			}

			CL_FramerateCounter frameratecounter;

			// Run until someone presses escape
			while (!quit)
			{
				// Clear the display
				CL_Display::fill_rect(
					CL_Rect(0,0,CL_Display::get_width(),CL_Display::get_height()),
					CL_Gradient(
						CL_Color::red,
						CL_Color::chartreuse,
						CL_Color::cornflowerblue,
						CL_Color::darksalmon));

				int yy = 20;
				int xx = 203;
				for (index_dst = 0; blend_strings_dst[index_dst] != 0; index_dst++)
				{
					if (xx + font_text.get_width(blend_strings_dst[index_dst]) > CL_Display::get_width())
					{
						yy += 20;
						xx = 203;
					}
					if (selected_dst == index_dst)
					{
						CL_Display::fill_rect(
							CL_Rect(
								xx-2,
								yy-2,
								xx+font_text.get_width(blend_strings_dst[index_dst])+2,
								yy+18),
							CL_Color::blue);
					}
					font_text.draw(
						xx, yy, blend_strings_dst[index_dst], window.get_gc());
					xx += font_text.get_width(blend_strings_dst[index_dst])+10;
				}

				yy += 22;

				font_text.set_alignment(origin_top_right);
				font_text.draw(200, 3, "dest blend:");
				font_text.set_alignment(origin_bottom_left);
				font_text.draw(3, yy-2, "source blend:");
				font_text.set_alignment(origin_top_left);

				CL_Display::draw_line(0, yy, CL_Display::get_width(), yy, CL_Color::firebrick);
				CL_Display::draw_line(200, 0, 200, CL_Display::get_height(), CL_Color::firebrick);

				yy += 3;
				int start_y = yy;

				for (index_src = 0; blend_strings_src[index_src] != 0; index_src++)
				{
					if (selected_src == index_src)
					{
						CL_Display::fill_rect(
							CL_Rect(
								1,
								yy-2,
								3+font_text.get_width(blend_strings_src[index_src])+2,
								yy+18),
							CL_Color::blue);
					}
					font_text.draw(
						3, yy, blend_strings_src[index_src], window.get_gc());

					yy += 20;
				}

				sur_logo.set_blend_func(
					(CL_BlendFunc) blend_table_src[selected_src],
					(CL_BlendFunc) blend_table_dst[selected_dst]);
				sur_logo.draw(203, start_y, window.get_gc());

				int fps = frameratecounter.get_fps();
				font_text.set_alignment(origin_bottom_right);
				font_text.draw(198, start_y-5, CL_String::from_int(fps) + " fps");
				font_text.set_alignment(origin_top_left);

				// Flip the display, showing on the screen what we have drawed
				// since last call to flip_display()
				CL_Display::flip();

				// This call updates input and performs other "housekeeping"
				// call this each frame
				CL_System::keep_alive();
			}
		}
		catch(CL_Error error)
		{
			std::cout << "Exception caught : " << error.message.c_str() << std::endl;			

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}

private:
	bool quit;

	void on_input_down(const CL_InputEvent &key)
	{
		if(key.id == CL_KEY_ESCAPE)
			quit = true;

		if (key.id == CL_KEY_LEFT && selected_dst > 0)
			selected_dst--;
		if (key.id == CL_KEY_RIGHT && blend_table_dst[selected_dst+1] != -1)
			selected_dst++;

		if (key.id == CL_KEY_UP && selected_src > 0)
			selected_src--;
		if (key.id == CL_KEY_DOWN && blend_table_src[selected_src+1] != -1)
			selected_src++;
	}

	void on_window_close()
	{
		quit = true;
	}
} my_app;
