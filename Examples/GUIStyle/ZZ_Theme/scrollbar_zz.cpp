#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "scrollbar_zz.h"
#include "stylemanager_zz.h"
#include "button_zz.h"

CL_ScrollBar_ZZ::CL_ScrollBar_ZZ(
	CL_ScrollBar * scrollbar,
	CL_StyleManager_ZZ * style,
    ScrollBarType scrollbar_type)
:
	CL_ComponentStyle(scrollbar),
	timer_scroll_(250),
	scrollbar_(scrollbar),
    initialized_(false),
    style_(style),
    resources_(style->get_resources()),
    scrollbar_type_(scrollbar_type)
{
	CL_Rect rect_decrease;
	CL_Rect rect_increase;

	CL_Component *client_area = scrollbar_->get_client_area();

    if (scrollbar_type_ == type_normal)
    {
        if (scrollbar_->is_vertical())
	    {
		    int button_size = scrollbar_->get_width();
		    rect_decrease = CL_Rect(0, 0, button_size, button_size);
		    rect_increase = CL_Rect(0, scrollbar_->get_height() - button_size, button_size, scrollbar_->get_height());
	    }
	    else
	    {
		    int button_size = scrollbar_->get_height();
		    rect_decrease = CL_Rect(0, 0, button_size, button_size);
		    rect_increase = CL_Rect(scrollbar_->get_width() - button_size, 0, scrollbar_->get_width(), button_size);
	    }
        button_increase_ = new CL_Button(rect_increase, "", scrollbar, style);
        button_decrease_ = new CL_Button(rect_decrease, "", scrollbar, style);
    }
    else
    {
        // TODO: add horizontal
        button_increase_ = new CL_Button(scrollbar_, style_);
        CL_Button_ZZ * btn_style = (CL_Button_ZZ*)button_increase_->get_style();
        if (btn_style)
        {
            btn_style->set_surfaces(
                new CL_Surface("ListBox/scroll_btn_down", resources_),
                new CL_Surface("ListBox/scroll_btn_down", resources_),
                new CL_Surface("ListBox/scroll_btn_down", resources_),
                new CL_Surface("ListBox/scroll_btn_down", resources_));
            btn_style->set_button_type(CL_Button_ZZ::type_bitmap);
        }
        button_decrease_ = new CL_Button(scrollbar_, style_);
        btn_style = (CL_Button_ZZ*)button_decrease_->get_style();
        if (btn_style)
        {
            btn_style->set_surfaces(
                new CL_Surface("ListBox/scroll_btn_up", resources_),
                new CL_Surface("ListBox/scroll_btn_up", resources_),
                new CL_Surface("ListBox/scroll_btn_up", resources_),
                new CL_Surface("ListBox/scroll_btn_up", resources_));
            btn_style->set_button_type(CL_Button_ZZ::type_bitmap);
        }
    }

	slots_.connect(client_area->sig_paint(), this, &CL_ScrollBar_ZZ::on_paint);
	slots_.connect(button_increase_->sig_pressed(), this, &CL_ScrollBar_ZZ::start_scroll, 1);
	slots_.connect(button_decrease_->sig_pressed(), this, &CL_ScrollBar_ZZ::start_scroll, -1);
	slots_.connect(button_increase_->sig_released(), this, &CL_ScrollBar_ZZ::stop_scroll);
	slots_.connect(button_decrease_->sig_released(),	this, &CL_ScrollBar_ZZ::stop_scroll);
	slots_.connect(scrollbar_->sig_get_preferred_size(), this, &CL_ScrollBar_ZZ::on_get_preferred_size);
    slots_.connect(timer_scroll_.sig_timer(), this, &CL_ScrollBar_ZZ::on_timer_scroll);
}

CL_ScrollBar_ZZ::~CL_ScrollBar_ZZ()
{
    delete button_increase_;
    delete button_decrease_;
}

void CL_ScrollBar_ZZ::start_scroll(int delta)
{
	scroll_delta_ = delta;
	timer_scroll_.enable();
	on_timer_scroll();
}

void CL_ScrollBar_ZZ::stop_scroll()
{
	timer_scroll_.disable();
}

void CL_ScrollBar_ZZ::on_timer_scroll()
{
	scrollbar_->set_value(scrollbar_->get_value() + scroll_delta_);
}

void CL_ScrollBar_ZZ::on_paint()
{
    if (!initialized_)
    {
        CL_Rect rect_decrease;
        CL_Rect rect_increase;
        
        CL_Component *client_area = scrollbar_->get_client_area();
        
		if (scrollbar_->is_vertical())
		{
			int button_size = scrollbar_->get_width();
			rect_decrease = CL_Rect(0, 0, button_size, button_size);
			rect_increase = CL_Rect(0, scrollbar_->get_height() - button_size, button_size, scrollbar_->get_height());
		}
		else
		{
			int button_size = scrollbar_->get_height();
			rect_decrease = CL_Rect(0, 0, button_size, button_size);
			rect_increase = CL_Rect(scrollbar_->get_width() - button_size, 0, scrollbar_->get_width(), button_size);
		}

        CL_Rect client_area_rc = CL_Rect(0, 0, scrollbar_->get_position().get_width() - 1,
            scrollbar_->get_position().get_height());

        button_increase_->set_position(rect_increase);
        button_decrease_->set_position(rect_decrease);
        
        if (scrollbar_->is_vertical())
        {
            client_area_rc.top += rect_decrease.get_height();
            client_area_rc.bottom -= rect_increase.get_height();
        }
        else
        {
            client_area_rc.left += rect_decrease.get_width();
            client_area_rc.right -= rect_increase.get_width();
        }

        client_area->set_position(client_area_rc);
        scrollbar_->set_max_value(scrollbar_->get_max_value());

		initialized_ = true;
    }
    
    int width = scrollbar_->get_width();
	int height = scrollbar_->get_height();

	CL_Rect rect = scrollbar_->get_slider_rect();

	// Background
	CL_Display::fill_rect(
		scrollbar_->client_to_screen(CL_Rect(0,0, width, height)),
		CL_Color(42, 97, 29));

	// Slider

    CL_Color const clr_pup_1(72, 139, 60);
    CL_Color const clr_pup_2(49, 111, 36);
    CL_Color const clr_pup_3(34, 96, 21);

    CL_Color const clr_slider_bgnd_norm(60, 120, 48);
    CL_Color const clr_slider_bgnd_drag(60, 120, 48);

	if(scrollbar_->is_dragging_slider())
	{
		CL_Display::fill_rect(
			scrollbar_->get_client_area()->client_to_screen(rect),
			clr_slider_bgnd_drag);
	}
	else
	{
		CL_Display::fill_rect(
			scrollbar_->get_client_area()->client_to_screen(rect),
            clr_slider_bgnd_norm);
	}

    CL_Color const clr_high_sld(102, 155, 87);
    CL_Color const clr_low_sld(33, 83, 22);

}

void CL_ScrollBar_ZZ::on_get_preferred_size(CL_Size &size)
{
}

void CL_ScrollBar_ZZ::set_type(ScrollBarType scrollbar_type)
{
    if (scrollbar_type_ != scrollbar_type)
    {
        if (scrollbar_type == type_listbox)
        {
            // TODO: add horizontal
            CL_Button_ZZ * btn_style = (CL_Button_ZZ*)button_increase_->get_style();
            if (btn_style)
            {
                btn_style->set_surfaces(
                    new CL_Surface("ListBox/scroll_btn_down", resources_),
                    new CL_Surface("ListBox/scroll_btn_down", resources_),
                    new CL_Surface("ListBox/scroll_btn_down", resources_),
                    new CL_Surface("ListBox/scroll_btn_down", resources_));
                btn_style->set_button_type(CL_Button_ZZ::type_bitmap);
            }
            btn_style = (CL_Button_ZZ*)button_decrease_->get_style();
            if (btn_style)
            {
                btn_style->set_surfaces(
                    new CL_Surface("ListBox/scroll_btn_up", resources_),
                    new CL_Surface("ListBox/scroll_btn_up", resources_),
                    new CL_Surface("ListBox/scroll_btn_up", resources_),
                    new CL_Surface("ListBox/scroll_btn_up", resources_));
                btn_style->set_button_type(CL_Button_ZZ::type_bitmap);
            }
        }
        else
            if (scrollbar_type_ == type_normal)
            {
                CL_Rect rect_decrease;
                CL_Rect rect_increase;
                if (scrollbar_type_ == type_normal)
                {
                    if (scrollbar_->is_vertical())
                    {
                        int button_size = scrollbar_->get_width();
                        rect_decrease = CL_Rect(0, 0, button_size, button_size);
                        rect_increase = CL_Rect(0, scrollbar_->get_height() - button_size, button_size, scrollbar_->get_height());
                    }
                    else
                    {
                        int button_size = scrollbar_->get_height();
                        rect_decrease = CL_Rect(0, 0, button_size, button_size);
                        rect_increase = CL_Rect(scrollbar_->get_width() - button_size, 0, scrollbar_->get_width(), button_size);
                    }
                    CL_Button_ZZ * btn_style = (CL_Button_ZZ*)button_decrease_->get_style();
                    if (btn_style)
                        btn_style->set_button_type(CL_Button_ZZ::type_text);
                    btn_style = (CL_Button_ZZ*)button_increase_->get_style();
                    if (btn_style)
                        btn_style->set_button_type(CL_Button_ZZ::type_text);
                }
            }
    }
}
