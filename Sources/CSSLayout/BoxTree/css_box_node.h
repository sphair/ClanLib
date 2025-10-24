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

#pragma once

class CL_CSSLayoutUserData;

class CL_CSSBoxNode
{
public:
	CL_CSSBoxNode();
	virtual ~CL_CSSBoxNode();

	void push_front(CL_CSSBoxNode *new_child);
	void push_back(CL_CSSBoxNode *new_child);
	void insert(CL_CSSBoxNode *new_child, CL_CSSBoxNode *insert_point);
	void remove();
	void set_user_data(std::auto_ptr<CL_CSSLayoutUserData> data);
	CL_CSSLayoutUserData *get_user_data();
	const CL_CSSLayoutUserData *get_user_data() const;

	CL_CSSBoxNode *get_parent() { return parent; }
	CL_CSSBoxNode *get_next_sibling() { return next; }
	CL_CSSBoxNode *get_prev_sibling() { return prev; }
	CL_CSSBoxNode *get_first_child() { return first_child; }
	CL_CSSBoxNode *get_last_child() { return last_child; }

	const CL_CSSBoxNode *get_parent() const { return parent; }
	const CL_CSSBoxNode *get_next_sibling() const { return next; }
	const CL_CSSBoxNode *get_prev_sibling() const { return prev; }
	const CL_CSSBoxNode *get_first_child() const { return first_child; }
	const CL_CSSBoxNode *get_last_child() const { return last_child; }

private:
	CL_CSSBoxNode *parent;
	CL_CSSBoxNode *next;
	CL_CSSBoxNode *prev;
	CL_CSSBoxNode *first_child;
	CL_CSSBoxNode *last_child;
	std::auto_ptr<CL_CSSLayoutUserData> user_data;
};
