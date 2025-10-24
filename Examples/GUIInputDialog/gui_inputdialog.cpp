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
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			// Initialization
			CL_SetupCore setup_core;
			CL_SetupDisplay setup_display;
			CL_SetupGL setup_gl;
			CL_SetupGUI setup_gui;

			// Creating a window
			CL_DisplayWindow display("GUI Simple example", 640, 480);
			
			// Create the GUI using the silver style
			CL_ResourceManager gui_resources("../../Resources/GUIStyleSilver/gui.xml");
			CL_StyleManager_Silver style(&gui_resources);
			CL_GUIManager gui(&style);

			// Load components from .gui definition file
			CL_ComponentManager components("gui_inputdialog.xml", &gui);

			// Retrieve buttons, to connect signals
			CL_Button *button_1 = (CL_Button *)components.get_component("button_1");
			CL_Button *button_2 = (CL_Button *)components.get_component("button_2");
			CL_Button *button_3 = (CL_Button *)components.get_component("button_3");
			
			CL_SlotContainer slots;

			// Setup hooks for button clicking
			slots.connect(button_1->sig_clicked(), this, &App::on_button_1, &gui);
			slots.connect(button_2->sig_clicked(), this, &App::on_button_2, &gui);
			slots.connect(button_3->sig_clicked(), this, &App::on_button_3, &gui);

			// Setup hook for keypresses
			slots.connect(CL_Keyboard::sig_key_down(), this, &App::on_quit, &gui);

			// Clear screen on each frame
			slots.connect(gui.sig_paint(), this, &App::on_paint);

			// Connect the Window close event
			slots.connect(display.sig_window_close(), this, &App::on_window_close, &gui);

			// Main loop
			gui.run();
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
	void validator_numbers(char &character, bool &accept)
	{
		// I know this is slow and probably wrong at all,
		// but my VC6 don't like std::isdigit with locale :(
		if (character < '0' || character > '9')
			accept = false;
	}
	
	void on_button_1(CL_GUIManager *gui)
	{
		// Creating InputDialog
		CL_InputDialog dlg("Enter some coords...", "Ok", "Cancel", "Ignore", gui);
		
		// Save pointers to input_boxes to connect signals for character validating
		CL_InputBox *x_coord = dlg.add_input_box("X-Coord:", "100");
		CL_InputBox *y_coord = dlg.add_input_box("Y-Coord:", "200");
		CL_CheckBox *checkbox = dlg.add_check_box("Disable this", true);
		
		CL_SlotContainer slots;

		// Connecting signals, to allow only numbers
		slots.connect(x_coord->sig_validate_character(), this, &App::validator_numbers);
		slots.connect(y_coord->sig_validate_character(), this, &App::validator_numbers);

		// Run dialog
		dlg.run();

		// Show some feedback to user
		std::string clicked_button;
		switch(dlg.get_result_button())
		{
		case 0:
			clicked_button = "Ok"; break;
		case 1:
			clicked_button = "Cancel"; break;
		case 2:
			clicked_button = "Ignore"; break;
		default:
			clicked_button = "No dialog buttons clicked"; break;
		}
		CL_MessageBox::info("Clicked button: ", clicked_button, gui);

		// Here you can get entered data from input_boxes
	}
	
	void on_button_2(CL_GUIManager * gui)
	{
		CL_InputDialog dlg("Change password", "Ok", "Cancel", "", gui);
		dlg.add_input_box("Old password:", "", 150)->set_password_mode();
		dlg.add_input_box("New password:", "", 150)->set_password_mode();
		dlg.add_input_box("Confirm password:", "", 150)->set_password_mode();
		dlg.run();
	}
	
	void on_button_3(CL_GUIManager * gui)
	{
		CL_InputDialog dlg("Enter information about you", "Ok", "Cancel", "", gui);
		dlg.add_input_box("First Name:", "Sergey", 150);
		dlg.add_input_box("Last Name:", "Antonov", 150);
		dlg.add_input_box("Country:", "Russia", 150);
		dlg.add_input_box("City:", "Saint-Petersburg", 250);
		dlg.add_input_box("Adress:", "Nevsky, 99-c", 250);
		dlg.add_input_box("Phone:", "456-789-123", 150);
		dlg.add_input_box("Fax:", "456-789-123", 150);
		dlg.add_input_box("E-Mail:", "something@somewhere.com", 200);
		dlg.run();
	}

	void on_quit(const CL_InputEvent &key, CL_GUIManager *gui)
	{
		if (key.id == CL_KEY_ESCAPE)
			gui->quit();
	}

	void on_window_close(CL_GUIManager *gui)
	{
		gui->quit();
	}

	void on_paint()
	{
		CL_Display::clear(CL_Color::antiquewhite);
	}
} app;
