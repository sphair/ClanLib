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

#include "Display/precomp.h"
#include "span_layout_impl.h"

CL_SpanLayout_Impl::CL_SpanLayout_Impl() : alignment(cl_left)
{
}

void CL_SpanLayout_Impl::clear()
{
	text.clear();
	objects.clear();
	lines.clear();
}

void CL_SpanLayout_Impl::draw_layout(CL_GraphicContext &gc)
{
	int x = position.x;
	int y = position.y;
	for (std::vector<Line>::size_type line_index = 0; line_index < lines.size(); line_index++)
	{
		Line &line = lines[line_index];
		for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
		{
			LineSegment &segment = line.segments[segment_index];
			CL_StringRef segment_text = text.substr(segment.start, segment.end-segment.start);
			segment.font.draw_text(gc, x + segment.x_position, y+line.ascender/*-segment.ascender*/, segment_text, segment.color);
		}
		y += line.height;
	}
}

int CL_SpanLayout_Impl::hit_test(const CL_Point &pos) const
{
	int x = position.x;
	int y = position.y;
	for (std::vector<Line>::size_type line_index = 0; line_index < lines.size(); line_index++)
	{
		const Line &line = lines[line_index];
		for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
		{
			const LineSegment &segment = line.segments[segment_index];
			CL_Rect area(CL_Point(x + segment.x_position, y/* - line.height*/), CL_Size(segment.width, line.height));
			if (area.contains(pos))
			{
				return segment.id;
			}
		}
		y += line.height;
	}
	
	return -1;
}

CL_Size CL_SpanLayout_Impl::get_size() const
{
	CL_Size size(0,0);
	int x = 0;
	int y = 0;
	for (std::vector<Line>::size_type line_index = 0; line_index < lines.size(); line_index++)
	{
		const Line &line = lines[line_index];
		for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
		{
			const LineSegment &segment = line.segments[segment_index];
			CL_Rect area(CL_Point(x + segment.x_position, y - line.height), CL_Size(segment.width, line.height));
			size.width = cl_max(size.width, area.right);
			size.height = cl_max(size.height, area.bottom);
		}
		y += line.height;
	}
	size.height = cl_max(size.height, y);
	return size;
}

void CL_SpanLayout_Impl::add_text(const CL_String &more_text, const CL_Font &font, const CL_Colorf &color, int id)
{
	SpanObject object;
	object.start = text.length();
	object.end = object.start + more_text.length();
	object.font = font;
	object.color = color;
	object.id = id;
	objects.push_back(object);
	text += more_text;
}

void CL_SpanLayout_Impl::layout(CL_GraphicContext &gc, int max_width)
{
	lines.clear();
	if (objects.empty())
		return;

	layout_cache.metrics = -1;

	std::vector<Word> words = find_words();
	std::vector<SpanObject>::size_type object_index = 0;
	Line cur_line;
	int x_position = 0;
	for (std::vector<Word>::size_type word_index = 0; word_index < words.size(); word_index++)
	{
		TextSizeResult text_size_result = find_text_size(gc, words[word_index], object_index);
		if (x_position + text_size_result.width > max_width) // word wrap needed
		{
			if (text_size_result.width > max_width) // the single word is too large for the line - force line breaks until it fits
			{
				if (cur_line.height == 0)
					lines.push_back(cur_line);
				cur_line = Line();
				x_position = 0;
				object_index += text_size_result.objects_traversed;
			}
			else
			{
				lines.push_back(cur_line);
				cur_line = Line();
				x_position = 0;
				for (std::vector<LineSegment>::iterator it = text_size_result.segments.begin(); it != text_size_result.segments.end(); ++it)
				{
					LineSegment segment = *it;
					segment.x_position += x_position;
					cur_line.segments.push_back(segment);
				}

				cur_line.width = x_position + text_size_result.width;

				x_position += text_size_result.width + text_size_result.spacing;
				cur_line.height = cl_max(cur_line.height, text_size_result.height);
				cur_line.ascender = cl_max(cur_line.ascender, text_size_result.ascender);
				object_index += text_size_result.objects_traversed;
			}
		}
		else
		{
			for (std::vector<LineSegment>::iterator it = text_size_result.segments.begin(); it != text_size_result.segments.end(); ++it)
			{
				LineSegment segment = *it;
				segment.x_position += x_position;
				cur_line.segments.push_back(segment);
			}

			cur_line.width = x_position + text_size_result.width;

			x_position += text_size_result.width + text_size_result.spacing;
			cur_line.height = cl_max(cur_line.height, text_size_result.height);
			cur_line.ascender = cl_max(cur_line.ascender, text_size_result.ascender);
			object_index += text_size_result.objects_traversed;
		}
	}
	if (cur_line.height > 0)
		lines.push_back(cur_line);

	// Check for alignment
	switch (alignment)
	{
		case cl_right:
		{
			for (std::vector<Line>::size_type line_index = 0; line_index < lines.size(); line_index++)
			{
				Line &line = lines[line_index];
				int offset = max_width - line.width;
				if (offset < 0) offset = 0;

				for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
				{
					LineSegment &segment = line.segments[segment_index];
					segment.x_position += offset;
				}
			}
			break;
		}
		case cl_center:
		{
			for (std::vector<Line>::size_type line_index = 0; line_index < lines.size(); line_index++)
			{
				Line &line = lines[line_index];
				int offset = (max_width - line.width)/2;
				if (offset < 0) offset = 0;

				for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
				{
					LineSegment &segment = line.segments[segment_index];
					segment.x_position += offset;
				}
			}
			break;
		}

		case cl_justify:
		{
			// Note, we do not justify the last line
			if (lines.size() == 0) break;	// No lines exist
			for (std::vector<Line>::size_type line_index = 0; line_index < (lines.size()-1); line_index++)
			{
				Line &line = lines[line_index];
				int offset = max_width - line.width;
				if (offset < 0) offset = 0;

				if (line.segments.size() <= 1)	// Do not justify line if only one word exists
					continue;

				for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
				{
					LineSegment &segment = line.segments[segment_index];
					segment.x_position += (offset * segment_index) / (line.segments.size()-1);
				}
			}
			break;
		}

		case cl_left:
		default:
			break;
	}

}

CL_SpanLayout_Impl::TextSizeResult CL_SpanLayout_Impl::find_text_size(CL_GraphicContext &gc, const Word &word, int object_index, int forced_line_break)
{
	CL_Font font = objects[object_index].font;
	if (layout_cache.object_index != object_index)
	{
		layout_cache.object_index = object_index;
		layout_cache.metrics = font.get_font_metrics(gc);
	}

	TextSizeResult result;
	result.start = word.start;
	int pos = word.start;
	int x_position = 0;
	while (pos != word.end)
	{
		int end = cl_min(objects[object_index].end, word.end);
		CL_StringRef subtext = text.substr(pos, end-pos);

		CL_Size text_size = font.get_text_size(gc, subtext);

		result.width += text_size.width;
		result.height = cl_max(result.height, (int)(layout_cache.metrics.get_height()+layout_cache.metrics.get_external_leading())/*text_size.height*/);
		result.ascender = cl_max(result.ascender, (int)layout_cache.metrics.get_ascent());

		LineSegment segment;
		segment.start = pos;
		segment.end = end;
		segment.font = objects[object_index].font;
		segment.color = objects[object_index].color;
		segment.id = objects[object_index].id;
		segment.x_position = x_position;
		segment.width = text_size.width;
		segment.ascender = (int)layout_cache.metrics.get_ascent();
		x_position += text_size.width;
		result.segments.push_back(segment);

		pos = end;
		if (pos == objects[object_index].end)
		{
			object_index++;
			result.objects_traversed++;

			if (object_index < objects.size())
			{
				layout_cache.object_index = object_index;
				font = objects[object_index].font;
				layout_cache.metrics = font.get_font_metrics(gc);
			}
		}
	}
	result.end = pos;
	if (object_index < objects.size())
		result.spacing = font.get_text_size(gc, cl_text(" ")).width;
	return result;
}

std::vector<CL_SpanLayout_Impl::Word> CL_SpanLayout_Impl::find_words()
{
	std::vector<Word> words;
	CL_String::size_type pos = 0;
	while (true)
	{
		CL_String::size_type start_pos = text.find_first_not_of(cl_text(" \t\r\n"), pos);
		if (start_pos == CL_String::npos)
			break;

		CL_String::size_type end_pos = text.find_first_of(cl_text(" \t\r\n"), start_pos);
		if (end_pos == CL_String::npos)
			end_pos = text.length();

		Word word;
		word.start = start_pos;
		word.end = end_pos;
		words.push_back(word);

		pos = end_pos;
	}
	return words;
}

void CL_SpanLayout_Impl::set_align(CL_SpanAlign align)
{
	alignment = align;
}
