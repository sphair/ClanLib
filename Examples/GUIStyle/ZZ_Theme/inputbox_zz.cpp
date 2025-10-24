#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "stylemanager_zz.h"
#include "inputbox_zz.h"

CL_InputBox_ZZ::CL_InputBox_ZZ(
	CL_InputBox *inputbox,
	CL_StyleManager_ZZ *style)
:   
    CL_ComponentStyle(inputbox), inputbox(inputbox),
    first_paint(false)
{
	this->style = style;

	resources = style->get_resources();
	font = new CL_Font("InputBox/font", resources);

	show_cursor = false;
	character_offset = 0;
	cursor_blink_time = CL_System::get_time();
	
	slots.connect(inputbox->sig_paint(), this, &CL_InputBox_ZZ::on_paint);
}

CL_InputBox_ZZ::~CL_InputBox_ZZ()
{
	delete font;
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
	const char *text = inputbox->get_text().c_str();

	int pixel_y_offset = 4;

    int const font_width_offset = font->get_width_offset();
    font->set_width_offset(0);

	// Calculate scroll offset
	int character_offset = 0;
	int pixel_offset = 0;

    int const BORDER_SIZE = 6;

	int width = (int)font->get_width(text);
	if (width > inputbox->get_width() - BORDER_SIZE * 2)
	{
		while (
			width >= inputbox->get_width() - BORDER_SIZE * 2 &&
			character_offset + 1 < inputbox->get_cursor_position())
		{
			int w = font->get_width(text[character_offset]);
			character_offset++;
			pixel_offset += w;
			width -= w;
		}
	}

	// Display marked text, if any
	if (inputbox->has_marked_text())
	{
		int start = inputbox->get_selection_start();
		int end = start + inputbox->get_selection_length();

		int mark_x1 = 0, mark_x2 = 0;

		int i;
		for(i = 0; i < start; i++)
			mark_x1 += font->get_width(text[i]);// + font->get_width_offset();

		for(i = start; i < end; i++)
			mark_x2 += font->get_width(text[i]);

		CL_Display::fill_rect(
			CL_Rect(inputbox->get_screen_x() + BORDER_SIZE + mark_x1 - 1,
					inputbox->get_screen_y() + pixel_y_offset,
					inputbox->get_screen_x() + BORDER_SIZE + mark_x1 + mark_x2,
					inputbox->get_screen_y() + pixel_y_offset + font->get_height() + 1),
			CL_Color(150, 180, 150, static_cast<unsigned int>(0.5f * 255)));
	}

	// Display text
    font->draw(inputbox->get_screen_x() + BORDER_SIZE,
		inputbox->get_screen_y() + pixel_y_offset, &text[character_offset]);

	// Show blinking cursor
	if(inputbox->has_focus()) 
	{
		if (show_cursor)
		{
			int cursor_x = BORDER_SIZE - pixel_offset;

			for(int i = 0; i < inputbox->get_cursor_position(); i++)
				cursor_x += font->get_width(text[i]);
				
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
