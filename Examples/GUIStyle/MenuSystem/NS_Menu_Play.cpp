// NS_Menu_Play.cpp: implementation of the NS_Menu_Play class.
//
//////////////////////////////////////////////////////////////////////

#include "NS_Menu_Play.h"
#include <ClanLib/display.h>
#include <ClanLib/display.h>

#include <iostream>
#include "NS_MenuManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NS_Menu_Play::NS_Menu_Play(NS_MenuManager & menu_manager,
                           CL_StyleManager & style_manager,
                           std::string const & path_to_resource)
    :   NS_MenuGeneric(menu_manager, style_manager, path_to_resource),
    level_number(0),
    state(NS_Menu_Play::game_running)
{
    slots.connect(CL_Keyboard::sig_key_down(), this, &NS_Menu_Play::on_key_down);
}

NS_Menu_Play::~NS_Menu_Play()
{

}

void NS_Menu_Play::on_key_down(CL_InputEvent const & key)
{
    if (!is_input_enabled())
        return;

    if (key.id == CL_KEY_ESCAPE)
    {
        // Do cleanup
        on_switch_to_main_menu();
    }
}

void NS_Menu_Play::run_task()
{
	NS_MenuGeneric::run_task();
}

void NS_Menu_Play::on_timer()
{
    on_switch_to_main_menu();
}

void NS_Menu_Play::on_quit()
{
    NS_MenuGeneric::quit();
}

void NS_Menu_Play::on_switch_to_main_menu()
{
    switch_to("menu_main");
}

void NS_Menu_Play::switch_to(std::string const & menu_name)
{
    NS_MenuGeneric::switch_to(menu_name);
}
