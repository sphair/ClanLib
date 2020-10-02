/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/Display/Render/texture_cube_array.h"
#include "API/Display/TargetProviders/texture_provider.h"
#include "graphic_context_impl.h"
#include "texture_impl.h"

namespace clan
{
	TextureCubeArray::TextureCubeArray()
	{
	}

	TextureCubeArray::TextureCubeArray(GraphicContext &context, int width, int height, int array_size, TextureFormat texture_format, int levels)
		: Texture(std::shared_ptr<Texture_Impl>(new Texture_Impl))
	{
		if ((width <= 0) || (height <= 0))
		{
			throw Exception("An attempt was made to create a Texture with an invalid size");
		}

		GraphicContextProvider *gc_provider = context.get_provider();

		impl->provider = gc_provider->alloc_texture(TextureDimensions::_cube_array);
		impl->provider->create(width, height, 1, array_size, texture_format, levels);
		impl->width = width;
		impl->height = height;
		impl->array_size = array_size;

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t);
	}

	TextureCubeArray::TextureCubeArray(GraphicContext &context, const Size &size, int array_size, TextureFormat texture_format, int levels)
		: Texture(std::shared_ptr<Texture_Impl>(new Texture_Impl))
	{
		if ((size.width <= 0) || (size.height <= 0))
		{
			throw Exception("An attempt was made to create a Texture with an invalid size");
		}

		GraphicContextProvider *gc_provider = context.get_provider();

		impl->provider = gc_provider->alloc_texture(TextureDimensions::_cube_array);
		impl->provider->create(size.width, size.height, 1, array_size, texture_format, levels);
		impl->width = size.width;
		impl->height = size.height;
		impl->array_size = array_size;

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t);
	}

	int TextureCubeArray::get_width() const
	{
		return impl->width;
	}

	int TextureCubeArray::get_height() const
	{
		return impl->height;
	}

	int TextureCubeArray::get_array_size() const
	{
		return impl->array_size;
	}

	Size TextureCubeArray::get_size() const
	{
		return Size(impl->width, impl->height);
	}

	void TextureCubeArray::set_image(GraphicContext &context, int array_index, TextureCubeDirection cube_direction, PixelBuffer &image, int level)
	{
		int slice = array_index * 6 + static_cast<int>(cube_direction);
		impl->provider->copy_from(context, 0, 0, slice, level, image, image.get_size());
	}

	void TextureCubeArray::set_subimage(GraphicContext &context, int array_index, TextureCubeDirection cube_direction, int x, int y, const PixelBuffer &image, const Rect &src_rect, int level)
	{
		int slice = array_index * 6 + static_cast<int>(cube_direction);
		impl->provider->copy_from(context, x, y, slice, level, image, src_rect);
	}

	void TextureCubeArray::set_subimage(GraphicContext &context, int array_index, TextureCubeDirection cube_direction, const Point &point, const PixelBuffer &image, const Rect &src_rect, int level)
	{
		int slice = array_index * 6 + static_cast<int>(cube_direction);
		impl->provider->copy_from(context, point.x, point.y, slice, level, image, src_rect);
	}
}
