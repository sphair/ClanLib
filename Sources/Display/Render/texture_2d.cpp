/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/TargetProviders/texture_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_format.h"
#include "graphic_context_impl.h"
#include "texture_impl.h"
#include "API/Display/Resources/display_cache.h"

namespace clan
{
	Texture2D::Texture2D()
	{
	}

	Texture2D::Texture2D(GraphicContext &context, int width, int height, TextureFormat texture_format, int levels)
		: Texture(std::shared_ptr<Texture_Impl>(new Texture_Impl))
	{
		if ((width <= 0) || (height <= 0))
		{
			throw Exception("An attempt was made to create a Texture with an invalid size");
		}

		GraphicContextProvider *gc_provider = context.get_provider();

		impl->provider = gc_provider->alloc_texture(TextureDimensions::_2d);
		impl->provider->create(width, height, 1, 1, texture_format, levels);
		impl->width = width;
		impl->height = height;

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t);
	}

	Texture2D::Texture2D(GraphicContext &context, const Size &size, TextureFormat texture_format, int levels)
		: Texture(std::shared_ptr<Texture_Impl>(new Texture_Impl))
	{
		if ((size.width <= 0) || (size.height <= 0))
		{
			throw Exception("An attempt was made to create a Texture with an invalid size");
		}

		GraphicContextProvider *gc_provider = context.get_provider();

		impl->provider = gc_provider->alloc_texture(TextureDimensions::_2d);
		impl->provider->create(size.width, size.height, 1, 1, texture_format, levels);
		impl->width = size.width;
		impl->height = size.height;

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t);
	}

	Texture2D::Texture2D(GraphicContext &context, const std::string &fullname, const ImageImportDescription &import_desc)
	{
		std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
		std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
		FileSystem vfs(path);
		*this = Texture2D(context, filename, vfs, import_desc);
	}

	Texture2D::Texture2D(GraphicContext &context, const std::string &filename, const FileSystem &fs, const ImageImportDescription &import_desc)
	{
		PixelBuffer pb = ImageProviderFactory::load(filename, fs, std::string());
		pb = import_desc.process(pb);

		*this = Texture2D(context, pb.get_width(), pb.get_height(), import_desc.is_srgb() ? TextureFormat::srgb8_alpha8 : TextureFormat::rgba8);

		set_pixel_ratio(pb.get_pixel_ratio());
		set_subimage(context, Point(0, 0), pb, Rect(pb.get_size()), 0);

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t);
	}

	Texture2D::Texture2D(GraphicContext &context, IODevice &file, const std::string &image_type, const ImageImportDescription &import_desc)
	{
		PixelBuffer pb = ImageProviderFactory::load(file, image_type);
		pb = import_desc.process(pb);
		*this = Texture2D(context, pb.get_width(), pb.get_height(), import_desc.is_srgb() ? TextureFormat::srgb8_alpha8 : TextureFormat::rgba8);

		set_pixel_ratio(pb.get_pixel_ratio());
		set_subimage(context, Point(0, 0), pb, Rect(pb.get_size()), 0);

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t);
	}

	Texture2D::Texture2D(GraphicContext &context, const PixelBuffer &image, bool is_srgb)
	{
		*this = Texture2D(context, image, image.get_size(), is_srgb);
	}

	Texture2D::Texture2D(GraphicContext &context, const PixelBuffer &image, const Rect &src_rect, bool is_srgb)
	{
		*this = Texture2D(context, src_rect.get_width(), src_rect.get_height(), is_srgb ? TextureFormat::srgb8_alpha8 : TextureFormat::rgba8);

		set_pixel_ratio(image.get_pixel_ratio());
		set_subimage(context, Point(0, 0), image, src_rect, 0);

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t);
	}

	int Texture2D::get_width() const
	{
		return impl->width;
	}

	int Texture2D::get_height() const
	{
		return impl->height;
	}

	PixelBuffer Texture2D::get_pixeldata(GraphicContext &gc, int level) const
	{
		return impl->provider->get_pixeldata(gc, TextureFormat::rgba8, level);
	}

	PixelBuffer Texture2D::get_pixeldata(GraphicContext &gc, TextureFormat texture_format, int level) const
	{
		return impl->provider->get_pixeldata(gc, texture_format, level);
	}

	TextureWrapMode Texture2D::get_wrap_mode_s() const
	{
		return impl->wrap_mode_s;
	}

	TextureWrapMode Texture2D::get_wrap_mode_t() const
	{
		return impl->wrap_mode_t;
	}

	void Texture2D::set_image(GraphicContext &context, const PixelBuffer &image, int level)
	{
		impl->provider->copy_from(context, 0, 0, 0, level, image, image.get_size());
	}

	void Texture2D::set_subimage(GraphicContext &context, int x, int y, const PixelBuffer &image, const Rect &src_rect, int level)
	{
		impl->provider->copy_from(context, x, y, 0, level, image, src_rect);
	}

	void Texture2D::set_subimage(GraphicContext &context, const Point &point, const PixelBuffer &image, const Rect &src_rect, int level)
	{
		impl->provider->copy_from(context, point.x, point.y, 0, level, image, src_rect);
	}

	void Texture2D::copy_image_from(GraphicContext &context, int level, TextureFormat texture_format)
	{
		impl->provider->copy_image_from(0, 0, impl->width, impl->height, level, texture_format, context.get_provider());
	}

	void Texture2D::copy_image_from(GraphicContext &context, int x, int y, int width, int height, int level, TextureFormat texture_format)
	{
		impl->provider->copy_image_from(x, y, width, height, level, texture_format, context.get_provider());
	}

	void Texture2D::copy_image_from(GraphicContext &context, const Rect &pos, int level, TextureFormat texture_format)
	{
		impl->provider->copy_image_from(pos.left, pos.top, pos.get_width(), pos.get_height(), level, texture_format, context.get_provider());
	}

	void Texture2D::copy_subimage_from(GraphicContext &context, int offset_x, int offset_y, int x, int y, int width, int height, int level)
	{
		impl->provider->copy_subimage_from(offset_x, offset_y, x, y, width, height, level, context.get_provider());
	}

	void Texture2D::copy_subimage_from(GraphicContext &context, const Point &offset, const Rect &pos, int level)
	{
		impl->provider->copy_subimage_from(offset.x, offset.y, pos.left, pos.top, pos.get_width(), pos.get_height(), level, context.get_provider());
	}

	void Texture2D::set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t)
	{
		if (impl->wrap_mode_s != wrap_s || impl->wrap_mode_t != wrap_t)
		{
			impl->provider->set_wrap_mode(wrap_s, wrap_t);
			impl->wrap_mode_s = wrap_s;
			impl->wrap_mode_t = wrap_t;
		}
	}

	float Texture2D::get_pixel_ratio() const
	{
		return impl->pixel_ratio;
	}

	void Texture2D::set_pixel_ratio(float ratio)
	{
		impl->pixel_ratio = ratio;
	}
}
