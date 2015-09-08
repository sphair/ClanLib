/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/Display/Render/texture_cube.h"
#include "API/Display/TargetProviders/texture_provider.h"
#include "graphic_context_impl.h"
#include "texture_impl.h"

namespace clan
{
	TextureCube::TextureCube()
	{
	}

	TextureCube::TextureCube(GraphicContext &context, int width, int height, TextureFormat texture_format, int levels)
		: Texture(std::shared_ptr<Texture_Impl>(new Texture_Impl))
	{
		if ((width <= 0) || (height <= 0))
		{
			throw Exception("An attempt was made to create a Texture with an invalid size");
		}

		GraphicContextProvider *gc_provider = context.get_provider();

		impl->provider = gc_provider->alloc_texture(texture_cube);
		impl->provider->create(width, height, 1, 1, texture_format, levels);
		impl->width = width;
		impl->height = height;

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t);
	}

	TextureCube::TextureCube(GraphicContext &context, const Size &size, TextureFormat texture_format, int levels)
		: Texture(std::shared_ptr<Texture_Impl>(new Texture_Impl))
	{
		if ((size.width <= 0) || (size.height <= 0))
		{
			throw Exception("An attempt was made to create a Texture with an invalid size");
		}

		GraphicContextProvider *gc_provider = context.get_provider();

		impl->provider = gc_provider->alloc_texture(texture_cube);
		impl->provider->create(size.width, size.height, 1, 1, texture_format, levels);
		impl->width = size.width;
		impl->height = size.height;

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t);
	}

	int TextureCube::get_width() const
	{
		return impl->width;
	}

	int TextureCube::get_height() const
	{
		return impl->height;
	}

	Size TextureCube::get_size() const
	{
		return Size(impl->width, impl->height);
	}

	void TextureCube::set_image(GraphicContext &context, TextureCubeDirection cube_direction, PixelBuffer &image, int level)
	{
		int array_index = static_cast<int>(cube_direction);
		impl->provider->copy_from(context, 0, 0, array_index, level, image, image.get_size());
	}

	void TextureCube::set_subimage(GraphicContext &context, TextureCubeDirection cube_direction, int x, int y, const PixelBuffer &image, const Rect &src_rect, int level)
	{
		int array_index = static_cast<int>(cube_direction);
		impl->provider->copy_from(context, x, y, array_index, level, image, src_rect);
	}

	void TextureCube::set_subimage(GraphicContext &context, TextureCubeDirection cube_direction, const Point &point, const PixelBuffer &image, const Rect &src_rect, int level)
	{
		int array_index = static_cast<int>(cube_direction);
		impl->provider->copy_from(context, point.x, point.y, array_index, level, image, src_rect);
	}
}
