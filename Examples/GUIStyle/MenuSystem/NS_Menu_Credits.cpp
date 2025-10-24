// NS_Menu_Credits.cpp: implementation of the NS_Menu_Credits class.
//
//////////////////////////////////////////////////////////////////////

#include "NS_Menu_Credits.h"
#include <ClanLib/display.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NS_Menu_Credits::NS_Menu_Credits(NS_MenuManager & menu_manager,
                           CL_StyleManager & style_manager,
                           std::string const & path_to_resource)
    :   NS_MenuGeneric(menu_manager, style_manager, path_to_resource),
	back_ground(new CL_Sprite("menu_right_bottom", style_manager.get_resources()))
{
	CL_Button * btn_to_main_menu = dynamic_cast<CL_Button*>(comp_manager->get_component(("btn_to_main_menu")));
	if (btn_to_main_menu)
		slots.connect(btn_to_main_menu->sig_clicked(), this, &NS_Menu_Credits::on_switch_to_main_menu);
}

NS_Menu_Credits::~NS_Menu_Credits()
{
	delete back_ground;
}

void NS_Menu_Credits::on_quit()
{
    NS_MenuGeneric::quit();
}

void NS_Menu_Credits::on_switch_to_main_menu()
{
    switch_to("menu_main");
}

void NS_Menu_Credits::switch_to(std::string const & menu_name)
{
    NS_MenuGeneric::switch_to(menu_name);
}

void NS_Menu_Credits::on_begin_paint()
{
    back_ground->draw(0,0);
}
