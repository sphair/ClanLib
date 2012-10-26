
#include "precomp.h"
#include "hid_window.h"
#include "hid.h"

HIDWindow::HIDWindow()
{
	std::vector<HIDInputDeviceInfo> register_devices;
	HIDInputDeviceList device_list = HID::get_device_list();
	for (unsigned int i = 0; i < device_list.get_count(); i++)
	{
		Console::write_line("Device %1: %2", i, HID::get_device_name(device_list.get_entry(i).hDevice));
		HIDInputDeviceInfo device_info = HID::get_device_info(device_list.get_entry(i).hDevice);
		register_devices.push_back(device_info);
	}
	HID::register_input_devices(register_devices, get_handle());
}

HIDWindow::~HIDWindow()
{
}

LRESULT HIDWindow::window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_INPUT)
	{
		int input_code = GET_RAWINPUT_CODE_WPARAM(wparam); // RIM_INPUT or RIM_INPUTSINK
		HRAWINPUT handle_raw_input = reinterpret_cast<HRAWINPUT>(lparam);
		HIDRawInput raw_input = HID::get_raw_input(handle_raw_input);

		if (raw_input.get_input()->header.dwType == RIM_TYPEHID)
		{
			if (raw_input.get_input()->data.hid.dwCount > 0)
			{
				int message_index = 0;
				int message_size = raw_input.get_input()->data.hid.dwSizeHid;
				BYTE *message = raw_input.get_input()->data.hid.bRawData + message_size*message_index;

				Console::write_line("WM_INPUT %1 %2 %3 %4", (int) message[0], (int) message[1], (int) message[2], (int) message[3]);
			}
		}

		return Window::window_proc(hwnd, msg, wparam, lparam);
	}
	else if (msg == WM_CLOSE)
	{
		PostQuitMessage(42);
		return 0;
	}
	else
	{
		return Window::window_proc(hwnd, msg, wparam, lparam);
	}
}
