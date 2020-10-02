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

#pragma once

#include "API/Display/Render/texture.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/TargetProviders/texture_provider.h"

namespace clan
{
	class Texture_Impl
	{
	public:
		Texture_Impl()
			: provider(nullptr),
			width(0),
			height(0),
			depth(0),
			array_size(0),
			min_lod(0),
			max_lod(0),
			lod_bias(0),
			base_level(0),
			max_level(0),
			wrap_mode_s(TextureWrapMode::clamp_to_edge),
			wrap_mode_t(TextureWrapMode::clamp_to_edge),
			wrap_mode_r(TextureWrapMode::clamp_to_edge),
			min_filter(TextureFilter::linear),
			mag_filter(TextureFilter::linear),
			max_anisotropy(1.0f),
			resident(0),
			compare_mode(TextureCompareMode::none),
			compare_function(CompareFunction::lequal)
		{
		}

		~Texture_Impl()
		{
			if (provider)
				delete provider;
		}

		bool operator<(const Texture_Impl &other) const
		{
			return provider < other.provider;
		}

		TextureProvider *provider;
		int width, height, depth;
		int array_size;
		float min_lod;
		float max_lod;
		float lod_bias;
		int base_level;
		int max_level;
		TextureWrapMode wrap_mode_s;
		TextureWrapMode wrap_mode_t;
		TextureWrapMode wrap_mode_r;
		TextureFilter min_filter;
		TextureFilter mag_filter;
		float max_anisotropy;
		bool resident;
		TextureCompareMode compare_mode;
		CompareFunction compare_function;

		float pixel_ratio = 0.0f;
	};
}
