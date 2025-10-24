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
*/

#include "GUI/precomp.h"
#include "API/Core/Math/cl_math.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/listview_icon_list.h"
#include "API/Display/2D/draw.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Image/pixel_buffer.h"
#include <map>

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewIcon_Impl Class:

class CL_ListViewIcon_Impl
{
public:
	CL_ListViewIcon_Impl() : scalable(true)
	{
	}

	std::map<CL_ListViewDisplayMode, CL_Sprite> sprites;
	std::map<CL_ListViewDisplayMode, CL_PixelBuffer> pbufs;
	std::map<CL_ListViewDisplayMode, CL_Point> offsets;

	bool scalable;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewIcon Construction:

CL_ListViewIcon::CL_ListViewIcon()
: impl(new CL_ListViewIcon_Impl)
{
}

CL_ListViewIcon::~CL_ListViewIcon()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewIcon Attributes:

CL_PixelBuffer CL_ListViewIcon::get_pixel_buffer(CL_ListViewDisplayMode mode)
{
	std::map<CL_ListViewDisplayMode, CL_PixelBuffer>::iterator it;
	it = impl->pbufs.find(mode);
	if (it != impl->pbufs.end())
		return impl->pbufs[mode];

	return CL_PixelBuffer();
}

CL_Sprite CL_ListViewIcon::get_sprite(CL_ListViewDisplayMode mode)
{
	std::map<CL_ListViewDisplayMode, CL_Sprite>::iterator it;
	it = impl->sprites.find(mode);
	if (it != impl->sprites.end())
		return impl->sprites[mode];

	return CL_Sprite();
}

CL_Point CL_ListViewIcon::get_offset(CL_ListViewDisplayMode mode)
{
	std::map<CL_ListViewDisplayMode, CL_Point>::iterator it;
	it = impl->offsets.find(mode);
	if (it != impl->offsets.end())
		return impl->offsets[mode];

	return CL_Point(0,0);
}

bool CL_ListViewIcon::is_scalable() const
{
	return impl->scalable;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewIcon Operations:

void CL_ListViewIcon::set_sprite(const CL_Sprite &sprite, CL_ListViewDisplayMode mode)
{
	impl->sprites[mode] = sprite;
}

void CL_ListViewIcon::set_pixel_buffer(CL_PixelBuffer &pbuf, CL_ListViewDisplayMode mode)
{
	impl->pbufs[mode] = pbuf;
}

void CL_ListViewIcon::set_offset(const CL_Point &offset, CL_ListViewDisplayMode mode)
{
	impl->offsets[mode] = offset;
}

void CL_ListViewIcon::draw(CL_GraphicContext &gc, const CL_Rect &rect, CL_ListViewDisplayMode mode, const CL_Colorf &color)
{
	CL_Sprite sp = get_sprite(mode);
	if (!sp.is_null())
	{
		float scale = 1.0;

		sp.set_color(color);
		//		if (mode == listview_mode_thumbnails && sp.get_frame_size(0) != rect.get_size())
		if (impl->scalable && (sp.get_frame_size(0) != rect.get_size()))
		{
			// Scale to max vertically or horizontally.
			float sx, sy;
			sx = rect.get_width()/float(sp.get_width());
			sy = rect.get_height()/(float)sp.get_height();
			if (sx <= 0 || sy <= 0)
			{
				return;
			}
			scale = cl_min(sx, sy);
			sp.set_scale(scale,scale);
		}

		CL_Rect R = rect;
		CL_Point offset = get_offset(mode);

		R.translate(offset);

		if (!impl->scalable && offset == CL_Point(0,0))
		{
			// center in cell rect.
			int center_offset_x = int((float)rect.get_center().x - (float)rect.left - scale*(float)sp.get_width()/2.0); 
			int center_offset_y = int((float)rect.get_center().y - (float)rect.top - scale*(float)sp.get_height()/2.0); 
			R.left += center_offset_x;
			R.top += center_offset_y;
		}

		if (!impl->scalable)
			sp.draw(gc, (float)R.left, (float)R.top);
		else
			sp.draw(gc, R);

		return;
	}

	CL_PixelBuffer pb = get_pixel_buffer(mode);
	if (!pb.is_null())
	{
		float scale = 1.0;
		float center_offset_x = 0, center_offset_y = 0;

//		if (mode == listview_mode_thumbnails && pb.get_size() != rect.get_size())
		if (impl->scalable && (pb.get_size() != rect.get_size()))
		{
			float sx = 1.0, sy = 1.0;

			// Scale to max vertically or horizontally.
			sx = rect.get_width()/float(pb.get_width());
			sy = rect.get_height()/(float)pb.get_height();
			if (sx <= 0 || sy <= 0)
			{
				return;
			}

			scale = cl_min(sx, sy);

			// center in the rect.
			center_offset_x = (float)rect.get_center().x - (float)rect.left - scale*(float)pb.get_width()/2.0f; 
			center_offset_y = (float)rect.get_center().y - (float)rect.top - scale*(float)pb.get_height()/2.0f; 
		}

		CL_Point offset = get_offset(mode);

		gc.draw_pixels(rect.left + center_offset_x + offset.x, rect.top + center_offset_y + offset.y, scale, scale, pb, pb.get_size(), color);
	}
}

void CL_ListViewIcon::set_scalable(bool value)
{
	impl->scalable = value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewIcon Implementation:
