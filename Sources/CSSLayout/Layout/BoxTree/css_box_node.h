/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include <memory>

namespace clan
{

class CSSLayoutUserData;

class CSSBoxNode
{
public:
	CSSBoxNode();
	virtual ~CSSBoxNode();

	void push_front(CSSBoxNode *new_child);
	void push_back(CSSBoxNode *new_child);
	void insert(CSSBoxNode *new_child, CSSBoxNode *insert_point);
	void remove();
	void set_user_data(std::unique_ptr<CSSLayoutUserData> &data);
	CSSLayoutUserData *get_user_data();
	const CSSLayoutUserData *get_user_data() const;

	CSSBoxNode *get_parent() { return parent; }
	CSSBoxNode *get_next_sibling() { return next; }
	CSSBoxNode *get_prev_sibling() { return prev; }
	CSSBoxNode *get_first_child() { return first_child; }
	CSSBoxNode *get_last_child() { return last_child; }

	const CSSBoxNode *get_parent() const { return parent; }
	const CSSBoxNode *get_next_sibling() const { return next; }
	const CSSBoxNode *get_prev_sibling() const { return prev; }
	const CSSBoxNode *get_first_child() const { return first_child; }
	const CSSBoxNode *get_last_child() const { return last_child; }

private:
	CSSBoxNode *parent;
	CSSBoxNode *next;
	CSSBoxNode *prev;
	CSSBoxNode *first_child;
	CSSBoxNode *last_child;
	std::unique_ptr<CSSLayoutUserData> user_data;
};

}
