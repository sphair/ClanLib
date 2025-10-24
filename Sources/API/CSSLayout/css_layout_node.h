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

#include "api_csslayout.h"
#include "css_layout_user_data.h"

class CL_CSSLayoutText;
class CL_CSSLayoutElement;
class CL_CSSLayoutObject;
class CL_CSSLayoutNode_Impl;

class CL_API_CSSLAYOUT CL_CSSLayoutNode
{
public:
	CL_CSSLayoutNode();

	bool is_null() const;
	bool is_text() const;
	bool is_element() const;
	bool is_object() const;
	CL_CSSLayoutText to_text() const;
	CL_CSSLayoutElement to_element() const;
	CL_CSSLayoutObject to_object() const;

	CL_CSSLayoutNode get_parent() const;
	CL_CSSLayoutNode get_next_sibling() const;
	CL_CSSLayoutNode get_prev_sibling() const;
	CL_CSSLayoutNode get_first_child() const;
	CL_CSSLayoutNode get_last_child() const;

	CL_CSSLayoutUserData *get_user_data();
	const CL_CSSLayoutUserData *get_user_data() const;
	void set_user_data(CL_CSSLayoutUserData *data) { set_user_data(std::auto_ptr<CL_CSSLayoutUserData>(data)); }
	void set_user_data(std::auto_ptr<CL_CSSLayoutUserData> data);

	CL_String print_node() const;

	bool operator==(const CL_CSSLayoutNode &other) const;
	bool operator!=(const CL_CSSLayoutNode &other) const;

protected:
	CL_SharedPtr<CL_CSSLayoutNode_Impl> impl;
	CL_CSSLayoutNode(const CL_SharedPtr<CL_CSSLayoutNode_Impl> &impl);
	friend class CL_CSSLayout;
	friend class CL_CSSLayoutElement;
	friend class CL_CSSLayout_Impl;
};
