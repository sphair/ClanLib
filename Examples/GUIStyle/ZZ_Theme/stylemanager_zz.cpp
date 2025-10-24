#include <ClanLib/gl.h>

#include "stylemanager_zz.h"
#include "button_zz.h"
#include "checkbox_zz.h"
#include "frame_zz.h"
#include "inputbox_zz.h"
#include "window_zz.h"
#include "listbox_zz.h"
#include "scrollbar_zz.h"
#include "label_zz.h"

#pragma warning ( push, 1 )
#pragma warning ( disable: 4244 ) // conversion from 'int' to 'float', possible loss of data


// Construction:

CL_StyleManager_ZZ::CL_StyleManager_ZZ(CL_ResourceManager *resources)
: CL_StyleManager(resources)
{
}

CL_StyleManager_ZZ::~CL_StyleManager_ZZ()
{
}

// Attributes:

// Operations:

void CL_StyleManager_ZZ::fill_rect(
	int x1, int y1, int x2, int y2,
	float r, float g, float b,
	float a0, float a1, float a2, float a3)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glColor4f(r,g,b,a0);
		glVertex2i(x1, y1);
		glColor4f(r,g,b,a1);
		glVertex2i(x2, y1);
		glColor4f(r,g,b,a2);
		glVertex2i(x2, y2);
		glColor4f(r,g,b,a3);
		glVertex2i(x1, y2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void CL_StyleManager_ZZ::fill_rect(
	int x1, int y1, int x2, int y2,
	float r0, float g0, float b0, 
	float r1, float g1, float b1, 
	float r2, float g2, float b2, 
	float r3, float g3, float b3,
	float a)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glColor4f(r0,g0,b0,a);
		glVertex2i(x1, y1);
		glColor4f(r1,g1,b1,a);
		glVertex2i(x2, y1);
		glColor4f(r2,g2,b2,a);
		glVertex2i(x2, y2);
		glColor4f(r3,g3,b3,a);
		glVertex2i(x1, y2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

// Overridables:

void CL_StyleManager_ZZ::connect_styles(
	const std::string &type,
	CL_Component *owner)
{
	if (type == "button")
	{
		CL_Button *button = (CL_Button *) owner;
        button->set_style(new CL_Button_ZZ(button, this));
	}
	else if (type == "window")
	{
		CL_Window *window = (CL_Window *) owner;
		window->set_style(new CL_Window_ZZ(window, this));
	}
	else if (type == "frame")
	{
		CL_Frame *frame = (CL_Frame *) owner;
		frame->set_style(new CL_Frame_ZZ(frame, this));
	}
	else if (type == "inputbox")
	{
		CL_InputBox *inputbox = (CL_InputBox *) owner;
		inputbox->set_style(new CL_InputBox_ZZ(inputbox, this));
	}
	else if (type == "listbox")
	{
		CL_ListBox *listbox = (CL_ListBox *) owner;
		listbox->set_style(new CL_ListBox_ZZ(listbox, this));
	}
	else if (type == "scrollbar")
	{
		CL_ScrollBar *scrollbar = (CL_ScrollBar *) owner;
		scrollbar->set_style(new CL_ScrollBar_ZZ(scrollbar, this));
	}
	else if (type == "checkbox")
	{
		CL_CheckBox *checkbox = (CL_CheckBox *) owner;
		checkbox->set_style(new CL_CheckBox_ZZ(checkbox, this));
	}
	else if (type == "label")
	{
		CL_Label *label = (CL_Label *) owner;
		label->set_style(new CL_Label_ZZ(label, this));
	}
//	else
//	{
//		CL_StyleManager::connect_styles(type, owner);
//	}
}

void CL_StyleManager_ZZ::draw_rounded_rect(CL_Rect const & rc, CL_Color const & clr)
{
    CL_Display::draw_line(rc.left + 6, rc.top, rc.right - 6, rc.top, clr);
    CL_Display::draw_line(rc.left + 6, rc.bottom - 1, rc.right - 6, rc.bottom - 1, clr);
    
    CL_Display::draw_line(rc.left + 0, rc.top + 6, rc.left, rc.bottom - 6, clr);
    CL_Display::draw_line(rc.right - 1, rc.top + 6, rc.right - 1, rc.bottom - 6, clr);
    
    // left
    {
        CL_Display::draw_line(rc.left + 1, rc.top + 6, rc.left + 1, rc.top + 4, clr);
        CL_Display::draw_pixel(rc.left + 2, rc.top + 3, clr);
        CL_Display::draw_pixel(rc.left + 3, rc.top + 2, clr);
        CL_Display::draw_line(rc.left + 4, rc.top + 1, rc.left + 6, rc.top + 1, clr);
        
        
        CL_Display::draw_line(rc.left + 1, rc.bottom - 1 - 5, rc.left + 1, rc.bottom - 1 - 3, clr);                              
        CL_Display::draw_pixel(rc.left + 2, rc.bottom - 1 - 3, clr);
        CL_Display::draw_pixel(rc.left + 3, rc.bottom - 1 - 2, clr);
        CL_Display::draw_line(rc.left + 4, rc.bottom - 1 - 1, rc.left + 6, rc.bottom - 1 - 1, clr);
    }
    // right
    {
        CL_Display::draw_line(rc.right - 1 - 1, rc.top + 4, rc.right - 1 - 1, rc.top + 6, clr);
        CL_Display::draw_pixel(rc.right - 1 - 2, rc.top + 3, clr);
        CL_Display::draw_pixel(rc.right - 1 - 3, rc.top + 2, clr);
        CL_Display::draw_line(rc.right - 1 - 3, rc.top + 1, rc.right - 1 - 5, rc.top + 1, clr);
        
        
        CL_Display::draw_line(rc.right - 1 - 1, rc.bottom - 1 - 5, rc.right - 1 - 1, rc.bottom - 1 - 3, clr);
        CL_Display::draw_pixel(rc.right - 1 - 2, rc.bottom - 1 - 3, clr);
        CL_Display::draw_pixel(rc.right - 1 - 3, rc.bottom - 1 - 2, clr);
        CL_Display::draw_line(rc.right - 1 - 3, rc.bottom - 1 - 1, rc.right - 1 - 5, rc.bottom - 1 - 1, clr);
    }
}

void CL_StyleManager_ZZ::fill_rounded_rect(CL_Rect const & rc, CL_Color const & clr)
{
    // main
	CL_Display::fill_rect(CL_Rect(rc.left + 6, rc.top, rc.right - 6, rc.bottom - 1), clr);

    // left
    {
        CL_Display::draw_line(rc.left, rc.top + 6, rc.left, rc.bottom - 6 - 1, clr);
        CL_Display::draw_line(rc.left + 1, rc.top + 6 - 2, rc.left + 1, rc.bottom - 6 - 1 + 2, clr);
        CL_Display::draw_line(rc.left + 2, rc.top + 6 - 3, rc.left + 2, rc.bottom - 6 - 1 + 3, clr);
        CL_Display::draw_line(rc.left + 3, rc.top + 6 - 4, rc.left + 3, rc.bottom - 6 - 1 + 4, clr);
        CL_Display::draw_line(rc.left + 4, rc.top + 6 - 5, rc.left + 4, rc.bottom - 6 - 1 + 5, clr);
        CL_Display::draw_line(rc.left + 5, rc.top + 6 - 5, rc.left + 5, rc.bottom - 6 - 1 + 5, clr);
    }
    // right
    {
        CL_Display::draw_line(rc.right - 1, rc.top + 6, rc.right - 1, rc.bottom - 6 - 1, clr);
        CL_Display::draw_line(rc.right - 1 - 1, rc.top + 6 - 2, rc.right - 1 - 1, rc.bottom - 6 - 1 + 2, clr);
        CL_Display::draw_line(rc.right - 1 - 2, rc.top + 6 - 3, rc.right - 1 - 2, rc.bottom - 6 - 1 + 3, clr);
        CL_Display::draw_line(rc.right - 1 - 3, rc.top + 6 - 4, rc.right - 1 - 3, rc.bottom - 6 - 1 + 4, clr);
        CL_Display::draw_line(rc.right - 1 - 4, rc.top + 6 - 5, rc.right - 1 - 4, rc.bottom - 6 - 1 + 5, clr);
        CL_Display::draw_line(rc.right - 1 - 5, rc.top + 6 - 5, rc.right - 1 - 5, rc.bottom - 6 - 1 + 5, clr);
    }
}

#pragma warning ( pop )
