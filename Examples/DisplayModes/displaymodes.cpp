#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/guistylesilver.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

// The main application
class App : public CL_ClanApplication
{
public:
	int main(int argc, char** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console",80,1000);
		console.redirect_stdio();

		try
		{
			CL_SetupCore setup_core;
			CL_SetupDisplay setup_display;
			CL_SetupGL setup_gl;
			CL_SetupGUI setup_gui;

			CL_DisplayWindow display("DisplayModes example", 640, 480);
			
			// Create the GUI using the default style
			CL_ResourceManager gui_resources("../../Resources/GUIStyleSilver/gui.xml");
			CL_StyleManager_Silver style(&gui_resources);
			CL_GUIManager gui(&style);

			window = new CL_Window(CL_Rect(0, 0, 320, 200), "Mode selector", &gui, &style);
			listbox = new CL_ListBox(CL_Rect(10, 10, 140, 164), window->get_client_area());
			button = new CL_Button(CL_Rect(154, 144, 302, 164), "Apply", window->get_client_area());
			new CL_Label(CL_Rect(154, 10, 302, 30), "New mode:", window->get_client_area());
			label = new CL_Label(CL_Rect(164, 30, 302, 50), "Not selected", window->get_client_area());
			checkbox = new CL_CheckBox(CL_Point(164, 50), "Fullscreen", window->get_client_area());
			
			std::vector<CL_DisplayMode> &modes = CL_DisplayMode::get_display_modes();
			for(unsigned int i=0; i<modes.size(); ++i)
			{
				listbox->insert_item(modes[i].get_string());
			}
			
			CL_Slot slot_listbox = listbox->sig_highlighted().connect(this, &App::on_listbox);
			CL_Slot slot_apply = button->sig_clicked().connect(this, &App::on_apply);

			// Connect the Window close event
			CL_Slot slot_quit = display.sig_window_close().connect(this, &App::on_window_close);
			quit = false;
			
			// Main loop
			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && !quit)
			{
				CL_Display::clear();
				gui.show();

				CL_System::keep_alive();
				CL_Display::flip();
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

	void on_listbox(int selection)
	{
		label->set_text(listbox->get_current_text());
	}
	
	void on_apply()
	{
		if(listbox->get_current_item() != -1)
		{
			std::vector<CL_DisplayMode> &modes = CL_DisplayMode::get_display_modes();

			if(checkbox->is_checked())
			{
				CL_Display::set_fullscreen(modes[listbox->get_current_item()]);
			}
			else
			{
				CL_Size res = modes[listbox->get_current_item()].get_resolution();
				CL_Display::set_windowed();
				CL_Display::set_size(res.width, res.height);
			}
		}
	}
	
	bool quit;

	CL_Window *window;
	CL_ListBox *listbox;
	CL_Label *label;
	CL_Button *button;
	CL_CheckBox *checkbox;
} app;
