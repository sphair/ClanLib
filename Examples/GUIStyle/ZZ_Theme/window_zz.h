
#ifndef header_window_style_zz
#define header_window_style_zz

#if _MSC_VER > 1000
#pragma once
#endif

#include <ClanLib/gui.h>

class CL_Window;
class CL_ComponentOptions;
class CL_StyleManager_ZZ;
class CL_ResourceManager;
class CL_ComponentMoveHandler;

class CL_Window_ZZ : public CL_ComponentStyle
{
public:
	CL_Window_ZZ(CL_Window *window,	CL_StyleManager_ZZ *style);

	virtual ~CL_Window_ZZ();

private:
	void on_paint();
	void on_quit();

	CL_Slot slot_paint;
	CL_Slot slot_quit;

	CL_Button *button_quit;
	CL_Window *window;
	CL_ComponentMoveHandler * move_handler;
	CL_ResourceManager *resources;
	CL_StyleManager_ZZ *style;
    CL_Font * title_font;

    CL_Rect rc_title;
    CL_Rect rc_main;
    bool first_paint;
};

#endif
