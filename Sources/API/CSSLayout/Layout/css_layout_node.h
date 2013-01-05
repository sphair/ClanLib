/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "../api_csslayout.h"
#include "css_layout_user_data.h"
#include <memory>

namespace clan
{

class CSSLayoutText;
class CSSLayoutElement;
class CSSLayoutObject;
class CSSLayoutNode_Impl;

class CL_API_CSSLAYOUT CSSLayoutNode
{
public:
	CSSLayoutNode();

	bool is_null() const;
	bool is_text() const;
	bool is_element() const;
	bool is_object() const;
	CSSLayoutText to_text() const;
	CSSLayoutElement to_element() const;
	CSSLayoutObject to_object() const;

	CSSLayoutNode get_parent() const;
	CSSLayoutNode get_next_sibling() const;
	CSSLayoutNode get_prev_sibling() const;
	CSSLayoutNode get_first_child() const;
	CSSLayoutNode get_last_child() const;

	CSSLayoutUserData *get_user_data();
	const CSSLayoutUserData *get_user_data() const;
	void set_user_data(CSSLayoutUserData *data) { std::unique_ptr<CSSLayoutUserData> cmd(data); set_user_data(cmd); }
	void set_user_data(std::unique_ptr<CSSLayoutUserData> &data);

	std::string print_node() const;

	bool operator==(const CSSLayoutNode &other) const;
	bool operator!=(const CSSLayoutNode &other) const;

protected:
	std::shared_ptr<CSSLayoutNode_Impl> impl;
	CSSLayoutNode(const std::shared_ptr<CSSLayoutNode_Impl> &impl);
	friend class CSSLayout;
	friend class CSSLayoutElement;
	friend class CSSLayout_Impl;
};

}
