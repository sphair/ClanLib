/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "screen_info_provider_win32.h"
#include "API/Core/Math/rect.h"

namespace clan
{
	ScreenInfoProvider_Win32::ScreenInfoProvider_Win32()
		: ScreenInfoProvider()
	{
	}

	std::vector<Rectf> ScreenInfoProvider_Win32::get_screen_geometries(int &primary_screen_index) const
	{
		HDC dc = GetDC(0);
		int ppi = GetDeviceCaps(dc, LOGPIXELSX);
		ReleaseDC(0, dc);

		std::vector<Rectf> monitor_positions;
		primary_screen_index = 0;
		int index = 0;
		while (true)
		{
			DISPLAY_DEVICE display_device;
			memset(&display_device, 0, sizeof(DISPLAY_DEVICE));
			display_device.cb = sizeof(DISPLAY_DEVICE);
			BOOL result = EnumDisplayDevices(0, index++, &display_device, 0);
			if (result == FALSE)
				break;

			if ((display_device.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) == 0 &&
				(display_device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
			{
				std::wstring device_name = display_device.DeviceName;
				DEVMODE devmode;
				memset(&devmode, 0, sizeof(DEVMODE));
				devmode.dmSize = sizeof(DEVMODE);
				result = EnumDisplaySettingsEx(device_name.c_str(), ENUM_REGISTRY_SETTINGS, &devmode, 0);
				if (result)
				{
					if ((devmode.dmFields & DM_PELSWIDTH) && (devmode.dmFields & DM_PELSHEIGHT))
					{
						if (display_device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
							primary_screen_index = monitor_positions.size();

						Rectf pos(
							devmode.dmPosition.x * 96.0f / ppi,
							devmode.dmPosition.y * 96.0f / ppi,
							(devmode.dmPosition.x + devmode.dmPelsWidth) * 96.0f / ppi,
							(devmode.dmPosition.y + devmode.dmPelsHeight) * 96.0f / ppi);
						monitor_positions.push_back(pos);
					}
				}
			}
		}

		return monitor_positions;
	}
}
