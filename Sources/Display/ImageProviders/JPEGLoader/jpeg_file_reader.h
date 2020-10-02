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

#pragma once

#include "API/Core/IOData/iodevice.h"
#include "jpeg_define_huffman_table.h"
#include "jpeg_define_quantization_table.h"
#include "jpeg_markers.h"

namespace clan
{
	class JPEGStartOfFrame;
	class JPEGStartOfScan;

	typedef uint16_t JPEGDefineRestartInterval;
	typedef uint16_t JPEGDefineNumberOfLines;

	class JPEGFileReader
	{
	public:
		JPEGFileReader(IODevice iodevice);

		JPEGMarker read_marker();
		void skip_unknown();
		bool try_read_app0_jfif();
		bool try_read_app14_adobe(int &out_transform);
		JPEGStartOfFrame read_sof();
		JPEGDefineQuantizationTable read_dqt();
		JPEGDefineHuffmanTable read_dht();
		JPEGDefineRestartInterval read_dri();
		JPEGStartOfScan read_sos();
		JPEGDefineNumberOfLines read_dnl();
		std::string read_comment();
		int read_entropy_data(void *d, int size);

	private:
		IODevice iodevice;
	};
}
