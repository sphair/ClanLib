#include <ClanLib/core.h>
#include <ClanLib/gui.h>
//#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
/*
class App : public CL_ClanApplication
{*/
	int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
//	int main(int argc, char** argv)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
		CL_ConsoleWindow console("Console");

		try
		{
			// SCENARIO 1: DisplayWindow / Sprite
			CL_DisplayWindow window("RenderBox", 206, 232);
			CL_GraphicContext gc = window.get_gc();
			
			gc.set_map_mode(cl_map_2d_upper_left);

			CL_Sprite spr1(gc, "testimage.png");
			while(1)
			{
				gc.clear();

				CL_Draw::line(gc, 0, 0, 200, 200, CL_Colorf::yellow);
//				CL_Draw::box(gc, CL_Rectf(0,0,200,200), CL_Colorf::yellow);
//				CL_Draw::fill(gc, CL_Rectf(0,0,200,200), CL_Colorf::yellow);
//				spr1.draw(gc, CL_Rectf(0,0, 16,16), CL_Rectf(0,0,200,200));
//				spr1.draw(gc, 0, 0);
				window.flip(1);
				CL_KeepAlive::process();

				CL_System::sleep(10);
			}

			// SCENARIO 2: GUI
			
/*			CL_ResourceManager resources;
			resources.load("resources.xml");

			CL_GUIManager gui;

			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(&wm);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);

			gui.set_theme(&theme);
			gui.set_css_document("theme.css");

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("RenderBox"); 

			CL_GUIComponent root(CL_Rect(0,0,208,234), &gui, win_desc);
			root.set_class_name("root");

			CL_Label label1(&root);
			label1.set_geometry(CL_Rect(0, 0, 200, 200));
			label1.set_text("Yellow label");

			CL_AcceleratorTable at;
			gui.exec(at);
*/		}
		catch (CL_Exception e)
		{
 			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}
//} app;
