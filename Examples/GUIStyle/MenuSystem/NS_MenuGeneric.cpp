// NS_MenuGeneric.cpp: implementation of the NS_MenuGeneric class.
//
//////////////////////////////////////////////////////////////////////

#include "NS_MenuGeneric.h"
#include "NS_MenuManager.h"

#include <ClanLib/display.h>
#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/core.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NS_MenuGeneric::NS_MenuGeneric(NS_MenuManager & menu_manager,
                               CL_StyleManager & style_manager,
                               std::string const & path_to_resource)
    :   CL_GUIManager(&style_manager),
        menu_manager(menu_manager),
		comp_manager(0)
{
    comp_manager = new CL_ComponentManager(path_to_resource, this);
    slots.connect(sig_begin_paint(), this, &NS_MenuGeneric::on_begin_paint);
    slots.connect(sig_focus_changed(), this, &NS_MenuGeneric::on_focus_changed);
}

NS_MenuGeneric::~NS_MenuGeneric()
{
	delete comp_manager;
}

NS_MenuManager & NS_MenuGeneric::get_menu_manager() const
{
    return menu_manager;
}

void NS_MenuGeneric::on_exit()
{
    quit();
    menu_manager.quit();
}

void NS_MenuGeneric::run()
{
    quit_run = false;
    while (!quit_run)
    {
        run_task();
    }
}

void NS_MenuGeneric::show()
{
    CL_GUIManager::show();
}

void NS_MenuGeneric::quit()
{
    quit_run = true;
}

void NS_MenuGeneric::on_begin_paint()
{
    CL_Display::clear(CL_Color::black);
}

// Menu manager loops a current menu, and so here should be
// a things which usualy doing in main loop in games
// Do a whatever calculations you need in this method.
// Standard implementation just clears the screen
void NS_MenuGeneric::run_task()
{
    show();
    CL_Display::flip();
    CL_System::keep_alive(10);
}

void NS_MenuGeneric::switch_to(std::string const & menu_name)
{
    quit();
    menu_manager.switch_to(menu_name);
}

void NS_MenuGeneric::on_focus_changed(CL_Component * new_comp_focus)
{
}
