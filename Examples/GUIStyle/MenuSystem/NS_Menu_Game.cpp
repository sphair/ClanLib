// NS_Menu_Game.cpp: implementation of the NS_Menu_Game class.
//
//////////////////////////////////////////////////////////////////////

#include "NS_Menu_Game.h"
#include <iostream>

#include "ClanLib/display.h"
#include "../ZZ_Theme/listbox_zz.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NS_Menu_Game::NS_Menu_Game(NS_MenuManager & menu_manager,
                           CL_StyleManager & style_manager,
                           std::string const & path_to_resource)
    :   NS_MenuGeneric(menu_manager, style_manager, path_to_resource),
	back_ground(new CL_Sprite("menu_right_bottom", style_manager.get_resources()))
{
	CL_Button * btn_to_main_menu = dynamic_cast<CL_Button*>(comp_manager->get_component(("btn_to_main_menu")));
	if (btn_to_main_menu)
		slots.connect(btn_to_main_menu->sig_clicked(), this, &NS_Menu_Game::on_switch_to_main_menu);

	is_need_update = true;
}

NS_Menu_Game::~NS_Menu_Game()
{
	delete back_ground;
}

void NS_Menu_Game::on_quit()
{
    NS_MenuGeneric::quit();
}

void NS_Menu_Game::on_switch_to_main_menu()
{
    switch_to("menu_main");
}

void NS_Menu_Game::switch_to(std::string const & menu_name)
{
	is_need_update = true;
    NS_MenuGeneric::switch_to(menu_name);
}

void NS_Menu_Game::on_begin_paint()
{
    back_ground->draw(0,0);
}

//-----------------------------------------------------------------------------
void NS_Menu_Game::run_task()
{
	if (is_need_update)
	{
		CL_ListBox * levels = dynamic_cast<CL_ListBox*>(comp_manager->get_component("lb_level"));
		if (levels)
		{
			levels->clear();
			levels->insert_item(new CL_ListBoxItem_ZZ("level 3", true), -1, true);
			levels->insert_item(new CL_ListBoxItem_ZZ("level 5", false), -1, true);
			levels->insert_item(new CL_ListBoxItem_ZZ("level 2", true), -1, true);
			levels->insert_item(new CL_ListBoxItem_ZZ("level 4", false), -1, true);
			levels->insert_item(new CL_ListBoxItem_ZZ("level 1", true), -1, true);
			levels->insert_item(new CL_ListBoxItem_ZZ("level 6", false), -1, true);

			levels->sort();
			for(int i = 0; i < levels->get_count(); ++i)
			{
				CL_ListBoxItem_ZZ * level = (CL_ListBoxItem_ZZ *)levels->get_item(i);
				if (level->get_solved() == false)
				{
					levels->set_current_item(i);
					break;
				}
			}
		}
		is_need_update = false;
	}
	NS_MenuGeneric::run_task();
}
