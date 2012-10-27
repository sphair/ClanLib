/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "API/Display/2D/canvas.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Image/pixel_buffer.h"
#include <map>

#ifdef INCLUDE_COMPONENTS

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ListViewIcon_Impl Class:

class ListViewIcon_Impl
{
public:
	ListViewIcon_Impl() : scalable(true)
	{
	}

	std::map<ListViewDisplayMode, Sprite> sprites;
	std::map<ListViewDisplayMode, PixelBuffer> pbufs;
	std::map<ListViewDisplayMode, Point> offsets;

	bool scalable;
};

/////////////////////////////////////////////////////////////////////////////
// ListViewIcon Construction:

ListViewIcon::ListViewIcon()
: impl(new ListViewIcon_Impl)
{
}

ListViewIcon::~ListViewIcon()
{
}

/////////////////////////////////////////////////////////////////////////////
// ListViewIcon Attributes:

PixelBuffer ListViewIcon::get_pixel_buffer(ListViewDisplayMode mode)
{
	std::map<ListViewDisplayMode, PixelBuffer>::iterator it;
	it = impl->pbufs.find(mode);
	if (it != impl->pbufs.end())
		return impl->pbufs[mode];

	return PixelBuffer();
}

Sprite ListViewIcon::get_sprite(ListViewDisplayMode mode)
{
	std::map<ListViewDisplayMode, Sprite>::iterator it;
	it = impl->sprites.find(mode);
	if (it != impl->sprites.end())
		return impl->sprites[mode];

	return Sprite();
}

Point ListViewIcon::get_offset(ListViewDisplayMode mode)
{
	std::map<ListViewDisplayMode, Point>::iterator it;
	it = impl->offsets.find(mode);
	if (it != impl->offsets.end())
		return impl->offsets[mode];

	return Point(0,0);
}

bool ListViewIcon::is_scalable() const
{
	return impl->scalable;
}

/////////////////////////////////////////////////////////////////////////////
// ListViewIcon Operations:

void ListViewIcon::set_sprite(const Sprite &sprite, ListViewDisplayMode mode)
{
	impl->sprites[mode] = sprite;
}

void ListViewIcon::set_pixel_buffer(PixelBuffer &pbuf, ListViewDisplayMode mode)
{
	impl->pbufs[mode] = pbuf;
}

void ListViewIcon::set_offset(const Point &offset, ListViewDisplayMode mode)
{
	impl->offsets[mode] = offset;
}

void ListViewIcon::draw(Canvas &canvas, const Rect &rect, ListViewDisplayMode mode, const Colorf &color)
{
	Sprite sp = get_sprite(mode);
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
			scale = min(sx, sy);
			sp.set_scale(scale,scale);
		}

		Rect R = rect;
		Point offset = get_offset(mode);

		R.translate(offset);

		if (!impl->scalable && offset == Point(0,0))
		{
			// center in cell rect.
			int center_offset_x = int((float)rect.get_center().x - (float)rect.left - scale*(float)sp.get_width()/2.0); 
			int center_offset_y = int((float)rect.get_center().y - (float)rect.top - scale*(float)sp.get_height()/2.0); 
			R.left += center_offset_x;
			R.top += center_offset_y;
		}

		if (!impl->scalable)
			sp.draw(canvas, (float)R.left, (float)R.top);
		else
			sp.draw(canvas, R);

		return;
	}
/*
	Todo: this code needs to place the pb in an image object, instead of relying on the removed canvas.draw_pixels function

	PixelBuffer pb = get_pixel_buffer(mode);
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

			scale = min(sx, sy);

			// center in the rect.
			center_offset_x = (float)rect.get_center().x - (float)rect.left - scale*(float)pb.get_width()/2.0f; 
			center_offset_y = (float)rect.get_center().y - (float)rect.top - scale*(float)pb.get_height()/2.0f; 
		}

		Point offset = get_offset(mode);

		canvas.draw_pixels(rect.left + center_offset_x + offset.x, rect.top + center_offset_y + offset.y, scale, scale, pb, pb.get_size(), color);
	}
*/
}

void ListViewIcon::set_scalable(bool value)
{
	impl->scalable = value;
}

/////////////////////////////////////////////////////////////////////////////
// ListViewIcon Implementation:

}

#endif
