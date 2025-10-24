// NS_MenuManager.cpp: implementation of the NS_MenuManager class.
//
//////////////////////////////////////////////////////////////////////

#include "NS_MenuManager.h"
#include "NS_MenuGeneric.h"
#include "NS_Menu_Main.h"
#include "NS_Menu_Credits.h"
#include "NS_Menu_Play.h"
#include "NS_Menu_Game.h"

#include <iostream>

// needed for mouse hack :((
#include <ClanLib/display.h>
#include <ClanLib/core.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NS_MenuManager::NS_MenuManager(CL_ResourceManager & resource,
                               CL_StyleManager & style_manager)
    :   resource(resource),
        style_manager(style_manager),
        running(false),
		current_menu(0)
{
    menus["menu_main"] = new NS_Menu_Main(*this, style_manager, "Graphics/menu_main.xml");
    menus["menu_credits"] = new NS_Menu_Credits(*this, style_manager, "Graphics/menu_credits.xml");
    menus["menu_play"] = new NS_Menu_Play(*this, style_manager, "Graphics/menu_play.xml");
    menus["menu_game"] = new NS_Menu_Game(*this, style_manager, "Graphics/menu_game.xml");

    for(MCont_t::iterator it = menus.begin(); it != menus.end(); ++it)
        it->second->disable_input();

	current_menu = menus["menu_main"];
    current_menu->enable_input();
}

NS_MenuManager::~NS_MenuManager()
{
	for(MCont_t::iterator it = menus.begin(); it != menus.end(); ++it)
		delete it->second;
}

void NS_MenuManager::run()
{
    running = true;
    while(running)
    {
        current_menu->run();
    }
}

CL_ResourceManager & NS_MenuManager::get_resource_manager()
{
    return resource;
}

void NS_MenuManager::switch_to(std::string const & menu_name)
{
    MCont_t::iterator new_menu = menus.find(menu_name);
    if (new_menu != menus.end())
    {
        current_menu->disable_input();
        current_menu = new_menu->second;
        current_menu->enable_input();
        
        // anti buttons stuck hack
        CL_InputEvent key;
        key.device = CL_Mouse::get_device();
        key.mouse_pos = CL_Point(CL_Mouse::get_x(), CL_Mouse::get_y());
        key.type = CL_InputEvent::moved;
        CL_Mouse::get_device().sig_move().call(key);

        std::cout << "NS_MenuManager::switch to (\"" << menu_name
            << "\") success\n";
    }
    else
        std::cout << "NS_MenuManager::switch to (\"" << menu_name
            << "\") failed\n";
}

void NS_MenuManager::quit()
{
    running = false;
}
