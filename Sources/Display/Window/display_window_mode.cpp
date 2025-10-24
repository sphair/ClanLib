/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/precomp.h"
#include "API/Display/Window/display_window_mode.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Math/size.h"

#ifndef __APPLE__
#ifndef WIN32
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#define V_DBLSCAN       0x020
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowMode_Impl Class:

class CL_DisplayWindowMode_Impl
{
//! Attributes:
public:
	CL_Size resolution;
	
	int bpp;

	int refresh_rate;
};

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowMode construction:

CL_DisplayWindowMode::CL_DisplayWindowMode(const CL_Size &res, int bpp, int refresh_rate) : impl(0)
{
	impl = new CL_DisplayWindowMode_Impl;
	impl->resolution = res;
	impl->bpp = bpp;
	impl->refresh_rate = refresh_rate;
}

CL_DisplayWindowMode::CL_DisplayWindowMode(const CL_DisplayWindowMode &copy) : impl (new CL_DisplayWindowMode_Impl)
{
	*impl = *copy.impl;
}

CL_DisplayWindowMode::~CL_DisplayWindowMode()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowMode statics:

std::vector<CL_DisplayWindowMode> &CL_DisplayWindowMode::get_display_modes()
{
	static std::vector<CL_DisplayWindowMode> modes;

	if(modes.empty())
	{
#ifdef WIN32
		int index = 0;
		while (true)
		{
			DEVMODE cur_devmode;
			memset(&cur_devmode, 0, sizeof(DEVMODE));
			cur_devmode.dmSize = sizeof(DEVMODE);
			if (EnumDisplaySettings(0, index++, &cur_devmode) == 0) break;

			modes.push_back(CL_DisplayWindowMode(
				CL_Size(cur_devmode.dmPelsWidth, cur_devmode.dmPelsHeight),
				cur_devmode.dmBitsPerPel,
				cur_devmode.dmDisplayFrequency));
		}
#else
#ifndef __APPLE__
		Display *display = XOpenDisplay(NULL);

		int event_base;
		int error_base;

		// Check for VidMode extension
		if (XF86VidModeQueryExtension(display, &event_base, &error_base))
		{
			int screen = DefaultScreen(display);

			XF86VidModeModeInfo **vmodes;
			int nmodes;

			if (XF86VidModeGetAllModeLines(display, screen, &nmodes, &vmodes))
			{
				int bpp = DefaultDepth(display, screen);
				int refresh_rate;

				for (int i = 0; i < nmodes; i++)
				{
					if(vmodes[i]->htotal * vmodes[i]->vtotal != 0)
					{
						if ((vmodes[i]->flags) & V_DBLSCAN)
							refresh_rate = vmodes[i]->dotclock * 500 / (vmodes[i]->htotal * vmodes[i]->vtotal);
						else
							refresh_rate = vmodes[i]->dotclock * 1000 / (vmodes[i]->htotal * vmodes[i]->vtotal);
					}
					else
						refresh_rate = 0;

					modes.push_back(CL_DisplayWindowMode(
						CL_Size(vmodes[i]->hdisplay, vmodes[i]->vdisplay),
						bpp,
						refresh_rate));
				}
			}
		}

		XCloseDisplay(display);
#endif
#endif
	}

	return modes;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowMode attributes:

CL_Size CL_DisplayWindowMode::get_resolution() const
{
	return impl->resolution;
}

int CL_DisplayWindowMode::get_bpp() const
{
	return impl->bpp;
}

int CL_DisplayWindowMode::get_refresh_rate() const
{
	return impl->refresh_rate;
}

CL_String CL_DisplayWindowMode::get_string() const
{
	return cl_format("%1x%2x%3@%4", impl->resolution.width, impl->resolution.height, impl->bpp, impl->refresh_rate);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowMode attributes:

CL_DisplayWindowMode &CL_DisplayWindowMode::operator =(const CL_DisplayWindowMode &copy)
{
	*impl = *copy.impl;
	return *this;
}
