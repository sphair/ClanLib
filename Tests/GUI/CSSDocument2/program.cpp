
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
	root->func_resized().set(&Program::on_resized, root);

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

	GUIComponent *testHot1 = new GUIComponent(root);
	testHot1->set_class("test1");
	GUIComponent *testHot2 = new GUIComponent(testHot1);
	testHot2->set_class("test2");
	GUIComponent *testHot3 = new GUIComponent(testHot2);
	testHot3->set_class("test3");

	create_imageview_test(root);

	root->update_layout();

	gui.exec();

	return 0;
}

void Program::create_imageview_test(GUIComponent *root)
{
	Image image(root->get_canvas(), "../../../Examples/GUI/CommonCode/Resources/tux.png");

	GUIComponent *imagecontainer = new GUIComponent(root);
	imagecontainer->set_class("imagecontainer");

	Label *label1 = new Label(imagecontainer);
	label1->set_text("Once upon a time there was a label in a test program that had to include a lot of text so we could see it word wrap!\nBut we need a font.draw_text(Rect target_rect, alignment) for it to work correctly");

	ImageView *image_view = new ImageView(imagecontainer);
	image_view->set_image(image);
	image_view->set_scale_to_fit(false);

	ImageView *image_view_fixedwidth = new ImageView(imagecontainer);
	image_view_fixedwidth->set_image(image);
	image_view_fixedwidth->set_class("fixedwidth odd");
	image_view_fixedwidth->set_scale_to_fit(false);

	ImageView *image_view_fixedheight = new ImageView(imagecontainer);
	image_view_fixedheight->set_image(image);
	image_view_fixedheight->set_class("fixedheight");
	image_view_fixedheight->set_scale_to_fit(false);

	ImageView *image_view_fixedwidth_fixedheight = new ImageView(imagecontainer);
	image_view_fixedwidth_fixedheight->set_image(image);
	image_view_fixedwidth_fixedheight->set_class("fixedwidth fixedheight odd");
	image_view_fixedwidth_fixedheight->set_scale_to_fit(false);

	ImageView *image_view_double = new ImageView(imagecontainer);
	image_view_double->set_image(image);
	image_view_double->set_scale(2.0f, 2.0f);

	ImageView *image_view_scaletofit = new ImageView(imagecontainer);
	image_view_scaletofit->set_image(image);
	image_view_scaletofit->set_scale_to_fit(true);
	image_view_scaletofit->set_class("odd");

	ImageView *image_view_fixedwidth_scaletofit = new ImageView(imagecontainer);
	image_view_fixedwidth_scaletofit->set_image(image);
	image_view_fixedwidth_scaletofit->set_scale_to_fit(true);
	image_view_fixedwidth_scaletofit->set_class("fixedwidth");

	ImageView *image_view_fixedwidth_fixedheight_scaletofit = new ImageView(imagecontainer);
	image_view_fixedwidth_fixedheight_scaletofit->set_image(image);
	image_view_fixedwidth_fixedheight_scaletofit->set_scale_to_fit(true);
	image_view_fixedwidth_fixedheight_scaletofit->set_class("fixedwidth fixedheight odd");
}

bool Program::on_close(GUIComponent *component)
{
	component->exit_with_code(0);
	return true;
}

void Program::on_resized(GUIComponent *component)
{
	component->update_layout();
}
