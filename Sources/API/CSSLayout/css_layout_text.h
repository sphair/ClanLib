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
#include "../Core/Math/rect.h"

class CL_API_CSSLAYOUT CL_CSSLayoutText : public CL_CSSLayoutNode
{
public:
	CL_CSSLayoutText();

	CL_String get_text() const;
	void set_text(const CL_String &text);

	struct LineSpan
	{
		LineSpan() : text_start(0), text_end(0) { }

		CL_Rect box;
		size_t text_start;
		size_t text_end;
	};
	std::vector<LineSpan> get_line_spans();
	CL_Rect get_cursor_box(CL_GraphicContext &gc, CL_String::size_type pos);

private:
	CL_CSSLayoutText(const CL_SharedPtr<CL_CSSLayoutNode_Impl> &impl);
	friend class CL_CSSLayoutNode;
};
