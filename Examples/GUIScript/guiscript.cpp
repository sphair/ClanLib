#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/gl.h>
#include <ClanLib/guistylesilver.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class GUIScript : public CL_ClanApplication
{
public:
	int main(int argc, char** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore setup_core;
			CL_SetupGUI setup_gui;
			CL_SetupGL setup_gl;
			CL_SetupDisplay setup_display;
		 
			CL_DisplayWindow window("GUIScript", 640, 480);

			CL_ResourceManager resources("../../Resources/GUIStyleSilver/gui.xml");
			CL_ResourceManager extra_resources("guiscript.xml");
			resources.add_resources(extra_resources);

			CL_StyleManager_Silver style(&resources);
			CL_GUIManager gui(&style);
			gui_manager = &gui;

			CL_ComponentManager manager("gui", &resources, &gui);
			component_manager = &manager;

			CL_Slot s = ((CL_Button *) manager.get_component("button1"))->sig_clicked().connect(
				this, &GUIScript::on_button_clicked);

			CL_Slot slot_paint = gui.sig_paint().connect(this, &GUIScript::on_gui_paint);

			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &GUIScript::on_window_close);
			quit = false;

			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && !quit)
			{
				gui.show();

				CL_Display::flip();
				CL_System::keep_alive(10);
			}
		}
		catch (CL_Error e)
		{
			std::cout << e.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}

private:
	void on_window_close()
	{
		quit = true;
	}

	void on_gui_paint()
	{
		CL_Display::clear(CL_Color(150, 150, 150));
	}

	void on_button_clicked()
	{
		CL_MessageBox::info(((CL_InputBox *) component_manager->get_component("inputbox1"))->get_text(), gui_manager);
	}

	CL_GUIManager *gui_manager;
	CL_ComponentManager *component_manager;

	bool quit;
} app;
