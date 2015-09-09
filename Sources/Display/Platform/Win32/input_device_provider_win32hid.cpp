/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Display/Window/input_event.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Math/cl_math.h"
#include "input_device_provider_win32hid.h"
#include <algorithm>

#undef min
#undef max

namespace clan
{
	InputDeviceProvider_Win32Hid::InputDeviceProvider_Win32Hid(HANDLE rawinput_device) : rawinput_device(rawinput_device)
	{
		DataBuffer preparse_data = get_preparse_data();
		HANDLE device = open_device();
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

	InputDeviceProvider_Win32Hid::~InputDeviceProvider_Win32Hid()
	{
		dispose();
	}

	void InputDeviceProvider_Win32Hid::on_dispose()
	{
	}

	void InputDeviceProvider_Win32Hid::update(RAWINPUT *raw_input)
	{
		if (raw_input->header.hDevice == rawinput_device)
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
	}

	std::string InputDeviceProvider_Win32Hid::get_name() const
	{
		return product_name;
	}

	std::string InputDeviceProvider_Win32Hid::get_device_name() const
	{
		UINT name_size = 0;
		UINT result = GetRawInputDeviceInfo(rawinput_device, RIDI_DEVICENAME, 0, &name_size);
		if (result == (UINT)-1 || name_size == 0)
			throw Exception("GetRawInputDeviceInfo failed");

		std::unique_ptr<WCHAR[]> name_buffer(new WCHAR[name_size]);
		result = GetRawInputDeviceInfo(rawinput_device, RIDI_DEVICENAME, name_buffer.get(), &name_size);
		if (result == (UINT)-1)
			throw Exception("GetRawInputDeviceInfo failed");

		return StringHelp::ucs2_to_utf8(name_buffer.get());
	}

	InputDevice::Type InputDeviceProvider_Win32Hid::get_type() const
	{
		return InputDevice::joystick;
	}

	std::string InputDeviceProvider_Win32Hid::get_key_name(int id) const
	{
		if (id >= 0 && id < button_names.size())
			return button_names[id];
		else
			return std::string();
	}

	bool InputDeviceProvider_Win32Hid::get_keycode(int id) const
	{
		if (id >= 0 && id < button_names.size())
			return buttons[id];
		else
			return false;
	}

	float InputDeviceProvider_Win32Hid::get_axis(int id) const
	{
		if (id >= 0 && id < axis_names.size())
			return axis_values[id];
		else
			return 0.0f;
	}

	std::vector<int> InputDeviceProvider_Win32Hid::get_axis_ids() const
	{
		return axis_ids;
	}

	int InputDeviceProvider_Win32Hid::get_hat(int id) const
	{
		if (id >= 0 && id < hat_names.size())
			return hat_values[id];
		else
			return -1;
	}

	int InputDeviceProvider_Win32Hid::get_button_count() const
	{
		return buttons.size();
	}

	HANDLE InputDeviceProvider_Win32Hid::open_device()
	{
		UINT name_size = 0;
		UINT result = GetRawInputDeviceInfo(rawinput_device, RIDI_DEVICENAME, 0, &name_size);
		if (result == (UINT)-1 || name_size == 0)
			throw Exception("GetRawInputDeviceInfo failed");

		std::unique_ptr<WCHAR[]> name_buffer(new WCHAR[name_size + 1]);
		name_buffer.get()[name_size] = 0;
		result = GetRawInputDeviceInfo(rawinput_device, RIDI_DEVICENAME, name_buffer.get(), &name_size);
		if (result == (UINT)-1)
			throw Exception("GetRawInputDeviceInfo failed");

		//  Windows XP fix: The raw device path in its native form (\??\...). When you have the form \\?\ that is a crutch MS invented to make long path names available on Win32 when NT arrived despite the limitation of the Win32 subsystem to the \?? object directory
		if (name_size > 2)
		{
			WCHAR *ptr = name_buffer.get();
			if ((ptr[0] == '\\') && (ptr[1] == '?'))
				ptr[1] = '\\';
		}

		HANDLE device_handle = CreateFile(name_buffer.get(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, OPEN_EXISTING, 0, 0);
		if (device_handle == INVALID_HANDLE_VALUE)
			throw Exception("Unable to open input device");

		return device_handle;
	}

	DataBuffer InputDeviceProvider_Win32Hid::get_preparse_data()
	{
		UINT preparse_data_size = 0;
		UINT result = GetRawInputDeviceInfo(rawinput_device, RIDI_PREPARSEDDATA, 0, &preparse_data_size);
		if (result == (UINT)-1)
			throw Exception("GetRawInputDeviceInfo failed");

		DataBuffer preparse_data(preparse_data_size);
		result = GetRawInputDeviceInfo(rawinput_device, RIDI_PREPARSEDDATA, preparse_data.get_data(), &preparse_data_size);
		if (result == (UINT)-1)
			throw Exception("GetRawInputDeviceInfo failed");

		return preparse_data;
	}

	void InputDeviceProvider_Win32Hid::find_names(HANDLE device)
	{
		const int max_name_length = 1024;
		WCHAR name[max_name_length];

		if (hid.GetProductString(device, name, max_name_length * sizeof(WCHAR)))
			product_name = StringHelp::ucs2_to_utf8(name);

		if (hid.GetManufacturerString(device, name, max_name_length * sizeof(WCHAR)))
			manufacturer_name = StringHelp::ucs2_to_utf8(name);

		if (hid.GetSerialNumberString(device, name, max_name_length * sizeof(WCHAR)))
			serial_number = StringHelp::ucs2_to_utf8(name);
	}

	void InputDeviceProvider_Win32Hid::find_button_names(HANDLE device, void *preparse_data)
	{
		Hid::CAPS caps;
		hid.GetCaps(preparse_data, &caps);

		std::vector<Hid::BUTTON_CAPS> button_caps(caps.NumberInputButtonCaps);
		if (!button_caps.empty())
		{
			USHORT num_button_caps = button_caps.size();
			hid.GetButtonCaps(Hid::HidP_Input, &button_caps[0], &num_button_caps, preparse_data);
			for (size_t collection = 0; collection < button_caps.size(); collection++)
			{
				for (Hid::USAGE usage = button_caps[collection].Range.UsageMin; usage <= button_caps[collection].Range.UsageMax; usage++)
				{
					std::string name;

					if (button_caps[collection].IsStringRange)
					{
						const int max_name_length = 1024;
						WCHAR buffer[max_name_length];

						int offset = usage - button_caps[collection].Range.UsageMin;
						int string_index = button_caps[collection].Range.StringMin + offset;
						if (hid.GetIndexedString(device, string_index, buffer, max_name_length * sizeof(WCHAR)))
							name = StringHelp::ucs2_to_utf8(buffer);
					}

					if (name.empty())
						name = string_format("Button %1", buttons.size() + 1);

					button_names.push_back(name);
					usage_to_button_index[usage] = buttons.size();
					buttons.push_back(false);
				}
			}
		}
	}

	void InputDeviceProvider_Win32Hid::find_value_names(HANDLE device, void *preparse_data)
	{
		// Place all standard axis and hats at the beginning of the list
		axis_values.resize(9);
		axis_names.push_back("X");
		axis_names.push_back("Y");
		axis_names.push_back("Z");
		axis_names.push_back("Rx");
		axis_names.push_back("Ry");
		axis_names.push_back("Rz");
		axis_names.push_back("Slider");
		axis_names.push_back("Dial");
		axis_names.push_back("Wheel");
		for (int i = 0; i < 9; i++)
			usage_to_axis_index[0x30 + i] = i;
		int next_axis_index = 9;

		hat_values.push_back(-1);
		hat_names.push_back("Hat");
		usage_to_hat_index[0x39] = 0;
		int next_hat_index = 2;

		Hid::CAPS caps;
		hid.GetCaps(preparse_data, &caps);

		std::vector<Hid::VALUE_CAPS> value_caps(caps.NumberInputValueCaps);
		if (!value_caps.empty())
		{
			USHORT num_value_caps = value_caps.size();
			hid.GetValueCaps(Hid::HidP_Input, &value_caps[0], &num_value_caps, preparse_data);
			for (size_t collection = 0; collection < value_caps.size(); collection++)
			{
				for (Hid::USAGE usage = value_caps[collection].Range.UsageMin; usage <= value_caps[collection].Range.UsageMax; usage++)
				{
					if (usage < 0x30 || usage > 0x39)
					{
						if (value_caps[collection].LogicalMin == 0 && value_caps[collection].LogicalMax == 3 && value_caps[collection].HasNull) // Four direction hat
						{
							hat_names.push_back(string_format("Hat%1", next_hat_index++));
							usage_to_hat_index[usage] = hat_values.size();
							//hat_ids.push_back(hat_values.size());
							hat_values.push_back(-1);
						}
						else if (value_caps[collection].LogicalMin == 0 && value_caps[collection].LogicalMax == 7 && value_caps[collection].HasNull) // Eight direction hat
						{
							hat_names.push_back(string_format("Hat%1", next_hat_index++));
							usage_to_hat_index[usage] = hat_values.size();
							//hat_ids.push_back(hat_values.size());
							hat_values.push_back(-1);
						}
						else
						{
							axis_names.push_back(string_format("Axis%1", next_axis_index++));
							usage_to_axis_index[usage] = axis_values.size();
							axis_ids.push_back(axis_values.size());
							axis_values.push_back(0.0f);
						}
					}
					else
					{
						switch (usage)
						{
						case 0x30: axis_ids.push_back(joystick_x); break;
						case 0x31: axis_ids.push_back(joystick_y); break;
						case 0x32: axis_ids.push_back(joystick_z); break;
						case 0x33: axis_ids.push_back(joystick_rx); break;
						case 0x34: axis_ids.push_back(joystick_ry); break;
						case 0x35: axis_ids.push_back(joystick_rz); break;
						case 0x36: axis_ids.push_back(joystick_slider); break;
						case 0x37: axis_ids.push_back(joystick_dial); break;
						case 0x38: axis_ids.push_back(joystick_wheel); break;
							//case 0x39: hat_ids.push_back(joystick_hat); break;
						}
					}
				}
			}
		}
	}

	void InputDeviceProvider_Win32Hid::update(void *preparse_data, void *report, int report_size)
	{
		update_buttons(preparse_data, report, report_size);
		update_values(preparse_data, report, report_size);
	}

	void InputDeviceProvider_Win32Hid::update_buttons(void *preparse_data, void *report, int report_size)
	{
		for (size_t i = 0; i < buttons.size(); i++)
			buttons[i] = false;

		Hid::CAPS caps;
		hid.GetCaps(preparse_data, &caps);

		std::vector<Hid::BUTTON_CAPS> button_caps(caps.NumberInputButtonCaps);
		if (!button_caps.empty())
		{
			USHORT num_button_caps = button_caps.size();
			hid.GetButtonCaps(Hid::HidP_Input, &button_caps[0], &num_button_caps, preparse_data);
			for (size_t collection = 0; collection < button_caps.size(); collection++)
			{
				ULONG array_length = button_caps[collection].Range.UsageMax - button_caps[collection].Range.UsageMin + 1;

				std::vector<Hid::USAGE> usages(array_length);
				std::vector<bool> button_values(array_length);

				ULONG usage_length = array_length;
				hid.GetUsages(Hid::HidP_Input, button_caps[collection].UsagePage, button_caps[collection].LinkCollection, &usages[0], &usage_length, preparse_data, report, report_size);
				usage_length = clan::min(usage_length, array_length);

				for (size_t i = 0; i < usage_length; i++)
				{
					std::map<Hid::USAGE, int>::iterator it = usage_to_button_index.find(usages[i]);
					if (it != usage_to_button_index.end())
						buttons[it->second] = true;
				}
			}
		}
	}

	void InputDeviceProvider_Win32Hid::update_values(void *preparse_data, void *report, int report_size)
	{
		for (size_t i = 0; i < axis_values.size(); i++)
			axis_values[i] = 0.0f;
		for (size_t i = 0; i < hat_values.size(); i++)
			hat_values[i] = -1;

		Hid::CAPS caps;
		hid.GetCaps(preparse_data, &caps);

		std::vector<Hid::VALUE_CAPS> value_caps(caps.NumberInputValueCaps);
		if (!value_caps.empty())
		{
			USHORT num_value_caps = value_caps.size();
			hid.GetValueCaps(Hid::HidP_Input, &value_caps[0], &num_value_caps, preparse_data);
			for (size_t collection = 0; collection < value_caps.size(); collection++)
			{
				ULONG value = 0;
				for (Hid::USAGE usage = value_caps[collection].Range.UsageMin; usage <= value_caps[collection].Range.UsageMax; usage++)
				{
					hid.GetUsageValue(Hid::HidP_Input, value_caps[collection].UsagePage, 0, usage, &value, preparse_data, report, report_size);

					if (value_caps[collection].LogicalMin == 0 && value_caps[collection].LogicalMax == 3 && value_caps[collection].HasNull) // Four direction hat
					{
						std::map<Hid::USAGE, int>::iterator it = usage_to_hat_index.find(usage);
						if (it != usage_to_hat_index.end())
							hat_values[it->second] = (value == 8) ? -1 : (value * 360 / 4);
					}
					else if (value_caps[collection].LogicalMin == 0 && value_caps[collection].LogicalMax == 7 && value_caps[collection].HasNull) // Eight direction hat
					{
						std::map<Hid::USAGE, int>::iterator it = usage_to_hat_index.find(usage);
						if (it != usage_to_hat_index.end())
							hat_values[it->second] = (value == 8) ? -1 : (value * 360 / 8);
					}
					else
					{
						std::map<Hid::USAGE, int>::iterator it = usage_to_axis_index.find(usage);
						if (it != usage_to_axis_index.end())
							axis_values[it->second] = clamp((value - value_caps[collection].LogicalMin) / (float)(value_caps[collection].LogicalMax - value_caps[collection].LogicalMin) * 2.0f - 1.0f, -1.0f, 1.0f);
					}
				}
			}
		}
	}
}
