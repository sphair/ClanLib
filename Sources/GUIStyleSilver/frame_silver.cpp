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
**    (if your name is missing here, please add it)
*/

#include "API/Display/display.h"
#include "API/Display/surface.h"
#include "API/GUIStyleSilver/frame_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/System/error.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Frame_Silver::CL_Frame_Silver(
	CL_Frame *_frame,
	CL_StyleManager_Silver *style) 
:
	CL_ComponentStyle(_frame),
	frame(_frame),
	style(style)
{
	resources = style->get_resources();
	sur_surface = 0;
	mode = tile;

	slot_set_options = frame->sig_set_options().connect(
		this, &CL_Frame_Silver::on_set_options);
	slot_paint = frame->sig_paint().connect(
		this, &CL_Frame_Silver::on_paint);
	slot_resize = frame->sig_resize().connect(
		this, &CL_Frame_Silver::on_resize);

	on_resize(0,0);
}

CL_Frame_Silver::~CL_Frame_Silver()
{
	if(sur_surface)
		delete sur_surface;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_Frame_Silver::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("surface"))
	{
		std::string surface = options.get_attribute("surface");
		sur_surface = new CL_Surface(surface, resources);
	}
	
	if (options.has_attribute("mode"))
	{
		std::string v = options.get_attribute("mode");
		
		if (v == "center") mode = center;
		else if (v == "stretch") mode = stretch;
		else if (v == "tile") mode = tile;
		else throw CL_Error("unknown frame mode");
	}
}

void CL_Frame_Silver::on_resize(int old_width, int old_height)
{
	CL_Rect client_rect(1, 1, frame->get_width() - 1, frame->get_height() - 1);
	frame->get_client_area()->set_position(client_rect);
}

void CL_Frame_Silver::on_paint()
{
	int width = frame->get_width();
	int height = frame->get_height();

	if (sur_surface)
	{
		switch (mode)
		{
		case center:
			sur_surface->draw(
				frame->get_screen_x() + width / 2 - sur_surface->get_width() / 2,
				frame->get_screen_x() + height / 2 - sur_surface->get_height() / 2);
			break;
			
		case stretch:
			sur_surface->draw(frame->get_screen_rect());
			break;

		case tile:
			{
			for (int y = 0; y < height; y += sur_surface->get_height())
				for (int x = 0; x < width; x += sur_surface->get_width())
					sur_surface->draw(frame->get_screen_x()+x, frame->get_screen_y()+y);
			}
		}
	}
	else
	{
		CL_Display::draw_rect(frame->get_screen_rect(), CL_Color(204, 212, 222));

		if(frame->is_fill_enabled())
		{
			CL_Rect rect = frame->get_screen_rect();
			rect.left++;
			rect.top++;
			rect.right--;
			rect.bottom--;
			CL_Display::fill_rect(rect, CL_Color::white);
		}
	}
}
