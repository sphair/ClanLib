
#pragma once

#include "hid.h"
#include <map>

class Win32HidInputDevice
{
public:
	Win32HidInputDevice(RAWINPUTDEVICELIST device_list_entry);

	static std::vector<std::shared_ptr<Win32HidInputDevice> > create_devices();
	static void register_raw_input_devices(HWND window_handle);
	static clan::DataBuffer get_rawinput_data(HRAWINPUT rawinput_handle);

	void update(RAWINPUT *raw_input);

	std::string product_name;
	std::string manufacturer_name;
	std::string serial_number;
	std::string physical_descriptor;

	std::map<int, std::string> button_id_to_name;
	std::map<int, std::string> value_id_to_name;

	// See "HID Game Controllers and DirectInput" at MSDN for a full list of usages: http://msdn.microsoft.com/en-us/library/windows/hardware/gg487464.aspx

	static const int id_x = 0x30;
	static const int id_y = 0x31;
	static const int id_z = 0x32;
	static const int id_rx = 0x33;
	static const int id_ry = 0x34;
	static const int id_rz = 0x35;
	static const int id_slider = 0x36;
	static const int id_dial = 0x37;
	static const int id_wheel = 0x38;
	static const int id_hat = 0x39;

	std::map<int, bool> buttons;
	std::map<int, float> values;

private:
	HANDLE open_device(RAWINPUTDEVICELIST device_list_entry);
	clan::DataBuffer get_preparse_data();

	void find_names(HANDLE device);
	void find_button_names(HANDLE device, void *preparse_data);
	void find_value_names(HANDLE device, void *preparse_data);

	void update(void *preparse_data, void *report, int report_size);
	void update_buttons(void *preparse_data, void *report, int report_size);
	void update_values(void *preparse_data, void *report, int report_size);

	Hid hid;
	HANDLE rawinput_device_handle;
};
