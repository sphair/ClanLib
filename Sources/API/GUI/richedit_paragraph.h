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

//! clanGUI="Framework"
//! header=gui.h

#ifndef header_richedit_item
#define header_richedit_item

#ifdef CL_API_DLL
#ifdef CL_GUI_EXPORT
#define CL_API_GUI __declspec(dllexport)
#else
#define CL_API_GUI __declspec(dllimport)
#endif
#else
#define CL_API_GUI
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include "../Display/color.h"

class CL_Sprite;
class CL_Font;

//: RichEdit paragraph class.
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_RichEdit_Paragraph
{
public:
	const static int SPACE_BETWEEN_ITEMS;
	const static int SPACE_BETWEEN_LINES;
	//const static int SPACE_BETWEEN_PARAGRAPHS;

	struct Item
	{
		enum ITEM_TYPE
		{
			ITEM_STRING,
			ITEM_IMAGE,
		} item_type;

		// for ITEM_STRING
		std::string str;
		CL_Color color;
		CL_Font const * font;

		// for ITEM_IMAGE
		CL_Sprite * img;

		//
		Item( std::string const &str, CL_Font const * font, CL_Color const & color );
		Item( CL_Sprite * img_, std::string const & descr );
	};

	struct OneLineInfo
	{
		int item_idx;
		int char_idx;
		int height;
		std::vector<Item> items;

		OneLineInfo( int item_idx, int char_idx, int height )
			: item_idx(item_idx), char_idx(char_idx), height(height)
		{}
	};

public:
	CL_RichEdit_Paragraph( int textarea_width );
	virtual ~CL_RichEdit_Paragraph();

	//
	void add_text( std::string const & text, CL_Font const * font, CL_Color const & color );
	void add_img( CL_Sprite * img, std::string const & descr );

	//
	OneLineInfo const * get_line( int line_idx ) const;
	int get_total_height() const;
	int get_lines_count() const;

	//
	void on_resize( int textarea_width );

private:
	void check_is_new_line( int &cur_w, int w, int h, int &cur_max_height, int item_idx, int char_idx );
	void update( int line_from );
	void generate_line_info( int line_idx );

private:
	std::vector<Item> items;
	std::vector<OneLineInfo> lines; // lines.size() === amount of lines in this paragraph (depends on the textarea width)
	int total_height;
	int textarea_width;
};

#endif
