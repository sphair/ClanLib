/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "Core/precomp.h"
#include "API/Core/Math/rect.h"
#include "rect_packer_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_RectPacker_Impl construction:

CL_RectPacker_Impl::CL_RectPacker_Impl(const CL_Size &max_group_size)
: active_root_node(0), next_node_id(0), max_group_size(max_group_size)
{
}

CL_RectPacker_Impl::~CL_RectPacker_Impl()
{
	std::vector<RootNode *>::size_type index, size;
	size = root_nodes.size();
	for(index = 0; index < size; ++index)
	{
		root_nodes[index]->node.clear();
		delete root_nodes[index];
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_RectPacker_Impl attributes:

int CL_RectPacker_Impl::get_total_rect_count() const
{
	int count = 0;

	std::vector<RootNode *>::size_type index, size;
	size = root_nodes.size();
	for(index = 0; index < size; ++index)
		count += root_nodes[index]->node.get_rect_count();

	return count;
}

int CL_RectPacker_Impl::get_rect_count(unsigned int group_index) const
{
	int count = 0;

	if(group_index >= 0 && group_index < root_nodes.size())
		count = root_nodes[group_index]->node.get_rect_count();

	return count;
}

/////////////////////////////////////////////////////////////////////////////
// CL_RectPacker_Impl operations:

CL_RectPacker::AllocatedRect CL_RectPacker_Impl::add_new_node(const CL_Size &rect_size)
{
	// Try inserting in current active group
	Node *node;
	if (!active_root_node)
	{
		// Create an initial root, if it does not exist
		node = 0;
		next_node_id = 1;
	}
	else
	{
		node = active_root_node->node.insert(rect_size, next_node_id);
	}

	if(node == 0) // Couldn't find a fit in current active group
	{
		if(allocation_policy == CL_RectPacker::fail_if_full && root_nodes.size() > 0)
		{
			throw CL_Exception("Unable to pack rect into group: full");
		}

		if(allocation_policy == CL_RectPacker::search_previous_groups)
		{
			std::vector<RootNode *>::size_type index, size;
			size = root_nodes.size();
			for(index = 0; index < size; ++index)
			{
				node = root_nodes[index]->node.insert(rect_size, next_node_id);
				if(node)	// We found space in a previous group
					break;
			}
		}

		if(node == 0) // Couldn't find a fit, so create a new group
		{
			if(rect_size.width <= max_group_size.width && rect_size.height <= max_group_size.height)
			{
				node = add_new_root()->node.insert(rect_size, next_node_id);
			}
			else
			{
				throw CL_Exception("Unable to pack rect into group: Larger than max_group_size");
			}
		}

		if(node == 0)
			throw CL_Exception("Unable to pack rect into group: Unknown reason");
	}

	next_node_id++;

	return CL_RectPacker::AllocatedRect(active_root_node->group_id, node->node_rect);
}

CL_RectPacker_Impl::RootNode *CL_RectPacker_Impl::add_new_root()
{
	Node node(CL_Rect(CL_Point(0, 0), max_group_size));

	active_root_node = new RootNode();
	active_root_node->group_id = (int)root_nodes.size();
	active_root_node->node = node;
	
	root_nodes.push_back(active_root_node);

	return active_root_node;
}

/////////////////////////////////////////////////////////////////////////////
// CL_RectPacker_Impl::Node:

CL_RectPacker_Impl::Node::Node()
{
	child[0] = NULL;
	child[1] = NULL;

	id = 0;
}

CL_RectPacker_Impl::Node::Node(const CL_Rect &new_rect)
{
	node_rect = new_rect;

	child[0] = NULL;
	child[1] = NULL;

	id = 0;
}

CL_RectPacker_Impl::Node::~Node()
{
	clear();
}

int CL_RectPacker_Impl::Node::get_rect_count() const
{
	int count = 0;

	if(child[0])
		count += child[0]->get_rect_count();
	if(child[1])
		count += child[1]->get_rect_count();

	if(id)
		count++;

	return count;
}

void CL_RectPacker_Impl::Node::clear()
{
	if(child[0])
	{
		delete child[0];
		child[0] = NULL;
	}
	if(child[1])
	{
		delete child[1];
		child[1] = NULL;
	}
	id = 0;
}

CL_RectPacker_Impl::Node *CL_RectPacker_Impl::Node::insert(const CL_Size &rect_size, int rect_id)
{
	// If we're not a leaf
	if(child[0] && child[1])
	{
		// Try inserting into first child
		Node *new_node = child[0]->insert(rect_size, rect_id);
		if(new_node != NULL)
			return new_node;
        
		// No room, insert into second
		return child[1]->insert(rect_size, rect_id);
	}
	else
	{
		// If there's already a rect here, return
		if (id)
			return NULL;

		// If we're too small, return
		if (rect_size.width > node_rect.get_width() || rect_size.height > node_rect.get_height())
			return NULL;

		// If we're just right, accept
		if (rect_size.width == node_rect.get_width() && rect_size.height == node_rect.get_height())
		{
			id = rect_id;
			return this;
		}
        
		// Otherwise, decide which way to split
		int dw = node_rect.get_width() - rect_size.width;
		int dh = node_rect.get_height() - rect_size.height;

		if (dw > dh)
		{
			child[0] = new Node(CL_Rect(node_rect.left, node_rect.top, node_rect.left + rect_size.width, node_rect.bottom));
			child[1] = new Node(CL_Rect(node_rect.left + rect_size.width, node_rect.top, node_rect.right, node_rect.bottom));
		}
		else
		{
			child[0] = new Node(CL_Rect(node_rect.left, node_rect.top, node_rect.right, node_rect.top + rect_size.height));
			child[1] = new Node(CL_Rect(node_rect.left, node_rect.top + rect_size.height, node_rect.right, node_rect.bottom));
		}
        
		// Insert into first child we created
		return child[0]->insert(rect_size, rect_id);
	}
}
