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
#include "css_layout_node.h"

class CL_CSSPropertyList2;
class CL_Rect;

class CL_API_CSSLAYOUT CL_CSSLayoutElement : public CL_CSSLayoutNode
{
public:
	CL_CSSLayoutElement();

	CL_String get_name() const;
	void set_name(const CL_String &name);

	void apply_properties(const CL_CSSPropertyList2 &properties);
	void apply_properties(const CL_String &style_string);

	void insert_before(CL_CSSLayoutNode &new_child, CL_CSSLayoutNode &ref_child);
	void append_child(CL_CSSLayoutNode &new_child);
	void remove_child(CL_CSSLayoutNode &old_child);

	CL_Rect get_content_box();

	CL_CSSLayoutObject create_object();
	CL_CSSLayoutElement create_element(const CL_String &name = CL_String());
	CL_CSSLayoutText create_text(const CL_String &text);

protected:
	CL_CSSLayoutElement(const CL_SharedPtr<CL_CSSLayoutNode_Impl> &impl);
	friend class CL_CSSLayoutNode;
};
