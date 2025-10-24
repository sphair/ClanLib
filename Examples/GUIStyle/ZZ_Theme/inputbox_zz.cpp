#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "stylemanager_zz.h"
#include "inputbox_zz.h"

#pragma warning ( push, 1 )
#pragma warning ( disable: 4244 ) // conversion from 'int' to 'float', possible loss of data


CL_InputBox_ZZ::CL_InputBox_ZZ(
	CL_InputBox *inputbox,
	CL_StyleManager_ZZ *style)
:   
    CL_ComponentStyle(inputbox),
	CL_InputBox_StyleBase(inputbox, style->get_resources(), "InputBox/font", 3),
    first_paint(false)
{
	//this->style = style;
	//resources = style->get_resources();

	slots.connect(inputbox->sig_paint(), this, &CL_InputBox_ZZ::on_paint);
}

CL_InputBox_ZZ::~CL_InputBox_ZZ()
{
}

void CL_InputBox_ZZ::on_paint()
{
    on_paint_default();
}

void CL_InputBox_ZZ::on_changed()
{
    // TODO: 
}

void CL_InputBox_ZZ::on_paint_default()
{
    // Fill
    //-------
    CL_Color const clr_background(0, 40, 0, 60);
    CL_Rect const rc_main(inputbox->get_screen_rect());
	CL_StyleManager_ZZ::fill_rounded_rect(rc_main, clr_background);

    // Outline
    CL_Color const clr_outline(175, 205, 148);
	CL_StyleManager_ZZ::draw_rounded_rect(rc_main, clr_outline);

	on_paint_input();
}

void CL_InputBox_ZZ::on_paint_input()
{
    // That code is from NetObject, with small
    // correction to width_offset for font

	// For easy reference, put inputfield-text into local variable
	std::string const & text = inputbox->get_text();
	int width = inputbox->get_width();

	int pixel_y_offset = 4;

    int const font_width_offset = font->get_width_offset();
    font->set_width_offset(0);

	// Calculate scroll offset
	int drawing_text_width = 0;
	int draw_to = draw_from;

	const int BORDER_SIZE = 6;

	while (draw_to < text.length())
	{
		int w = get_chars_width( draw_to, draw_to );
		if (drawing_text_width + w > width - BORDER_SIZE * 2)
			break;

		drawing_text_width += w;
		draw_to ++;
	}

	// Display marked text, if any
	if (inputbox->has_marked_text())
	{
		int start = inputbox->get_selection_start();
		int end = start + inputbox->get_selection_length();

		if (start < draw_from) start = draw_from;
		if (end   > draw_to  ) end   = draw_to;

		int mark_x1 = get_chars_width( draw_from, start-1 );
		int mark_x2 = get_chars_width( draw_from, end-1 );

		CL_Display::fill_rect(
			CL_Rect(inputbox->get_screen_x() + BORDER_SIZE + mark_x1 - 1,
					inputbox->get_screen_y() + pixel_y_offset,
					inputbox->get_screen_x() + BORDER_SIZE + mark_x2,
					inputbox->get_screen_y() + pixel_y_offset + font->get_height() + 1),
			CL_Color(150, 180, 150, static_cast<unsigned int>(0.5f * 255)));
	}

	// Display text
	int draw_count = draw_to - draw_from;

	if (draw_from >= 0 && draw_count > 0)
	{
		font->draw(inputbox->get_screen_x() + BORDER_SIZE,
			inputbox->get_screen_y() + pixel_y_offset, text.substr( draw_from, draw_count ));
	}

	// Show blinking cursor
	if(inputbox->has_focus()) 
	{
		if (show_cursor)
		{
			int cursor_pos = inputbox->get_cursor_position();
			int cursor_x = border_size + get_chars_width( draw_from, cursor_pos - 1 );
				
			//style->
            CL_Display::draw_line(
				inputbox->get_screen_x() + cursor_x,
				inputbox->get_screen_y() + pixel_y_offset,
				inputbox->get_screen_x() + cursor_x,
				inputbox->get_screen_y() + pixel_y_offset + font->get_height() + 1,
                CL_Color::whitesmoke);
		}
		unsigned int cur_time = CL_System::get_time();
		if (cur_time >= cursor_blink_time)
		{
			cursor_blink_time = cur_time + 400;
			show_cursor = !show_cursor;
		}
	}
    font->set_width_offset(font_width_offset);
}


#pragma warning ( pop )
