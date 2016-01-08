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
#include "API/Display/Render/texture_2d_array.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/TargetProviders/texture_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "texture_impl.h"

namespace clan
{
	Texture2DArray::Texture2DArray()
	{
	}

	Texture2DArray::Texture2DArray(GraphicContext &context, int width, int height, int array_size, TextureFormat texture_format, int levels)
		: Texture(std::shared_ptr<Texture_Impl>(new Texture_Impl))
	{
		if ((width <= 0) || (height <= 0))
			throw Exception("An attempt was made to create a Texture with an invalid size");

		GraphicContextProvider *gc_provider = context.get_provider();
		impl->provider = gc_provider->alloc_texture(texture_2d_array);
		impl->provider->create(width, height, 1, array_size, texture_format, levels);
		impl->width = width;
		impl->height = height;
		impl->array_size = array_size;

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t);
	}

	Texture2DArray::Texture2DArray(GraphicContext &context, const Size &size, int array_size, TextureFormat texture_format, int levels)
		: Texture(std::shared_ptr<Texture_Impl>(new Texture_Impl))
	{
		if ((size.width <= 0) || (size.height <= 0))
			throw Exception("An attempt was made to create a Texture with an invalid size");

		GraphicContextProvider *gc_provider = context.get_provider();
		impl->provider = gc_provider->alloc_texture(texture_2d_array);
		impl->provider->create(size.width, size.height, 1, array_size, texture_format, levels);
		impl->width = size.width;
		impl->height = size.height;
		impl->array_size = array_size;

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t);
	}

	int Texture2DArray::get_width() const
	{
		return impl->width;
	}

	int Texture2DArray::get_height() const
	{
		return impl->height;
	}

	int Texture2DArray::get_array_size() const
	{
		return impl->array_size;
	}

	TextureWrapMode Texture2DArray::get_wrap_mode_s() const
	{
		return impl->wrap_mode_s;
	}

	TextureWrapMode Texture2DArray::get_wrap_mode_t() const
	{
		return impl->wrap_mode_t;
	}

	void Texture2DArray::set_image(GraphicContext &context, int array_index, const PixelBuffer &image, int level)
	{
		impl->provider->copy_from(context, 0, 0, array_index, level, image, image.get_size());
	}

	void Texture2DArray::set_subimage(GraphicContext &context, int array_index, int x, int y, const PixelBuffer &image, const Rect &src_rect, int level)
	{
		impl->provider->copy_from(context, x, y, array_index, level, image, src_rect);
	}

	void Texture2DArray::set_subimage(GraphicContext &context, int array_index, const Point &point, const PixelBuffer &image, const Rect &src_rect, int level)
	{
		impl->provider->copy_from(context, point.x, point.y, array_index, level, image, src_rect);
	}

	void Texture2DArray::set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t)
	{
		if (impl->wrap_mode_s != wrap_s || impl->wrap_mode_t != wrap_t)
		{
			impl->provider->set_wrap_mode(wrap_s, wrap_t);
			impl->wrap_mode_s = wrap_s;
			impl->wrap_mode_t = wrap_t;
		}
	}

	Texture2D Texture2DArray::create_2d_view(int array_index, TextureFormat texture_format, int min_level, int num_levels)
	{
		// To do: create and initialize the impl in a way that would actually pass a code review..
		Texture view(std::shared_ptr<Texture_Impl>(new Texture_Impl));
		view.impl->width = impl->width;
		view.impl->height = impl->height;
		view.impl->array_size = impl->array_size;
		view.impl->provider = impl->provider->create_view(texture_2d, texture_format, min_level, num_levels, array_index, 1);
		return view.to_texture_2d();
	}

	float Texture2DArray::get_pixel_ratio() const
	{
		return impl->pixel_ratio;
	}

	void Texture2DArray::set_pixel_ratio(float ratio)
	{
		impl->pixel_ratio = ratio;
	}
}
