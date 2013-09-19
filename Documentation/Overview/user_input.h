
/*! \page UserInput User Input

MainDocs:User Input

From ClanLib Game SDK
 

Input in ClanLib is accessed via a CL_InputContext object, which is usually acquired from a CL_DisplayWindow object. The input context interface is a container holding one or more CL_InputDevice objects, each acting as either a keyboard, mouse, joystick or tablet source. The input device interface provides access to various polling functions or signal callbacks, which are invoked when ClanLib receives these input messages. 

The following example shows how to get the input devices for the first keyboard, mouse and joystick: 


 
CL_DisplayWindow window(640, 480, "Hello Input");
CL_InputContext ic = window.get_ic();
CL_InputDevice keyboard = ic.get_keyboard();
CL_InputDevice mouse = ic.get_mouse();
CL_InputDevice joystick = ic.get_joystick();

Polling a device is fairly simple and straight forward (just call the appropriate functions), but the actual state of an input device does not always update immediately - in some cases they are updated based on input messages received from the underlying windowing system, which means that they will not update until you have made a call to CL_KeepAlive::process(). The following code correctly polls for hitting escape to quit the loop: 


 
CL_DisplayWindow window(640, 480, "Hello Input");
CL_InputContext ic = window.get_ic();
CL_InputDevice keyboard = ic.get_keyboard();
while (true)
{
	bool escape_down = keyboard.get_keycode(CL_KEY_ESCAPE);
	if (escape_down)
		break;
 
	CL_KeepAlive::process();
 
	window.get_gc().clear(CL_Color::white);
	window.flip();
	CL_System::sleep(10);
}

An alternative to polling input devices is to set up callback functions that get invoked whenever an input event occurs. To do this, simply connect a slot function to the signal you want to listen in on: 


 
bool quit = false;
void my_callback(const CL_InputEvent &event, const CL_InputState &state)
{
	if (event.id == CL_KEY_ESCAPE)
		quit = true;
}
 
int main(int, char**)
{
	try
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
 
		CL_DisplayWindow window(640, 480, "Hello Input");
		CL_InputContext ic = window.get_ic();
		CL_InputDevice keyboard = ic.get_keyboard();
		CL_Slot slot = keyboard.sig_key_up().connect(&my_callback);
 
		while (!quit)
		{
			CL_KeepAlive::process();
			window.get_gc().clear(CL_Color::white);
			window.flip();
			CL_System::sleep(10);
		}
	}
	catch(CL_Exception &exception)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console", 80, 160);
		CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();
 
		return -1;
	}
 
	return 0;
}


*/
