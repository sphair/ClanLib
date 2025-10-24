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
**    Kenneth Gangstoe
*/

#include "Display/precomp.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/2D/image.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/2D/sprite_description.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "render_batch2d.h"
#include "../Render/graphic_context_impl.h"

class CL_Image_Impl
{
public:
	CL_Image_Impl() :
		color(1.0f, 1.0f, 1.0f, 1.0f),
		scale_x(1.0f),
		scale_y(1.0f),
		translation_hotspot(0,0),
		translated_hotspot(0,0),
		translation_origin(origin_top_left) {};
	~CL_Image_Impl() {};

	void calc_hotspot();

	CL_Colorf color;

	float scale_x, scale_y;

	CL_Point translation_hotspot;
	CL_Origin translation_origin;

	CL_Point translated_hotspot;	// Preculated from calc_hotspot()

	CL_Texture texture;
	CL_Rect texture_rect;
};

void CL_Image_Impl::calc_hotspot()
{
	switch(translation_origin)
	{
		case origin_top_left:
		default:
			translated_hotspot = CL_Point(translation_hotspot.x, translation_hotspot.y);
			break;
		case origin_top_center:
			translated_hotspot = CL_Point(translation_hotspot.x - texture_rect.get_width() * scale_x / 2, translation_hotspot.y);
			break;
		case origin_top_right:
			translated_hotspot = CL_Point(translation_hotspot.x - texture_rect.get_width() * scale_x, translation_hotspot.y);
			break;
		case origin_center_left:
			translated_hotspot = CL_Point(translation_hotspot.x, translation_hotspot.y - texture_rect.get_height() * scale_y / 2);
			break;
		case origin_center:
			translated_hotspot = CL_Point(translation_hotspot.x - texture_rect.get_width() * scale_x / 2, translation_hotspot.y - texture_rect.get_height() * scale_y / 2);
			break;
		case origin_center_right:
			translated_hotspot = CL_Point(translation_hotspot.x - texture_rect.get_width() * scale_x, translation_hotspot.y - texture_rect.get_height() * scale_y / 2);
			break;
		case origin_bottom_left:
			translated_hotspot = CL_Point(translation_hotspot.x, translation_hotspot.y - texture_rect.get_height() * scale_y);
			break;
		case origin_bottom_center:
			translated_hotspot = CL_Point(translation_hotspot.x - texture_rect.get_width() * scale_x / 2, translation_hotspot.y - texture_rect.get_height() * scale_y);
			break;
		case origin_bottom_right:
			translated_hotspot = CL_Point(translation_hotspot.x - texture_rect.get_width() * scale_x, translation_hotspot.y - texture_rect.get_height() * scale_y);
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Image Construction:

CL_Image::CL_Image()
{
}

CL_Image::CL_Image(CL_GraphicContext &gc, const CL_PixelBuffer &pb, const CL_Rect &rect)
: impl(new CL_Image_Impl())
{
	impl->texture = CL_Texture(gc, pb.get_width(), pb.get_height());
	impl->texture.set_subimage(0, 0, pb, rect);
	impl->texture_rect = CL_Rect(0, 0, pb.get_width(), pb.get_height());
}

CL_Image::CL_Image(CL_GraphicContext &gc, CL_Texture texture, CL_Rect rect)
: impl(new CL_Image_Impl())
{
	impl->texture = texture;
	impl->texture_rect = rect;
}

CL_Image::CL_Image(CL_GraphicContext &gc, CL_Subtexture &sub_texture)
: impl(new CL_Image_Impl())
{
	impl->texture = sub_texture.get_texture();
	impl->texture_rect = sub_texture.get_geometry();
}

CL_Image::CL_Image(CL_GraphicContext &gc, const CL_StringRef &filename, CL_VirtualDirectory &dir, const CL_ImageImportDescription &import_desc)
: impl(new CL_Image_Impl())
{
	impl->texture = CL_SharedGCData::load_texture(gc, filename, dir, import_desc);
	impl->texture_rect = impl->texture.get_size();
}

CL_Image::CL_Image(CL_GraphicContext &gc, const CL_StringRef &fullname, const CL_ImageImportDescription &import_desc)
: impl(new CL_Image_Impl())
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);

	impl->texture = CL_SharedGCData::load_texture(gc, filename, vfs.get_root_directory(), import_desc);
	impl->texture_rect = impl->texture.get_size();
}

CL_Image::CL_Image(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources, const CL_ImageImportDescription &import_desc)
: impl(new CL_Image_Impl())
{
	CL_Resource resource = resources->get_resource(resource_id);
	CL_String type = resource.get_element().get_tag_name();

	if (type != "image")
		throw CL_Exception(cl_format("Resource '%1' is not of type 'image'", resource_id));

	CL_SpriteDescription desc(gc, resource_id, resources, import_desc);
	
	const std::vector<CL_SpriteDescriptionFrame> &frames = desc.get_frames();
	if(frames[0].type == CL_SpriteDescriptionFrame::type_texture)
	{
		impl->texture = frames[0].texture;
		impl->texture_rect = frames[0].rect;
	}
	else
	{
		throw CL_Exception("Only Textures supported in CL_Image");
	}

	// TODO: Create textures

	CL_DomNode cur_node = resource.get_element().get_first_child();
	while (!cur_node.is_null())
	{
		if (!cur_node.is_element())
			continue;

		CL_DomElement cur_element = cur_node.to_element();

		CL_String tag_name = cur_element.get_tag_name();

		// <color red="float" green="float" blue="float" alpha="float" />
		if (tag_name == "color")
		{
			impl->color.r = (float)CL_StringHelp::text_to_float(cur_element.get_attribute("red", "1.0"));
			impl->color.g = (float)CL_StringHelp::text_to_float(cur_element.get_attribute("green", "1.0"));
			impl->color.b = (float)CL_StringHelp::text_to_float(cur_element.get_attribute("blue", "1.0"));
			impl->color.a = (float)CL_StringHelp::text_to_float(cur_element.get_attribute("alpha", "1.0"));
		}
		// <scale x="float" y="float />
		else if (tag_name == "scale")
		{
			impl->scale_x = CL_StringHelp::text_to_float(cur_element.get_attribute("x", "1.0"));
			impl->scale_y = CL_StringHelp::text_to_float(cur_element.get_attribute("y", "1.0"));
		}
		// <translation origin="string" x="integer" y="integer" />
		else if (tag_name == "translation")
		{
			CL_String hotspot = cur_element.get_attribute("origin", "top_left");
			CL_Origin origin;

			if(hotspot == "center")
				origin = origin_center;
			else if(hotspot == "top_center")
				origin = origin_top_center;
			else if(hotspot == "top_right")
				origin = origin_top_right;
			else if(hotspot == "center_left")
				origin = origin_center_left;
			else if(hotspot == "center_right")
				origin = origin_center_right;
			else if(hotspot == "bottom_left")
				origin = origin_bottom_left;
			else if(hotspot == "bottom_center")
				origin = origin_bottom_center;
			else if(hotspot == "bottom_right")
				origin = origin_bottom_right;
			else
				origin = origin_top_left;

			int xoffset = CL_StringHelp::text_to_int(cur_element.get_attribute("x", "0"));
			int yoffset = CL_StringHelp::text_to_int(cur_element.get_attribute("y", "0"));

			impl->translation_origin = origin;
			impl->translation_hotspot.x = xoffset;
			impl->translation_hotspot.y = yoffset;
		}

		cur_node = cur_node.get_next_sibling();
	}

	impl->calc_hotspot();
}

CL_Image::~CL_Image()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Image Attributes:

void CL_Image::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_Image is null");
}


float CL_Image::get_scale_x() const
{
	return impl->scale_x;
}

float CL_Image::get_scale_y() const
{
	return impl->scale_y;
}

float CL_Image::get_alpha() const
{
	return impl->color.a;
}

CL_Colorf CL_Image::get_color() const
{
	return impl->color;
}

void CL_Image::get_alignment(CL_Origin &origin, int &x, int &y) const
{
	origin = impl->translation_origin;
	x = impl->translation_hotspot.x;
	y = impl->translation_hotspot.y;
}

int CL_Image::get_width() const
{
	return impl->texture_rect.get_width();
}

int CL_Image::get_height() const
{
	return impl->texture_rect.get_height();
}

CL_Size CL_Image::get_size() const
{
	return impl->texture_rect.get_size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Image Operations:

void CL_Image::draw(CL_GraphicContext &gc, float x, float y) const
{
	CL_Rectf dest(
		x + impl->translated_hotspot.x, y + impl->translated_hotspot.y, 
		CL_Sizef(impl->texture_rect.get_width() * impl->scale_x, impl->texture_rect.get_height() * impl->scale_y));

	CL_RenderBatcherSprite *batcher = gc.impl->current_internal_batcher;
	batcher->draw_image(gc, impl->texture_rect, dest, impl->color, impl->texture);
}

void CL_Image::draw(CL_GraphicContext &gc, int x, int y) const
{
	draw(gc, (float) x, (float) y);
}

void CL_Image::draw(CL_GraphicContext &gc, const CL_Rectf &src, const CL_Rectf &dest) const
{
	CL_Rectf new_src = src;
	new_src.translate( impl->texture_rect.left, impl->texture_rect.top );

	CL_Rectf new_dest = dest;
	new_dest.translate(impl->translated_hotspot);

	CL_RenderBatcherSprite *batcher = gc.impl->current_internal_batcher;
	batcher->draw_image(gc, new_src, new_dest, impl->color, impl->texture);
}

void CL_Image::draw(CL_GraphicContext &gc, const CL_Rectf &dest) const
{
	CL_Rectf new_dest = dest;
	new_dest.translate(impl->translated_hotspot);

	CL_RenderBatcherSprite *batcher = gc.impl->current_internal_batcher;
	batcher->draw_image(gc, impl->texture_rect, new_dest, impl->color, impl->texture);
}

void CL_Image::set_scale(float x, float y)
{
	impl->scale_x = x;
	impl->scale_y = y;
	impl->calc_hotspot();
}

void CL_Image::set_alpha(float alpha)
{
	impl->color.a = alpha;
}

void CL_Image::set_color(const CL_Colorf &color)
{
	impl->color = color;
}

void CL_Image::set_alignment(CL_Origin origin, int x, int y)
{
	impl->translation_origin = origin;
	impl->translation_hotspot.x = x;
	impl->translation_hotspot.y = y;
	impl->calc_hotspot();
}
