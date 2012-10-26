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
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/imageview.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/sprite_description.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/2D/canvas.h"
#include "../gui_css_strings.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ImageView_Impl Class:

class ImageView_Impl
{
public:
	ImageView_Impl() : scale_to_fit(false), scale_x(1.0), scale_y(1.0)
	{
	}

	void on_process_message(GUIMessage &msg);

	void on_render(Canvas &canvas, const Rect &update_rect);
	
	void on_style_changed();

	void create_parts();

	ImageView *image;

	GUIThemePart part_component;

	Sprite sprite;

	bool scale_to_fit;
	float scale_x;
	float scale_y;
};

/////////////////////////////////////////////////////////////////////////////
// ImageView Construction:

ImageView::ImageView(GUIComponent *parent)
: GUIComponent(parent), impl(new ImageView_Impl)
{
	set_type_name(CssStr::ImageView::type_name);
	impl->image = this;
	func_process_message().set(impl.get(), &ImageView_Impl::on_process_message);
	func_render().set(impl.get(), &ImageView_Impl::on_render);
	func_style_changed().set(impl.get(), &ImageView_Impl::on_style_changed);

	impl->create_parts();
}

ImageView::~ImageView()
{
}

/////////////////////////////////////////////////////////////////////////////
// ImageView Attributes:

ImageView *ImageView::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	ImageView *object = NULL;
	if (reference_component)
		object = dynamic_cast<ImageView*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find ImageView named item: %1", id));

	return object;
}

Sprite ImageView::get_image() const
{
	return impl->sprite;
}

/////////////////////////////////////////////////////////////////////////////
// ImageView Operations:

void ImageView::set_image(const Sprite &image)
{
	impl->sprite = image;
	request_repaint();
}

void ImageView::set_image(const PixelBuffer &image)
{
	if (image.is_null())
	{
		impl->sprite = Sprite();
		return;
	}

	SpriteDescription sd;
	sd.add_frame(image);
	impl->sprite = Sprite(get_canvas(), sd);
	request_repaint();
}

void ImageView::set_scale_to_fit()
{
	impl->scale_to_fit = true;
	request_repaint();
}

void ImageView::set_scale( float x, float y )
{
	impl->scale_x = x;
	impl->scale_y = y;
	impl->scale_to_fit = false;
	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// ImageView Implementation:

void ImageView_Impl::on_process_message(GUIMessage &msg)
{
}

void ImageView_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect(Point(0,0), image->get_geometry().get_size());
	part_component.render_box(canvas, rect, update_rect);

	Rect content = part_component.get_content_box(rect);
	if (!sprite.is_null())
	{
		if (scale_to_fit)
		{
			sprite.draw(canvas, content);
		}
		else
		{
			sprite.set_scale(scale_x, scale_y);
			Point pos = content.get_center();
			pos.x -= sprite.get_width()/2;
			pos.y -= sprite.get_height()/2;

			image->push_cliprect(canvas, content);
			sprite.draw(canvas, (float)pos.x, (float)pos.y);
			image->pop_cliprect(canvas);
		}
	}
}

void ImageView_Impl::create_parts()
{
	part_component = GUIThemePart(image);
}

void ImageView_Impl::on_style_changed()
{
	create_parts();
}

}
