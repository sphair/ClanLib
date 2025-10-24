#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class App
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		try
		{
			CL_SetupCore setup_core; 
			CL_SetupDisplay setup_disp;
			CL_SetupGL setup_gl;

			CL_DisplayWindow window("Viewport Grid Test", 640, 480, false, true);
			CL_GraphicContext gc = window.get_gc();
			
			CL_BlendMode blend_mode1;
			blend_mode1.enable_blending(true);
			gc.set_blend_mode(blend_mode1);

			gc.set_map_mode(cl_map_2d_upper_left);

			while(!window.get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE))
			{
				gc.clear(CL_Colorf::white);
				CL_Rect viewport = window.get_viewport();
				for (int y = viewport.top; y < viewport.bottom; y+=2)
				{
					CL_Draw::line(gc, viewport.left, y, viewport.right, y, CL_Colorf::black);
				}
				for (int x = viewport.left; x < viewport.right; x+=2)
				{
					CL_Draw::line(gc, x, viewport.top, x, viewport.bottom, CL_Colorf::black);
				}

				window.flip(1);
				CL_KeepAlive::process();

				CL_System::sleep(10);
			}

			return 0;
		}
		catch(CL_Exception error)
		{
			CL_ConsoleWindow console("Console");
			CL_Console::write_line("Exception caught:");
			CL_Console::write_line(error.message);
			console.display_close_message();

			return -1;
		}

		return 0;
	}
};

CL_ClanApplication app(&App::main); 