/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/imageview.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/sprite_description.h"
#include "API/Display/2D/sprite.h"
#include "../gui_css_strings.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ImageView_Impl Class:

class CL_ImageView_Impl
{
public:
	CL_ImageView_Impl() : scale_to_fit(false), scale_x(1.0), scale_y(1.0)
	{
	}

	void on_process_message(CL_GUIMessage &msg);

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	
	void on_style_changed();

	void create_parts();

	CL_ImageView *image;

	CL_GUIThemePart part_component;

	CL_Sprite sprite;

	bool scale_to_fit;
	float scale_x;
	float scale_y;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ImageView Construction:

CL_ImageView::CL_ImageView(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_ImageView_Impl)
{
	set_type_name(CssStr::ImageView::type_name);
	impl->image = this;
	func_process_message().set(impl.get(), &CL_ImageView_Impl::on_process_message);
	func_render().set(impl.get(), &CL_ImageView_Impl::on_render);
	func_style_changed().set(impl.get(), &CL_ImageView_Impl::on_style_changed);

	impl->create_parts();
}

CL_ImageView::~CL_ImageView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ImageView Attributes:

CL_ImageView *CL_ImageView::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_ImageView *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_ImageView*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_ImageView named item: %1", id));

	return object;
}

CL_Sprite CL_ImageView::get_image() const
{
	return impl->sprite;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ImageView Operations:

void CL_ImageView::set_image(const CL_Sprite &image)
{
	impl->sprite = image;
	request_repaint();
}

void CL_ImageView::set_image(const CL_PixelBuffer &image)
{
	if (image.is_null())
	{
		impl->sprite = CL_Sprite();
		return;
	}

	CL_SpriteDescription sd;
	sd.add_frame(image);
	impl->sprite = CL_Sprite(get_gc(), sd);
	request_repaint();
}

void CL_ImageView::set_scale_to_fit()
{
	impl->scale_to_fit = true;
	request_repaint();
}

void CL_ImageView::set_scale( float x, float y )
{
	impl->scale_x = x;
	impl->scale_y = y;
	impl->scale_to_fit = false;
	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ImageView Implementation:

void CL_ImageView_Impl::on_process_message(CL_GUIMessage &msg)
{
}

void CL_ImageView_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect(CL_Point(0,0), image->get_geometry().get_size());
	part_component.render_box(gc, rect, update_rect);

	CL_Rect content = part_component.get_content_box(rect);
	if (!sprite.is_null())
	{
		if (scale_to_fit)
		{
			sprite.draw(gc, content);
		}
		else
		{
			sprite.set_scale(scale_x, scale_y);
			CL_Point pos = content.get_center();
			pos.x -= sprite.get_width()/2;
			pos.y -= sprite.get_height()/2;

			image->push_cliprect(gc, content);
			sprite.draw(gc, (float)pos.x, (float)pos.y);
			image->pop_cliprect(gc);
		}
	}
}

void CL_ImageView_Impl::create_parts()
{
	part_component = CL_GUIThemePart(image);
}

void CL_ImageView_Impl::on_style_changed()
{
	create_parts();
}
