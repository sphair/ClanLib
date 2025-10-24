// NS_Menu_Credits.h: interface for the NS_Menu_Credits class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NS_MENU_CREDITS_H__D37D041F_8C34_444D_AE42_AFA62BE2E89C__INCLUDED_)
#define AFX_NS_MENU_CREDITS_H__D37D041F_8C34_444D_AE42_AFA62BE2E89C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NS_MenuGeneric.h"

class CL_Sprite;

class NS_Menu_Credits : public NS_MenuGeneric
{
public:
    NS_Menu_Credits(NS_MenuManager & menu_manager,
        CL_StyleManager & style_manager,
        std::string const & path_to_resource);
    virtual ~NS_Menu_Credits();
public:
    void on_switch_to_main_menu();
    void on_begin_paint();
    // virtual functions
public:
    void on_quit();
    void switch_to(std::string const & menu_name);
private:
	CL_Sprite * back_ground;
};

#endif // !defined(AFX_NS_MENU_CREDITS_H__D37D041F_8C34_444D_AE42_AFA62BE2E89C__INCLUDED_)
