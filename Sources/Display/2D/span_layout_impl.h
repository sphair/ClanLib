/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "API/Display/Font/font.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/span_layout.h"

class CL_SpanLayout_Impl
{
public:
	CL_SpanLayout_Impl();

	void clear();
	void add_text(const CL_String &text, const CL_Font &font, const CL_Colorf &color, int id = -1);
	void layout(CL_GraphicContext &gc, int max_width);
	int hit_test(const CL_Point &pos) const;
	void draw_layout(CL_GraphicContext &gc);
	void set_position(const CL_Point &pos) { position = pos; }
	CL_Size get_size() const;
	void set_align(CL_SpanAlign align);

private:
	struct Word
	{
		Word() : start(0), end(0) { }

		int start, end;
	};

	struct SpanObject
	{
		SpanObject() : start(0), end(0), id(-1) { }

		CL_Font font;
		CL_Colorf color;
		int start, end;
		int id;
	};

	struct LineSegment
	{
		LineSegment() : id(-1), start(0), end(0), x_position(0), width(0) { }

		CL_Font font;
		CL_Colorf color;
		int id;
		int start, end;
		int x_position;
		int width;
		int ascender;
	};

	struct Line
	{
		Line() : height(0), ascender(0), width(0) { }

		int width;	// Width of the entire line (including spaces)
		int height;
		int ascender;
		std::vector<LineSegment> segments;
	};

	struct TextSizeResult
	{
		TextSizeResult() : width(0),spacing(0),height(0),ascender(0),objects_traversed(0) { }
		int start, end;
		int width;
		int spacing;
		int height;
		int ascender;
		int objects_traversed;
		std::vector<LineSegment> segments;
	};

	TextSizeResult find_text_size(CL_GraphicContext &gc, const Word &word, int object_index, int forced_line_break = -1);
	std::vector<Word> find_words();

	CL_String text;
	std::vector<SpanObject> objects;
	std::vector<Line> lines;
	CL_Point position;

	CL_SpanAlign alignment;

	struct LayoutCache
	{
		LayoutCache() : object_index(-1) { }
		int object_index;
		CL_FontMetrics metrics;
	};
	LayoutCache layout_cache;
};
