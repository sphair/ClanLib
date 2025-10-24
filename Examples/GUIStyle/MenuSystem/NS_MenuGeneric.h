// NS_MenuGeneric.h: interface for the NS_MenuGeneric class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NS_MENUGENERIC_H__15FC6E14_CE64_4603_96A7_7A10E4350EE4__INCLUDED_)
#define AFX_NS_MENUGENERIC_H__15FC6E14_CE64_4603_96A7_7A10E4350EE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ClanLib/gui.h>
#include <ClanLib/gui.h>
#include <ClanLib/signals.h>
#include <ClanLib/signals.h>

#include <string>

class NS_MenuManager;
class CL_StyleManager;
class CL_ComponentManager;

// Generic menu class, serves as base of all other menus in
// the game. Derive from it and overload virtual methods if
// you want to add a new menu.

class NS_MenuGeneric : public CL_GUIManager
{
public:
    NS_MenuGeneric(NS_MenuManager & menu_manager,
        CL_StyleManager & style_manager,
        std::string const & path_to_resource);
    virtual ~NS_MenuGeneric();
public:
    NS_MenuManager & get_menu_manager() const;
public:
    virtual void on_begin_paint();
    // Exit from app
    void on_exit();
    virtual void switch_to(std::string const & menu) = 0;
    virtual void on_focus_changed(CL_Component * new_comp_focus);
    void run();
    void show();
    // Menu manager loops a current menu, and so here should be
    // a things which usualy doing in main loop in games
    // Do a whatever calculations you need in this method.
    // Standard implementation just clears the screen
    virtual void run_task();
    void quit();
private:
    NS_MenuManager & menu_manager;
    volatile bool quit_run;
protected:
    CL_SlotContainer slots;
    CL_ComponentManager * comp_manager;
};

#endif // !defined(AFX_NS_MENUGENERIC_H__15FC6E14_CE64_4603_96A7_7A10E4350EE4__INCLUDED_)
