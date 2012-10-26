
#pragma once

class HIDInputDeviceList;
class HIDInputDeviceInfo;
class HIDRawInput;

class HID
{
public:
	static HIDInputDeviceList get_device_list();
	static HIDInputDeviceInfo get_device_info(HANDLE device);
	static std::string get_device_name(HANDLE device);
	static void register_input_devices(const std::vector<HIDInputDeviceInfo> &devices, HWND target);
	static HIDRawInput get_raw_input(HRAWINPUT handle);
	static HIDRawInput get_raw_input_header(HRAWINPUT handle);
};

class HIDInputDeviceList
{
public:
	HIDInputDeviceList() : num_devices(0) { }

	unsigned int get_count() const { return num_devices; }
	const RAWINPUTDEVICELIST &get_entry(int index) const { return get_list()[index]; }
	RAWINPUTDEVICELIST *get_list() { return reinterpret_cast<RAWINPUTDEVICELIST *>(data.get_data()); }
	const RAWINPUTDEVICELIST *get_list() const { return reinterpret_cast<const RAWINPUTDEVICELIST *>(data.get_data()); }

private:
	unsigned int num_devices;
	DataBuffer data;
	friend class HID;
};

class HIDInputDeviceInfo
{
public:
	RID_DEVICE_INFO *get_info() { return reinterpret_cast<RID_DEVICE_INFO *>(data.get_data()); }
	const RID_DEVICE_INFO *get_info() const { return reinterpret_cast<const RID_DEVICE_INFO *>(data.get_data()); }

private:
	DataBuffer data;
	friend class HID;
};

class HIDRawInput
{
public:
	RAWINPUT *get_input() { return reinterpret_cast<RAWINPUT *>(data.get_data()); }
	const RAWINPUT *get_input() const { return reinterpret_cast<const RAWINPUT *>(data.get_data()); }

private:
	DataBuffer data;
	friend class HID;
};
