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
#include "jpeg_huffman_decoder.h"
#include "jpeg_define_huffman_table.h"
#include "jpeg_bit_reader.h"

namespace clan
{
	unsigned int JPEGHuffmanDecoder::decode(JPEGBitReader &reader, const JPEGHuffmanTable &table)
	{
		int node = 0;
		while (true)
		{
			unsigned int bit = reader.get_bit();
			node = table.tree[node].children[bit];
			if (node == 0 || node == table.tree.size())
				break;
			else if (table.tree[node].leaf)
				return table.tree[node].value;
		}
		throw Exception("Invalid JPEG Huffman encoding");
	}

	short JPEGHuffmanDecoder::decode_number(JPEGBitReader &reader, int length)
	{
		if (length == 0)
			return 0;
		int v = reader.get_bits(length);
		if ((v) < (1 << ((length)-1)))
			return (v)+(((-1) << (length)) + 1);
		else
			return v;
	}
}
