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

#include "API/Core/System/disposable_object.h"
#include "BoxTree/css_box_node.h"

namespace clan
{

class CSSLayout_Impl;
class CSSValueDisplay;
class CSSValueColor;
class CSSValueFontSize;
class CSSValueFloat;
class CSSValueWidth;

class CSSLayoutNode_Impl : public DisposableObject
{
public:
	CSSLayoutNode_Impl() : box_node(0) { }
	~CSSLayoutNode_Impl();

	CSSLayoutNode get_node(CSSBoxNode *box_node) const;

	static std::string print_node(CSSBoxNode *node, int indent = 0);
	static std::string escape_text(std::string text);
	static std::string to_string(const CSSValueDisplay &display);
	static std::string to_string(const CSSValueColor &color);
	static std::string to_string(const CSSValueFontSize &font_size);
	static std::string to_string(const CSSValueFloat &float_box);
	static std::string to_string(const CSSValueWidth &width);

	CSSBoxNode *box_node;
	std::weak_ptr<CSSLayout_Impl> layout_impl;

private:
	void on_dispose();
};

}
