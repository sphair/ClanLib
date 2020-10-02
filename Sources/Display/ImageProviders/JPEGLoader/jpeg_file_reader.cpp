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
*/

#include "Display/precomp.h"
#include "jpeg_file_reader.h"
#include "jpeg_define_huffman_table.h"
#include "jpeg_define_quantization_table.h"
#include "jpeg_start_of_frame.h"
#include "jpeg_start_of_scan.h"
#include "jpeg_markers.h"

namespace clan
{
	JPEGFileReader::JPEGFileReader(IODevice iodevice)
		: iodevice(iodevice)
	{
		iodevice.set_big_endian_mode();
	}

	JPEGMarker JPEGFileReader::read_marker()
	{
		uint8_t b1 = iodevice.read_uint8();
		if (b1 != 0xff)
			throw Exception("Invalid JPEG file");
		return (JPEGMarker)iodevice.read_uint8();
	}

	void JPEGFileReader::skip_unknown()
	{
		uint16_t size = iodevice.read_uint16();
		if (!iodevice.seek(size - 2, IODevice::SeekMode::cur))
			throw Exception("Invalid JPEG file");
	}

	bool JPEGFileReader::try_read_app0_jfif()
	{
		uint16_t size = iodevice.read_uint16();
		if (size < 7)
		{
			if (!iodevice.seek(size - 2, IODevice::SeekMode::cur))
				throw Exception("Invalid JPEG file");
			return false;
		}

		char magic[5];
		iodevice.read(magic, 5);
		bool is_jfif = (memcmp(magic, "JFIF", 5) == 0);

		if (!iodevice.seek(size - 7, IODevice::SeekMode::cur))
			throw Exception("Invalid JPEG file");

		return is_jfif;
	}

	bool JPEGFileReader::try_read_app14_adobe(int &out_transform)
	{
		/*
			Adobe TN 5116:

			Length of APP14 block   2 bytes
			Block ID                5 bytes - ASCII "Adobe"
			Version Number          2 bytes - currently 100
			Flags0                  2 bytes - currently 0
			Flags1                  2 bytes - currently 0
			Color transform         1 byte
			*/

		out_transform = 1;
		uint16_t size = iodevice.read_uint16();
		if (size != 2 + 5 + 2 + 2 + 2 + 1)
		{
			if (!iodevice.seek(size - 2, IODevice::SeekMode::cur))
				throw Exception("Invalid JPEG file");
			return false;
		}

		char magic[5];
		iodevice.read(magic, 5);
		bool is_app14_adobe = (memcmp(magic, "Adobe", 5) == 0);
		if (!is_app14_adobe)
		{
			if (!iodevice.seek(size - 7, IODevice::SeekMode::cur))
				throw Exception("Invalid JPEG file");
			return false;
		}

		iodevice.read_uint16();
		iodevice.read_uint16();
		iodevice.read_uint16();
		out_transform = iodevice.read_uint8();
		return true;
	}

	JPEGStartOfFrame JPEGFileReader::read_sof()
	{
		JPEGStartOfFrame header;

		uint16_t size = iodevice.read_uint16();
		if (size < 8)
			throw Exception("Invalid JPEG file");

		header.sample_precision = iodevice.read_uint8();
		if (header.sample_precision != 8 && header.sample_precision != 16)
			throw Exception("Invalid JPEG file");

		header.height = iodevice.read_uint16();
		header.width = iodevice.read_uint16();
		uint8_t num_components = iodevice.read_uint8();
		if (num_components == 0 || num_components > 4) // To do: Only do this check for progressive; sequential allows up to 255 components
			throw Exception("Invalid JPEG file");

		if (size != 8 + 3 * num_components)
			throw Exception("Invalid JPEG file");

		for (int i = 0; i < num_components; i++)
		{
			JPEGStartOfFrameComponent c;
			c.id = iodevice.read_uint8();
			uint8_t sampling_factor = iodevice.read_uint8();
			c.horz_sampling_factor = (sampling_factor & 0xf0) >> 4;
			c.vert_sampling_factor = (sampling_factor & 0x0f);
			c.quantization_table_selector = iodevice.read_uint8();

			if (c.horz_sampling_factor == 0 || c.horz_sampling_factor > 4 ||
				c.vert_sampling_factor == 0 || c.horz_sampling_factor == 0 ||
				c.quantization_table_selector > 3)
			{
				throw Exception("Invalid JPEG file");
			}

			header.components.push_back(c);
		}

		return header;
	}

	JPEGDefineQuantizationTable JPEGFileReader::read_dqt()
	{
		JPEGDefineQuantizationTable tables;

		uint16_t size = iodevice.read_uint16();
		if (size < 3)
			throw Exception("Invalid JPEG file");

		int p = 2;
		while (p < size)
		{
			JPEGQuantizationTable table;
			uint8_t v = iodevice.read_uint8();
			uint8_t precision = (v & 0xf0) >> 4;
			table.table_index = (v & 0x0f);

			if (precision > 1 || table.table_index > 3)
				throw Exception("Invalid JPEG file");

			if (precision == 0) // 8 bit
			{
				if (size < p + 1 + 64)
					throw Exception("Invalid JPEG file");

				uint8_t values[64];
				iodevice.read(values, 64);
				for (int i = 0; i < 64; i++)
					table.values[i] = values[i];

				p += 1 + 64;
			}
			else // 16 bit
			{
				if (size < p + 1 + 64 * 2)
					throw Exception("Invalid JPEG file");
				for (int i = 0; i < 64; i++)
					table.values[i] = iodevice.read_uint16();

				p += 1 + 64 * 2;
			}

			tables.push_back(table);
		}

		return tables;
	}

	JPEGDefineHuffmanTable JPEGFileReader::read_dht()
	{
		JPEGDefineHuffmanTable tables;

		uint16_t size = iodevice.read_uint16();
		if (size < 2 + 17)
			throw Exception("Invalid JPEG file");

		int p = 2;
		while (p < size)
		{
			JPEGHuffmanTable table;

			if (size < p + 17)
				throw Exception("Invalid JPEG file");

			uint8_t v = iodevice.read_uint8();
			uint8_t table_class = (v & 0xf0) >> 4;
			if (table_class > 1)
				throw Exception("Invalid JPEG file");
			table.table_class = (table_class == 0) ? JPEGHuffmanTable::dc_table : JPEGHuffmanTable::ac_table;
			table.table_index = (v & 0x0f);
			if (table.table_index > 3)
				throw Exception("Invalid JPEG file");

			iodevice.read(table.bits, 16);
			int bindings = 0;
			for (int i = 0; i < 16; i++)
				bindings += table.bits[i];
			table.values.resize(bindings);
			if (bindings > 0)
				iodevice.read(&table.values[0], bindings);

			if (size < p + 17 + bindings)
				throw Exception("Invalid JPEG file");

			p += 17 + bindings;

			table.build_tree();
			tables.push_back(table);
		}

		return tables;
	}

	JPEGDefineRestartInterval JPEGFileReader::read_dri()
	{
		JPEGDefineRestartInterval interval = 0;
		uint16_t size = iodevice.read_uint16();
		if (size != 4)
			throw Exception("Invalid JPEG file");
		interval = iodevice.read_uint16();
		return interval;
	}

	JPEGStartOfScan JPEGFileReader::read_sos()
	{
		JPEGStartOfScan header;

		uint16_t size = iodevice.read_uint16();
		if (size < 6)
			throw Exception("Invalid JPEG file");

		uint8_t num_components = iodevice.read_uint8();
		if (num_components == 0 || num_components > 4)
			throw Exception("Invalid JPEG file");

		if (size != 6 + 2 * num_components)
			throw Exception("Invalid JPEG file");

		for (int i = 0; i < num_components; i++)
		{
			JPEGStartOfScanComponent c;
			c.component_selector = iodevice.read_uint8();
			uint8_t table_selector = iodevice.read_uint8();
			c.dc_table_selector = (table_selector & 0xf0) >> 4;
			c.ac_table_selector = (table_selector & 0x0f);

			if (c.ac_table_selector > 3 || c.dc_table_selector > 3)
				throw Exception("Invalid JPEG file");

			header.components.push_back(c);
		}

		header.start_dct_coefficient = iodevice.read_uint8();
		header.end_dct_coefficient = iodevice.read_uint8();

		uint8_t ahal = iodevice.read_uint8();
		header.preceding_point_transform = (ahal & 0xf0) >> 4;
		header.point_transform = (ahal & 0x0f);

		if (//(header.start_dct_coefficient == 0 && header.end_dct_coefficient != 0) ||
			header.start_dct_coefficient > 63 || header.end_dct_coefficient > 63 ||
			header.start_dct_coefficient > header.end_dct_coefficient)
			throw Exception("Invalid JPEG file");

		return header;
	}

	JPEGDefineNumberOfLines JPEGFileReader::read_dnl()
	{
		JPEGDefineNumberOfLines lines = 0;
		uint16_t size = iodevice.read_uint16();
		if (size != 4)
			throw Exception("Invalid JPEG file");
		lines = iodevice.read_uint16();
		return lines;
	}

	std::string JPEGFileReader::read_comment()
	{
		uint16_t size = iodevice.read_uint16();
		if (size < 2)
			throw Exception("Invalid JPEG file");

		size -= 2;

		std::vector<char> text;
		text.resize(size + 1, ' ');
		text[size] = 0;

		iodevice.read(text.data(), size);
		return &text[0];
	}

	int JPEGFileReader::read_entropy_data(void *d, int size)
	{
		// FF is always followed by 00 in entropy data. This is done to allow an
		// application to scan for JPEG markers without matching entropy data by
		// accident.

		uint8_t *data = reinterpret_cast<uint8_t*>(d);

		int start = iodevice.get_position();
		int len = iodevice.read(data, size, false);
		if (len == 0)
			return 0;

		int j = 0;
		for (int i = 0; i < len; i++)
		{
			if (data[i] == 0xff && i + 1 < len && data[i + 1] == 0x00)
			{
				data[j] = 0xff;
				j++;
				i++;
			}
			else if (data[i] == 0xff)
			{
				iodevice.seek(start + i);
				break;
			}
			else
			{
				data[j] = data[i];
				j++;
			}
		}
		return j;
	}
}
