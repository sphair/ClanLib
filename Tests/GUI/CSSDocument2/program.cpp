
#include "precomp.h"
#include "program.h"
#include <ClanLib/application.h>

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupDisplay setup_display;
	SetupGL setup_gl;
	SetupGUI setup_gui;

	GUIManager gui(".");
	gui.add_resources("../../../Resources/GUIThemeAero/resources.xml");

	GUITopLevelDescription window_desc;
	window_desc.set_title("GUI Test");
	window_desc.set_size(Size(1024, 768), false);
	window_desc.set_allow_resize(true);

	GUIComponent *root = new GUIComponent(&gui, window_desc);
	root->set_class("root");
	root->func_close().set(&Program::on_close, root);

	GUIComponent *ribbon = new GUIComponent(root);
	ribbon->set_tag_name("ribbon");

	GUIComponent *ribbon_header = new GUIComponent(ribbon);
	ribbon_header->set_tag_name("ribbon-header");

	GUIComponent *ribbon_menu_button = new GUIComponent(ribbon_header);
	ribbon_menu_button->set_tag_name("ribbon-menu-button");

	GUIComponent *ribbon_page_tab_0 = new GUIComponent(ribbon_header);
	ribbon_page_tab_0->set_tag_name("ribbon-tab");
	ribbon_page_tab_0->set_pseudo_class("selected", true);

	GUIComponent *ribbon_page_area = new GUIComponent(ribbon);
	ribbon_page_area->set_tag_name("ribbon-page");

	GUIComponent *ribbon_section_0 = new GUIComponent(ribbon_page_area);
	ribbon_section_0->set_tag_name("ribbon-section");

	GUIComponent *ribbon_section_1 = new GUIComponent(ribbon_page_area);
	ribbon_section_1->set_tag_name("ribbon-section");

	GUIComponent *ribbon_control_0 = new GUIComponent(ribbon_section_0);
	ribbon_control_0->set_tag_name("button");

	GUIComponent *ribbon_control_1 = new GUIComponent(ribbon_section_0);
	ribbon_control_1->set_tag_name("button");
	ribbon_control_1->set_class("big");

//	ribbon->set_geometry(Rect(0, 0, root->get_width(), 117));

	root->update_layout();

	gui.exec();

	return 0;
}

bool Program::on_close(GUIComponent *component)
{
	component->exit_with_code(0);
	return true;
}
