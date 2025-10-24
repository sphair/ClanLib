// NS_MessageBox.h: interface for the NS_MessageBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NS_MESSAGEBOX_H__543C9C0A_F4E2_4410_A105_8C4D957CBF93__INCLUDED_)
#define AFX_NS_MESSAGEBOX_H__543C9C0A_F4E2_4410_A105_8C4D957CBF93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ClanLib/gui.h>
#include <ClanLib/gui.h>
#include <ClanLib/signals.h>
#include <ClanLib/signals.h>

#include <map>
#include <string>

class CL_Font;

class NS_MessageBox : public CL_Window
{
public:
    NS_MessageBox(CL_Component * parent,
        std::string const & title,
        std::string const & text,
        std::string const & button1,
        std::string const & button2,
        std::string const & button3);
    ~NS_MessageBox();
public:
    void on_button1();
    void on_button2();
    void on_button3();
    static int info(CL_Component * parent,
        std::string const & title,
        std::string const & text,
        std::string const & button1,
        std::string const & button2 = "",
        std::string const & button3 = "");
    int get_result() const;
    void on_key_down(CL_InputEvent const & key);
    void on_end_paint();
private:
    std::map<std::string, CL_Component * > components;
    int result;
    CL_SlotContainer slots;
    std::string title;
    std::string text;
    std::string button1_text;
    std::string button2_text;
    std::string button3_text;
    CL_Font * font;
};

#endif // !defined(AFX_NS_MESSAGEBOX_H__543C9C0A_F4E2_4410_A105_8C4D957CBF93__INCLUDED_)
