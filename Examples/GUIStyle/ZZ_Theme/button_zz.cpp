#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "button_zz.h"
#include "stylemanager_zz.h"
#include <ClanLib/gui.h>

#pragma warning ( push, 1 )
#pragma warning ( disable: 4244 ) // conversion from 'int' to 'float', possible loss of data


CL_Button_ZZ::CL_Button_ZZ(
	CL_Button *button,
	CL_StyleManager_ZZ *style,
    ButtonType button_type,
    ButtonSize button_size)
:
	CL_ComponentStyle(button),
    button_type_(button_type),
    button_size_(button_size),
	style_(style),
	button_(button),
	timer_(10),
    surface_up_(0),
    surface_down_(0),
    surface_highlighted_(0),
    surface_disabled_(0),
    down_(false),
    highlighted_(false),
    resources_(style->get_resources())
{
   	font_ = new CL_Font("Button/font", resources_);

    CL_Size text_size = font_->get_size(button_->get_text().c_str());
	{
		slots_.connect(button_->sig_mouse_enter(),
			this, &CL_Button_ZZ::on_mouse_enter);
		slots_.connect(button_->sig_mouse_leave(),
			this, &CL_Button_ZZ::on_mouse_leave);
		slots_.connect(timer_.sig_timer(),
			this, &CL_Button_ZZ::on_timer);
	    slots_.connect(button_->sig_get_preferred_size(),
		    this, &CL_Button_ZZ::on_get_preferred_size);
    }

	timer_.disable();
    highlight_modifier_ = 200;
    clr_font_.set_color(
        highlight_modifier_,
        highlight_modifier_,
        highlight_modifier_);

    slots_.connect(button_->sig_paint(), this, &CL_Button_ZZ::on_paint);
	slots_.connect(button_->sig_pressed(), this, &CL_Button_ZZ::on_pressed);
	slots_.connect(button_->sig_released(), this, &CL_Button_ZZ::on_released);
	slots_.connect(button_->sig_set_options(), this, &CL_Button_ZZ::on_set_options);

	button->find_preferred_size();
}

CL_Button_ZZ::~CL_Button_ZZ()
{
    delete font_;
    delete surface_disabled_;
    delete surface_up_;
    delete surface_down_;
    delete surface_highlighted_;
}

void CL_Button_ZZ::on_get_preferred_size(CL_Size &size)
{
    if (button_type_ == type_bitmap)
    {
        if (surface_up_)
        {
            size.width = surface_up_->get_width();
            size.height = surface_up_->get_height();
        }
    }
    else
    {
		size.width = font_->get_width(button_->get_text()) + 4;
		size.height = font_->get_height(button_->get_text()) + 4;
    }    
}

void CL_Button_ZZ::on_mouse_enter()
{
	timer_.enable();
	highlighted_ = true;

    highlighting_direction_ = true;
}

void CL_Button_ZZ::on_mouse_leave()
{
	timer_.enable();
	highlighted_ = false;
    highlighting_direction_ = false;
}

void CL_Button_ZZ::on_timer()
{
    if (highlighting_direction_)
    {
        highlight_modifier_ += 2;
        if (highlight_modifier_ > 255)
        {
            highlight_modifier_ = 255;
            timer_.disable();
        }
    }
    else
    {
        highlight_modifier_ -= 2;
        if (highlight_modifier_ < 200)
        {
            highlight_modifier_ = 200;
            timer_.disable();
        }
    }
    clr_font_.set_color(
        highlight_modifier_,
        highlight_modifier_,
        highlight_modifier_);
}

void CL_Button_ZZ::on_paint()
{
	if(button_type_ == type_text)
		on_paint_anim();
    else
        if (button_type_ == type_bitmap)
            on_paint_texture();
        else
            cl_throw_error("CL_Button_ZZ::on_paint, Unknown button type " +
                CL_String::from_int(button_type_));
}

void CL_Button_ZZ::on_paint_texture()
{
    CL_Surface * surface = 0;
	if(down_)
	{
		surface = surface_down_;
	}
	else
        if (highlighted_)
        {
            surface = surface_highlighted_;
        }
        else
            if (!button_->is_enabled())
            {
                surface = surface_disabled_;
            }
            else
                surface = surface_up_;

    if (surface)
        surface->draw(button_->get_screen_x(), button_->get_screen_y());
    else
        if (surface_up_)
            surface_up_->draw(button_->get_screen_x(), button_->get_screen_y());
}

void CL_Button_ZZ::on_paint_anim()
{
    int text_width = font_->get_width(button_->get_text());
	int button_width = button_->get_width();
	int text_height = font_->get_height();
	int button_height = button_->get_height();

	int x_pos = button_->get_screen_x() + (button_width - text_width) / 2;
	int y_pos = button_->get_screen_y() + (button_height - text_height) / 2;

    bool being_clicked = button_->has_mouse_over() ? down_ : false;
	if((button_->is_down() && !being_clicked) || (!button_->is_down() && being_clicked))
	{
            font_->set_color(
                255 / 255.0f,
                255 / 255.0f,
                255 / 255.0f);
            x_pos += 1;
            y_pos += 2;
	}
	else
	{
		if(highlighted_)
		{
		}
        font_->set_color(
            clr_font_.get_red() / 255.0f,
            clr_font_.get_green()  / 255.0f,
            clr_font_.get_blue()  / 255.0f);
	}
	font_->draw(x_pos, y_pos, button_->get_text());
}

void CL_Button_ZZ::on_pressed()
{
	down_ = true;
}

void CL_Button_ZZ::on_released()
{
	down_ = false;
}

void CL_Button_ZZ::set_button_type(ButtonType const button_type)
{
    button_type_ = button_type;
	button_->find_preferred_size();
}

void CL_Button_ZZ::set_button_size(ButtonSize const button_size)
{
    if (button_type_ == type_text)
    {
        if (button_size_ != button_size)
        {
            if (button_size_ == size_big)
            {
                delete font_;
                font_ = new CL_Font("Button/font_big", resources_);
            }
            else
            {
                delete font_;
                font_ = new CL_Font("Button/font", resources_);
            }
            button_size_ = button_size;

			button_->find_preferred_size();
        }
    }
    else
        button_size_ = button_size;
}

void CL_Button_ZZ::set_surface_up(CL_Surface * surface_up)
{
    delete surface_up_;
    surface_up_ = surface_up;

    if (button_type_ == type_bitmap)
		button_->find_preferred_size();
}

void CL_Button_ZZ::set_surface_down(CL_Surface * surface_down)
{
    delete surface_down_;
    surface_down_ = surface_down;
}

void CL_Button_ZZ::set_surface_highlighted(CL_Surface * surface_highlighted)
{
    delete surface_highlighted_;
    surface_highlighted_ = surface_highlighted;
}

void CL_Button_ZZ::set_surface_disabled(CL_Surface * surface_disabled)
{
    delete surface_disabled_;
    surface_disabled_ = surface_disabled;
}

void CL_Button_ZZ::set_surfaces(CL_Surface * surface_up,
                                CL_Surface * surface_down,
                                CL_Surface * surface_highlighted,
                                CL_Surface * surface_disabled)
{
    delete surface_up_;
    surface_up_ = surface_up;
    delete surface_down_;
    surface_down_ = surface_down;
    delete surface_highlighted_;
    surface_highlighted_ = surface_highlighted;
    delete surface_disabled_;
    surface_disabled_ = surface_disabled;

    if (button_type_ == type_bitmap)
		button_->find_preferred_size();
}

void CL_Button_ZZ::set_font(CL_Font * font)
{
    if (font)
    {
        delete font_;
        font_ = font;

        if (button_type_ == type_text)
			button_->find_preferred_size();
    }
}

void CL_Button_ZZ::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("surface_up"))
		set_surface_up(new CL_Surface(options.get_attribute("surface_up"), resources_));
	
	if (options.has_attribute("surface_down"))
		set_surface_down(new CL_Surface(options.get_attribute("surface_down"), resources_));
	
	if (options.has_attribute("surface_disabled"))
		set_surface_disabled(new CL_Surface(options.get_attribute("surface_disabled"), resources_));
	
	if (options.has_attribute("surface_highlighted"))
		set_surface_highlighted(new CL_Surface(options.get_attribute("surface_highlighted"), resources_));
	
	if (options.has_attribute("font"))
		set_font(new CL_Font(options.get_attribute("font"), resources_));
	
	std::string const btn_type(options.get_attribute("type"));
	if (btn_type == "bitmap")
		set_button_type(type_bitmap);
	else
		set_button_type(type_text);

	std::string const btn_size = options.get_attribute("size");
	if (btn_size == "big")
		set_button_size(size_big);
	else
		set_button_size(size_normal);

    // Calculate type of button, if not setuped
    if (!options.has_attribute("text"))
    {
        if (surface_up_)
            set_button_type(type_bitmap);
    }

	if(options.has_attribute("font"))
        set_font(new CL_Font(options.get_attribute("font"), resources_));
    else
    {
        if (button_type_ == type_text)
        {
            if (button_size_ == size_big)
            {
                delete font_;
                font_ = new CL_Font("Button/font_big", resources_);
            }
            else
            {
                delete font_;
                font_ = new CL_Font("Button/font", resources_);
            }
        }
    }
}

#pragma warning ( pop )
