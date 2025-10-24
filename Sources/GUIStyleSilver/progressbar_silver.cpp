/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    Kenneth Gangstø
**    (if your name is missing here, please add it)
*/

#include "API/Display/display.h"
#include "API/Display/surface.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUIStyleSilver/progressbar_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_ProgressBar_Silver::CL_ProgressBar_Silver(
	CL_ProgressBar *progressbar,
	CL_StyleManager_Silver *style)
:
	CL_ComponentStyle(progressbar),
	style(style),
	progressbar(progressbar)
{
	CL_ResourceManager *resources = style->get_resources();

	sur_foreground = new CL_Surface("ProgressBar/surface_foreground", resources);
	sur_background = new CL_Surface("ProgressBar/surface_background", resources);

	slot_paint = progressbar->sig_paint().connect(
		this, &CL_ProgressBar_Silver::on_paint);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_ProgressBar_Silver::on_paint()
{
	int width = progressbar->get_width();
	float percent = progressbar->get_percentage();

	int length = (int) ((width - 4) * percent);

	CL_Rect rect = progressbar->get_screen_rect();

	// Outline
	CL_Display::draw_rect(rect, CL_Color(128, 142, 159));

	sur_background->draw(CL_Rect(rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1));

	sur_foreground->draw(CL_Rect(rect.left + 1, rect.top + 1, rect.left + length - 1, rect.bottom - 1));

	// TODO: Write percentage in text
//	font->draw(...);
}
