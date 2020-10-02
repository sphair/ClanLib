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
#include "jpeg_loader.h"
#include "jpeg_bit_reader.h"
#include "jpeg_huffman_decoder.h"
#include "jpeg_mcu_decoder.h"
#include "jpeg_rgb_decoder.h"

namespace clan
{
	PixelBuffer JPEGLoader::load(IODevice iodevice, bool srgb)
	{
		JPEGLoader loader(iodevice);
		JPEGMCUDecoder mcu_decoder(&loader);
		JPEGRGBDecoder rgb_decoder(&loader);

		int image_width = loader.start_of_frame.width;
		int image_height = loader.start_of_frame.height;
		PixelBuffer image(image_width, image_height, srgb ? TextureFormat::srgb8_alpha8 : TextureFormat::rgba8);
		unsigned int *image_pixels = reinterpret_cast<unsigned int *>(image.get_data());

		const unsigned int *block_pixels = rgb_decoder.get_pixels();
		int block_width = rgb_decoder.get_width();
		int block_height = rgb_decoder.get_height();

		for (int curMcuY = 0, y = 0; curMcuY < loader.mcu_height; curMcuY++, y += block_height)
		{
			for (int curMcuX = 0, x = 0; curMcuX < loader.mcu_width; curMcuX++, x += block_width)
			{
				mcu_decoder.decode(curMcuX + curMcuY * loader.mcu_width);
				rgb_decoder.decode(&mcu_decoder);

				int w = min(block_width, image_width - x);
				int h = min(block_height, image_height - y);
				for (int yy = 0; yy < h; yy++)
				{
					for (int xx = 0; xx < w; xx++)
					{
						unsigned int p = block_pixels[xx + yy*block_width];
						unsigned int red = (p >> 16) & 0xff;
						unsigned int green = (p >> 8) & 0xff;
						unsigned int blue = p & 0xff;
						unsigned int alpha = (p >> 24) & 0xff;
						image_pixels[x + xx + (y + yy)*image_width] = (alpha << 24) | (blue << 16) | (green << 8) | red;
					}
				}
			}
		}

		return image;
	}

	JPEGLoader::JPEGLoader(IODevice iodevice)
		: progressive(false), scan_count(0), mcu_x(0), mcu_y(0), mcu_width(0), mcu_height(0), restart_interval(0), eobrun(0), is_jfif_jpeg(false), is_adobe_jpeg(false), adobe_app14_transform(1)
	{
		JPEGFileReader reader(iodevice);

		JPEGMarker marker = reader.read_marker();
		if (marker != marker_soi)
			throw Exception("Not a JPEG file");

		marker = reader.read_marker();
		while (marker != marker_eoi)
		{
			if (marker == marker_app0)
			{
				process_app0(reader);
			}
			else if (marker == marker_app14)
			{
				process_app14(reader);
			}
			else if ((marker >= marker_sof0 && marker <= marker_sof3) || (marker >= marker_sof5 && marker <= marker_sof15))
			{
				process_sof(marker, reader);
			}
			else if (marker == marker_dht)
			{
				process_dht(reader);
			}
			else if (marker == marker_dqt)
			{
				process_dqt(reader);
			}
			else if (marker == marker_dri)
			{
				restart_interval = reader.read_dri();
			}
			else if (marker == marker_sos)
			{
				/*try
				{*/
				process_sos(reader);
				/*}
				catch (...)
				{
				char buffer[16*1024];
				while (reader.read_entropy_data(buffer, 16*1024) != 0);
				}*/
			}
			else if (marker == marker_dnl)
			{
				process_dnl(reader);
			}
			else
			{
				reader.skip_unknown();
			}

			marker = reader.read_marker();
		}

		if (scan_count == 0 || start_of_frame.height == 0)
			throw Exception("Invalid JPEG Image");
	}

	void JPEGLoader::process_app0(JPEGFileReader &reader)
	{
		if (reader.try_read_app0_jfif())
			is_jfif_jpeg = true;
	}

	void JPEGLoader::process_app14(JPEGFileReader &reader)
	{
		int transform = 0;
		if (reader.try_read_app14_adobe(transform))
		{
			is_adobe_jpeg = true;
			adobe_app14_transform = transform;
		}
	}

	JPEGLoader::ColorSpace JPEGLoader::get_colorspace() const
	{
		if (start_of_frame.components.size() == 1)
			return colorspace_grayscale;
		else if (is_jfif_jpeg)
			return colorspace_ycrcb;
		else if (is_adobe_jpeg && adobe_app14_transform == 1)
			return colorspace_ycrcb;
		else if (is_adobe_jpeg && adobe_app14_transform == 2)
			return colorspace_ycck;
		else if (is_adobe_jpeg && adobe_app14_transform == 0 && start_of_frame.components.size() == 3)
			return colorspace_rgb;
		else if (is_adobe_jpeg && adobe_app14_transform == 0 && start_of_frame.components.size() == 4)
			return colorspace_cmyk;
		else if (start_of_frame.components.size() == 3)
			return colorspace_ycrcb;
		else
			throw Exception("Unknown color space in JPEG file");
	}

	void JPEGLoader::process_sof(JPEGMarker marker, JPEGFileReader &reader)
	{
		if (marker == marker_sof0 || marker == marker_sof2)
		{
			progressive = (marker == marker_sof2);

			start_of_frame = reader.read_sof();
			component_dcts.resize(start_of_frame.components.size());

			for (auto & elem : start_of_frame.components)
			{
				mcu_x = max(mcu_x, elem.horz_sampling_factor);
				mcu_y = max(mcu_y, elem.vert_sampling_factor);
			}

			mcu_width = (start_of_frame.width + (mcu_x * 8 - 1)) / (mcu_x * 8);
			mcu_height = (start_of_frame.height + (mcu_y * 8 - 1)) / (mcu_y * 8);

			for (size_t c = 0; c < component_dcts.size(); c++)
				component_dcts[c].resize(mcu_width*mcu_height*start_of_frame.components[c].horz_sampling_factor*start_of_frame.components[c].vert_sampling_factor);

			last_dc_values.resize(start_of_frame.components.size());
		}
		else
		{
			throw Exception("JPEG compression method not supported");
		}
	}

	void JPEGLoader::process_dht(JPEGFileReader &reader)
	{
		JPEGDefineHuffmanTable tables = reader.read_dht();
		for (auto & table : tables)
		{
			if (table.table_class == JPEGHuffmanTable::ac_table)
				huffman_ac_tables[table.table_index] = table;
			else // JPEGHuffmanTable::dc_table
				huffman_dc_tables[table.table_index] = table;
		}
	}

	void JPEGLoader::process_dqt(JPEGFileReader &reader)
	{
		JPEGDefineQuantizationTable tables = reader.read_dqt();
		for (auto & table : tables)
		{
			for (size_t j = 0; j < 64; j++)
				quantization_tables[table.table_index].values[zigzag_map[j]] = table.values[j];
		}
	}

	void JPEGLoader::process_sos(JPEGFileReader &reader)
	{
		JPEGStartOfScan start_of_scan = reader.read_sos();

		std::vector<int > component_to_sof;
		for (auto & elem : start_of_scan.components)
		{
			bool found = false;
			for (size_t i = 0; i < start_of_frame.components.size(); i++)
			{
				if (elem.component_selector == start_of_frame.components[i].id)
				{
					component_to_sof.push_back(i);
					found = true;
					break;
				}
			}
			if (!found)
				throw Exception("Invalid JPEG file");
		}

		if (progressive)
			process_sos_progressive(start_of_scan, component_to_sof, reader);
		else
			process_sos_sequential(start_of_scan, component_to_sof, reader);

		scan_count++;
	}

	void JPEGLoader::verify_dc_table_selector(const JPEGStartOfScan &start_of_scan)
	{
		for (auto & elem : start_of_scan.components)
		{
			if (huffman_dc_tables[elem.dc_table_selector].tree.empty())
				throw Exception("Invalid JPEG file");
		}
	}

	void JPEGLoader::verify_ac_table_selector(const JPEGStartOfScan &start_of_scan)
	{
		for (auto & elem : start_of_scan.components)
		{
			if (huffman_ac_tables[elem.ac_table_selector].tree.empty())
				throw Exception("Invalid JPEG file");
		}
	}

	void JPEGLoader::process_dnl(JPEGFileReader &reader)
	{
		if (scan_count != 1)
			throw Exception("Invalid JPEG file");
		start_of_frame.height = reader.read_dnl();

		mcu_width = (start_of_frame.width + (mcu_x * 8 - 1)) / (mcu_x * 8);
		mcu_height = (start_of_frame.height + (mcu_y * 8 - 1)) / (mcu_y * 8);
		for (size_t c = 0; c < component_dcts.size(); c++)
			component_dcts[c].resize(mcu_width*mcu_height*start_of_frame.components[c].horz_sampling_factor*start_of_frame.components[c].vert_sampling_factor);
	}

	int JPEGLoader::zigzag_map[64] =
	{
		0, 1, 1 * 8, 2 * 8, 1 * 8 + 1,
		2, 3, 1 * 8 + 2, 2 * 8 + 1, 3 * 8, 4 * 8, 3 * 8 + 1, 2 * 8 + 2, 1 * 8 + 3,
		4, 5, 1 * 8 + 4, 2 * 8 + 3, 3 * 8 + 2, 4 * 8 + 1, 5 * 8, 6 * 8, 5 * 8 + 1, 4 * 8 + 2, 3 * 8 + 3, 2 * 8 + 4, 1 * 8 + 5,
		6, 7, 1 * 8 + 6, 2 * 8 + 5, 3 * 8 + 4, 4 * 8 + 3, 5 * 8 + 2, 6 * 8 + 1,

		7 * 8, 7 * 8 + 1, 6 * 8 + 2, 5 * 8 + 3, 4 * 8 + 4, 3 * 8 + 5, 2 * 8 + 6, 1 * 8 + 7, 2 * 8 + 7, 3 * 8 + 6, 4 * 8 + 5, 5 * 8 + 4, 6 * 8 + 3,
		7 * 8 + 2, 7 * 8 + 3, 6 * 8 + 4, 5 * 8 + 5, 4 * 8 + 6, 3 * 8 + 7, 4 * 8 + 7, 5 * 8 + 6, 6 * 8 + 5,
		7 * 8 + 4, 7 * 8 + 5, 6 * 8 + 6, 5 * 8 + 7, 6 * 8 + 7,
		7 * 8 + 6, 7 * 8 + 7
	};

	void JPEGLoader::process_sos_sequential(JPEGStartOfScan &start_of_scan, std::vector<int> component_to_sof, JPEGFileReader &reader)
	{
		verify_dc_table_selector(start_of_scan);
		verify_ac_table_selector(start_of_scan);

		JPEGBitReader bit_reader(&reader);
		int restart_counter = 0;
		for (int mcu_block = 0; mcu_block < mcu_width*mcu_height; mcu_block++)
		{
			if (restart_interval != 0 && restart_counter == restart_interval)
			{
				JPEGMarker marker = reader.read_marker();
				if (marker < marker_rst0 || marker > marker_rst7)
				{
					throw Exception("Restart marker missing between JPEG entropy data");
				}
				restart_counter = 0;
				for (auto & elem : last_dc_values)
					elem = 0;
				bit_reader.reset();
				eobrun = 0;
			}
			restart_counter++;

			for (size_t c = 0; c < start_of_scan.components.size(); c++)
			{
				int c_sof = component_to_sof[c];
				const JPEGHuffmanTable &dc_table = huffman_dc_tables[start_of_scan.components[c].dc_table_selector];
				const JPEGHuffmanTable &ac_table = huffman_ac_tables[start_of_scan.components[c].ac_table_selector];
				int scale_x = start_of_frame.components[c_sof].horz_sampling_factor;
				int scale_y = start_of_frame.components[c_sof].vert_sampling_factor;
				for (int i = 0; i < scale_x * scale_y; i++)
				{
					short *dct = component_dcts[c_sof].get(mcu_block*scale_x*scale_y + i);
					for (int j = start_of_scan.start_dct_coefficient; j <= start_of_scan.end_dct_coefficient; j++)
					{
						if (j == 0) // DCT DC coefficient
						{
							unsigned int code = JPEGHuffmanDecoder::decode(bit_reader, dc_table);
							if (code != huffman_eob)
								dct[0] = JPEGHuffmanDecoder::decode_number(bit_reader, code);
							dct[0] <<= start_of_scan.point_transform;

							dct[0] += last_dc_values[c_sof];
							last_dc_values[c_sof] = dct[0];
						}
						else // DCT AC coefficient
						{
							unsigned int code = JPEGHuffmanDecoder::decode(bit_reader, ac_table);
							if (code != huffman_eob)
							{
								unsigned int zeros = (code >> 4);
								j += zeros;
								if (j <= start_of_scan.end_dct_coefficient)
								{
									dct[zigzag_map[j]] = JPEGHuffmanDecoder::decode_number(bit_reader, code & 0x0f);
									dct[zigzag_map[j]] <<= start_of_scan.point_transform;
								}
							}
							else
							{
								break;
							}
						}
					}
				}
			}
		}
	}

	void JPEGLoader::process_sos_progressive(JPEGStartOfScan &start_of_scan, std::vector<int> component_to_sof, JPEGFileReader &reader)
	{
		JPEGBitReader bit_reader(&reader);
		int restart_counter = 0;
		if (start_of_scan.start_dct_coefficient == 0 && start_of_scan.end_dct_coefficient == 0)
		{
			verify_dc_table_selector(start_of_scan);

			for (int mcu_block = 0; mcu_block < mcu_width*mcu_height; mcu_block++)
			{
				if (restart_interval != 0 && restart_counter == restart_interval)
				{
					JPEGMarker marker = reader.read_marker();
					if (marker < marker_rst0 || marker > marker_rst7)
					{
						throw Exception("Restart marker missing between JPEG entropy data");
					}
					restart_counter = 0;
					for (auto & elem : last_dc_values)
						elem = 0;
					bit_reader.reset();
					eobrun = 0;
				}
				restart_counter++;

				for (size_t c = 0; c < start_of_scan.components.size(); c++)
				{
					int c_sof = component_to_sof[c];
					const JPEGHuffmanTable &dc_table = huffman_dc_tables[start_of_scan.components[c].dc_table_selector];
					int scale_x = start_of_frame.components[c_sof].horz_sampling_factor;
					int scale_y = start_of_frame.components[c_sof].vert_sampling_factor;

					for (int i = 0; i < scale_x * scale_y; i++)
					{
						short *dct = component_dcts[c_sof].get(mcu_block*scale_x*scale_y + i);

						if (start_of_scan.preceding_point_transform == 0)
						{
							short s = JPEGHuffmanDecoder::decode(bit_reader, dc_table);
							if (s != huffman_eob)
								s = JPEGHuffmanDecoder::decode_number(bit_reader, s);
							s += last_dc_values[c_sof];
							last_dc_values[c_sof] = s;
							dct[0] = (s << start_of_scan.point_transform);
						}
						else
						{
							dct[0] |= (bit_reader.get_bit() << start_of_scan.point_transform);
						}
					}
				}
			}
		}
		else
		{
			/*if (start_of_scan.components[0].component_selector != 1)
			{
			char buffer[16*1024];
			while (reader.read_entropy_data(buffer, 16*1024) > 0);
			return;
			}*/
			verify_ac_table_selector(start_of_scan);

			int c_sof = component_to_sof[0];
			const JPEGHuffmanTable &ac_table = huffman_ac_tables[start_of_scan.components[0].ac_table_selector];
			int scale_x = start_of_frame.components[c_sof].horz_sampling_factor;
			int scale_y = start_of_frame.components[c_sof].vert_sampling_factor;

			int width_in_blocks = (start_of_frame.width * scale_x + (mcu_x * 8 - 1)) / (mcu_x * 8);
			int height_in_blocks = (start_of_frame.height * scale_y + (mcu_y * 8 - 1)) / (mcu_y * 8);

			for (int i = 0; i < width_in_blocks * height_in_blocks; i++)
			{
				if (restart_interval != 0 && restart_counter == restart_interval)
				{
					JPEGMarker marker = reader.read_marker();
					if (marker < marker_rst0 || marker > marker_rst7)
					{
						throw Exception("Restart marker missing between JPEG entropy data");
					}
					restart_counter = 0;
					for (auto & elem : last_dc_values)
						elem = 0;
					bit_reader.reset();
					eobrun = 0;
				}
				restart_counter++;

				int x = (i%width_in_blocks);
				int y = (i / width_in_blocks);
				int mcu_block_x = x / scale_x;
				int mcu_block_y = y / scale_y;
				int mcu_block = mcu_block_x + mcu_block_y * mcu_width;
				int index = mcu_block * scale_x * scale_y + x%scale_x + (y%scale_y) * scale_x;
				short *dct = component_dcts[c_sof].get(index);
				if (start_of_scan.preceding_point_transform == 0)
				{
					if (eobrun > 0)
					{
						eobrun--;
					}
					else
					{
						for (int j = start_of_scan.start_dct_coefficient; j <= start_of_scan.end_dct_coefficient; j++)
						{
							unsigned int code = JPEGHuffmanDecoder::decode(bit_reader, ac_table);
							unsigned int r = (code >> 4);
							unsigned int s = code & 0x0f;
							if (s)
							{
								j += r;
								short v = JPEGHuffmanDecoder::decode_number(bit_reader, s) << start_of_scan.point_transform;
								if (j < 64)
									dct[zigzag_map[j]] = v;
							}
							else if (r >= huffman_eob0 && r <= huffman_eob14)
							{
								eobrun = (1 << r);
								if (r > 0)
									eobrun += bit_reader.get_bits(r);
								eobrun--;
								break;
							}
							else if (r == huffman_zrl)
							{
								j += 15;
							}
						}
					}
				}
				else
				{
					short p1 = (((short)1) << start_of_scan.point_transform);
					short m1 = (((short)-1) << start_of_scan.point_transform);

					int j = start_of_scan.start_dct_coefficient;
					if (eobrun == 0)
					{
						for (; j <= start_of_scan.end_dct_coefficient; j++)
						{
							unsigned int code = JPEGHuffmanDecoder::decode(bit_reader, ac_table);
							unsigned int r = (code >> 4);
							int s = code & 0x0f;

							if (s)
							{
								s = bit_reader.get_bit() ? p1 : m1;
							}
							else if (r != huffman_zrl)
							{
								eobrun = (1 << r);
								if (r > 0)
									eobrun += bit_reader.get_bits(r);
								break;
							}

							do
							{
								if (dct[zigzag_map[j]] == 0)
								{
									if (r == 0)
										break;
									r--;
								}
								else if (bit_reader.get_bit())
								{
									if ((dct[zigzag_map[j]] & p1) == 0)
									{
										if (dct[zigzag_map[j]] >= 0)
											dct[zigzag_map[j]] += p1;
										else
											dct[zigzag_map[j]] += m1;
									}
								}
								j++;
							} while (j <= start_of_scan.end_dct_coefficient);

							if (s && j < 64)
							{
								dct[zigzag_map[j]] = s;
							}
						}
					}

					if (eobrun > 0)
					{
						for (; j <= start_of_scan.end_dct_coefficient; j++)
						{
							if (dct[zigzag_map[j]] != 0)
							{
								if (bit_reader.get_bit())
								{
									if ((dct[zigzag_map[j]] & p1) == 0)
									{
										if (dct[zigzag_map[j]] >= 0)
											dct[zigzag_map[j]] += p1;
										else
											dct[zigzag_map[j]] += m1;
									}
								}
							}
						}
						eobrun--;
					}
				}
			}
		}
	}
}
