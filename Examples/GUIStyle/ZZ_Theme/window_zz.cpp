#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "window_zz.h"
#include "stylemanager_zz.h"

CL_Window_ZZ::CL_Window_ZZ(
	CL_Window *window,
	CL_StyleManager_ZZ *style)
:
	CL_ComponentStyle(window),
	window(window),
	style(style),
    move_handler(0),
	resources(style->get_resources()),
    title_font(0),
    first_paint(true)
{
	slot_paint = window->sig_paint().connect(this, &CL_Window_ZZ::on_paint);

	button_quit = new CL_Button(CL_Rect(window->get_width() - 12, 3, window->get_width(), 15), "X", window, style);

    title_font = new CL_Font("Window/font", resources);

    rc_title = CL_Rect(0, 0,title_font->get_width(window->get_title()) + 16,
        title_font->get_height(window->get_title()) + 10);

    move_handler = new CL_ComponentMoveHandler(rc_title, window);
    rc_main = CL_Rect(12, 12, window->get_width(), window->get_height());

	slot_quit = button_quit->sig_clicked().connect(this, &CL_Window_ZZ::on_quit);
}

CL_Window_ZZ::~CL_Window_ZZ()
{
    delete title_font;
    delete move_handler;
	delete button_quit;
}

void CL_Window_ZZ::on_paint()
{
    rc_title = CL_Rect(0, 0,title_font->get_width(window->get_title()) + 16,
        title_font->get_height(window->get_title()) + 10);

    rc_main = CL_Rect(12, 12, window->get_width(), window->get_height());

	if (first_paint)
    {
        button_quit->set_position(CL_Rect(window->get_width() - 12,3,
            window->get_width(),15));
        
        delete move_handler;
        move_handler = new CL_ComponentMoveHandler(rc_title, window);

        window->get_client_area()->set_position(CL_Rect(rc_main.left, rc_main.top,
            window->get_width(), window->get_height()));

        first_paint = false;
    }

	int width = window->get_width();
	int height = window->get_height();

	// Fill
    //-------
    CL_Color clr_background(100, 120, 100, 180);
    style->fill_rounded_rect(window->client_to_screen(rc_main), clr_background);

    // Outline
    CL_Color clr_outline(175, 205, 148);
    style->draw_rounded_rect(window->client_to_screen(rc_main), clr_outline);

    
    CL_Color clr_title_bgnd(93, 156, 39);
    // title
    style->fill_rounded_rect(window->client_to_screen(rc_title), clr_title_bgnd);
    style->draw_rounded_rect(window->client_to_screen(rc_title), clr_outline);

    // draw title text
	CL_Point title_coord = window->client_to_screen(CL_Point(rc_title.left + 8, rc_title.top + 6));
    title_font->draw(title_coord.x, title_coord.y, window->get_title());

}

void CL_Window_ZZ::on_quit()
{
	if (window->is_modal())
		window->quit();
	else
        window->close();
}
