// NS_Menu_Main.cpp: implementation of the NS_Menu_Main class.
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include "NS_Menu_Main.h"
#include "NS_Profile.h"
//#include "../Game/NS_Players.h"

#include <ClanLib/display.h>
//#include <ClanLib/core.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NS_Menu_Main::NS_Menu_Main(NS_MenuManager & menu_manager,
                           CL_StyleManager & style_manager,
                           std::string const & path_to_resource)
    : NS_MenuGeneric(menu_manager, style_manager, path_to_resource),
	back_ground(new CL_Sprite("menu_right_bottom", style_manager.get_resources()))
{
	CL_Button * btn_to_profile = dynamic_cast<CL_Button*>(comp_manager->get_component(("btn_to_profile")));
	if (btn_to_profile)
		slots.connect(btn_to_profile->sig_clicked(), this, &NS_Menu_Main::on_profile);

	CL_Button * btn_to_game = dynamic_cast<CL_Button*>(comp_manager->get_component(("btn_to_game")));
	if (btn_to_game)
		slots.connect(btn_to_game->sig_clicked(), this, &NS_Menu_Main::on_switch_to_game_menu);

	CL_Button * btn_to_credits = dynamic_cast<CL_Button*>(comp_manager->get_component(("btn_to_credits")));
	if (btn_to_credits)
		slots.connect(btn_to_credits->sig_clicked(), this, &NS_Menu_Main::on_switch_to_credits_menu);

	CL_Button * btn_quit = dynamic_cast<CL_Button*>(comp_manager->get_component(("btn_quit")));
	if (btn_quit)
		slots.connect(btn_quit->sig_clicked(), this, &NS_Menu_Main::on_quit);
}

NS_Menu_Main::~NS_Menu_Main()
{
	delete back_ground;
}

void NS_Menu_Main::on_profile()
{
    NS_Profile::open(this);
}

void NS_Menu_Main::on_quit()
{
    NS_MenuGeneric::on_exit();
}

void NS_Menu_Main::on_switch_to_credits_menu()
{
    switch_to("menu_credits");
}

void NS_Menu_Main::on_switch_to_game_menu()
{
	switch_to("menu_game");
}

void NS_Menu_Main::switch_to(std::string const & menu_name)
{
    NS_MenuGeneric::switch_to(menu_name);
}

void NS_Menu_Main::on_begin_paint()
{
    back_ground->draw(0,0);
}
