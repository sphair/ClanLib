/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/2D/image.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Resources/xml_resource_document.h"
#include "render_batch_triangle.h"
#include "../Render/graphic_context_impl.h"
#include "canvas_impl.h"

namespace clan
{

class Image_Impl
{
public:
	Image_Impl() :
		color(1.0f, 1.0f, 1.0f, 1.0f),
		scale_x(1.0f),
		scale_y(1.0f),
		translation_hotspot(0,0),
		translated_hotspot(0,0),
		translation_origin(origin_top_left) {};
	~Image_Impl() {};

	void calc_hotspot();

	Colorf color;

	float scale_x, scale_y;

	Point translation_hotspot;
	Origin translation_origin;

	Point translated_hotspot;	// Precalculated from calc_hotspot()

	Texture2D texture;
	Rect texture_rect;
};

void Image_Impl::calc_hotspot()
{
	switch(translation_origin)
	{
		case origin_top_left:
		default:
			translated_hotspot = Point(translation_hotspot.x, translation_hotspot.y);
			break;
		case origin_top_center:
			translated_hotspot = Point(translation_hotspot.x - texture_rect.get_width() * scale_x / 2, translation_hotspot.y);
			break;
		case origin_top_right:
			translated_hotspot = Point(translation_hotspot.x - texture_rect.get_width() * scale_x, translation_hotspot.y);
			break;
		case origin_center_left:
			translated_hotspot = Point(translation_hotspot.x, translation_hotspot.y - texture_rect.get_height() * scale_y / 2);
			break;
		case origin_center:
			translated_hotspot = Point(translation_hotspot.x - texture_rect.get_width() * scale_x / 2, translation_hotspot.y - texture_rect.get_height() * scale_y / 2);
			break;
		case origin_center_right:
			translated_hotspot = Point(translation_hotspot.x - texture_rect.get_width() * scale_x, translation_hotspot.y - texture_rect.get_height() * scale_y / 2);
			break;
		case origin_bottom_left:
			translated_hotspot = Point(translation_hotspot.x, translation_hotspot.y - texture_rect.get_height() * scale_y);
			break;
		case origin_bottom_center:
			translated_hotspot = Point(translation_hotspot.x - texture_rect.get_width() * scale_x / 2, translation_hotspot.y - texture_rect.get_height() * scale_y);
			break;
		case origin_bottom_right:
			translated_hotspot = Point(translation_hotspot.x - texture_rect.get_width() * scale_x, translation_hotspot.y - texture_rect.get_height() * scale_y);
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Image Construction:

Image::Image()
{
}

Image::Image(GraphicContext &gc, const PixelBuffer &pb, const Rect &rect)
: impl(new Image_Impl())
{
	impl->texture = Texture2D(gc, pb.get_width(), pb.get_height(), pb.get_format());
	impl->texture.set_subimage(gc, 0, 0, pb, rect);
	impl->texture_rect = Rect(0, 0, pb.get_width(), pb.get_height());
}

Image::Image(GraphicContext &gc, Texture2D texture, const Rect &rect)
: impl(new Image_Impl())
{
	impl->texture = texture;
	impl->texture_rect = rect;
}

Image::Image(GraphicContext &gc, Subtexture &sub_texture)
: impl(new Image_Impl())
{
	impl->texture = sub_texture.get_texture();
	impl->texture_rect = sub_texture.get_geometry();
}

Image::Image(GraphicContext &gc, const std::string &filename, FileSystem &fs, const ImageImportDescription &import_desc)
: impl(new Image_Impl())
{
	impl->texture = Texture2D(gc, filename, fs, import_desc);
	impl->texture_rect = impl->texture.get_size();
}

Image::Image(GraphicContext &gc, const std::string &fullname, const ImageImportDescription &import_desc)
: impl(new Image_Impl())
{
	std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
	std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
	FileSystem vfs(path);

	impl->texture = Texture2D(gc, filename, vfs, import_desc);
	impl->texture_rect = impl->texture.get_size();
}

Image::~Image()
{
}

/////////////////////////////////////////////////////////////////////////////
// Image Attributes:

void Image::throw_if_null() const
{
	if (!impl)
		throw Exception("Image is null");
}

float Image::get_scale_x() const
{
	return impl->scale_x;
}

float Image::get_scale_y() const
{
	return impl->scale_y;
}

float Image::get_alpha() const
{
	return impl->color.a;
}

Colorf Image::get_color() const
{
	return impl->color;
}

void Image::get_alignment(Origin &origin, int &x, int &y) const
{
	origin = impl->translation_origin;
	x = impl->translation_hotspot.x;
	y = impl->translation_hotspot.y;
}

int Image::get_width() const
{
	return impl->texture_rect.get_width();
}

int Image::get_height() const
{
	return impl->texture_rect.get_height();
}

Size Image::get_size() const
{
	return impl->texture_rect.get_size();
}

/////////////////////////////////////////////////////////////////////////////
// Image Operations:

void Image::set_subimage(
	GraphicContext &gc,
	int x,
	int y,
	const PixelBuffer &image,
	const Rect &src_rect,

	int level)
{
	impl->texture.set_subimage(gc, x, y, image, src_rect, level);
}

void Image::draw(Canvas &canvas, float x, float y) const
{
	Rectf dest(
		x + impl->translated_hotspot.x, y + impl->translated_hotspot.y, 
		Sizef(impl->texture_rect.get_width() * impl->scale_x, impl->texture_rect.get_height() * impl->scale_y));

	RenderBatchTriangle *batcher = canvas.impl->get_triangle_batcher();
	batcher->draw_image(canvas, impl->texture_rect, dest, impl->color, impl->texture);
}

void Image::draw(Canvas &canvas, int x, int y) const
{
	draw(canvas, (float) x, (float) y);
}

void Image::draw(Canvas &canvas, const Rectf &src, const Rectf &dest) const
{
	Rectf new_src = src;
	new_src.translate( impl->texture_rect.left, impl->texture_rect.top );

	Rectf new_dest = dest;
	new_dest.translate(impl->translated_hotspot);

	RenderBatchTriangle *batcher = canvas.impl->get_triangle_batcher();
	batcher->draw_image(canvas, new_src, new_dest, impl->color, impl->texture);
}

void Image::draw(Canvas &canvas, const Rectf &dest) const
{
	Rectf new_dest = dest;
	new_dest.translate(impl->translated_hotspot);

	RenderBatchTriangle *batcher = canvas.impl->get_triangle_batcher();
	batcher->draw_image(canvas, impl->texture_rect, new_dest, impl->color, impl->texture);
}

void Image::set_scale(float x, float y)
{
	impl->scale_x = x;
	impl->scale_y = y;
	impl->calc_hotspot();
}

void Image::set_alpha(float alpha)
{
	impl->color.a = alpha;
}

void Image::set_color(const Colorf &color)
{
	impl->color = color;
}

void Image::set_alignment(Origin origin, int x, int y)
{
	impl->translation_origin = origin;
	impl->translation_hotspot.x = x;
	impl->translation_hotspot.y = y;
	impl->calc_hotspot();
}

void Image::set_wrap_mode(
	TextureWrapMode wrap_s,
	TextureWrapMode wrap_t)
{
	impl->texture.set_wrap_mode(wrap_s, wrap_t);
}

void Image::set_linear_filter(bool linear_filter)
{
	impl->texture.set_mag_filter(linear_filter ? filter_linear : filter_nearest);
	impl->texture.set_min_filter(linear_filter ? filter_linear : filter_nearest);
}

}
