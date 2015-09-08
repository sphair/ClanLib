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
#include "API/Display/Render/texture.h"
#include "API/Display/Render/texture_1d.h"
#include "API/Display/Render/texture_1d_array.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/Render/texture_2d_array.h"
#include "API/Display/Render/texture_3d.h"
#include "API/Display/Render/texture_cube.h"
#include "API/Display/Render/texture_cube_array.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Image/pixel_buffer_set.h"
#include "API/Display/Resources/display_cache.h"
#include "API/Core/Resources/xml_resource_manager.h"
#include "API/Core/Resources/xml_resource_node.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_format.h"
#include "texture_impl.h"

namespace clan
{
	Texture::Texture()
	{
	}

	Texture::Texture(GraphicContext &gc, PixelBufferSet pixelbuffer_set)
	{
		if (pixelbuffer_set.get_dimensions() == texture_1d)
		{
			Texture1D texture(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
			{
				PixelBuffer buffer = pixelbuffer_set.get_image(0, level);
				texture.set_image(gc, buffer, level);
			}
			*this = texture;
		}
		else if (pixelbuffer_set.get_dimensions() == texture_1d_array)
		{
			Texture1DArray texture(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_slice_count(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int slice = 0; slice < pixelbuffer_set.get_slice_count(); slice++)
			{
				for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
				{
					PixelBuffer buffer = pixelbuffer_set.get_image(slice, level);
					texture.set_image(gc, slice, buffer, level);
				}
			}
			*this = texture;
		}
		else if (pixelbuffer_set.get_dimensions() == texture_2d)
		{
			Texture2D texture(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_height(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
			{
				PixelBuffer buffer = pixelbuffer_set.get_image(0, level);
				texture.set_image(gc, buffer, level);
			}
			*this = texture;
		}
		else if (pixelbuffer_set.get_dimensions() == texture_2d_array)
		{
			Texture2DArray texture(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_height(), pixelbuffer_set.get_slice_count(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int slice = 0; slice < pixelbuffer_set.get_slice_count(); slice++)
			{
				for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
				{
					PixelBuffer buffer = pixelbuffer_set.get_image(slice, level);
					texture.set_image(gc, slice, buffer, level);
				}
			}
			*this = texture;
		}
		else if (pixelbuffer_set.get_dimensions() == texture_3d)
		{
			Texture3D texture(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_height(), pixelbuffer_set.get_slice_count(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int slice = 0; slice < pixelbuffer_set.get_slice_count(); slice++)
			{
				for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
				{
					PixelBuffer buffer = pixelbuffer_set.get_image(slice, level);
					texture.set_image(gc, buffer, slice, level);
				}
			}
			*this = texture;
		}
		else if (pixelbuffer_set.get_dimensions() == texture_cube)
		{
			TextureCube texture(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_height(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int slice = 0; slice < pixelbuffer_set.get_slice_count(); slice++)
			{
				for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
				{
					PixelBuffer buffer = pixelbuffer_set.get_image(slice, level);
					texture.set_image(gc, (TextureCubeDirection)slice, buffer, level);
				}
			}
			*this = texture;
		}
		else if (pixelbuffer_set.get_dimensions() == texture_cube_array)
		{
			TextureCubeArray texture(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_height(), pixelbuffer_set.get_slice_count(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int slice = 0; slice < pixelbuffer_set.get_slice_count(); slice++)
			{
				for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
				{
					PixelBuffer buffer = pixelbuffer_set.get_image(slice, level);
					texture.set_image(gc, slice / 6, (TextureCubeDirection)(slice % 6), buffer, level);
				}
			}
			*this = texture;
		}
		else
		{
			throw Exception("Unsupported texture dimensions");
		}
	}

	Texture::Texture(TextureProvider *provider)
		: impl(std::make_shared<Texture_Impl>())
	{
		impl->provider = provider;
	}

	Texture::Texture(const std::shared_ptr<Texture_Impl> &impl)
		: impl(impl)
	{
	}

	Texture::~Texture()
	{
	}

	Resource<Texture> Texture::resource(GraphicContext &gc, const std::string &id, const ResourceManager &resources)
	{
		return DisplayCache::get(resources).get_texture(gc, id);
	}

	void Texture::throw_if_null() const
	{
		if (!impl)
			throw Exception("Texture is null");
	}

	float Texture::get_min_lod() const
	{
		return impl->min_lod;
	}

	float Texture::get_max_lod() const
	{
		return impl->max_lod;
	}

	float Texture::get_lod_bias() const
	{
		return impl->lod_bias;
	}

	int Texture::get_base_level() const
	{
		return impl->base_level;
	}

	int Texture::get_max_level() const
	{
		return impl->max_level;
	}

	TextureFilter Texture::get_min_filter() const
	{
		return impl->min_filter;
	}

	TextureFilter Texture::get_mag_filter() const
	{
		return impl->mag_filter;
	}

	bool Texture::is_resident() const
	{
		return impl->resident;
	}

	TextureCompareMode Texture::get_compare_mode() const
	{
		return impl->compare_mode;
	}

	CompareFunction Texture::get_compare_function() const
	{
		return impl->compare_function;
	}

	TextureProvider *Texture::get_provider() const
	{
		if (!impl)
			return nullptr;
		else
			return impl->provider;
	}

	std::weak_ptr<Texture_Impl> Texture::get_impl() const
	{
		return std::weak_ptr<Texture_Impl>(impl);
	}

	void Texture::generate_mipmap()
	{
		impl->provider->generate_mipmap();
	}

	void Texture::set_min_lod(float min_lod)
	{
		if (impl->min_lod != min_lod)
		{
			impl->provider->set_min_lod(min_lod);
			impl->min_lod = min_lod;
		}
	}

	void Texture::set_max_lod(float max_lod)
	{
		if (impl->max_lod != max_lod)
		{
			impl->provider->set_max_lod(max_lod);
			impl->max_lod = max_lod;
		}
	}

	void Texture::set_lod_bias(float lod_bias)
	{
		if (impl->lod_bias != lod_bias)
		{
			impl->provider->set_lod_bias(lod_bias);
			impl->lod_bias = lod_bias;
		}
	}

	void Texture::set_base_level(int base_level)
	{
		if (impl->base_level != base_level)
		{
			impl->provider->set_base_level(base_level);
			impl->base_level = base_level;
		}
	}

	void Texture::set_max_level(int max_level)
	{
		if (impl->max_level != max_level)
		{
			impl->provider->set_max_level(max_level);
			impl->max_level = max_level;
		}
	}

	void Texture::set_min_filter(TextureFilter min_filter)
	{
		if (impl->min_filter != min_filter)
		{
			impl->provider->set_min_filter(min_filter);
			impl->min_filter = min_filter;
		}
	}

	void Texture::set_mag_filter(TextureFilter mag_filter)
	{
		if (impl->mag_filter != mag_filter)
		{
			impl->provider->set_mag_filter(mag_filter);
			impl->mag_filter = mag_filter;
		}
	}

	void Texture::set_max_anisotropy(float max_anisotropy)
	{
		if (impl->max_anisotropy != max_anisotropy)
		{
			impl->provider->set_max_anisotropy(max_anisotropy);
			impl->max_anisotropy = max_anisotropy;
		}
	}

	void Texture::set_texture_compare(TextureCompareMode mode, CompareFunction func)
	{
		if (impl->compare_mode != mode || impl->compare_function != func)
		{
			impl->compare_mode = mode;
			impl->compare_function = func;
			impl->provider->set_texture_compare(mode, func);
		}
	}

	Texture1D Texture::to_texture_1d() const
	{
		return Texture1D(impl);
	}

	Texture1DArray Texture::to_texture_1d_array() const
	{
		return Texture1DArray(impl);
	}

	Texture2D Texture::to_texture_2d() const
	{
		return Texture2D(impl);
	}

	Texture2DArray Texture::to_texture_2d_array() const
	{
		return Texture2DArray(impl);
	}

	Texture3D Texture::to_texture_3d() const
	{
		return Texture3D(impl);
	}

	TextureCube Texture::to_texture_cube() const
	{
		return TextureCube(impl);
	}

	TextureCubeArray Texture::to_texture_cube_array() const
	{
		return TextureCubeArray(impl);
	}
}
