// NS_MenuManager.h: interface for the NS_MenuManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NS_MENUMANAGER_H__5EBB7102_97E9_433A_898C_45923769508C__INCLUDED_)
#define AFX_NS_MENUMANAGER_H__5EBB7102_97E9_433A_898C_45923769508C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <map>

class CL_ResourceManager;
class CL_GUIManager;
class NS_MenuGeneric;
class CL_StyleManager;

// Menu manager is a main object of the game. It manage all menus
// and answer to switching requests from one menu to another

class NS_MenuManager
{
public:
    // In constructor MenuManager creates all menus from resourses
    // and set curent menu to "menu_main"
    NS_MenuManager(CL_ResourceManager & resource,
        CL_StyleManager & style_manager);
    ~NS_MenuManager();
public:
    // Main loop of the game
    void run();
    // Switch request, sended from one of the managed menu
    // param: menu_name - name of menu to switch on
    void switch_to(std::string const & menu_name);
    // Quit from main loop
    void quit();
    CL_ResourceManager & get_resource_manager();
private:
    CL_ResourceManager & resource;
    CL_StyleManager & style_manager;

    typedef std::map<std::string, NS_MenuGeneric * > MCont_t;
    // This contaner holds all menus connected to Manager
    MCont_t menus;
    // Current menu
    NS_MenuGeneric * current_menu;
    // Indicator of running main loop
    bool running;
};

#endif // !defined(AFX_NS_MENUMANAGER_H__5EBB7102_97E9_433A_898C_45923769508C__INCLUDED_)
