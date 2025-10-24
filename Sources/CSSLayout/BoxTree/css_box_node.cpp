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
**    Magnus Norddahl
*/

#include "CSSLayout/precomp.h"
#include "css_box_node.h"
#include "API/CSSLayout/css_layout_user_data.h"

CL_CSSBoxNode::CL_CSSBoxNode()
: parent(0), next(0), prev(0), first_child(0), last_child(0)
{
}

CL_CSSBoxNode::~CL_CSSBoxNode()
{
	remove();
}

void CL_CSSBoxNode::push_front(CL_CSSBoxNode *new_child)
{
	insert(new_child, first_child);
}

void CL_CSSBoxNode::push_back(CL_CSSBoxNode *new_child)
{
	insert(new_child, 0);
}

void CL_CSSBoxNode::insert(CL_CSSBoxNode *new_child, CL_CSSBoxNode *insert_point)
{
	if (new_child->parent)
		throw CL_Exception("CSSBoxNode already inserted");
	if (insert_point && insert_point->parent != this)
		throw CL_Exception("CSSBoxNode::insert misuse!");

	new_child->parent = this;
	if (insert_point)
	{
		new_child->prev = insert_point->prev;
		new_child->next = insert_point;
		if (new_child->prev)
			new_child->prev->next = new_child;
		if (new_child->next)
			new_child->next->prev = new_child;
		if (first_child == insert_point)
			first_child = new_child;
	}
	else
	{
		if (last_child)
		{
			last_child->next = new_child;
			new_child->prev = last_child;
		}
		last_child = new_child;
		if (!first_child)
			first_child = new_child;
	}
}

void CL_CSSBoxNode::remove()
{
	if (prev)
		prev->next = next;

	if (parent && parent->first_child == this)
		parent->first_child = next;

	if (next)
		next->prev = prev;

	if (parent && parent->last_child == this)
		parent->last_child = prev;

	prev = 0;
	next = 0;
	parent = 0;
}

void CL_CSSBoxNode::set_user_data(std::auto_ptr<CL_CSSLayoutUserData> data)
{
	user_data = data;
}

CL_CSSLayoutUserData *CL_CSSBoxNode::get_user_data()
{
	return user_data.get();
}

const CL_CSSLayoutUserData *CL_CSSBoxNode::get_user_data() const
{
	return user_data.get();
}
