
#include "precomp.h"
#include "hid.h"

HIDInputDeviceList HID::get_device_list()
{
	HIDInputDeviceList result;
	int retval = GetRawInputDeviceList(0, &result.num_devices, sizeof(RAWINPUTDEVICELIST));
	if (retval != 0)
		throw Exception("GetRawInputDeviceList failed (1)");

	result.data = DataBuffer(result.num_devices * sizeof(RAWINPUTDEVICELIST));
	retval = GetRawInputDeviceList(result.get_list(), &result.num_devices, sizeof(RAWINPUTDEVICELIST));
	if (retval != result.num_devices)
		throw Exception("GetRawInputDeviceList failed (2)");
	return result;
}

HIDInputDeviceInfo HID::get_device_info(HANDLE device)
{
	HIDInputDeviceInfo result;
	result.data = DataBuffer(sizeof(RID_DEVICE_INFO));
	result.get_info()->cbSize = sizeof(RID_DEVICE_INFO);
	UINT size = result.data.get_size();
	UINT retval = GetRawInputDeviceInfo(device, RIDI_DEVICEINFO, result.get_info(), &size);
	if (retval < 0)
	{
		result.data = DataBuffer(size);
		result.get_info()->cbSize = sizeof(RID_DEVICE_INFO);
		retval = GetRawInputDeviceInfo(device, RIDI_DEVICEINFO, result.get_info(), &size);
		if (retval < 0)
			throw Exception("GetRawInputDeviceInfo failed for RIDI_DEVICEINFO");
	}
	return result;
}

std::string HID::get_device_name(HANDLE device)
{
	UINT name_len = 1024;
	TCHAR device_name[1024];
	UINT result = GetRawInputDeviceInfo(device, RIDI_DEVICENAME, device_name, &name_len);
	if (result < 0 || result >= 1024)
		throw Exception("GetRawInputDeviceInfo failed for RIDI_DEVICENAME");
	device_name[result] = 0;
	return device_name;
}

void HID::register_input_devices(const std::vector<HIDInputDeviceInfo> &devices, HWND target)
{
	if (!devices.empty())
	{
		std::vector<RAWINPUTDEVICE> raw_input_devices;
		raw_input_devices.resize(devices.size());
		for (std::vector<HIDInputDeviceInfo>::size_type i = 0; i < devices.size(); i++)
		{
			RAWINPUTDEVICE *dev = &raw_input_devices[i];
			dev->hwndTarget = target;
			dev->dwFlags = 0;
			switch (devices[i].get_info()->dwType)
			{
			case RIM_TYPEMOUSE:
				dev->usUsagePage = 1;
				dev->usUsage = 2;
				break;

			case RIM_TYPEKEYBOARD:
				dev->usUsagePage = 1;
				dev->usUsage = 6; 
				break;

			default:
				dev->usUsage = devices[i].get_info()->hid.usUsage;
				dev->usUsagePage = devices[i].get_info()->hid.usUsagePage;
				break;
			}
		}

		BOOL result = RegisterRawInputDevices(&raw_input_devices[0], raw_input_devices.size(), sizeof(RAWINPUTDEVICE));
		if (result == FALSE)
			throw Exception("RegisterRawInputDevices");
	}
}

HIDRawInput HID::get_raw_input(HRAWINPUT handle)
{
	HIDRawInput result;
	UINT header_size = 0;
	UINT retval = GetRawInputData(handle, RID_INPUT, 0, &header_size, sizeof(RAWINPUTHEADER));
	if (retval < 0)
		throw Exception("GetRawInputData failed (1)");
	result.data = DataBuffer(header_size);
	retval = GetRawInputData(handle, RID_INPUT, result.get_input(), &header_size, sizeof(RAWINPUTHEADER));
	if (retval < 0)
		throw Exception("GetRawInputData failed (2)");

	return result;
}

HIDRawInput HID::get_raw_input_header(HRAWINPUT handle)
{
	HIDRawInput result;
	UINT header_size = 0;
	UINT retval = GetRawInputData(handle, RID_HEADER, 0, &header_size, sizeof(RAWINPUTHEADER));
	if (retval < 0)
		throw Exception("GetRawInputData failed (1)");
	result.data = DataBuffer(header_size);
	retval = GetRawInputData(handle, RID_HEADER, result.get_input(), &header_size, sizeof(RAWINPUTHEADER));
	if (retval < 0)
		throw Exception("GetRawInputData failed (2)");
	return result;
}
