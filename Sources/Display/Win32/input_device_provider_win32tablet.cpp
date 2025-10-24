/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "API/Core/System/thread.h"
#include "API/Display/Window/input_event.h"

#include "input_device_provider_win32tablet.h"
#include "win32_window.h"
#include "API/Display/Window/keys.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_Win32Tablet Construction:

CL_InputDeviceProvider_Win32Tablet::CL_InputDeviceProvider_Win32Tablet(CL_Win32Window *window)
: CL_InputDeviceProvider(),
  sig_provider_event(0), 
  window(window),
  htab(0),
  wintab_dll(0),
  num_keys(0),
  mouse_pos(0,0),
  packet_queue_size(128),
  packet_queue(0),
  init_successfull(0),
  button_count(0),
  styuls_in_proximity(false)
{
	if( load_wintab() == false )
	{
		return;
	}

	if( init_tablet() == false )
	{
		return;
	}

	init_successfull = true;
}

CL_InputDeviceProvider_Win32Tablet::~CL_InputDeviceProvider_Win32Tablet()
{
	dispose();
}

void CL_InputDeviceProvider_Win32Tablet::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_Win32Tablet Attributes:

int CL_InputDeviceProvider_Win32Tablet::get_x() const
{
	return mouse_pos.x;
}

int CL_InputDeviceProvider_Win32Tablet::get_y() const
{
	return mouse_pos.y;
}

bool CL_InputDeviceProvider_Win32Tablet::get_keycode(int keycode) const
{
	if (keycode >=0 && keycode < button_count)
	{
		return key_states[keycode];
	}

	return false;
}

CL_String CL_InputDeviceProvider_Win32Tablet::get_key_name(int id) const
{
	return CL_String();
}

float CL_InputDeviceProvider_Win32Tablet::get_axis(int index) const
{
	if (!init_successfull)
		return 0.0f;

	if (index >= 0 && index < get_axis_count())
	{
		return float(axis[index].pos)/(axis[index].max_value - axis[index].min_value);
	}

	return 0.0f;
}

CL_String CL_InputDeviceProvider_Win32Tablet::get_name() const
{
	return CL_String();
}

CL_String CL_InputDeviceProvider_Win32Tablet::get_device_name() const
{
	return CL_String();
}

int CL_InputDeviceProvider_Win32Tablet::get_axis_count() const
{
	return axis.size();
}

int CL_InputDeviceProvider_Win32Tablet::get_button_count() const
{
	return button_count;
}

bool CL_InputDeviceProvider_Win32Tablet::device_present() const
{
	return init_successfull;
}

bool CL_InputDeviceProvider_Win32Tablet::is_context_on_top()
{
	LOGCONTEXT context;
	WTGet(htab, &context);

	return context.lcStatus & CXS_ONTOP;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_Win32Tablet Operations:

void CL_InputDeviceProvider_Win32Tablet::set_position(int x, int y)
{
}

void CL_InputDeviceProvider_Win32Tablet::set_context_on_top(bool enable)
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

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_Win32Tablet Implementation:

bool CL_InputDeviceProvider_Win32Tablet::init_tablet()
{
	// connects tablet to a hwnd

	AXIS            TabletX, TabletY; // The maximum tablet size

	// get default region
	int tablet_connected = WTInfo(WTI_DEFCONTEXT, 0, &logcontext);

	if( !tablet_connected  )
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
	WTInfo(WTI_DEVICES,DVC_X,&TabletX);
	WTInfo(WTI_DEVICES,DVC_Y,&TabletY);
	logcontext.lcInOrgX = 0;
	logcontext.lcInOrgY = 0;
	logcontext.lcInExtX = TabletX.axMax;
	logcontext.lcInExtY = TabletY.axMax;

	HWND hwnd = window->get_hwnd();

    // output the data in screen coords

	RECT rect;
	if (GetWindowRect(hwnd, &rect) == 0 )
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

	if( htab == 0 )
	{
		return false;
	}

	int packet_queue_size = 128;

	// try with smaller and smaller queue size, untill a supported size is found.
	// the wintab default is 8.
	while(!WTQueueSizeSet(htab, packet_queue_size) && packet_queue_size > 8 )
		packet_queue_size /= 2;

	packet_queue = new PACKET[packet_queue_size];

	init_axis();

	WTOverlap(htab, TRUE); // raise to top of context overlap stack.

	// get button count of cursor
	WTInfo(WTI_CURSORS, CSR_BUTTONS, &button_count );

	return true;
}

void CL_InputDeviceProvider_Win32Tablet::init_axis()
{
	// setup x,y and pressure axis... the rest: todo
	axis.clear();

	AXIS TabletX, TabletY, TabletPres;

	// x axis
	WTInfo(WTI_DEVICES, DVC_X, &TabletX);
	axis.push_back( AxisInfo(TabletX.axMin, TabletX.axMax, TabletX.axResolution) );

	// y axis
	WTInfo(WTI_DEVICES, DVC_Y, &TabletY);
	axis.push_back( AxisInfo(TabletY.axMin, TabletY.axMax, TabletY.axResolution) );

	// pressure axis
	WTInfo(WTI_DEVICES, DVC_NPRESSURE, &TabletPres);
	axis.push_back( AxisInfo(TabletPres.axMin, TabletPres.axMax, TabletPres.axResolution) );

	old_axis = axis;
}


BOOL CL_InputDeviceProvider_Win32Tablet::process_packet(WPARAM wParam, LPARAM lParam)
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
				key_states[keycode] = true;
				CL_InputEvent e;

				e.id           = keycode + CL_KEY_TABLET1;
				e.type         = CL_InputEvent::pressed;
				e.mouse_pos    = CL_Point(pkt.pkX - winfo.rcClient.left, pkt.pkY - winfo.rcClient.top);
				e.axis_pos     = 0;
				e.repeat_count = 0;
				window->set_modifier_keys(e);
				sig_provider_event->invoke(e);
				return TRUE;
			}
			else if (HIWORD(pkt.pkButtons) == TBN_UP)
			{
				int keycode = LOWORD(pkt.pkButtons);
				key_states[keycode] = false;
				CL_InputEvent e;

				e.id           = keycode + CL_KEY_TABLET1;
				e.type         = CL_InputEvent::released;
				e.mouse_pos    = CL_Point(pkt.pkX - winfo.rcClient.left, pkt.pkY - winfo.rcClient.top);
				e.axis_pos     = 0;
				e.repeat_count = 0;
				window->set_modifier_keys(e);
				sig_provider_event->invoke(e);
				return TRUE;
			}
			else
			{
				CL_InputEvent e;

				e.id           = CL_InputEvent::z_axis; // TODO: support tilt, rotation.
				e.type         = CL_InputEvent::axis_moved; // x,y as pointer movements
				e.mouse_pos    = CL_Point(pkt.pkX - winfo.rcClient.left, pkt.pkY - winfo.rcClient.top);
				e.axis_pos     = get_axis(2);
				e.repeat_count = 0;
				window->set_modifier_keys(e);
				sig_provider_event->invoke(e);
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CL_InputDeviceProvider_Win32Tablet::process_proximity( WPARAM wParam, LPARAM lParam )
{
	CL_InputEvent e;
	e.id = -1;
	e.alt = false;
	e.ctrl = false;
	e.shift = false;
	e.mouse_pos.x = get_x();
	e.mouse_pos.y = get_y();

	e.type = CL_InputEvent::proximity_change;

	if (LOWORD(wParam) == 0)
	{
		e.id = CL_InputEvent::proximity_exit;
		styuls_in_proximity = false;
	}
	else
	{
		e.id = CL_InputEvent::proximity_enter;
		styuls_in_proximity = true;
	}

	sig_provider_event->invoke(e);

	return TRUE;
}

bool CL_InputDeviceProvider_Win32Tablet::load_wintab()
{
	if (wintab_dll != 0)
		return true;

	wintab_dll = LoadLibrary(TEXT("Wintab32.dll"));

	if (wintab_dll == 0)
		return false;

	WTInfoA = (ptr_WTInfoA) GetProcAddress(wintab_dll, "WTInfoA");
	WTInfoW = (ptr_WTInfoW) GetProcAddress(wintab_dll, "WTInfoW");
	WTOpenA = (ptr_WTOpenA) GetProcAddress(wintab_dll, "WTOpenA");
	WTOpenW = (ptr_WTOpenW) GetProcAddress(wintab_dll, "WTOpenW");
	WTClose = (ptr_WTClose) GetProcAddress(wintab_dll, "WTClose");
	WTPacketsGet = (ptr_WTPacketsGet) GetProcAddress(wintab_dll, "WTPacketsGet");
	WTPacket = (ptr_WTPacket) GetProcAddress(wintab_dll, "WTPacket");
	WTEnable = (ptr_WTEnable) GetProcAddress(wintab_dll, "WTEnable");
	WTOverlap = (ptr_WTOverlap) GetProcAddress(wintab_dll, "WTOverlap");
	WTConfig = (ptr_WTConfig) GetProcAddress(wintab_dll, "WTConfig");
	WTGetA = (ptr_WTGetA) GetProcAddress(wintab_dll, "WTGetA");
	WTGetW = (ptr_WTGetW) GetProcAddress(wintab_dll, "WTGetW");
	WTSetA = (ptr_WTSetA) GetProcAddress(wintab_dll, "WTSetA");
	WTSetW = (ptr_WTSetW) GetProcAddress(wintab_dll, "WTSetW");
	WTExtGet = (ptr_WTExtGet) GetProcAddress(wintab_dll, "WTExtGet");
	WTExtSet = (ptr_WTExtSet) GetProcAddress(wintab_dll, "WTExtSet");
	WTSave = (ptr_WTSave) GetProcAddress(wintab_dll, "WTSave");
	WTRestore = (ptr_WTRestore) GetProcAddress(wintab_dll, "WTRestore");
	WTPacketsPeek = (ptr_WTPacketsPeek) GetProcAddress(wintab_dll, "WTPacketsPeek");
	WTDataGet = (ptr_WTDataGet) GetProcAddress(wintab_dll, "WTDataGet");
	WTDataPeek = (ptr_WTDataPeek) GetProcAddress(wintab_dll, "WTDataPeek");
	WTQueueSizeGet = (ptr_WTQueueSizeGet) GetProcAddress(wintab_dll, "WTQueueSizeGet");
	WTQueueSizeSet = (ptr_WTQueueSizeSet) GetProcAddress(wintab_dll, "WTQueueSizeSet");
	WTMgrOpen = (ptr_WTMgrOpen) GetProcAddress(wintab_dll, "WTMgrOpen");
	WTMgrClose = (ptr_WTMgrClose) GetProcAddress(wintab_dll, "WTMgrClose");
	WTMgrContextEnum = (ptr_WTMgrContextEnum) GetProcAddress(wintab_dll, "WTMgrContextEnum");
	WTMgrContextOwner = (ptr_WTMgrContextOwner) GetProcAddress(wintab_dll, "WTMgrContextOwner");
	WTMgrDefContext = (ptr_WTMgrDefContext) GetProcAddress(wintab_dll, "WTMgrDefContext");
	WTMgrDefContextEx = (ptr_WTMgrDefContextEx) GetProcAddress(wintab_dll, "WTMgrDefContextEx");
	WTMgrDeviceConfig = (ptr_WTMgrDeviceConfig) GetProcAddress(wintab_dll, "WTMgrDeviceConfig");
	WTMgrExt = (ptr_WTMgrExt) GetProcAddress(wintab_dll, "WTMgrExt");
	WTMgrCsrEnable = (ptr_WTMgrCsrEnable) GetProcAddress(wintab_dll, "WTMgrCsrEnable");
	WTMgrCsrButtonMap = (ptr_WTMgrCsrButtonMap) GetProcAddress(wintab_dll, "WTMgrCsrButtonMap");
	WTMgrCsrPressureBtnMarks = (ptr_WTMgrCsrPressureBtnMarks) GetProcAddress(wintab_dll, "WTMgrCsrPressureBtnMarks");
	WTMgrCsrPressureResponse = (ptr_WTMgrCsrPressureResponse) GetProcAddress(wintab_dll, "WTMgrCsrPressureResponse");
	WTMgrCsrExt = (ptr_WTMgrCsrExt) GetProcAddress(wintab_dll, "WTMgrCsrExt");
	WTQueuePacketsEx = (ptr_WTQueuePacketsEx) GetProcAddress(wintab_dll, "WTQueuePacketsEx");
	WTMgrConfigReplaceExA = (ptr_WTMgrConfigReplaceExA) GetProcAddress(wintab_dll, "WTMgrConfigReplaceExA");
	WTMgrConfigReplaceExW = (ptr_WTMgrConfigReplaceExW) GetProcAddress(wintab_dll, "WTMgrConfigReplaceExW");
	WTMgrPacketHookExA = (ptr_WTMgrPacketHookExA) GetProcAddress(wintab_dll, "WTMgrPacketHookExA");
	WTMgrPacketHookExW = (ptr_WTMgrPacketHookExW) GetProcAddress(wintab_dll, "WTMgrPacketHookExW");
	WTMgrPacketUnhook = (ptr_WTMgrPacketUnhook) GetProcAddress(wintab_dll, "WTMgrPacketUnhook");
	WTMgrPacketHookNext = (ptr_WTMgrPacketHookNext) GetProcAddress(wintab_dll, "WTMgrPacketHookNext");
	WTMgrCsrPressureBtnMarksEx = (ptr_WTMgrCsrPressureBtnMarksEx) GetProcAddress(wintab_dll, "WTMgrCsrPressureBtnMarksEx");

	return true;
}

bool CL_InputDeviceProvider_Win32Tablet::in_proximity() const
{
	return styuls_in_proximity;
}

void CL_InputDeviceProvider_Win32Tablet::on_dispose()
{
	if(wintab_dll && htab)
	{
		if (!WTClose(htab))
		{
			throw CL_Exception("CL_InputDeviceProvider_Win32Tablet: Error closing tablet context");
		}
	}

	delete[] packet_queue;

	if (wintab_dll)
		FreeLibrary(wintab_dll);
}

void CL_InputDeviceProvider_Win32Tablet::check_monitor_changed()
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




