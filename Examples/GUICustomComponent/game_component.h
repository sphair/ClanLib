#pragma once

#include <ClanLib/gui.h>
#include <ClanLib/display.h>

class RadialMenu;

class GameComponent : public CL_GUIComponent
{
public:
	GameComponent(const CL_Rect &position, CL_GUIManager *gui_manager);
	~GameComponent();

private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &cliprect);
	void on_message(CL_GUIMessage &message);

	void show_radial_menu(CL_Point &position);
	void on_radial_menu_itemselected(int selectedIndex);

	RadialMenu *radial_menu;

	CL_Image background;

	CL_String selected_text;

	CL_Font font;
};
