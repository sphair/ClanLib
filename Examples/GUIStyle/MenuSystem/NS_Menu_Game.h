// NS_Menu_Game.h: interface for the NS_Menu_Game class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NS_MENU_GAME_H__C4BBA54B_4EE0_400B_91AA_44F8E55F6955__INCLUDED_)
#define AFX_NS_MENU_GAME_H__C4BBA54B_4EE0_400B_91AA_44F8E55F6955__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NS_MenuGeneric.h"
#include <list>

class CL_Sprite;

class NS_Menu_Game : public NS_MenuGeneric
{
public:
    NS_Menu_Game(NS_MenuManager & menu_manager,
        CL_StyleManager & style_manager,
        std::string const & path_to_resource);
    virtual ~NS_Menu_Game();
public:
    void on_begin_paint();
    void on_switch_to_main_menu();
    
	// virtual functions
	virtual void run_task();
public:
    void on_quit();
    void switch_to(std::string const & menu_name);
private:
    CL_Sprite * back_ground;
	bool is_need_update;
};

#endif // !defined(AFX_NS_MENU_GAME_H__C4BBA54B_4EE0_400B_91AA_44F8E55F6955__INCLUDED_)
