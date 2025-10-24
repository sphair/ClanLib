
#ifndef header_button_style_zz
#define header_button_style_zz

#if _MSC_VER > 1000
#pragma once
#endif

#include <ClanLib/gui.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>

class CL_Button;
class CL_StyleManager_ZZ;
class CL_ResourceManager;
class CL_Font;
class CL_DomElement;

class CL_Button_ZZ : public CL_ComponentStyle
{
public:
    enum ButtonType {
        type_text,
        type_bitmap
    };
    enum ButtonSize {
        size_normal,
        size_big
    };
public:
	CL_Button_ZZ(
		CL_Button *button,
		CL_StyleManager_ZZ *style, ButtonType btn_type = type_text,
            ButtonSize = size_normal);

	virtual ~CL_Button_ZZ();
public:
    void set_button_type(ButtonType const button_type);
    void set_button_size(ButtonSize const button_size);

    void set_font(CL_Font * font);
    void set_surface_up(CL_Surface * surface);
    void set_surface_down(CL_Surface * surface);
    void set_surface_highlighted(CL_Surface * surface);
    void set_surface_disabled(CL_Surface * surface);
    void set_surfaces(CL_Surface * surface_up,
        CL_Surface * surface_down,        
        CL_Surface * surface_highlighted,
        CL_Surface * surface_disabled);
private:
	void on_paint();
	void on_paint_texture();
	void on_paint_anim();

	void on_mouse_enter();
	void on_mouse_leave();
	void on_pressed();
	void on_released();
	void on_set_options(const CL_DomElement &options);

    void on_get_preferred_size(CL_Size &size);
	void on_timer();

private:
    CL_Button *button_;
    ButtonType button_type_;
    ButtonSize button_size_;
	
    // Button state
	bool highlighted_;
    bool down_;

    // Highlight relaited
    int highlight_modifier_;
    bool highlighting_direction_;

	CL_Timer timer_;
    CL_Color clr_font_;

	CL_StyleManager_ZZ *style_;
	CL_ResourceManager *resources_;

	CL_Font *font_;

	CL_Surface *surface_up_;
	CL_Surface *surface_down_;
	CL_Surface *surface_highlighted_;
	CL_Surface *surface_disabled_;

    CL_SlotContainer slots_;
};

#endif
