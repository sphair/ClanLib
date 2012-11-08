
#include "precomp.h"
#include "program.h"
#include <ClanLib/application.h>
#include "win32_hid_window.h"

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;

	ConsoleWindow console("Test");

	Win32HidWindow hid_window;

	for (size_t i = 0; i < hid_window.devices.size(); i++)
	{
		Console::write_line("product: %1, manufacturer: %2, serial: %3, physical descriptor: %4", hid_window.devices[i]->product_name, hid_window.devices[i]->manufacturer_name, hid_window.devices[i]->serial_number, hid_window.devices[i]->physical_descriptor);
	}

	while (!kbhit())
	{
		for (size_t i = 0; i < hid_window.devices.size(); i++)
		{
			for (auto it = hid_window.devices[i]->buttons.begin(); it != hid_window.devices[i]->buttons.end(); ++it)
			{
				int id = it->first;
				bool down = it->second;
				if (down)
				{
					Console::write_line(hid_window.devices[i]->button_id_to_name[id]);
				}
			}

			for (auto it = hid_window.devices[i]->values.begin(); it != hid_window.devices[i]->values.end(); ++it)
			{
				int id = it->first;
				float value = it->second;
				if (value < -0.1 || value > 0.1)
				{
					Console::write_line("%1: %2", hid_window.devices[i]->value_id_to_name[id], value);
				}
			}
		}

		KeepAlive::process(10);
	}

	console.display_close_message();
	return 0;
}
