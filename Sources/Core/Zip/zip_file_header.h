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
*/

#pragma once

#include "API/Core/System/cl_platform.h"
#include "API/Core/System/databuffer.h"

namespace clan
{
	class IODevice;

	class ZipFileHeader
	{
	public:
		ZipFileHeader();
		~ZipFileHeader();

		int32_t signature; // 0x02014b50
		int16_t version_made_by;
		int16_t version_needed_to_extract;
		int16_t general_purpose_bit_flag;
		int16_t compression_method;
		int16_t last_mod_file_time;
		int16_t last_mod_file_date;
		uint32_t crc32;
		int32_t compressed_size;
		int32_t uncompressed_size;
		int16_t file_name_length;
		int16_t extra_field_length;
		int16_t file_comment_length;
		int16_t disk_number_start;
		int16_t internal_file_attributes;
		int32_t external_file_attributes;
		int32_t relative_offset_of_local_header;
		std::string filename;
		DataBuffer extra_field;
		std::string file_comment;

		void load(IODevice &input);
		void save(IODevice &output);
	};
}
