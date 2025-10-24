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

#include "API/Display/font.h"
#include "API/Display/display.h"
#include "API/Display/keys.h"
#include "API/Display/sprite.h"
#include "API/Display/pixel_format.h"
#include "API/Display/canvas.h"
#include "API/Display/graphic_context.h"
//#include "../Display/graphic_context_generic.h"
#include "../GL/graphic_context_opengl.h"
#include "API/GUI/scrollbar.h"
#include "API/GUI/richedit_paragraph.h"
#include "API/GUIStyleSilver/richedit_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"


const int RICHEDIT_BOUND_X = 3;
const int RICHEDIT_BOUND_Y = 2;


/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_RichEdit_Silver::CL_RichEdit_Silver(
	CL_RichEdit *richedit,
	CL_StyleManager_Silver *style)
:
	CL_ComponentStyle(richedit),
	richedit(richedit),
	style(style)
{
	resources = style->get_resources();
	font = new CL_Font("InputBox/font", resources);
	font->set_color( CL_Color::black );

	// Create a vertical scrollbar
	scrollbar = new CL_ScrollBar(false, richedit);
	richedit->add_child(scrollbar, false);

	CL_Component *client_area = richedit->get_client_area();

//	slots.connect(richedit->sig_set_options(), this, &CL_RichEdit_Silver::on_set_options);
	slots.connect(richedit->sig_paint(), this, &CL_RichEdit_Silver::on_paint_background);
	slots.connect(client_area->sig_mouse_down(), this, &CL_RichEdit_Silver::on_mouse_down);
	slots.connect(client_area->sig_paint(), this, &CL_RichEdit_Silver::on_paint_richedit);
	slots.connect(richedit->sig_resize(), this, &CL_RichEdit_Silver::on_resize);
	slots.connect(richedit->sig_text_changed(), this, &CL_RichEdit_Silver::on_text_changed);
	slots.connect(richedit->sig_draw_from_changed(), this, &CL_RichEdit_Silver::on_draw_from_changed);
	slots.connect(richedit->sig_clear(), this, &CL_RichEdit_Silver::on_clear);
	slots.connect(scrollbar->sig_value_changed(), this, &CL_RichEdit_Silver::on_scroll_change);

	// init the scrollbox
    on_resize(0,0);

	richedit->set_bounds( RICHEDIT_BOUND_X*3 + scrollbar->get_width(), RICHEDIT_BOUND_Y*2 );
}

CL_RichEdit_Silver::~CL_RichEdit_Silver()
{
	delete font;
	delete scrollbar;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_RichEdit_Silver::on_clear()
{
	update_scrollbar();
}

void CL_RichEdit_Silver::on_text_changed()
{
	redraw_richedit_into_buffer( false );
	update_scrollbar();
}

void CL_RichEdit_Silver::on_draw_from_changed()
{
	redraw_richedit_into_buffer( false );
	update_scrollbar();
}

/*
void CL_RichEdit_Silver::on_set_options(const CL_DomElement &options)
{
}
*/
void CL_RichEdit_Silver::on_scroll_change(int new_offset)
{
	redraw_richedit_into_buffer( false );
	richedit->set_top_item(new_offset);
}

void CL_RichEdit_Silver::on_mouse_down(const CL_InputEvent &key)
{
	if(key.id == CL_MOUSE_WHEEL_UP)
	{
		richedit->set_top_item(richedit->get_top_item() - 5);
		update_scrollbar();
		return;
	}
	if(key.id == CL_MOUSE_WHEEL_DOWN)
	{
		richedit->set_top_item(richedit->get_top_item() + 5);
		update_scrollbar();
		return;
	}
}

void CL_RichEdit_Silver::on_resize(int old_width, int old_height)
{
	richedit->on_resize();

	redraw_richedit_into_buffer( true );

	// Position the scrollbar
	CL_Rect rect(richedit->get_width() - 19, 3, richedit->get_width() - 3, richedit->get_height() - 3);
	scrollbar->set_position(rect);

	update_scrollbar();
}

void CL_RichEdit_Silver::on_paint_background()
{
	// Outline
	CL_Display::draw_rect(richedit->get_screen_rect(), CL_Color(128, 142, 159));

	// Background
	CL_Rect rect = richedit->get_screen_rect();
	rect.left++;
	rect.top++;
	rect.right--;
	rect.bottom--;
	CL_Display::fill_rect(rect, CL_Color::white);
}

void CL_RichEdit_Silver::redraw_richedit_into_buffer( bool resize )
{
/*
	//if (resize)
	//{
		int w = richedit->get_width();
		int h = richedit->get_height();

		CL_PixelFormat my_pf = CL_PixelFormat::rgba8888;
		CL_PixelBuffer * my_pb = new CL_PixelBuffer( w, h, w*4, my_pf );
		CL_Surface * my_surface = new CL_Surface( *my_pb );
		CL_Canvas * my_canvas = new CL_Canvas( *my_surface );
		//my_canvas->select_surface( *my_surface );

		//CL_Canvas_OpenGL * my_canvas_gl = new CL_Canvas_OpenGL(
		CL_GraphicContext_OpenGL * my_gc_gl = new CL_GraphicContext_OpenGL( my_canvas );
		CL_GraphicContext * my_gc = new CL_GraphicContext( my_gc_gen );
	//}

	font->draw( 10, 10, "Test string!!!", my_gc );
*/
	//CL_Display::

/*
*/
}

void CL_RichEdit_Silver::on_paint_richedit()
{
	int draw_from = richedit->get_top_item();

	int xx = richedit->get_screen_x() + RICHEDIT_BOUND_X;
	int yy = richedit->get_screen_y() + RICHEDIT_BOUND_Y;
	int xx2 = xx + richedit->get_width() - RICHEDIT_BOUND_X - scrollbar->get_width();

	CL_Display::push_cliprect( CL_Rect(xx, yy, xx2, yy + richedit->get_height() - RICHEDIT_BOUND_Y*2) );

	// get the cursor position
	std::pair<int,int> cur_pos = richedit->get_cursor_pos();

	// iterate all visible paragraphs
	int lines_count = richedit->get_total_lines_count();

	for (int i=draw_from; i<lines_count; ++i)
	{
		CL_RichEdit_Paragraph::OneLineInfo const * line = richedit->get_line(i);
		if (!line)
		{
			line = richedit->get_line(i);
		}

		// iterate through items in the line
		for (size_t j=0; j<line->items.size(); ++j)
		{
			int was_xx = xx;
			switch (line->items[j].item_type)
			{
			case CL_RichEdit_Paragraph::Item::ITEM_STRING:
				{
					const_cast<CL_Font*>( line->items[j].font ) -> set_color( line->items[j].color );
					line->items[j].font->draw( xx, yy, line->items[j].str );

					xx += font->get_width( line->items[j].str );
				}
				break;
			case CL_RichEdit_Paragraph::Item::ITEM_IMAGE:
				{
					CL_Sprite & img = const_cast<CL_Sprite&>( *line->items[j].img );

					img.set_alignment( origin_top_left );
					img.draw( (float)xx, (float)yy );

					xx += img.get_width();
				}
				break;
			}

			if (xx > was_xx)
				xx += CL_RichEdit_Paragraph::SPACE_BETWEEN_ITEMS;

		} // iteration in the current line

		// new paragraph
		xx = richedit->get_screen_x() + RICHEDIT_BOUND_X;
		yy += line->height + CL_RichEdit_Paragraph::SPACE_BETWEEN_LINES;

		// check if next line is (maybe, partially) visible
		if ( yy >= richedit->get_screen_y() + richedit->get_height() - RICHEDIT_BOUND_Y )
		{
			CL_Display::pop_cliprect();
			return;
		}

	} // iteration throw all visible lines

	CL_Display::pop_cliprect();
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CL_RichEdit_Silver::update_scrollbar()
{	
	scrollbar->set_max_value( richedit->get_total_lines_count() );
	scrollbar->set_value( richedit->get_top_item() );
}
