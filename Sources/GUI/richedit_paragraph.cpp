/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    AlexeyKharkov
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "API/Display/font.h"
#include "API/Display/color.h"
#include "API/GUI/image.h"
#include "API/GUI/richedit_paragraph.h"
#include "API/Display/sprite.h"

const int CL_RichEdit_Paragraph::SPACE_BETWEEN_ITEMS = 8;
const int CL_RichEdit_Paragraph::SPACE_BETWEEN_LINES = 1;
//const int CL_RichEdit_Paragraph::SPACE_BETWEEN_PARAGRAPHS = 3;

/////////////////////////////////////////////////////////////////////////////
// CL_RichEdit_Paragraph::Item

CL_RichEdit_Paragraph::Item::Item( std::string const &str, CL_Font const * font, CL_Color const & color )
	: str(str)
	, color(color)
	, font(font)
	, item_type(ITEM_STRING)
	, img(NULL)
{}

CL_RichEdit_Paragraph::Item::Item( CL_Sprite * img_, std::string const & descr )
	: img(img_)
	, item_type(ITEM_IMAGE)
	, str(descr)
	, font(NULL)
	, color(CL_Color::black)
{}



/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_RichEdit_Paragraph::CL_RichEdit_Paragraph( int textarea_width )
	: total_height ( 0 )
	, textarea_width ( textarea_width )
{
}

CL_RichEdit_Paragraph::~CL_RichEdit_Paragraph()
{
	for (size_t i=0; i<items.size(); ++i)
		if (items[i].item_type == Item::ITEM_IMAGE)
			delete items[i].img;
}

/////////////////////////////////////////////////////////////////////////////

CL_RichEdit_Paragraph::OneLineInfo const * CL_RichEdit_Paragraph::get_line( int line_idx ) const
{
	if (line_idx < 0 || line_idx >= (int)lines.size())
		return NULL;

	return &lines[line_idx];
}

void CL_RichEdit_Paragraph::generate_line_info( int line_idx )
{
	if (line_idx < 0 || line_idx >= (int)lines.size())
		return;

	//lines[line_idx].items.clear(); // commented, because we assume, that it is already empty here

	// init bounds
	int item_from = (line_idx == 0) ? 0 : lines[line_idx - 1].item_idx;
	int char_from = (line_idx == 0) ? 0 : lines[line_idx - 1].char_idx;

	int item_to = lines[line_idx].item_idx;
	int char_to = lines[line_idx].char_idx;

	// iterate through items
	for (int i = item_from; i <= item_to; ++i)  //Changed back to "<=". (See the update() function).  -AKharkov
	{
		switch (items[i].item_type)
		{
		case Item::ITEM_STRING:
			{
				int i1 = (i == item_from) ? char_from : 0;
				int i2 = (i == item_to  ) ? char_to   : items[i].str.length();
				lines[line_idx].items.push_back( Item( items[i].str.substr(i1,i2-i1), items[i].font, items[i].color ) );
			}
			break;
		case Item::ITEM_IMAGE:
			lines[line_idx].items.push_back( items[i] );
			break;
		}
	}
}

int CL_RichEdit_Paragraph::get_total_height() const
{
	return total_height;
}

int CL_RichEdit_Paragraph::get_lines_count() const
{
	return lines.size();
}


/////////////////////////////////////////////////////////////////////////////

void CL_RichEdit_Paragraph::add_text( std::string const & text, CL_Font const * font, CL_Color const & color )
{
	if ( items.size() > 0 )
	{
		Item & item = items[items.size()-1];
		if (item.item_type == Item::ITEM_STRING  &&  item.font == font  &&  item.color == color)
		{
			item.str += text; // just append this text to the last string
			update( lines.size() - 1 );
			return;
		}
	}
	// insert a new string
	items.push_back( Item(text, font, color) );
	update( lines.size() - 1 );
}

void CL_RichEdit_Paragraph::add_img( CL_Sprite * img, std::string const & descr )
{
	items.push_back( Item(img, descr) );
	update( lines.size() - 1 );
}

void CL_RichEdit_Paragraph::check_is_new_line( int &cur_w, int w, int h, int &cur_max_height, int item_idx, int char_idx )
{
	if (cur_max_height < h)
		cur_max_height = h;

	if (cur_w + w > textarea_width  &&  (w < textarea_width || cur_w > 0))
	{
		lines.push_back( OneLineInfo( item_idx, char_idx, cur_max_height ) );

		total_height += cur_max_height + SPACE_BETWEEN_LINES;
		cur_max_height = h;
		cur_w = w;
	} else
	{
		cur_w += w;
	}
}


void CL_RichEdit_Paragraph::on_resize( int _textarea_width )
{
	if (textarea_width == _textarea_width)
		return;

	textarea_width = _textarea_width;
	update(0);
}


void CL_RichEdit_Paragraph::update( int line_from )
{
	total_height = 0;
	int cur_max_height = 0;
	int cur_w = 0;

	if (line_from > (int)lines.size() - 1)
		line_from = (int)lines.size() - 1;

	if (line_from < 0)
		line_from = 0;

	// update all lines in the paragraph after 'line_from' (including)
	if (line_from > 0)
		lines.erase( lines.begin() + line_from, lines.end() );
	else
		lines.clear();

	if (items.size() == 0) // no items - no lines
		return;

	// divide all items on lines (using textarea_width)
	int item_from = (line_from == 0) ? 0 : lines[line_from-1].item_idx;
	int char_from = (line_from == 0) ? 0 : lines[line_from-1].char_idx;
	int i;

	for (i = item_from; i<(int)items.size(); ++i)
	{
		switch (items[i].item_type)
		{
		case Item::ITEM_STRING:
			{
				std::string const & s = items[i].str;
				int font_height = items[i].font->get_height();

				size_t j = (i==item_from) ? char_from : 0;

				for (; j<s.length(); ++j)
				{
					int w = items[i].font->get_width( s[j] );
					check_is_new_line( cur_w, w, font_height, cur_max_height, i, j );
				}
			}
			break;
		case Item::ITEM_IMAGE:
			{
				CL_Sprite const & img = *items[i].img;
				int w = img.get_width();
				int h = img.get_height();
				check_is_new_line( cur_w, w, h, cur_max_height, i, -1 );
			}
			break;
		}

		if (cur_w != 0)
			cur_w += SPACE_BETWEEN_ITEMS;
	}

	total_height += cur_max_height;
	lines.push_back( OneLineInfo( items.size()-1, -1, cur_max_height ) ); // added "-1" to stop the crash found by SAR

	//
	for (i = line_from; i<(int)lines.size(); ++i)
		generate_line_info( i );
}


