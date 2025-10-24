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
#include "API/GUI/richedit.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"
#include "API/GUI/richedit_paragraph.h"
#include "richedit_generic.h"
#include "component_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_RichEdit::CL_RichEdit(CL_Component *parent, CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_RichEdit_Generic(this);
	get_style_manager()->connect_styles("richedit", this);
	find_preferred_size();
}

CL_RichEdit::CL_RichEdit(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_RichEdit_Generic(this);
	get_style_manager()->connect_styles("richedit", this);
}

CL_RichEdit::~CL_RichEdit()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

//std::vector<CL_RichEdit_Paragraph *> const & CL_RichEdit::get_paragraphs() const
//{
//	return impl->get_paragraphs();
//}

int CL_RichEdit::get_total_lines_count() const
{
	return impl->get_total_lines_count();
}

int CL_RichEdit::get_top_item() const
{
	return impl->get_top_item();
}

CL_RichEdit_Paragraph::OneLineInfo const * CL_RichEdit::get_line( int line_idx ) const
{
	return impl->get_line( line_idx );
}

std::pair<int,int> CL_RichEdit::get_cursor_pos() const
{
	return impl->get_cursor_pos();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_RichEdit::add_text( std::string const & text, CL_Font const * font, CL_Color const & color )
{
	impl->add_text(text, font, color);
}

void CL_RichEdit::add_img( CL_Sprite * img, const std::string &descr )
{
	impl->add_img(img, descr);
}

void CL_RichEdit::make_new_paragraph()
{
	impl->make_new_paragraph();
}

void CL_RichEdit::set_bounds( int bounds_width, int bounds_height )
{
	impl->bounds_width = bounds_width;
	impl->bounds_height = bounds_height;
}

void CL_RichEdit::set_top_item( int idx )
{
	impl->set_top_item( idx );
}

void CL_RichEdit::scroll_to_the_end()
{
	impl->scroll_to_the_end();
}

void CL_RichEdit::clear()
{
	impl->clear();
}

/////////////////////////////////////////////////////////////////////////////
// Feedbacks:

void CL_RichEdit::on_resize()
{
	impl->on_resize();
}


/*
/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<int> &CL_RichEdit::sig_highlighted()
{
	return impl->sig_highlighted;
}

CL_Signal_v1<int> &CL_RichEdit::sig_activated()
{
	return impl->sig_activated;
}

CL_Signal_v0 &CL_RichEdit::sig_selection_changed()
{
	return impl->sig_selection_changed;
}
*/

CL_Signal_v0 &CL_RichEdit::sig_text_changed()
{
	return impl->sig_text_changed;
}

CL_Signal_v0 &CL_RichEdit::sig_draw_from_changed()
{
	return impl->sig_draw_from_changed;
}

CL_Signal_v0 &CL_RichEdit::sig_clear()
{
	return impl->sig_clear;
}
