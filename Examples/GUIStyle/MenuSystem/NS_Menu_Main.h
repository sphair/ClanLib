// NS_Menu_Main.h: interface for the NS_Menu_Main class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NS_MENU_MAIN_H__2FF44AC1_57A7_4CBC_AA78_B9EDC584A886__INCLUDED_)
#define AFX_NS_MENU_MAIN_H__2FF44AC1_57A7_4CBC_AA78_B9EDC584A886__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NS_MenuGeneric.h"

class CL_Sprite;

class NS_Menu_Main : public NS_MenuGeneric
{
public:
    NS_Menu_Main(NS_MenuManager & menu_manager,
        CL_StyleManager & style_manager,
        std::string const & path_to_resource);
    virtual ~NS_Menu_Main();
public:
    void on_switch_to_credits_menu();
    void on_switch_to_game_menu();
    // virtual functions
public:
    void on_begin_paint();

    void on_quit();
    void switch_to(std::string const & menu_name);
public:
    void on_profile();
private:
    CL_Sprite * back_ground;
};

#endif // !defined(AFX_NS_MENU_MAIN_H__2FF44AC1_57A7_4CBC_AA78_B9EDC584A886__INCLUDED_)
