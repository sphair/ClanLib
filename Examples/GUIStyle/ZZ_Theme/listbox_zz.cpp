#include <ClanLib/core.h> // for CL_DomElement
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "listbox_zz.h"
#include "stylemanager_zz.h"
#include "scrollbar_zz.h"

CL_ListBox_ZZ::CL_ListBox_ZZ(
	CL_ListBox * listbox,
	CL_StyleManager_ZZ * style)
:
	CL_ComponentStyle(listbox),
	listbox_(listbox),
	style_(style),
	resources_(style->get_resources()),
    first_paint_(true),
    listbox_type_(type_normal)
{
	font_ = new CL_Font("ListBox/font", resources_);
    font_->set_color(152.0f / 255, 211.0f / 255, 101.0f / 255);

	// Create a vertical scrollbar
	scrollbar_ = new CL_ScrollBar(false, listbox);
    {
        CL_ScrollBar_ZZ * sb_type = (CL_ScrollBar_ZZ*)scrollbar_->get_style();
        if (sb_type)
            sb_type->set_type(CL_ScrollBar_ZZ::type_listbox);
    }
	visible_scrollbar_ = true;

	int font_height = font_->get_height();
	listbox_->set_item_height(font_height);

	CL_Component *client_area = listbox_->get_client_area();

	slots_.connect(listbox_->sig_set_options(), this, &CL_ListBox_ZZ::on_set_options);
	slots_.connect(listbox_->sig_paint(), this, &CL_ListBox_ZZ::on_paint_background);
	slots_.connect(client_area->sig_mouse_down(), this, &CL_ListBox_ZZ::on_mouse_down);
	slots_.connect(client_area->sig_paint(), this, &CL_ListBox_ZZ::on_paint_listbox);
	slots_.connect(listbox_->sig_resize(), this, &CL_ListBox_ZZ::on_resize);
	slots_.connect(listbox_->sig_item_added(), this, &CL_ListBox_ZZ::on_item_added);
	slots_.connect(listbox_->sig_item_removed(), this, &CL_ListBox_ZZ::on_item_removed);
	slots_.connect(listbox_->sig_clear(), this, &CL_ListBox_ZZ::on_clear);
	slots_.connect(scrollbar_->sig_value_changed(), this, &CL_ListBox_ZZ::on_scroll_change);

	on_resize(0,0);
}

CL_ListBox_ZZ::~CL_ListBox_ZZ()
{
    delete font_;
    delete scrollbar_;
}

void CL_ListBox_ZZ::on_paint_background()
{
    if (first_paint_)
    {
        if (listbox_->get_count() > 0)
        {
            if (listbox_->get_current_item() == -1)
            {
                listbox_->set_current_item(0);
            }
            listbox_->insert_item("hack");
            listbox_->remove_item(listbox_->get_count() - 1);
        }
        first_paint_ = false;
    }


	int width = listbox_->get_width();
	int height = listbox_->get_height();

    // main
    CL_Color const clr_background(0, 40, 0, 60);
    CL_Rect rc_main = listbox_->get_screen_rect();
    style_->fill_rounded_rect(rc_main, clr_background);

    // Outline
    CL_Color clr_outline(175, 205, 148);
    CL_Rect rc_outline = rc_main;
    style_->draw_rounded_rect(rc_outline, clr_outline);
}

void CL_ListBox_ZZ::on_paint_listbox()
{
	int width = listbox_->get_client_area()->get_width();
	int height = listbox_->get_client_area()->get_height();

	int offset_x = listbox_->get_client_area()->get_screen_x();
	int offset_y = listbox_->get_client_area()->get_screen_y();

    if (listbox_type_ == type_normal)
    {
		CL_Display::push_cliprect(listbox_->get_client_area()->get_screen_rect());
		int pos = 0;
		int offset = listbox_->get_top_item();
        std::vector<CL_ListItem *> items = listbox_->get_items();
        std::vector<CL_ListItem *>::iterator it;

        font_->set_color(152.0f / 255, 211.0f / 255, 101.0f / 255);
        for (it = items.begin(); it != items.end(); ++it, ++pos)
        {
            if (pos < offset)
                continue;
            
            if (pos == listbox_->get_current_item())
            {
                CL_Display::fill_rect(
                    CL_Rect(offset_x, offset_y + (pos - offset) * font_->get_height(),
                    offset_x + width, offset_y + (pos + 1 - offset) * font_->get_height()),
                    CL_Color(93, 156, 39));
                font_->set_color(255.0f / 255, 255.0f / 255, 255.0f / 255);
                font_->draw(
                    offset_x + 4,
                    offset_y + (pos - offset) * font_->get_height(),
                    (*it)->str);
                font_->set_color(152.0f / 255, 211.0f / 255, 101.0f / 255);
            }
            else
            {
				font_->draw(
					offset_x + 4,
					offset_y + (pos - offset) * font_->get_height(),
					(*it)->str);                    
            }
            
            if((pos - offset) * font_->get_height() >= height)
                break;
        }
		CL_Display::pop_cliprect();
    }
    else
    if (listbox_type_ == type_levels_list)
    {
		CL_Display::push_cliprect(listbox_->get_client_area()->get_screen_rect());
		int pos = 0;
		int offset = listbox_->get_top_item();
        std::vector<CL_ListItem *> items = listbox_->get_items();
        std::vector<CL_ListItem *>::iterator it;

        for (it = items.begin(); it != items.end(); ++it, ++pos)
        {
            if (pos < offset)
                continue;

			CL_ListBoxItem_ZZ * item = (CL_ListBoxItem_ZZ *)(*it);
			std::string text;
			CL_Color text_color;
			if (item->get_solved())
			{
				text = "(+) " + item->get_levelname();
				text_color = CL_Color(211, 211, 101);
			}
			else
			{
				text = "(-) " + item->get_levelname();
				text_color = CL_Color(152, 211, 101);
			}
            
            if (pos == listbox_->get_current_item())
            {
                CL_Display::fill_rect(
                    CL_Rect(offset_x, offset_y + (pos - offset) * font_->get_height(),
                    offset_x + width, offset_y + (pos + 1 - offset) * font_->get_height()),
                    CL_Color(93, 156, 39));
                font_->set_color(255.0f / 255, 255.0f / 255, 255.0f / 255);
                font_->draw(
                    offset_x + 4,
                    offset_y + (pos - offset) * font_->get_height(),
                    text);
            }
            else
            {
				font_->set_color(text_color);
				font_->draw(
					offset_x + 4,
					offset_y + (pos - offset) * font_->get_height(),
					text);
            }
            
            if((pos - offset) * font_->get_height() >= height)
                break;
        }
		CL_Display::pop_cliprect();
    }
}

void CL_ListBox_ZZ::on_item_added(int index)
{
	update_scrollbar(true);
}

void CL_ListBox_ZZ::on_item_removed(int index)
{
	update_scrollbar(true);
}

void CL_ListBox_ZZ::on_clear()
{
	update_scrollbar(true);
}

void CL_ListBox_ZZ::on_set_options(const CL_DomElement &options)
{
	if (options.get_attribute("type") == "levels_list")
	{
		listbox_type_ = type_levels_list;
	}
}

void CL_ListBox_ZZ::on_scroll_change(int new_offset)
{
	listbox_->set_top_item(new_offset);
}

void CL_ListBox_ZZ::on_mouse_down(const CL_InputEvent &key)
{
	int height_of_all_items = listbox_->get_item_height() * listbox_->get_count();
	int height_of_client_area = listbox_->get_client_area()->get_height();

	if (	key.id == CL_MOUSE_WHEEL_UP
		&&	height_of_all_items > height_of_client_area )
	{
		listbox_->set_top_item(listbox_->get_top_item() - 5);
		update_scrollbar(true);
		return;
	}
	if (	key.id == CL_MOUSE_WHEEL_DOWN
		&&	height_of_all_items > height_of_client_area )
	{
		listbox_->set_top_item(listbox_->get_top_item() + 5);
		update_scrollbar(true);
		return;
	}
}

void CL_ListBox_ZZ::on_resize(int old_width, int old_height)
{
	// Position the listbox item-box
	CL_Rect rect(4, 4, 4 + listbox_->get_width() - 8 , 4 + listbox_->get_height() - 8);
	listbox_->get_client_area()->set_position(rect);

	update_scrollbar(false);

	// Scrollbar was added, shrink clientarea
	if(visible_scrollbar_)
	{
		listbox_->get_client_area()->set_width(listbox_->get_client_area()->get_width() - 16);

		// Position the scrollbar
		rect = CL_Rect(listbox_->get_width() - 19, 0, listbox_->get_width(), listbox_->get_height());
		scrollbar_->set_position(rect);
	}
}


/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CL_ListBox_ZZ::update_scrollbar(bool resize)
{	
	int max_visible_items = (listbox_->get_client_area()->get_height() - 4) / listbox_->get_item_height();

	if(listbox_->get_count() > max_visible_items)
	{
		if(visible_scrollbar_ == false)
		{
			visible_scrollbar_ = true;
			listbox_->add_child(scrollbar_, false);

			if(resize)
				on_resize(0,0);
		}

		scrollbar_->set_max_value(listbox_->get_count() - max_visible_items);
		scrollbar_->set_value(listbox_->get_top_item());
	}
	else
	{
		if(visible_scrollbar_)
		{
			visible_scrollbar_ = false;
			scrollbar_->set_value(0);
			listbox_->remove_child(scrollbar_);

			if(resize)
				on_resize(0,0);
		}
	}
}
