#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

class App
{
public:
	static int main(const std::vector<std::string> &args)
	{
		try
		{
			SetupCore setup_core; 
			SetupDisplay setup_disp;
			SetupGL setup_gl;

			DisplayWindow window("Viewport Grid Test", 640, 480, false, true);

			Canvas canvas(window);
			GraphicContext &gc = canvas.get_gc();
			
			canvas.set_map_mode(map_2d_upper_left);

			while(!window.get_ic().get_keyboard().get_keycode(keycode_escape))
			{
				canvas.clear(Colorf::white);

				Rect viewport = window.get_viewport();
				for (int y = viewport.top; y < viewport.bottom; y+=2)
				{
					canvas.line(viewport.left, y, viewport.right, y, Colorf::black);
				}
				for (int x = viewport.left; x < viewport.right; x+=2)
				{
					canvas.line(x, viewport.top, x, viewport.bottom, Colorf::black);
				}

				canvas.flush();
				window.flip(1);
				KeepAlive::process();

				System::sleep(10);
			}

			return 0;
		}
		catch(Exception error)
		{
			ConsoleWindow console("Console");
			Console::write_line("Exception caught:");
			Console::write_line(error.message);
			console.display_close_message();

			return -1;
		}

		return 0;
	}
};

Application app(&App::main); 