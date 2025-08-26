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

namespace clan
{
	struct JPEGHuffmanNode
	{
		JPEGHuffmanNode() : leaf(false), value(0) { children[0] = 0; children[1] = 0; }

		bool leaf;
		int children[2];
		unsigned int value;
	};

	class JPEGHuffmanTable
	{
	public:
		JPEGHuffmanTable() : table_class(dc_table), table_index(0) { for (auto & elem : bits) elem = 0; }
		void build_tree();

		enum TableClass
		{
			dc_table,
			ac_table
		};
		TableClass table_class;

		// Specifies one of four possible destinations at the decoder into which the Huffman table shall be installed.
		uint8_t table_index;

		uint8_t bits[16];
		std::vector<uint8_t> values;

		std::vector<JPEGHuffmanNode> tree;
	};

	typedef std::vector<JPEGHuffmanTable> JPEGDefineHuffmanTable;

	inline void JPEGHuffmanTable::build_tree()
	{
		int num_levels;
		for (num_levels = 16; num_levels > 0; num_levels--)
		{
			if (bits[num_levels - 1] != 0)
				break;
		}

		tree.clear();
		tree.push_back(JPEGHuffmanNode());
		size_t nodes = 1;
		size_t values_index = 0;
		for (int level = 0; level < num_levels; level++)
		{
			size_t parents = tree.size() - nodes;
			size_t child_nodes = nodes * 2;
			for (size_t c = 0; c < child_nodes; c++)
			{
				JPEGHuffmanNode child;
				if (c < bits[level])
				{
					if (values_index >= values.size())
						throw Exception("Invalid JPEG File");
					child.leaf = true;
					child.value = values[values_index];
					values_index++;
				}

				tree[parents + c / 2].children[c % 2] = tree.size();
				tree.push_back(child);
			}
			nodes = child_nodes - bits[level];
		}
	}
}
