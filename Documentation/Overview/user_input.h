
/*! \page UserInput User Input

Input in ClanLib is accessed via a \ref clan::InputContext object, which is usually acquired from a \ref clan::DisplayWindow object. The input context interface is a container holding one or more \ref clan::InputDevice objects, each acting as either a keyboard, mouse, joystick or tablet source. The input device interface provides access to various polling functions or signal callbacks, which are invoked when ClanLib receives these input messages.

The following example shows how to get the input devices for the first keyboard, mouse and joystick: 

\code
clan::DisplayWindow window(640, 480, "Hello Input");
clan::InputContext ic = window.get_ic();
clan::InputDevice keyboard = ic.get_keyboard();
clan::InputDevice mouse = ic.get_mouse();
clan::InputDevice joystick = ic.get_joystick();
\endcode

Polling a device is fairly simple and straight forward (just call the appropriate functions), but the actual state of an input device does not always update immediately - in some cases they are updated based on input messages received from the underlying windowing system, which means that they will not update until you have made a call to \ref clan::KeepAlive::process(). The following code correctly polls for hitting escape to quit the loop: 

\code
clan::DisplayWindow window(640, 480, "Hello Input");
clan::InputContext ic = window.get_ic();
clan::InputDevice keyboard = ic.get_keyboard();
while (true)
{
	bool escape_down = keyboard.get_keycode(clan::KEY_ESCAPE);
	if (escape_down)
		break;
 
	clan::KeepAlive::process();
 
	window.get_gc().clear(clan::Color::white);
	window.flip();
	clan::System::sleep(10);
}
\endcode

An alternative to polling input devices is to set up callback functions that get invoked whenever an input event occurs. To do this, simply connect a slot function to the signal you want to listen in on: 

\code
bool quit = false;
void my_callback(const clan::InputEvent &event, const clan::InputState &state)
{
	if (event.id == clan::KEY_ESCAPE)
		quit = true;
}
 
int main(int, char**)
{
	try
	{
		clan::SetupCore setup_core;
		clan::SetupDisplay setup_display;
		clan::SetupGL setup_gl;
 
		clan::DisplayWindow window(640, 480, "Hello Input");
		clan::InputContext ic = window.get_ic();
		clan::InputDevice keyboard = ic.get_keyboard();
		clan::Slot slot = keyboard.sig_key_up().connect(&my_callback);
 
		while (!quit)
		{
			clan::KeepAlive::process();
			window.get_gc().clear(clan::Color::white);
			window.flip();
			clan::System::sleep(10);
		}
	}
	catch(clan::Exception &exception)
	{
		// Create a console window for text-output if not available
		clan::ConsoleWindow console("Console", 80, 160);
		clan::Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();
 
		return -1;
	}
 
	return 0;
}
\endcode

*/
