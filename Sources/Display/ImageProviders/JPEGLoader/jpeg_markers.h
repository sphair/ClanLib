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

#pragma once

namespace clan
{
	enum JPEGMarker
	{
		// Start Of Frame markers, non-differential, Huffman coding

		marker_sof0 = 0xc0,   // Baseline DCT
		marker_sof1 = 0xc1,   // Extended sequential DCT
		marker_sof2 = 0xc2,   // Progressive DCT
		marker_sof3 = 0xc3,   // Lossless (sequential)

		// Start Of Frame markers, differential, Huffman coding

		marker_sof5 = 0xc5,   // Differential sequential DCT
		marker_sof6 = 0xc6,   // Differential progressive DCT
		marker_sof7 = 0xc7,   // Differential lossless (sequential)

		// Start Of Frame markers, non-differential, arithmetic coding

		marker_jpg = 0xc8,   // Reserved for JPEG extensions
		marker_sof9 = 0xc9,   // Extended sequential DCT
		marker_sof10 = 0xca,  // Progressive DCT
		marker_sof11 = 0xcb,  // Lossless (sequential)

		// Start Of Frame markers, differential, arithmetic coding

		marker_sof13 = 0xcd,  // Differential sequential DCT
		marker_sof14 = 0xce,  // Differential progressive DCT
		marker_sof15 = 0xcf,  // Differential lossless (sequential)

		// Huffman table specification

		marker_dht = 0xc4,    // Define Huffman table(s)

		// Arithmetic coding conditioning specification

		marker_dac = 0xcc,    // Define arithmetic coding conditioning(s)

		// Restart interval termination

		marker_rst0 = 0xd0,   // Restart with modulo 8 count "n" (RSTn)
		marker_rst1 = 0xd1,
		marker_rst2 = 0xd2,
		marker_rst3 = 0xd3,
		marker_rst4 = 0xd4,
		marker_rst5 = 0xd5,
		marker_rst6 = 0xd6,
		marker_rst7 = 0xd7,

		// Other markers

		marker_soi = 0xd8,    // Start of image
		marker_eoi = 0xd9,    // End of image
		marker_sos = 0xda,    // Start of scan
		marker_dqt = 0xdb,    // Define quantization table(s)
		marker_dnl = 0xdc,    // Define number of lines
		marker_dri = 0xdd,    // Define restart interval
		marker_dhp = 0xde,    // Define hierarchical progression
		marker_exp = 0xdf,    // Expand reference component(s)
		marker_app0 = 0xe0,   // Reserved for application segments
		marker_app1 = 0xe1,
		marker_app2 = 0xe2,
		marker_app3 = 0xe3,
		marker_app4 = 0xe4,
		marker_app5 = 0xe5,
		marker_app6 = 0xe6,
		marker_app7 = 0xe7,
		marker_app8 = 0xe8,
		marker_app9 = 0xe9,
		marker_app10 = 0xea,
		marker_app11 = 0xeb,
		marker_app12 = 0xec,
		marker_app13 = 0xed,
		marker_app14 = 0xee,
		marker_app15 = 0xef,
		marker_jpg0 = 0xf0,   // Reserved for JPEG extensions
		marker_jpg1 = 0xf1,
		marker_jpg2 = 0xf3,
		marker_jpg3 = 0xf4,
		marker_jpg4 = 0xf5,
		marker_jpg5 = 0xf6,
		marker_jpg6 = 0xf7,
		marker_jpg7 = 0xf8,
		marker_jpg8 = 0xf9,
		marker_jpg9 = 0xfa,
		marker_jpg10 = 0xfb,
		marker_jpg11 = 0xfc,
		marker_jpg12 = 0xfd,
		marker_com = 0xfe,    // Comment

		// Reserved markers

		marker_tem = 0x01     // For temporary private use in arithmetic coding
	};
}
