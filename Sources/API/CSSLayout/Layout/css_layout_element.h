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

#include "css_layout_node.h"

namespace clan
{
/// \addtogroup clanCSSLayout_Layout clanCSSLayout Layout
/// \{

class CSSPropertyValue;
class Rect;

class CSSLayoutElement : public CSSLayoutNode
{
public:
	CSSLayoutElement();

	std::string get_name() const;
	void set_name(const std::string &name);

	bool has_attribute(const std::string &name) const;
	void set_attribute(const std::string &name, const std::string &value);

	void set_col_span(int span);
	void set_row_span(int span);

	void insert_before(CSSLayoutNode &new_child, CSSLayoutNode &ref_child);
	void append_child(CSSLayoutNode &new_child);
	void remove_child(CSSLayoutNode &old_child);

	Rect get_content_box();

	CSSLayoutObject create_object();
	CSSLayoutElement create_element(const std::string &name = std::string());
	CSSLayoutText create_text(const std::string &text);

protected:
	CSSLayoutElement(const std::shared_ptr<CSSLayoutNode_Impl> &impl);
	friend class CSSLayoutNode;
};

/// \}
}
