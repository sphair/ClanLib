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
			CL_SetupCore setup_core;
			CL_SetupDisplay setup_display;
			CL_SetupGL setup_gl;
			CL_SetupGUI setup_gui;

			CL_DisplayWindow display("GUI Layout example", 640, 480, false, true);

			// Connect the Window close event
			CL_Slot slot_quit = display.sig_window_close().connect(this, &App::on_window_close);
			
			// Create the GUI using the default style
			CL_ResourceManager gui_resources("../../Resources/GUIStyleSilver/gui.xml");
			CL_StyleManager_Silver style(&gui_resources);
			CL_GUIManager guimanager(&style);
			gui = &guimanager;

			CL_TreeView tree(&guimanager);
			tree.add_column("Column 1", 100);
			tree.add_column("Column 2", 100);
			tree.add_column("Column 3", 100);
			CL_Label label1("Layout management:", &guimanager);
			CL_InputBox input1("foo", &guimanager);
			CL_Component spacer(&guimanager);
			CL_Button button1("Button1", &guimanager);
			CL_Button button2("Button2", &guimanager);
			CL_Button button3("Button3", &guimanager);
			CL_Button button4("Button4", &guimanager);
			CL_Button button5("Button5", &guimanager);
			CL_Button button6("Button6", &guimanager);

			label1.set_horizontal_policy(layout_minimum);
			button1.set_horizontal_policy(layout_minimum);
			button2.set_horizontal_policy(layout_minimum);
			button3.set_horizontal_policy(layout_minimum);
			button4.set_horizontal_policy(layout_minimum);
			button5.set_horizontal_policy(layout_minimum);
			button6.set_horizontal_policy(layout_minimum);
			label1.set_vertical_policy(layout_minimum);
			input1.set_vertical_policy(layout_minimum);
			spacer.set_horizontal_policy(layout_minimum);
			button1.set_vertical_policy(layout_minimum);
			button2.set_vertical_policy(layout_minimum);
			button3.set_vertical_policy(layout_minimum);
			button4.set_vertical_policy(layout_minimum);
			button5.set_vertical_policy(layout_minimum);
			button6.set_vertical_policy(layout_minimum);

			CL_Layout layout2(layout_horizontal);
			layout2.add_component(&label1);
			layout2.add_component(&input1);
//			layout2.add_component(&spacer);
			layout2.add_component(&button1);
			layout2.add_component(&button2);
			layout2.add_component(&button3);

			CL_Layout layout4(layout_vertical);
			layout4.add_component(&button4);
			layout4.add_component(&button5);
			layout4.add_component(&button6);
			layout4.add_component(&spacer);

			CL_Layout layout3(layout_horizontal);
			layout3.add_component(&tree);
			layout3.add_layout(layout4);

			CL_Layout layout1(layout_vertical);
			layout1.set_margin(4);
			layout1.add_layout(layout3);
			layout1.add_layout(layout2);
			guimanager.set_layout(layout1);
			layout1.set_position(CL_Rect(0, 0, guimanager.get_width(), guimanager.get_height())); // hack, shouldnt be needed.
			
			CL_Slot slot_paint = guimanager.sig_paint().connect(this, &App::on_paint);

			CL_Slot slot_resize1 = button5.sig_clicked().connect(this, &App::on_resize1);
			CL_Slot slot_resize2 = button6.sig_clicked().connect(this, &App::on_resize2);

			guimanager.run();
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
		gui->quit();
	}
	
	void on_paint()
	{
		CL_Display::clear(CL_Color::white);
	}
	
	void on_resize1()
	{
		CL_Display::set_size(640, 480);
	}

	void on_resize2()
	{
		CL_Display::set_size(800, 600);
	}

	CL_GUIManager *gui;
} app;
