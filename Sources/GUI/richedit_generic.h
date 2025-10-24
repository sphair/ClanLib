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

#ifndef header_richedit_generic
#define header_richedit_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <vector>

#include "API/signals.h"
#include "API/GUI/richedit_paragraph.h"

class CL_RichEdit;
class CL_Component;
class CL_DomElement;
class CL_StyleManager;
class CL_InputEvent;
class CL_InputDevice;
class CL_Rect;
class CL_ScrollBar;
class CL_Point;

class CL_RichEdit_Generic
{
//! Construction:
public:
	CL_RichEdit_Generic(CL_RichEdit *self);
	~CL_RichEdit_Generic();

//! Attributes:
public:
	CL_RichEdit_Paragraph::OneLineInfo const * get_line( int line_idx ) const;
	int get_total_lines_count() const;
	int get_top_item() const;

	std::pair<int,int> get_cursor_pos() const;

//! Operations:
public:
	void add_text( std::string const & text, CL_Font const * font, CL_Color const & color );
	void add_img( CL_Sprite * img, std::string const & descr );
	void make_new_paragraph();
	void clear_selection();
	void select_all(bool select);
	void clear();

	void set_top_item( int idx );
	void scroll_to_the_end();

//! Signals:
public:
//	CL_Signal_v0 sig_selection_changed;
	CL_Signal_v0 sig_text_changed;
	CL_Signal_v0 sig_draw_from_changed;
	CL_Signal_v0 sig_clear;

//! Connections
public:
	void on_mouse_down(const CL_InputEvent &key);
	void on_mouse_up(const CL_InputEvent &key);
	void on_mouse_move(const CL_InputEvent &key);
	void on_key_down(const CL_InputEvent &key);
	void on_resize();

//! Private functions
private:
	void update_total_lines_count();

//! Data members
private:
	int draw_from;
	int total_lines_count;

	std::pair<int,int> cur_pos;

	CL_RichEdit *richedit;
	CL_ScrollBar *scrollbar;

	std::vector<CL_RichEdit_Paragraph *> pars;

	CL_SlotContainer slots;

//! Public data members
public:
	int bounds_width;
	int bounds_height;
};

#endif
