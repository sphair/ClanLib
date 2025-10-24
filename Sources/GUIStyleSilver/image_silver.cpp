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

#include "API/Display/surface.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUIStyleSilver/image_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Image_Silver::CL_Image_Silver(
	CL_Image *image,
	CL_StyleManager_Silver *style) 
:
	CL_ComponentStyle(image),
	image(image)
{
	resources = style->get_resources();

	slot_paint = image->sig_paint().connect(
		this, &CL_Image_Silver::on_paint);

	slot_get_preferred_size = image->sig_get_preferred_size().connect(
		this, &CL_Image_Silver::on_get_preferred_size);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_Image_Silver::on_get_preferred_size(CL_Size &size)
{
	CL_Surface * s = image->get_surface();
	if(s)
	{
		size.width = s->get_width();
		size.height = s->get_height();
	}
}

void CL_Image_Silver::on_paint()
{
	CL_Surface *surface = image->get_surface();

	if(surface == 0)
		return;

	int width = image->get_width();
	int height = image->get_height();

	CL_Image::Mode mode = image->get_mode();

	switch (mode)
	{
	case CL_Image::normal:
		surface->draw(0.0f, 0.0f);
		break;

	case CL_Image::center:
		surface->draw(
			image->get_screen_x() + (width - surface->get_width()) / 2,
			image->get_screen_y() + (height - surface->get_height()) / 2);
		break;
		
	case CL_Image::stretch:
		surface->draw(image->get_screen_rect());
		break;

	case CL_Image::tile:
		{
			for (int y = 0; y < height; y += surface->get_height())
				for (int x = 0; x < width; x += surface->get_width())
					surface->draw(image->get_screen_x(), image->get_screen_y());
		}
		break;
	}
}
