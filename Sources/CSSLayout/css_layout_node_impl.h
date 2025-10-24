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

#include "API/Core/System/disposable_object.h"
#include "BoxTree/css_box_node.h"

class CL_CSSLayout_Impl;
class CL_CSSBoxDisplay;
class CL_CSSBoxColor;
class CL_CSSBoxFontSize;
class CL_CSSBoxFloat;
class CL_CSSBoxWidth;

class CL_CSSLayoutNode_Impl : public CL_DisposableObject
{
public:
	CL_CSSLayoutNode_Impl() : box_node(0) { }
	~CL_CSSLayoutNode_Impl();

	CL_CSSLayoutNode get_node(CL_CSSBoxNode *box_node) const;

	static CL_String print_node(CL_CSSBoxNode *node, int indent = 0);
	static CL_String escape_text(CL_String text);
	static CL_String to_string(const CL_CSSBoxDisplay &display);
	static CL_String to_string(const CL_CSSBoxColor &color);
	static CL_String to_string(const CL_CSSBoxFontSize &font_size);
	static CL_String to_string(const CL_CSSBoxFloat &float_box);
	static CL_String to_string(const CL_CSSBoxWidth &width);

	CL_CSSBoxNode *box_node;
	CL_WeakPtr<CL_CSSLayout_Impl> layout_impl;

private:
	void on_dispose();
};
