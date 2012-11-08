
#include "precomp.h"
#include "win32_hid_input_device.h"

using namespace clan;

// See http://msdn.microsoft.com/en-us/library/ms789918.aspx

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC		((USHORT) 0x01)
#endif

#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE	((USHORT) 0x02)
#endif

#ifndef HID_USAGE_GENERIC_JOYSTICK
#define HID_USAGE_GENERIC_JOYSTICK	((USHORT) 0x04)
#endif

#ifndef HID_USAGE_GENERIC_GAMEPAD
#define HID_USAGE_GENERIC_GAMEPAD	((USHORT) 0x05)
#endif

#ifndef RIDEV_INPUTSINK
#define RIDEV_INPUTSINK	(0x100)
#endif

Win32HidInputDevice::Win32HidInputDevice(RAWINPUTDEVICELIST device_list)
: rawinput_device_handle(device_list.hDevice)
{
	DataBuffer preparse_data = get_preparse_data();
	HANDLE device = open_device(device_list);
	try
	{
		find_names(device);
		find_button_names(device, preparse_data.get_data());
		find_value_names(device, preparse_data.get_data());
		CloseHandle(device);
	}
	catch (...)
	{
		CloseHandle(device);
		throw;
	}
}

void Win32HidInputDevice::update(RAWINPUT *raw_input)
{
	DataBuffer preparse_data = get_preparse_data();

	for (DWORD i = 0; i < raw_input->data.hid.dwCount; i++)
	{
		BYTE *raw_data = const_cast<BYTE *>(raw_input->data.hid.bRawData);
		DWORD offset = i * raw_input->data.hid.dwSizeHid;

		void *report = raw_data + offset;
		int report_size = raw_input->data.hid.dwSizeHid;
		update(preparse_data.get_data(), report, report_size);
	}
}

DataBuffer Win32HidInputDevice::get_rawinput_data(HRAWINPUT rawinput_handle)
{
	UINT size = 0;
	UINT result = GetRawInputData(rawinput_handle, RID_INPUT, 0, &size, sizeof(RAWINPUTHEADER));
	if (result != 0 || size < 0)
		throw Exception("GetRawInputData failed");

	DataBuffer buffer(size);
	result = GetRawInputData(rawinput_handle, RID_INPUT, buffer.get_data(), &size, sizeof(RAWINPUTHEADER));
	if (result <= 0)
		throw Exception("GetRawInputData failed");

	return buffer;
}

void Win32HidInputDevice::update(void *preparse_data, void *report, int report_size)
{
	update_buttons(preparse_data, report, report_size);
	update_values(preparse_data, report, report_size);
}

void Win32HidInputDevice::update_buttons(void *preparse_data, void *report, int report_size)
{
	buttons.clear();

	Hid::CAPS caps;
	hid.GetCaps(preparse_data, &caps);

	std::vector<Hid::BUTTON_CAPS> button_caps(caps.NumberInputButtonCaps);
	if (!button_caps.empty())
	{
		USHORT num_button_caps = button_caps.size();
		hid.GetButtonCaps(Hid::HidP_Input, &button_caps[0], &num_button_caps, preparse_data);
		for (size_t j = 0; j < button_caps.size(); j++)
		{
			ULONG usage_length = button_caps[j].Range.UsageMax - button_caps[j].Range.UsageMin + 1;
			std::vector<Hid::USAGE> usages(usage_length);
			hid.GetUsages(Hid::HidP_Input, button_caps[j].UsagePage, 0, &usages[0], &usage_length, preparse_data, report, report_size);

			std::vector<bool> button_values(usage_length);
			for (size_t i = 0; i < usage_length; i++)
				buttons[usages[i]] = true;
		}
	}
}

void Win32HidInputDevice::update_values(void *preparse_data, void *report, int report_size)
{
	values.clear();

	Hid::CAPS caps;
	hid.GetCaps(preparse_data, &caps);

	std::vector<Hid::VALUE_CAPS> value_caps(caps.NumberInputValueCaps);
	if (!value_caps.empty())
	{
		USHORT num_value_caps = value_caps.size();
		hid.GetValueCaps(Hid::HidP_Input, &value_caps[0], &num_value_caps, preparse_data);
		for (size_t j = 0; j < value_caps.size(); j++)
		{
			ULONG value = 0;
			for (Hid::USAGE usage = value_caps[j].Range.UsageMin; usage <= value_caps[j].Range.UsageMax; usage++)
			{
				hid.GetUsageValue(Hid::HidP_Input, value_caps[j].UsagePage, 0, usage, &value, preparse_data, report, report_size);

				// To do: figure out how we can tell if a value should be normalized or not, and also what the normalization range is

				switch (usage)
				{
				default:
				case id_x:
				case id_y:
				case id_z:
				case id_rx:
				case id_ry:
				case id_rz:
				case id_slider:
				case id_dial:
				case id_wheel:
					values[usage] = clamp((value - value_caps[j].LogicalMin) / (float)(value_caps[j].LogicalMax - value_caps[j].LogicalMin) * 2.0f - 1.0f, -1.0f, 1.0f);
					break;
				case id_hat:
					values[usage] = static_cast<float>(value);
					break;
				}
			}
		}
	}
}

std::vector<std::shared_ptr<Win32HidInputDevice> > Win32HidInputDevice::create_devices()
{
	std::vector<std::shared_ptr<Win32HidInputDevice> > devices;

	UINT num_devices = 0;
	UINT result = GetRawInputDeviceList(0, &num_devices, sizeof(RAWINPUTDEVICELIST));
	if (result != (UINT)-1 && num_devices > 0)
	{
		std::vector<RAWINPUTDEVICELIST> device_list(num_devices);
		result = GetRawInputDeviceList(&device_list[0], &num_devices, sizeof(RAWINPUTDEVICELIST));
		if (result == (UINT)-1)
			throw Exception("GetRawInputDeviceList failed");

		for (size_t i = 0; i < device_list.size(); i++)
		{
			RID_DEVICE_INFO device_info;
			UINT device_info_size = sizeof(RID_DEVICE_INFO);
			result = GetRawInputDeviceInfo(device_list[i].hDevice, RIDI_DEVICEINFO, &device_info, &device_info_size);
			if (result == (UINT)-1)
				throw Exception("GetRawInputDeviceInfo failed");

			if (device_info.dwType == RIM_TYPEHID)
			{
				if (device_info.hid.usUsagePage == HID_USAGE_PAGE_GENERIC && (device_info.hid.usUsage == HID_USAGE_GENERIC_JOYSTICK || device_info.hid.usUsage == HID_USAGE_GENERIC_GAMEPAD))
				{
					devices.push_back(std::shared_ptr<Win32HidInputDevice>(new Win32HidInputDevice(device_list[i])));
				}
			}
		}
	}

	return devices;
}

void Win32HidInputDevice::find_names(HANDLE device)
{
	const int max_name_length = 1024;
	WCHAR name[max_name_length];

	if (hid.GetProductString(device, name, max_name_length * sizeof(WCHAR)))
		product_name = StringHelp::ucs2_to_utf8(name);

	if (hid.GetManufacturerString(device, name, max_name_length * sizeof(WCHAR)))
		manufacturer_name = StringHelp::ucs2_to_utf8(name);

	if (hid.GetSerialNumberString(device, name, max_name_length * sizeof(WCHAR)))
		serial_number = StringHelp::ucs2_to_utf8(name);

	if (hid.GetPhysicalDescriptor(device, name, max_name_length * sizeof(WCHAR)))
		physical_descriptor = StringHelp::ucs2_to_utf8(name);
}

void Win32HidInputDevice::find_button_names(HANDLE device, void *preparse_data)
{
	Hid::CAPS caps;
	hid.GetCaps(preparse_data, &caps);

	std::vector<Hid::BUTTON_CAPS> button_caps(caps.NumberInputButtonCaps);
	if (!button_caps.empty())
	{
		USHORT num_button_caps = button_caps.size();
		hid.GetButtonCaps(Hid::HidP_Input, &button_caps[0], &num_button_caps, preparse_data);
		for (size_t i = 0; i < button_caps.size(); i++)
		{
			for (size_t j = button_caps[i].Range.UsageMin; j <= button_caps[i].Range.UsageMax; j++)
			{
				int button_id = j;
				button_id_to_name[button_id] = string_format("button_%1", button_id);
			}
		}
	}
}

void Win32HidInputDevice::find_value_names(HANDLE device, void *preparse_data)
{
	Hid::CAPS caps;
	hid.GetCaps(preparse_data, &caps);

	std::vector<Hid::VALUE_CAPS> value_caps(caps.NumberInputValueCaps);
	if (!value_caps.empty())
	{
		USHORT num_value_caps = value_caps.size();
		hid.GetValueCaps(Hid::HidP_Input, &value_caps[0], &num_value_caps, preparse_data);
		for (size_t i = 0; i < value_caps.size(); i++)
		{
			for (size_t j = value_caps[i].Range.UsageMin; j <= value_caps[i].Range.UsageMax; j++)
			{
				int value_id = j;
				value_id_to_name[value_id] = string_format("value_%1", value_id);
			}
		}
	}
}

HANDLE Win32HidInputDevice::open_device(RAWINPUTDEVICELIST device_list)
{
	UINT name_size = 0;
	UINT result = GetRawInputDeviceInfo(device_list.hDevice, RIDI_DEVICENAME, 0, &name_size);
	if (result == (UINT)-1 || name_size == 0)
		throw Exception("GetRawInputDeviceInfo failed");

	std::unique_ptr<WCHAR[]> name_buffer(new WCHAR[name_size]);
	result = GetRawInputDeviceInfo(device_list.hDevice, RIDI_DEVICENAME, name_buffer.get(), &name_size);
	if (result == (UINT)-1)
		throw Exception("GetRawInputDeviceInfo failed");

	HANDLE device_handle = CreateFile(name_buffer.get(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, OPEN_EXISTING, 0, 0);
	if (device_handle == INVALID_HANDLE_VALUE)
		throw Exception("Unable to open input device");

	return device_handle;
}

DataBuffer Win32HidInputDevice::get_preparse_data()
{
	UINT preparse_data_size = 0;
	UINT result = GetRawInputDeviceInfo(rawinput_device_handle, RIDI_PREPARSEDDATA, 0, &preparse_data_size);
	if (result == (UINT)-1)
		throw Exception("GetRawInputDeviceInfo failed");

	DataBuffer preparse_data(preparse_data_size);
	result = GetRawInputDeviceInfo(rawinput_device_handle, RIDI_PREPARSEDDATA, preparse_data.get_data(), &preparse_data_size);
	if (result == (UINT)-1)
		throw Exception("GetRawInputDeviceInfo failed");

	return preparse_data;
}

void Win32HidInputDevice::register_raw_input_devices(HWND window_handle)
{
	#ifndef RIDEV_INPUTSINK
	#define RIDEV_INPUTSINK	(0x100)
	#endif

	RAWINPUTDEVICE rid[3];

	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[0].dwFlags = RIDEV_INPUTSINK;
	rid[0].hwndTarget = window_handle;

	rid[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[1].usUsage = HID_USAGE_GENERIC_JOYSTICK;
	rid[1].dwFlags = RIDEV_INPUTSINK;
	rid[1].hwndTarget = window_handle;

	rid[2].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[2].usUsage = HID_USAGE_GENERIC_GAMEPAD;
	rid[2].dwFlags = RIDEV_INPUTSINK;
	rid[2].hwndTarget = window_handle;

	BOOL result = RegisterRawInputDevices(rid, 3, sizeof(RAWINPUTDEVICE));
	if (result == FALSE)
		throw Exception("RegisterRawInputDevices failed");
}
