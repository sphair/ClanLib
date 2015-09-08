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

#pragma once

#include <windows.h> // wintab.h (also) needs windows.h to be included

// #define NOWTFUNCTIONS
#include "wintab.h"

// #define PACKETDATA (PK_STATUS | PK_TIME | PK_BUTTONS | PK_X | PK_Y | PK_NORMAL_PRESSURE | PK_TANGENT_PRESSURE | PK_CHANGED | PK_ORIENTATION | PK_CURSOR | PK_ROTATION)

#define PACKETDATA (PK_CONTEXT | PK_TIME | PK_BUTTONS | PK_X | PK_Y | PK_NORMAL_PRESSURE)

#define PACKETMODE PK_BUTTONS

#include "pktdef.h" // PACKETDATA and PACKETMODE must be defined before pktdef include.

#include "API/Display/TargetProviders/input_device_provider.h"
#include "API/Core/Math/point.h"

namespace clan
{
	class Win32Window;

	class InputDeviceProvider_Win32Tablet : public InputDeviceProvider
	{
	public:
		InputDeviceProvider_Win32Tablet(Win32Window *window);
		virtual ~InputDeviceProvider_Win32Tablet();

		InputDevice::Type get_type() const override { return InputDevice::tablet; }

		/// \warn Value is the same as get_device_position() and not scaled.
		Pointf get_position() const override;

		Point get_device_position() const override;
		bool get_keycode(int keycode) const override;
		std::string get_key_name(int id) const override;
		float get_axis(int index) const override;
		std::string get_name() const override;
		std::string get_device_name() const override;
		std::vector<int> get_axis_ids() const override;
		int get_button_count() const override;
		bool in_proximity() const override;

		/// \brief Returns true if a tablet device is connected and was initialised successfully.
		bool device_present() const;

		/// \brief Returns true if the tablet context is currently enabled (enabled and on top in context overlap order).
		bool is_context_on_top();

		void init(Signal<void(const InputEvent &)> *new_sig_provider_event) override
		{
			sig_provider_event = new_sig_provider_event;
		}

		/// \brief Raises the tablet context to the top of the tablet context stack and enables it, or disable the context if false.
		void set_context_on_top(bool enable);

		/// \brief Checks if tablet context should be moved to another screen.
		void check_monitor_changed();

		BOOL process_packet(WPARAM wParam, LPARAM lParam);

		BOOL process_proximity(WPARAM wParam, LPARAM lParam);

	private:
		void on_dispose();
		bool init_tablet();
		bool load_wintab();
		void init_axis();
		void packet_received();

		Signal<void(const InputEvent &)> *sig_provider_event;
		Win32Window *window;
		HCTX htab; // Tablet context handle
		HMODULE wintab_dll;
		int packet_queue_size;
		PACKET *packet_queue;

		struct AxisInfo
		{
			int min_value;
			int max_value;
			int resolution;
			int pos;

			AxisInfo(int min_value_, int max_value_, int resolution_)
				: min_value(min_value_), max_value(max_value_), resolution(resolution_), pos(0)
			{}
		};

		std::vector<AxisInfo> axis, old_axis;
		bool stylus_in_proximity;
		int num_keys;
		Point mouse_pos;
		bool init_successfull;
		char button_count;
		bool key_states[32];
		LOGCONTEXT logcontext;
		HMONITOR previous_monitor;

	public:
		typedef UINT(API *ptr_WTInfoA)(UINT, UINT, LPVOID);
		typedef UINT(API *ptr_WTInfoW)(UINT, UINT, LPVOID);
		typedef HCTX(API *ptr_WTOpenA)(HWND, LPLOGCONTEXTA, BOOL);
		typedef HCTX(API *ptr_WTOpenW)(HWND, LPLOGCONTEXTW, BOOL);
		typedef BOOL(API *ptr_WTClose)(HCTX);
		typedef int (API *ptr_WTPacketsGet)(HCTX, int, LPVOID);
		typedef BOOL(API *ptr_WTPacket)(HCTX, UINT, LPVOID);
		typedef BOOL(API *ptr_WTEnable)(HCTX, BOOL);
		typedef BOOL(API *ptr_WTOverlap)(HCTX, BOOL);
		typedef BOOL(API *ptr_WTConfig)(HCTX, HWND);
		typedef BOOL(API *ptr_WTGetA)(HCTX, LPLOGCONTEXTA);
		typedef BOOL(API *ptr_WTGetW)(HCTX, LPLOGCONTEXTW);
		typedef BOOL(API *ptr_WTSetA)(HCTX, LPLOGCONTEXTA);
		typedef BOOL(API *ptr_WTSetW)(HCTX, LPLOGCONTEXTW);
		typedef BOOL(API *ptr_WTExtGet)(HCTX, UINT, LPVOID);
		typedef BOOL(API *ptr_WTExtSet)(HCTX, UINT, LPVOID);
		typedef BOOL(API *ptr_WTSave)(HCTX, LPVOID);
		typedef HCTX(API *ptr_WTRestore)(HWND, LPVOID, BOOL);
		typedef int (API *ptr_WTPacketsPeek)(HCTX, int, LPVOID);
		typedef int (API *ptr_WTDataGet)(HCTX, UINT, UINT, int, LPVOID, LPINT);
		typedef int (API *ptr_WTDataPeek)(HCTX, UINT, UINT, int, LPVOID, LPINT);
		typedef int (API *ptr_WTQueueSizeGet)(HCTX);
		typedef BOOL(API *ptr_WTQueueSizeSet)(HCTX, int);
		typedef HMGR(API *ptr_WTMgrOpen)(HWND, UINT);
		typedef BOOL(API *ptr_WTMgrClose)(HMGR);
		typedef BOOL(API *ptr_WTMgrContextEnum)(HMGR, WTENUMPROC, LPARAM);
		typedef HWND(API *ptr_WTMgrContextOwner)(HMGR, HCTX);
		typedef HCTX(API *ptr_WTMgrDefContext)(HMGR, BOOL);
		typedef HCTX(API *ptr_WTMgrDefContextEx)(HMGR, UINT, BOOL);
		typedef UINT(API *ptr_WTMgrDeviceConfig)(HMGR, UINT, HWND);
		typedef BOOL(API *ptr_WTMgrExt)(HMGR, UINT, LPVOID);
		typedef BOOL(API *ptr_WTMgrCsrEnable)(HMGR, UINT, BOOL);
		typedef BOOL(API *ptr_WTMgrCsrButtonMap)(HMGR, UINT, LPBYTE, LPBYTE);
		typedef BOOL(API *ptr_WTMgrCsrPressureBtnMarks)(HMGR, UINT, DWORD, DWORD);
		typedef BOOL(API *ptr_WTMgrCsrPressureResponse)(HMGR, UINT, UINT FAR *, UINT FAR *);
		typedef BOOL(API *ptr_WTMgrCsrExt)(HMGR, UINT, UINT, LPVOID);
		typedef BOOL(API *ptr_WTQueuePacketsEx)(HCTX, UINT FAR *, UINT FAR *);
		typedef BOOL(API *ptr_WTMgrConfigReplaceExA)(HMGR, BOOL, LPSTR, LPSTR);
		typedef BOOL(API *ptr_WTMgrConfigReplaceExW)(HMGR, BOOL, LPWSTR, LPSTR);
		typedef HWTHOOK(*ptr_WTMgrPacketHookExA)(HMGR, int, LPSTR, LPSTR);
		typedef HWTHOOK(*ptr_WTMgrPacketHookExW)(HMGR, int, LPWSTR, LPSTR);
		typedef BOOL(API *ptr_WTMgrPacketUnhook)(HWTHOOK);
		typedef LRESULT(API *ptr_WTMgrPacketHookNext)(HWTHOOK, int, WPARAM, LPARAM);
		typedef BOOL(API *ptr_WTMgrCsrPressureBtnMarksEx)(HMGR, UINT, UINT FAR *, UINT FAR *);

		ptr_WTInfoA WTInfoA;
		ptr_WTInfoW WTInfoW;
		ptr_WTOpenA WTOpenA;
		ptr_WTOpenW WTOpenW;
		ptr_WTClose WTClose;
		ptr_WTPacketsGet WTPacketsGet;
		ptr_WTPacket WTPacket;
		ptr_WTEnable WTEnable;
		ptr_WTOverlap WTOverlap;
		ptr_WTConfig WTConfig;
		ptr_WTGetA WTGetA;
		ptr_WTGetW WTGetW;
		ptr_WTSetA WTSetA;
		ptr_WTSetW WTSetW;
		ptr_WTExtGet WTExtGet;
		ptr_WTExtSet WTExtSet;
		ptr_WTSave WTSave;
		ptr_WTRestore WTRestore;
		ptr_WTPacketsPeek WTPacketsPeek;
		ptr_WTDataGet WTDataGet;
		ptr_WTDataPeek WTDataPeek;
		ptr_WTQueueSizeGet WTQueueSizeGet;
		ptr_WTQueueSizeSet WTQueueSizeSet;
		ptr_WTMgrOpen WTMgrOpen;
		ptr_WTMgrClose WTMgrClose;
		ptr_WTMgrContextEnum WTMgrContextEnum;
		ptr_WTMgrContextOwner WTMgrContextOwner;
		ptr_WTMgrDefContext WTMgrDefContext;
		ptr_WTMgrDefContextEx WTMgrDefContextEx;
		ptr_WTMgrDeviceConfig WTMgrDeviceConfig;
		ptr_WTMgrExt WTMgrExt;
		ptr_WTMgrCsrEnable WTMgrCsrEnable;
		ptr_WTMgrCsrButtonMap WTMgrCsrButtonMap;
		ptr_WTMgrCsrPressureBtnMarks WTMgrCsrPressureBtnMarks;
		ptr_WTMgrCsrPressureResponse WTMgrCsrPressureResponse;
		ptr_WTMgrCsrExt WTMgrCsrExt;
		ptr_WTQueuePacketsEx WTQueuePacketsEx;
		ptr_WTMgrConfigReplaceExA WTMgrConfigReplaceExA;
		ptr_WTMgrConfigReplaceExW WTMgrConfigReplaceExW;
		ptr_WTMgrPacketHookExA WTMgrPacketHookExA;
		ptr_WTMgrPacketHookExW WTMgrPacketHookExW;
		ptr_WTMgrPacketUnhook WTMgrPacketUnhook;
		ptr_WTMgrPacketHookNext WTMgrPacketHookNext;
		ptr_WTMgrCsrPressureBtnMarksEx WTMgrCsrPressureBtnMarksEx;
	};
}
