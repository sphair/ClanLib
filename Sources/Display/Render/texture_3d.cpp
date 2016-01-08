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
#include "API/Display/Render/texture_3d.h"
#include "API/Display/TargetProviders/texture_provider.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "texture_impl.h"

namespace clan
{
	Texture3D::Texture3D()
	{
	}

	Texture3D::Texture3D(GraphicContext &context, int width, int height, int depth, TextureFormat texture_format, int levels)
		: Texture(std::shared_ptr<Texture_Impl>(new Texture_Impl))
	{
		if ((width <= 0) || (height <= 0) || (depth <= 0))
		{
			throw Exception("An attempt was made to create a Texture with an invalid size");
		}

		GraphicContextProvider *gc_provider = context.get_provider();

		impl->provider = gc_provider->alloc_texture(texture_3d);
		impl->provider->create(width, height, depth, 1, texture_format, levels);
		impl->width = width;
		impl->height = height;
		impl->depth = depth;

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t, impl->wrap_mode_r);
	}

	Texture3D::Texture3D(GraphicContext &context, const Vec3i &size, TextureFormat texture_format, int levels)
		: Texture(std::shared_ptr<Texture_Impl>(new Texture_Impl))
	{
		if ((size.x <= 0) || (size.y <= 0) || (size.z <= 0))
		{
			throw Exception("An attempt was made to create a Texture with an invalid size");
		}

		GraphicContextProvider *gc_provider = context.get_provider();

		impl->provider = gc_provider->alloc_texture(texture_3d);
		impl->provider->create(size.x, size.y, size.z, 1, texture_format, levels);
		impl->width = size.x;
		impl->height = size.y;
		impl->depth = size.z;

		impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t, impl->wrap_mode_r);
	}

	int Texture3D::get_width() const
	{
		return impl->width;
	}

	int Texture3D::get_height() const
	{
		return impl->height;
	}

	int Texture3D::get_depth() const
	{
		return impl->depth;
	}

	Vec3i Texture3D::get_size() const
	{
		return Vec3i(impl->width, impl->height, impl->depth);
	}

	TextureWrapMode Texture3D::get_wrap_mode_s() const
	{
		return impl->wrap_mode_s;
	}

	TextureWrapMode Texture3D::get_wrap_mode_t() const
	{
		return impl->wrap_mode_t;
	}

	TextureWrapMode Texture3D::get_wrap_mode_r() const
	{
		return impl->wrap_mode_r;
	}

	void Texture3D::set_image(GraphicContext &context, PixelBuffer &image, int depth, int level)
	{
		impl->provider->copy_from(context, 0, 0, depth, level, image, image.get_size());
	}

	void Texture3D::set_subimage(GraphicContext &context, int x, int y, int z, const PixelBuffer &image, const Rect &src_rect, int level)
	{
		impl->provider->copy_from(context, x, y, z, level, image, src_rect);
	}

	void Texture3D::set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t, TextureWrapMode wrap_r)
	{
		if (impl->wrap_mode_s != wrap_s || impl->wrap_mode_t != wrap_t || impl->wrap_mode_r != wrap_r)
		{
			impl->provider->set_wrap_mode(wrap_s, wrap_t, wrap_r);
			impl->wrap_mode_s = wrap_s;
			impl->wrap_mode_t = wrap_t;
			impl->wrap_mode_r = wrap_r;
		}
	}
}
