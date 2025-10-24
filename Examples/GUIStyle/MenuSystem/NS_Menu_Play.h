// NS_Menu_Play.h: interface for the NS_Menu_Play class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NS_MENU_PLAY_H__1931A600_09B6_40E1_BAA4_B153D0C0C7AB__INCLUDED_)
#define AFX_NS_MENU_PLAY_H__1931A600_09B6_40E1_BAA4_B153D0C0C7AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NS_MenuGeneric.h"

class NS_Game;
class NS_LevelSet;
class NS_Panel_In_Game;

#include <ClanLib/core.h> // for Timer

class NS_Menu_Play : public NS_MenuGeneric
{
public:
    NS_Menu_Play(NS_MenuManager & menu_manager,
        CL_StyleManager & style_manager,
        std::string const & path_to_resource);
    virtual ~NS_Menu_Play();
public:
    // virtual overwrite for kill CL_KeepAlive(10)
    void run_task();
public:
    // TODO: Check about sig_pre_paing() for double clearing
    void on_switch_to_main_menu();
    void on_key_down(CL_InputEvent const & key);
    // virtual functions
public:
    void on_quit();
    void switch_to(std::string const & menu_name);
    void on_timer();
public:
    enum State {
        game_running,
        game_failed,
        game_successful
    };
private:
    size_t level_number;
    CL_Timer timer;
    State state;
};

#endif // !defined(AFX_NS_MENU_PLAY_H__1931A600_09B6_40E1_BAA4_B153D0C0C7AB__INCLUDED_)
