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
#include "API/Display/Render/texture_1d.h"
#include "API/Display/TargetProviders/texture_provider.h"
#include "graphic_context_impl.h"
#include "texture_impl.h"

namespace clan
{
	Texture1D::Texture1D()
	{
	}

	Texture1D::Texture1D(GraphicContext &context, int size, TextureFormat texture_format, int levels)
		: Texture(std::shared_ptr<Texture_Impl>(new Texture_Impl))
	{
		if ((size <= 0))
		{
			throw Exception("An attempt was made to create a Texture with an invalid size");
		}

		GraphicContextProvider *gc_provider = context.get_provider();

		impl->provider = gc_provider->alloc_texture(texture_1d);
		impl->provider->create(size, 1, 1, 1, texture_format, levels);
		impl->width = size;

		impl->provider->set_wrap_mode(impl->wrap_mode_s);
	}

	int Texture1D::get_size() const
	{
		return impl->width;
	}

	TextureWrapMode Texture1D::get_wrap_mode_s() const
	{
		return impl->wrap_mode_s;
	}

	void Texture1D::set_image(GraphicContext &context, PixelBuffer &image, int level)
	{
		impl->provider->copy_from(context, 0, 0, 0, level, image, image.get_size());
	}

	void Texture1D::set_subimage(GraphicContext &context, int x, const PixelBuffer &image, const int src_x, const int src_width, int level)
	{
		impl->provider->copy_from(context, x, 0, 0, level, image, Rect(src_x, 0, src_x + src_width, 1));
	}

	void Texture1D::set_wrap_mode(TextureWrapMode wrap_s)
	{
		if (impl->wrap_mode_s != wrap_s)
		{
			impl->provider->set_wrap_mode(wrap_s);
			impl->wrap_mode_s = wrap_s;
		}
	}

	Texture1D::Texture1D(GraphicContext &context, const PixelBuffer &image, bool is_srgb)
	{
		*this = Texture1D(context, image, 0, image.get_width(), is_srgb);
	}

	Texture1D::Texture1D(GraphicContext &context, const PixelBuffer &image, int src_x, int width, bool is_srgb)
	{
		*this = Texture1D(context, width, is_srgb ? tf_srgb8_alpha8 : tf_rgba8);

		set_subimage(context, 0, image, src_x, width, 0);
		impl->provider->set_wrap_mode(impl->wrap_mode_s);
	}
}
