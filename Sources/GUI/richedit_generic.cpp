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

#include <algorithm>

#include "richedit_generic.h"
#include "API/GUI/richedit.h"
#include "API/GUI/richedit_paragraph.h"
#include "API/GUI/image.h"
#include "API/GUI/component.h"
#include "API/GUI/stylemanager.h"
//#include "API/Core/XML/dom_element.h"
#include "API/Display/keys.h"
#include "API/Core/System/error.h"


/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_RichEdit_Generic::CL_RichEdit_Generic(CL_RichEdit *self)
	: richedit ( self )
	, bounds_width ( 0 )
	, draw_from ( 0 )
	, total_lines_count ( 0 )
	, cur_pos ( 0, 0 )
{

	CL_Component *client_area = richedit->get_client_area();

//	slots.connect(richedit->sig_set_options(), this, &CL_RichEdit_Generic::on_set_options);
	slots.connect(client_area->sig_mouse_down(), this, &CL_RichEdit_Generic::on_mouse_down);
	slots.connect(client_area->sig_key_down(), this, &CL_RichEdit_Generic::on_key_down);
}

CL_RichEdit_Generic::~CL_RichEdit_Generic()
{
	clear();
}


void CL_RichEdit_Generic::update_total_lines_count()
{
	total_lines_count = 0;
	for (size_t i=0; i<pars.size(); ++i)
		total_lines_count += pars[i]->get_lines_count();
}


/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_RichEdit_Paragraph::OneLineInfo const * CL_RichEdit_Generic::get_line( int line_idx ) const
{
	int lines_before = 0;
	for (size_t i=0; i<pars.size(); ++i)
	{
		int cur_lines = (int)pars[i]->get_lines_count();
		if ( lines_before + cur_lines > line_idx )
		{
			return pars[i]->get_line( line_idx - lines_before );
		}
		lines_before += cur_lines;
	}
	return NULL;
}

int CL_RichEdit_Generic::get_top_item() const
{
	return draw_from;
}

int CL_RichEdit_Generic::get_total_lines_count() const
{
	return total_lines_count;
}

std::pair<int,int> CL_RichEdit_Generic::get_cursor_pos() const
{
	return cur_pos;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_RichEdit_Generic::add_text( std::string const & text, CL_Font const * font, CL_Color const & color )
{
	if (pars.size() == 0)
		make_new_paragraph();

	pars[pars.size() - 1]->add_text( text, font, color );

	update_total_lines_count();
	sig_text_changed();
}

void CL_RichEdit_Generic::add_img( CL_Sprite * img, const std::string &descr )
{
	if (pars.size() == 0)
		make_new_paragraph();

	pars[pars.size() - 1]->add_img( img, descr );

	update_total_lines_count();
	sig_text_changed();
}

void CL_RichEdit_Generic::make_new_paragraph()
{
	CL_RichEdit_Paragraph * par = new CL_RichEdit_Paragraph( richedit->get_width() - bounds_width );
	pars.push_back( par );
}

void CL_RichEdit_Generic::set_top_item( int idx )
{
	if (pars.size() == 0  ||  total_lines_count == 0)
		return;

	int was_draw_from = draw_from;

	if (idx < 0)
		idx = 0;
	if (idx > total_lines_count - 1)
		idx = total_lines_count - 1;

	draw_from = idx;
	if (draw_from != was_draw_from)
		sig_draw_from_changed();
}

void CL_RichEdit_Generic::scroll_to_the_end()
{
	if (pars.size() == 0  ||  total_lines_count == 0)
		return;

	const int SOME_RESERVED_SPACE = 5;

	int was_draw_from = draw_from;
	draw_from = total_lines_count - 1;

	int used_height = get_line(draw_from)->height + CL_RichEdit_Paragraph::SPACE_BETWEEN_LINES;
	int height = richedit->get_height() - bounds_height - CL_RichEdit_Paragraph::SPACE_BETWEEN_LINES * 2;

	// correct used_height, if it is too small
	while ( draw_from > 0 )
	{
		draw_from --;
		int h = get_line(draw_from)->height + CL_RichEdit_Paragraph::SPACE_BETWEEN_LINES;
		if (used_height + h >= height - SOME_RESERVED_SPACE)
			break;
		used_height += h;
	}

	if (draw_from != was_draw_from)
		sig_draw_from_changed();
}

void CL_RichEdit_Generic::clear_selection()
{
	// TODO:

	update_total_lines_count();
	sig_text_changed();
}

void CL_RichEdit_Generic::clear()
{
	std::vector<CL_RichEdit_Paragraph *>::iterator it;
	for(it = pars.begin(); it != pars.end(); ++it)
		delete (*it);

	pars.clear();

	update_total_lines_count();
	sig_clear();
}

void CL_RichEdit_Generic::select_all(bool select)
{
	// TODO:
	// ...
	//sig_selection_changed();
}


/////////////////////////////////////////////////////////////////////////////

void CL_RichEdit_Generic::on_resize()
{
	for (size_t i=0; i<pars.size(); ++i)
	{
		pars[i]->on_resize( richedit->get_width() - bounds_width );
	}
	update_total_lines_count();
}


/////////////////////////////////////////////////////////////////////////////
// Callbacks:
/*
void CL_RichEdit_Generic::on_set_options(const CL_DomElement &options)
{
	CL_DomNode node = options.get_first_child();
	while (!node.is_null())
	{
		if (node.is_element())
		{
			CL_DomElement element = node.to_element();
			if (element.get_tag_name() == "text")
			{
				CL_RichEdit_Paragraph *item = new CL_RichEdit_Paragraph;
				item->str = element.get_attribute("value");
				pars.push_back(item);
			}
		}
		node = node.get_next_sibling();
	}
}
*/

void CL_RichEdit_Generic::on_mouse_down(const CL_InputEvent &key)
{
	if(pars.size() == 0)
		return;

	if (key.id == CL_MOUSE_LEFT)
	{
		// TODO:
		// ...
		//sig_selection_changed();
	}
}

void CL_RichEdit_Generic::on_mouse_up(const CL_InputEvent &key)
{
	if(pars.size() == 0)
		return;

	if(key.id == CL_MOUSE_LEFT)
	{
		// TODO:
	}
}

void CL_RichEdit_Generic::on_mouse_move(const CL_InputEvent &key)
{
	if(pars.size() == 0)
		return;

	// TODO:
}

void CL_RichEdit_Generic::on_key_down(const CL_InputEvent &key)
{
	// TODO:
}
