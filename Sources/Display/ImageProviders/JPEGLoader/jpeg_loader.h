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

#include "API/Core/IOData/iodevice.h"
#include "API/Display/Image/pixel_buffer.h"
#include "jpeg_file_reader.h"
#include "jpeg_start_of_frame.h"
#include "jpeg_start_of_scan.h"
#include "jpeg_define_huffman_table.h"
#include "jpeg_define_quantization_table.h"
#include "jpeg_component_dcts.h"
#include "jpeg_markers.h"

namespace clan
{
	class JPEGBitReader;

	class JPEGLoader
	{
	public:
		static PixelBuffer load(IODevice iodevice, bool srgb);

	private:
		enum ColorSpace
		{
			colorspace_ycrcb,
			colorspace_rgb,
			colorspace_ycck,
			colorspace_cmyk,
			colorspace_grayscale
		};

		JPEGLoader(IODevice iodevice);

		void process_app0(JPEGFileReader &reader);
		void process_app14(JPEGFileReader &reader);
		void process_dnl(JPEGFileReader &reader);
		void process_sos(JPEGFileReader &reader);
		void process_sos_sequential(JPEGStartOfScan &start_of_scan, std::vector<int> component_to_sof, JPEGFileReader &reader);
		void process_sos_progressive(JPEGStartOfScan &start_of_scan, std::vector<int> component_to_sof, JPEGFileReader &reader);
		void process_dqt(JPEGFileReader &reader);
		void process_dht(JPEGFileReader &reader);
		void process_sof(JPEGMarker marker, JPEGFileReader &reader);
		void verify_dc_table_selector(const JPEGStartOfScan &start_of_scan);
		void verify_ac_table_selector(const JPEGStartOfScan &start_of_scan);
		ColorSpace get_colorspace() const;

		JPEGStartOfFrame start_of_frame;
		JPEGHuffmanTable huffman_dc_tables[4];
		JPEGHuffmanTable huffman_ac_tables[4];
		JPEGQuantizationTable quantization_tables[4];
		std::vector<JPEGComponentDCTs> component_dcts;
		bool progressive;
		int scan_count;
		int mcu_x;
		int mcu_y;
		int mcu_width;
		int mcu_height;
		int restart_interval;
		int eobrun;
		std::vector<short> last_dc_values;

		bool is_jfif_jpeg;
		bool is_adobe_jpeg;
		int adobe_app14_transform;

		static int zigzag_map[64];

		friend class JPEGMCUDecoder;
		friend class JPEGRGBDecoder;
	};
}
