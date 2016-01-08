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
	class JPEGBitReader;
	class JPEGHuffmanTable;

	class JPEGHuffmanDecoder
	{
	public:
		static unsigned int decode(JPEGBitReader &reader, const JPEGHuffmanTable &table);
		static short decode_number(JPEGBitReader &reader, int length);
	};

	enum JPEGHuffmanCodes
	{
		// Sequential Huffman
		huffman_eob = 0x00,

		// Progressive Huffman
		huffman_eob0 = 0x00,
		huffman_eob1 = 0x01,
		huffman_eob2 = 0x02,
		huffman_eob3 = 0x03,
		huffman_eob4 = 0x04,
		huffman_eob5 = 0x05,
		huffman_eob6 = 0x06,
		huffman_eob7 = 0x07,
		huffman_eob8 = 0x08,
		huffman_eob9 = 0x09,
		huffman_eob10 = 0x0a,
		huffman_eob11 = 0x0b,
		huffman_eob12 = 0x0c,
		huffman_eob13 = 0x0d,
		huffman_eob14 = 0x0e,
		huffman_zrl = 0x0f
	};
}
