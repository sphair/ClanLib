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

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_richedit
#define header_richedit

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

#include "component.h"
#include "listitem.h"
#include "richedit_paragraph.h"

class CL_RichEdit_Generic;
class CL_Sprite;
class CL_Color;
class CL_Font;

//: RichEdit component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_RichEdit : public CL_Component
{
//! Construction:
public:
	//: RichEdit constructor
	CL_RichEdit(
		CL_Component *parent,
		CL_StyleManager *style = NULL);
	
	//: RichEdit constructor
	CL_RichEdit(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: RichEdit destructor
	virtual ~CL_RichEdit();

//! Attributes:
public:
	//: Returns the number of lines in the richedit. 
	CL_RichEdit_Paragraph::OneLineInfo const * get_line( int line_idx ) const;

	//: Returns the number of lines in the richedit. 
	int get_total_lines_count() const;

	int get_top_item() const;

	std::pair<int,int> get_cursor_pos() const;
	
//! Operations:
public:
	//: Adds text string into the end of richedit's text.
	void add_text( std::string const & text, CL_Font const * font, CL_Color const & color );

	//: Adds image into the end of richedit's text.
	void add_img( CL_Sprite * img, const std::string &descr );

	//: Makes new paragraph after the end of richedit's text.
	void make_new_paragraph();

	void set_bounds( int bounds_width, int bounds_height );

	void set_top_item( int idx );

	void scroll_to_the_end();

	//: Deselects current selected item.
	void clear_selection();

	//: Should be called on each resize of the RichEdit window
	void on_resize();

	//: If select is true, all items get selected, else all get unselected.
	//: This works only in Multi selection mode.
	void select_all(bool select = true);

	//: Deletes all items in the list. 
	void clear();

//! Signals:
public:
	//: This signal is emitted when a new item is added to the richedit.
	CL_Signal_v0 &sig_text_changed();

	//: This signal is emitted when draw_from variable is changed
	CL_Signal_v0 &sig_draw_from_changed();

	//: This signal is emitted when the richedit is cleared.
	CL_Signal_v0 &sig_clear();

//! Implementation:
private:
	CL_RichEdit(const CL_RichEdit &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.

	CL_RichEdit_Generic *impl;
};

#endif
