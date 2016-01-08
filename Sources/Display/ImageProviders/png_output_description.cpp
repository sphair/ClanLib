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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/datetime.h"
#include "API/Display/2D/color.h"
#include "API/Display/ImageProviders/png_output_description.h"

namespace clan
{
	struct PNGOutputDescriptionComment
	{
		std::string comment;
		std::string keyword; // max 79 characters.
		std::string lang;
		std::string translated_keyword;
	};

	struct PNGOutputDescriptionUnknownChunk
	{
		std::string name;
		DataBuffer data;
		PNGUnknownChunkLocation location;
	};

	class PNGOutputDescription_Impl
	{
	public:
		PNGOutputDescription_Impl()
		{
		}

		Color background_color;
		int bit_depth;
		PNGColorType color_type;
		std::vector<std::shared_ptr<PNGOutputDescriptionComment> > comments;
		PNGFilterMethod filter_method;
		float gamma;
		PNGInterlaceType interlace_type;
		DataBuffer icc_profile_data;
		std::string icc_profile_name;
		Color key_color;
		int num_significant_bits;
		std::vector<PNGOutputDescriptionPalette> palettes;
		PNGsRGBIntent srgb_intent;
		bool write_srgb_chunk;
		Vec2i offset;
		PNGOffsetUnitType offset_unit_type;
		DateTime modification_time;
		std::vector<std::shared_ptr<PNGOutputDescriptionUnknownChunk> > unknown_chunks;
		Vec2i resolution;
		PNGResolutionUnitType resolution_unit_type;
		Vec2f chroma_white, chroma_red, chroma_green, chroma_blue;
		Vec3f chroma_xyz_red, chroma_xyz_green, chroma_xyz_blue;
		bool use_xyz_chroma;
		int physical_scale_units;
		Sized pixel_size_in_scale_units;
	};

	PNGOutputDescription::PNGOutputDescription(int bit_depth, PNGColorType color_type)
		: impl(std::make_shared<PNGOutputDescription_Impl>())
	{
		impl->bit_depth = bit_depth;
		impl->color_type = color_type;
	}

	void PNGOutputDescription::add_palette(const PNGOutputDescriptionPalette &palette)
	{
		impl->palettes.push_back(palette);
	}

	void PNGOutputDescription::add_comment(const std::string &comment, std::string &keyword, /* max 79 characters. */ const std::string &lang, const std::string &translated_keyword)
	{
		std::shared_ptr<PNGOutputDescriptionComment> c(new PNGOutputDescriptionComment);
		c->comment = comment;
		c->keyword = keyword;
		c->lang = lang;
		c->translated_keyword = translated_keyword;
		impl->comments.push_back(c);
	}

	void PNGOutputDescription::add_unknown_chunk(const std::string &name, const DataBuffer &data, PNGUnknownChunkLocation location)
	{
		std::shared_ptr<PNGOutputDescriptionUnknownChunk> chunk;
		chunk->name = name;
		chunk->data = data;
		chunk->location = location;
		impl->unknown_chunks.push_back(chunk);
	}

	void PNGOutputDescription::set_background_color(const Color &background_color)
	{
		impl->background_color = background_color;
	}

	void PNGOutputDescription::set_chromacities(const Vec2f &white, const Vec2f &red, const Vec2f &green, const Vec2f &blue)
	{
		impl->chroma_white = white;
		impl->use_xyz_chroma = false;
	}

	void PNGOutputDescription::set_chromacities_XYZ(const Vec3f &red, const Vec3f &green, const Vec3f &blue)
	{
		impl->use_xyz_chroma = true;
	}

	void PNGOutputDescription::set_key_color(const Color &color)
	{
		impl->key_color = color;
	}

	void PNGOutputDescription::set_filter_method(PNGFilterMethod filter)
	{
		impl->filter_method = filter;
	}

	void PNGOutputDescription::set_gamma(float gamma)
	{
		impl->gamma = gamma;
	}

	void PNGOutputDescription::set_icc_profile(const std::string &profile_name, const DataBuffer &profile_data)
	{
		impl->icc_profile_data = profile_data;
		impl->icc_profile_name = profile_name;
	}

	void PNGOutputDescription::set_interlace_type(PNGInterlaceType type)
	{
		impl->interlace_type = type;
	}

	void PNGOutputDescription::set_modification_time(const DateTime &datetime)
	{
		impl->modification_time = datetime;
	}

	void PNGOutputDescription::set_offset_from_screen_topleft(const Vec2i &offset, PNGOffsetUnitType offset_unit_type)
	{
		impl->offset = offset;
		impl->offset_unit_type = offset_unit_type;
	}

	void PNGOutputDescription::set_physical_resolution(const Vec2i &resolution, PNGResolutionUnitType resolution_type)
	{
		impl->resolution = resolution;
		impl->resolution_unit_type = resolution_type;
	}

	void PNGOutputDescription::set_physical_scale(int scale_units, const Sized &pixel_size_in_scale_units)
	{
		impl->physical_scale_units = scale_units;
		impl->pixel_size_in_scale_units = pixel_size_in_scale_units;
	}

	void PNGOutputDescription::set_srgb_intent(PNGsRGBIntent intent)
	{
		impl->srgb_intent = intent;
	}

	void PNGOutputDescription::set_significant_bits(int num_bits)
	{
		impl->num_significant_bits = num_bits;
	}
}
