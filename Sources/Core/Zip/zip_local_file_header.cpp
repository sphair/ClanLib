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
*/

#include "Core/precomp.h"
#include "zip_local_file_header.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_help.h"
#include "zip_flags.h"

namespace clan
{
	ZipLocalFileHeader::ZipLocalFileHeader()
	{
		signature = 0x04034b50;
	}

	ZipLocalFileHeader::~ZipLocalFileHeader()
	{
	}

	void ZipLocalFileHeader::load(IODevice &input)
	{
		signature = input.read_int32();
		if (signature != 0x04034b50)
		{
			throw Exception("Incorrect Local File Header signature");
		}
		version_needed_to_extract = input.read_int16();
		general_purpose_bit_flag = input.read_int16();
		compression_method = input.read_int16();
		last_mod_file_time = input.read_int16();
		last_mod_file_date = input.read_int16();
		crc32 = input.read_uint32();
		compressed_size = input.read_int32();
		uncompressed_size = input.read_int32();
		file_name_length = input.read_int16();
		extra_field_length = input.read_int16();
		auto str1 = new char[file_name_length];
		auto str2 = new char[extra_field_length];
		try
		{
			input.read(str1, file_name_length);
			input.read(str2, extra_field_length);
			if (general_purpose_bit_flag & ZIP_USE_UTF8)
				filename = StringHelp::utf8_to_text(std::string(str1, file_name_length));
			else
				filename = StringHelp::cp437_to_text(std::string(str1, file_name_length));

			extra_field = DataBuffer(str2, extra_field_length);

			delete[] str1;
			delete[] str2;
		}
		catch (...)
		{
			delete[] str1;
			delete[] str2;
			throw;
		}
	}

	void ZipLocalFileHeader::save(IODevice &output)
	{
		std::string str_filename;
		if (general_purpose_bit_flag & ZIP_USE_UTF8)
			str_filename = filename;
		else
			str_filename = StringHelp::text_to_cp437(filename);

		file_name_length = str_filename.length();

		output.write_int32(signature); // 0x04034b50
		output.write_int16(version_needed_to_extract);
		output.write_int16(general_purpose_bit_flag);
		output.write_int16(compression_method);
		output.write_int16(last_mod_file_time);
		output.write_int16(last_mod_file_date);
		output.write_uint32(crc32);
		output.write_int32(compressed_size);
		output.write_int32(uncompressed_size);
		output.write_int16(file_name_length);
		output.write_int16(extra_field_length);

		if (file_name_length > 0)
			output.write(str_filename.data(), file_name_length);

		if (extra_field_length > 0)
			output.write(extra_field.get_data(), extra_field_length);
	}
}
