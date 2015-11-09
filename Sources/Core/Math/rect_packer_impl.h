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
**    Kenneth Gangstoe
*/

#pragma once

#include "API/Core/Math/rect_packer.h"

namespace clan
{
	class RectPacker_Impl
	{
	public:
		class Node
		{
		public:
			Node();
			Node(const Rect &rect);
			~Node();

			int get_rect_count() const;

			Node *insert(const Size &rect_size, int rect_id);

			void clear();

			Node *child[2];
			Rect node_rect;

			int id;
		};

		struct RootNode
		{
		public:
			int group_id;
			Node node;
		};

	public:
		RectPacker_Impl(const Size &max_group_size);
		~RectPacker_Impl();

		int get_total_rect_count() const;
		int get_rect_count(unsigned int group_index) const;

		RectPacker::AllocatedRect add_new_node(const Size &rect_size);
		RootNode *add_new_root();

		std::vector<RootNode *> root_nodes;
		RootNode *active_root_node;

		int next_node_id;

		RectPacker::AllocationPolicy allocation_policy;

		Size max_group_size;
	};
}
