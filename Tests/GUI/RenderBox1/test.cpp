#include <ClanLib/core.h>
#include <ClanLib/gui.h>
//#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;
/*
class App : public Application
{*/
	int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
//	int main(int argc, char** argv)
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupGL setup_gl;
		ConsoleWindow console("Console");

		try
		{
			// SCENARIO 1: DisplayWindow / Sprite
			DisplayWindow window("RenderBox", 206, 232);
			GraphicContext gc = window.get_gc();
			
			gc.set_map_mode(cl_map_2d_upper_left);

			Sprite spr1(gc, "testimage.png");
			while(1)
			{
				gc.clear();

				Draw::line(gc, 0, 0, 200, 200, Colorf::yellow);
//				Draw::box(gc, Rectf(0,0,200,200), Colorf::yellow);
//				Draw::fill(gc, Rectf(0,0,200,200), Colorf::yellow);
//				spr1.draw(gc, Rectf(0,0, 16,16), Rectf(0,0,200,200));
//				spr1.draw(gc, 0, 0);
				window.flip(1);
				KeepAlive::process();

				System::sleep(10);
			}

			// SCENARIO 2: GUI
			
/*			DisplayCache resources;
			resources.load("resources.xml");

			GUIManager gui;

			GUIWindowManagerSystem wm;
			gui.set_window_manager(&wm);

			GUIThemeDefault theme;
			theme.set_resources(resources);

			gui.set_theme(&theme);
			gui.set_css_document("theme.css");

			DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("RenderBox"); 

			GUIComponent root(Rect(0,0,208,234), &gui, win_desc);
			root.set_class_name("root");

			Label label1(&root);
			label1.set_geometry(Rect(0, 0, 200, 200));
			label1.set_text("Yellow label");

			AcceleratorTable at;
			gui.exec(at);
*/		}
		catch (Exception e)
		{
 			Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}
//} app;
