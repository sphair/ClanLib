/** 
 ** Test application for ClanLib input devices
 **/

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>

class InputTest : public CL_ClanApplication
{
public:
	CL_CommandLine argp;

	int main(int argc, char** argv)
	{
		try
		{
			CL_SetupCore setup_core;
			
			parse_options(argc, argv);
			read_options();
		}
		catch (CL_Error& err) 
		{
			std::cout << argv[0] << err.message << std::endl;
		}
		
		return 0;
	}

	void parse_options(int argc, char** argv) 
	{
		argp.add_usage("[OPTIONS]... [FILES]...");
		argp.add_doc("Test application ofr ClanLib input devices");
		argp.add_option('l',	  "list", "", "List input devices");
		argp.add_option('t',	  "test", "DEVICENAME", "Test the given device");
		argp.add_option('a',	  "testall", "", "Test all devices");
		argp.add_option('h',	  "help",	"",	  "Print this help text");

		argp.parse_args(argc, argv);
	}

	void read_options() 
	{
		while (argp.next())
		{
			switch (argp.get_key()) 
			{
			case 'h':
				argp.print_help();
				break;
				
			case 'a':
				test_all();
				break;

			case 't':
				test_device(argp.get_argument());
				break;

			case 'l':
				list_devices();
				break;

			default:
				std::cout << "Got " << argp.get_key() << " " << argp.get_argument() << std::endl;
				break;
			}
		}

		argp.print_help();
	}

	void show_device(CL_InputDevice device)
	{
		std::cout << "Name:    " << device.get_name() << std::endl;
		std::cout << "Device:  " << device.get_device_name() << std::endl;
		std::cout << "Type:    " << device.get_type() << std::endl;
		std::cout << "Buttons: " << device.get_button_count() << std::endl;
		std::cout << "Axis:    " << device.get_axis_count() << std::endl;
		std::cout << std::endl;
	}

	void list_devices()
	{
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
		CL_SetupGUI setup_gui;
		
		CL_DisplayWindow display("Input Test", 640, 480);

		CL_InputContext* ic = display.get_ic();

		std::cout << "Device Overview:\n" 
					 << "================\n";
		std::cout << "Joysticks: " << ic->get_joystick_count() << std::endl;
		std::cout << "Mice:      " <<  ic->get_mouse_count()   << std::endl;
		std::cout << "Keyboards: " << ic->get_keyboard_count() << std::endl;
		std::cout << std::endl;

		std::cout << "Detailed Device List:\n" 
					 << "================\n";
		std::cout << std::endl;
		
		std::cout << "Joysticks:\n"
					 << "~~~~~~~~~~\n";
		for(int i = 0; i < ic->get_joystick_count(); ++i)
			show_device(ic->get_joystick(i));
		std::cout << std::endl;

		std::cout << "Mice:\n"
					 << "~~~~~\n";
		for(int i = 0; i < ic->get_mouse_count(); ++i)
			show_device(ic->get_mouse(i));
		std::cout << std::endl;

		std::cout << "Keyboards:\n"
					 << "~~~~~~~~~~\n";
		for(int i = 0; i < ic->get_keyboard_count(); ++i)
			show_device(ic->get_keyboard(i));
		std::cout << std::endl;
	}

	void test_all()
	{
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
		CL_SetupGUI setup_gui;
		
		CL_DisplayWindow display("Input Test", 640, 480);

		CL_InputContext* ic = display.get_ic();

		std::vector<CL_Slot> slots;		

		for(int i = 0; i < ic->get_joystick_count(); ++i)
		{
			CL_InputDevice device = ic->get_joystick(i);

			slots.push_back(device.sig_key_down().connect(this, &InputTest::key_down));
			slots.push_back(device.sig_key_up().connect(this, &InputTest::key_up));
			slots.push_back(device.sig_pointer_move().connect(this, &InputTest::pointer_move));
			slots.push_back(device.sig_ball_move().connect(this, &InputTest::ball_move));
			slots.push_back(device.sig_axis_move().connect(this, &InputTest::axis_move));
		}

		for(int i = 0; i < ic->get_mouse_count(); ++i)
		{
			CL_InputDevice device = ic->get_mouse(i);

			slots.push_back(device.sig_key_down().connect(this, &InputTest::key_down));
			slots.push_back(device.sig_key_up().connect(this, &InputTest::key_up));
			slots.push_back(device.sig_pointer_move().connect(this, &InputTest::pointer_move));
			slots.push_back(device.sig_ball_move().connect(this, &InputTest::ball_move));
			slots.push_back(device.sig_axis_move().connect(this, &InputTest::axis_move));
		}

		for(int i = 0; i < ic->get_keyboard_count(); ++i)
		{
			CL_InputDevice device = ic->get_keyboard(i);

			slots.push_back(device.sig_key_down().connect(this, &InputTest::key_down));
			slots.push_back(device.sig_key_up().connect(this, &InputTest::key_up));
			slots.push_back(device.sig_pointer_move().connect(this, &InputTest::pointer_move));
			slots.push_back(device.sig_ball_move().connect(this, &InputTest::ball_move));
			slots.push_back(device.sig_axis_move().connect(this, &InputTest::axis_move));
		}

		while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
		{
			CL_System::keep_alive(10);
		}
	}

	void test_device(const std::string& devicename)
	{
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
		CL_SetupGUI setup_gui;
		
		CL_DisplayWindow display("Input Test", 640, 480);

		CL_InputContext* ic = display.get_ic();
		
		CL_InputDevice device = ic->get_device(devicename);

		std::vector<CL_Slot> slots;
		
		slots.push_back(device.sig_key_down().connect(this, &InputTest::key_down));
		slots.push_back(device.sig_key_up().connect(this, &InputTest::key_up));
		slots.push_back(device.sig_pointer_move().connect(this, &InputTest::pointer_move));
		slots.push_back(device.sig_ball_move().connect(this, &InputTest::ball_move));
		slots.push_back(device.sig_axis_move().connect(this, &InputTest::axis_move));

		while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
		{
			CL_System::keep_alive(10);
		}
	}

	void key_down(const CL_InputEvent& event)
	{
		std::cout << "key_down: " << event << std::endl;
	}

	void key_up(const CL_InputEvent& event)
	{
		std::cout << "key_up: " << event << std::endl;		
	}

	void pointer_move(const CL_InputEvent& event)
	{
		std::cout << "pointer_move: " << event << std::endl;		
	}

	void axis_move(const CL_InputEvent& event)
	{
		std::cout << "axis_move: " << event << std::endl;		
	}

	void ball_move(const CL_InputEvent& event)
	{
		std::cout << "ball_move: " << event << std::endl;		
	}
	
} app;

/* EOF */
