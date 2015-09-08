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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Display/Window/input_event.h"
#ifdef __MINGW32__
#include "API/Display/Window/display_window.h"
#endif
#include "input_device_provider_win32tablet.h"
#include "win32_window.h"
#include "API/Display/Window/keys.h"

namespace clan
{
	InputDeviceProvider_Win32Tablet::InputDeviceProvider_Win32Tablet(Win32Window *window)
		: InputDeviceProvider(),
		sig_provider_event(0),
		window(window),
		htab(0),
		wintab_dll(0),
		num_keys(0),
		mouse_pos(0, 0),
		packet_queue_size(128),
		packet_queue(0),
		init_successfull(0),
		button_count(0),
		stylus_in_proximity(false)
	{
		if (load_wintab() == false)
		{
			return;
		}

		if (init_tablet() == false)
		{
			return;
		}

		init_successfull = true;
	}

	InputDeviceProvider_Win32Tablet::~InputDeviceProvider_Win32Tablet()
	{
		dispose();
	}

	Pointf InputDeviceProvider_Win32Tablet::get_position() const
	{
		return mouse_pos;
	}

	Point InputDeviceProvider_Win32Tablet::get_device_position() const
	{
		return mouse_pos;
	}

	bool InputDeviceProvider_Win32Tablet::get_keycode(int keycode) const
	{
		if (keycode >= 0 && keycode < button_count)
		{
			return key_states[keycode];
		}

		return false;
	}

	std::string InputDeviceProvider_Win32Tablet::get_key_name(int id) const
	{
		return std::string();
	}

	float InputDeviceProvider_Win32Tablet::get_axis(int index) const
	{
		if (!init_successfull)
			return 0.0f;

		if (index >= 0 && index < axis.size())
		{
			return float(axis[index].pos) / (axis[index].max_value - axis[index].min_value);
		}

		return 0.0f;
	}

	std::string InputDeviceProvider_Win32Tablet::get_name() const
	{
		return std::string();
	}

	std::string InputDeviceProvider_Win32Tablet::get_device_name() const
	{
		return std::string();
	}

	std::vector<int> InputDeviceProvider_Win32Tablet::get_axis_ids() const
	{
		std::vector<int> ids;
		for (size_t i = 0; i < axis.size(); i++)
			ids.push_back(i);
		return ids;
	}

	int InputDeviceProvider_Win32Tablet::get_button_count() const
	{
		return button_count;
	}

	bool InputDeviceProvider_Win32Tablet::device_present() const
	{
		return init_successfull;
	}

	bool InputDeviceProvider_Win32Tablet::is_context_on_top()
	{
		LOGCONTEXT context;
		WTGet(htab, &context);

		return context.lcStatus & CXS_ONTOP;
	}

	void InputDeviceProvider_Win32Tablet::set_context_on_top(bool enable)
	{
		if (enable == true)
		{
			WTEnable(htab, TRUE);
			WTOverlap(htab, TRUE);
		}
		else
		{
			WTEnable(htab, FALSE);
		}
	}

	bool InputDeviceProvider_Win32Tablet::init_tablet()
	{
		// connects tablet to a hwnd

		AXIS            TabletX, TabletY; // The maximum tablet size

		// get default region
		int tablet_connected = WTInfo(WTI_DEFCONTEXT, 0, &logcontext);

		if (!tablet_connected)
			return false;

		// modify the digitizing region

		logcontext.lcOptions |= CXO_MESSAGES;
		logcontext.lcOptions |= CXO_SYSTEM; // make wintab move the system cursor
		logcontext.lcPktData = PACKETDATA;
		logcontext.lcPktMode = PACKETMODE;
		logcontext.lcMoveMask = PACKETDATA;
		logcontext.lcBtnUpMask = logcontext.lcBtnDnMask;

		logcontext.lcPktRate = 100;

		// Set the entire tablet as active
		WTInfo(WTI_DEVICES, DVC_X, &TabletX);
		WTInfo(WTI_DEVICES, DVC_Y, &TabletY);
		logcontext.lcInOrgX = 0;
		logcontext.lcInOrgY = 0;
		logcontext.lcInExtX = TabletX.axMax;
		logcontext.lcInExtY = TabletY.axMax;

		HWND hwnd = window->get_hwnd();

		// output the data in screen coords

		RECT rect;
		if (GetWindowRect(hwnd, &rect) == 0)
		{
			return false;
		}

		HMONITOR monitor = MonitorFromRect(&rect, MONITOR_DEFAULTTONULL);
		previous_monitor = monitor;

		if (!monitor)
			return false;

		MONITORINFO monitor_info;
		ZeroMemory(&monitor_info, sizeof(monitor_info));
		monitor_info.cbSize = sizeof(monitor_info);

		if (GetMonitorInfo(monitor, &monitor_info) != 0)
		{
			logcontext.lcSysOrgX = monitor_info.rcMonitor.left;
			logcontext.lcSysOrgY = monitor_info.rcMonitor.top;
			logcontext.lcSysExtX = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
			logcontext.lcSysExtY = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;

			logcontext.lcOutOrgX = monitor_info.rcMonitor.left;
			logcontext.lcOutOrgY = monitor_info.rcMonitor.top;
			logcontext.lcOutExtX = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
			logcontext.lcOutExtY = -(monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top);

			// open the region
			htab = WTOpen(hwnd, &logcontext, TRUE);
		}

		if (htab == 0)
		{
			return false;
		}

		int packet_queue_size = 128;

		// try with smaller and smaller queue size, untill a supported size is found.
		// the wintab default is 8.
		while (!WTQueueSizeSet(htab, packet_queue_size) && packet_queue_size > 8)
			packet_queue_size /= 2;

		packet_queue = new PACKET[packet_queue_size];

		init_axis();

		WTOverlap(htab, TRUE); // raise to top of context overlap stack.

		// get button count of cursor
		WTInfo(WTI_CURSORS, CSR_BUTTONS, &button_count);

		return true;
	}

	void InputDeviceProvider_Win32Tablet::init_axis()
	{
		// setup x,y and pressure axis... the rest: todo
		axis.clear();

		AXIS TabletX, TabletY, TabletPres;

		// x axis
		WTInfo(WTI_DEVICES, DVC_X, &TabletX);
		axis.push_back(AxisInfo(TabletX.axMin, TabletX.axMax, TabletX.axResolution));

		// y axis
		WTInfo(WTI_DEVICES, DVC_Y, &TabletY);
		axis.push_back(AxisInfo(TabletY.axMin, TabletY.axMax, TabletY.axResolution));

		// pressure axis
		WTInfo(WTI_DEVICES, DVC_NPRESSURE, &TabletPres);
		axis.push_back(AxisInfo(TabletPres.axMin, TabletPres.axMax, TabletPres.axResolution));

		old_axis = axis;
	}

	BOOL InputDeviceProvider_Win32Tablet::process_packet(WPARAM wParam, LPARAM lParam)
	{
		WINDOWINFO winfo; // todo: updating this info could probably be connected to some window move signal
		memset(&winfo, 0, sizeof(WINDOWINFO));
		winfo.cbSize = sizeof(WINDOWINFO);
		if (!GetWindowInfo(window->get_hwnd(), &winfo))
		{
			//int error = GetLastError();
			return FALSE;
		}

		PACKET pkt;

		if (WTPacket((HCTX)wParam, lParam, &pkt))
		{
			if (pkt.pkContext == htab)
			{
				old_axis = axis;

				axis[0].pos = pkt.pkX; // x-axis
				axis[1].pos = pkt.pkY; // y-axis
				axis[2].pos = pkt.pkNormalPressure;	// pressure

				if (HIWORD(pkt.pkButtons) == TBN_DOWN)
				{
					SetActiveWindow(window->get_hwnd());

					int keycode = LOWORD(pkt.pkButtons);
					if ((keycode < 0) || (keycode > 32))
						throw Exception("Invalid keycode received");

					key_states[keycode] = true;
					InputEvent e;

					e.id = tablet_key;
					e.id_offset = keycode;
					e.type = InputEvent::pressed;
					e.mouse_pos = Pointf((pkt.pkX - winfo.rcClient.left) / window->get_pixel_ratio(), (pkt.pkY - winfo.rcClient.top) / window->get_pixel_ratio());
					e.mouse_device_pos = Point(pkt.pkX - winfo.rcClient.left, pkt.pkY - winfo.rcClient.top);
					e.axis_pos = 0;
					e.repeat_count = 0;
					window->set_modifier_keys(e);
					(*sig_provider_event)(e);
					return TRUE;
				}
				else if (HIWORD(pkt.pkButtons) == TBN_UP)
				{
					int keycode = LOWORD(pkt.pkButtons);
					key_states[keycode] = false;
					InputEvent e;

					e.id = tablet_key;
					e.id_offset = keycode;
					e.type = InputEvent::released;
					e.mouse_pos = Pointf((pkt.pkX - winfo.rcClient.left) / window->get_pixel_ratio(), (pkt.pkY - winfo.rcClient.top) / window->get_pixel_ratio());
					e.mouse_device_pos = Point(pkt.pkX - winfo.rcClient.left, pkt.pkY - winfo.rcClient.top);
					e.axis_pos = 0;
					e.repeat_count = 0;
					window->set_modifier_keys(e);
					(*sig_provider_event)(e);
					return TRUE;
				}
				else
				{
					InputEvent e;

					e.id = tablet_z_axis; // TODO: support tilt, rotation.
					e.type = InputEvent::axis_moved; // x,y as pointer movements
					e.mouse_pos = Pointf((pkt.pkX - winfo.rcClient.left) / window->get_pixel_ratio(), (pkt.pkY - winfo.rcClient.top) / window->get_pixel_ratio());
					e.mouse_device_pos = Point(pkt.pkX - winfo.rcClient.left, pkt.pkY - winfo.rcClient.top);
					e.axis_pos = get_axis(2);
					e.repeat_count = 0;
					window->set_modifier_keys(e);
					(*sig_provider_event)(e);
					return TRUE;
				}
			}
		}

		return FALSE;
	}

	BOOL InputDeviceProvider_Win32Tablet::process_proximity(WPARAM wParam, LPARAM lParam)
	{
		InputEvent e;
		e.alt = false;
		e.ctrl = false;
		e.shift = false;
		e.mouse_pos = get_position();
		e.mouse_device_pos = get_device_position();

		e.type = InputEvent::proximity_change;

		if (LOWORD(wParam) == 0)
		{
			e.id = tablet_proximity_exit;
			stylus_in_proximity = false;
		}
		else
		{
			e.id = tablet_proximity_enter;
			stylus_in_proximity = true;
		}

		(*sig_provider_event)(e);

		return TRUE;
	}

	bool InputDeviceProvider_Win32Tablet::load_wintab()
	{
		if (wintab_dll != 0)
			return true;

		wintab_dll = LoadLibrary(TEXT("Wintab32.dll"));

		if (wintab_dll == 0)
			return false;

		WTInfoA = (ptr_WTInfoA)GetProcAddress(wintab_dll, "WTInfoA");
		WTInfoW = (ptr_WTInfoW)GetProcAddress(wintab_dll, "WTInfoW");
		WTOpenA = (ptr_WTOpenA)GetProcAddress(wintab_dll, "WTOpenA");
		WTOpenW = (ptr_WTOpenW)GetProcAddress(wintab_dll, "WTOpenW");
		WTClose = (ptr_WTClose)GetProcAddress(wintab_dll, "WTClose");
		WTPacketsGet = (ptr_WTPacketsGet)GetProcAddress(wintab_dll, "WTPacketsGet");
		WTPacket = (ptr_WTPacket)GetProcAddress(wintab_dll, "WTPacket");
		WTEnable = (ptr_WTEnable)GetProcAddress(wintab_dll, "WTEnable");
		WTOverlap = (ptr_WTOverlap)GetProcAddress(wintab_dll, "WTOverlap");
		WTConfig = (ptr_WTConfig)GetProcAddress(wintab_dll, "WTConfig");
		WTGetA = (ptr_WTGetA)GetProcAddress(wintab_dll, "WTGetA");
		WTGetW = (ptr_WTGetW)GetProcAddress(wintab_dll, "WTGetW");
		WTSetA = (ptr_WTSetA)GetProcAddress(wintab_dll, "WTSetA");
		WTSetW = (ptr_WTSetW)GetProcAddress(wintab_dll, "WTSetW");
		WTExtGet = (ptr_WTExtGet)GetProcAddress(wintab_dll, "WTExtGet");
		WTExtSet = (ptr_WTExtSet)GetProcAddress(wintab_dll, "WTExtSet");
		WTSave = (ptr_WTSave)GetProcAddress(wintab_dll, "WTSave");
		WTRestore = (ptr_WTRestore)GetProcAddress(wintab_dll, "WTRestore");
		WTPacketsPeek = (ptr_WTPacketsPeek)GetProcAddress(wintab_dll, "WTPacketsPeek");
		WTDataGet = (ptr_WTDataGet)GetProcAddress(wintab_dll, "WTDataGet");
		WTDataPeek = (ptr_WTDataPeek)GetProcAddress(wintab_dll, "WTDataPeek");
		WTQueueSizeGet = (ptr_WTQueueSizeGet)GetProcAddress(wintab_dll, "WTQueueSizeGet");
		WTQueueSizeSet = (ptr_WTQueueSizeSet)GetProcAddress(wintab_dll, "WTQueueSizeSet");
		WTMgrOpen = (ptr_WTMgrOpen)GetProcAddress(wintab_dll, "WTMgrOpen");
		WTMgrClose = (ptr_WTMgrClose)GetProcAddress(wintab_dll, "WTMgrClose");
		WTMgrContextEnum = (ptr_WTMgrContextEnum)GetProcAddress(wintab_dll, "WTMgrContextEnum");
		WTMgrContextOwner = (ptr_WTMgrContextOwner)GetProcAddress(wintab_dll, "WTMgrContextOwner");
		WTMgrDefContext = (ptr_WTMgrDefContext)GetProcAddress(wintab_dll, "WTMgrDefContext");
		WTMgrDefContextEx = (ptr_WTMgrDefContextEx)GetProcAddress(wintab_dll, "WTMgrDefContextEx");
		WTMgrDeviceConfig = (ptr_WTMgrDeviceConfig)GetProcAddress(wintab_dll, "WTMgrDeviceConfig");
		WTMgrExt = (ptr_WTMgrExt)GetProcAddress(wintab_dll, "WTMgrExt");
		WTMgrCsrEnable = (ptr_WTMgrCsrEnable)GetProcAddress(wintab_dll, "WTMgrCsrEnable");
		WTMgrCsrButtonMap = (ptr_WTMgrCsrButtonMap)GetProcAddress(wintab_dll, "WTMgrCsrButtonMap");
		WTMgrCsrPressureBtnMarks = (ptr_WTMgrCsrPressureBtnMarks)GetProcAddress(wintab_dll, "WTMgrCsrPressureBtnMarks");
		WTMgrCsrPressureResponse = (ptr_WTMgrCsrPressureResponse)GetProcAddress(wintab_dll, "WTMgrCsrPressureResponse");
		WTMgrCsrExt = (ptr_WTMgrCsrExt)GetProcAddress(wintab_dll, "WTMgrCsrExt");
		WTQueuePacketsEx = (ptr_WTQueuePacketsEx)GetProcAddress(wintab_dll, "WTQueuePacketsEx");
		WTMgrConfigReplaceExA = (ptr_WTMgrConfigReplaceExA)GetProcAddress(wintab_dll, "WTMgrConfigReplaceExA");
		WTMgrConfigReplaceExW = (ptr_WTMgrConfigReplaceExW)GetProcAddress(wintab_dll, "WTMgrConfigReplaceExW");
		WTMgrPacketHookExA = (ptr_WTMgrPacketHookExA)GetProcAddress(wintab_dll, "WTMgrPacketHookExA");
		WTMgrPacketHookExW = (ptr_WTMgrPacketHookExW)GetProcAddress(wintab_dll, "WTMgrPacketHookExW");
		WTMgrPacketUnhook = (ptr_WTMgrPacketUnhook)GetProcAddress(wintab_dll, "WTMgrPacketUnhook");
		WTMgrPacketHookNext = (ptr_WTMgrPacketHookNext)GetProcAddress(wintab_dll, "WTMgrPacketHookNext");
		WTMgrCsrPressureBtnMarksEx = (ptr_WTMgrCsrPressureBtnMarksEx)GetProcAddress(wintab_dll, "WTMgrCsrPressureBtnMarksEx");

		return true;
	}

	bool InputDeviceProvider_Win32Tablet::in_proximity() const
	{
		return stylus_in_proximity;
	}

	void InputDeviceProvider_Win32Tablet::on_dispose()
	{
		if (wintab_dll && htab)
		{
			if (!WTClose(htab))
			{
				throw Exception("InputDeviceProvider_Win32Tablet: Error closing tablet context");
			}
		}

		delete[] packet_queue;

		if (wintab_dll)
			FreeLibrary(wintab_dll);
	}

	void InputDeviceProvider_Win32Tablet::check_monitor_changed()
	{
		HWND hwnd = window->get_hwnd();
		RECT rect;
		GetWindowRect(hwnd, &rect);

		HMONITOR monitor = MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST);

		if (monitor != previous_monitor)
		{
			MONITORINFO monitor_info;
			ZeroMemory(&monitor_info, sizeof(monitor_info));
			monitor_info.cbSize = sizeof(monitor_info);

			if (GetMonitorInfo(monitor, &monitor_info) != 0)
			{
				logcontext.lcSysOrgX = monitor_info.rcMonitor.left;
				logcontext.lcSysOrgY = monitor_info.rcMonitor.top;
				logcontext.lcSysExtX = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
				logcontext.lcSysExtY = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;

				logcontext.lcOutOrgX = monitor_info.rcMonitor.left;
				logcontext.lcOutOrgY = monitor_info.rcMonitor.top;
				logcontext.lcOutExtX = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
				logcontext.lcOutExtY = -(monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top);

				WTSet(htab, &logcontext);
			}

			previous_monitor = monitor;

			init_axis();
		}
	}
}
