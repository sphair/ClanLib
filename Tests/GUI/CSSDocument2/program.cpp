
#include "precomp.h"
#include "program.h"
#include <ClanLib/application.h>
#include "custom.h"

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupDisplay setup_display;
	SetupGL setup_gl;
	SetupGUI setup_gui;

	GUIManager gui(".");
	gui.add_theme("../../../Resources/GUIThemeAero");

	GUITopLevelDescription window_desc;
	window_desc.set_title("GUI Test");
	window_desc.set_size(Size(1024, 768), false);
	window_desc.set_allow_resize(true);

	GUIComponent *root = new GUIComponent(&gui, window_desc, "component");
	root->set_class("root");
	root->func_close().set(&Program::on_close, root);
	root->func_resized().set(&Program::on_resized, root);

	Ribbon *ribbon = new Ribbon(root);
	RibbonPage *page_home = new RibbonPage(ribbon, "Home");
	RibbonPage *page_view = new RibbonPage(ribbon, "View");
	ribbon->get_menu()->add_item(Image(ribbon->get_canvas(), "../../../Resources/GUIThemeAero/Images/messagebox_icon_info.png"), "New");
	ribbon->get_menu()->add_item(Image(ribbon->get_canvas(), "../../../Resources/GUIThemeAero/Images/messagebox_icon_info.png"), "Open");
	ribbon->get_menu()->add_separator();
	ribbon->get_menu()->add_item(Image(ribbon->get_canvas(), "../../../Resources/GUIThemeAero/Images/messagebox_icon_info.png"), "About");
	ribbon->get_menu()->add_separator();
	ribbon->get_menu()->add_item(Image(ribbon->get_canvas(), "../../../Resources/GUIThemeAero/Images/messagebox_icon_info.png"), "Exit");

	RibbonSection *section1 = new RibbonSection(page_home, "Section 1");
	RibbonSection *section2 = new RibbonSection(page_home, "Section 2");
	RibbonSection *section3 = new RibbonSection(page_view, "Section 3");

	RibbonGroup *group1 = new RibbonGroup(section1);
	RibbonGroup *group2 = new RibbonGroup(section2);
	RibbonGroup *group3 = new RibbonGroup(section3);

	PushButton *button1 = new PushButton(group1);
	PushButton *button11 = new PushButton(group1);
	PushButton *button2 = new PushButton(group2);
	PushButton *button3 = new PushButton(group3);
	button1->set_text("New");
	button11->set_text("Open");
	button2->set_text("Save");
	button3->set_text("View Something");

	GUIComponent *row = new GUIComponent(root);
	row->set_class("row");

	TextEdit *textedit = new TextEdit(row);
	textedit->set_text("This is a TextEdit control.");

	ListView *listview = new ListView(row);
	listview->get_header()->append(listview->get_header()->create_column("name", "Name"));
	listview->get_document_item().append_child(listview->create_item()).set_column_text("name", "Item 1");
	listview->get_document_item().append_child(listview->create_item()).set_column_text("name", "Item 2");
	listview->get_document_item().append_child(listview->create_item()).set_column_text("name", "Item 3");
	listview->get_document_item().append_child(listview->create_item()).set_column_text("name", "Item 4");

	StatusBar *statusbar = new StatusBar(root);
	statusbar->set_status_text("Ready");

/*
	DomDocument xml(File("layout.xml"), false); // Braindead constructors on DomDocument..

	for (DomElement xml_child = xml.get_document_element().get_first_child_element(); xml_child.is_element(); xml_child = xml_child.get_next_sibling_element())
	{
		create_component(xml_child, root);
	}

	create_imageview_test(root);
*/
	root->update_layout();

	gui.exec();

	return 0;
}

void Program::create_component(DomElement xml_element, GUIComponent *parent)
{
	if (xml_element.get_tag_name() == "label")
	{
		Label *component = new Label(parent);
		component->set_class(xml_element.get_attribute("class"));
		std::vector<std::string> pseudo_classes = StringHelp::split_text(xml_element.get_attribute("pseudo-class"), " ");
		for (size_t i = 0; i < pseudo_classes.size(); i++)
			component->set_pseudo_class(pseudo_classes[i], true);

		component->set_text(xml_element.get_text());
	}
	else if (xml_element.get_tag_name() == "imageview")
	{
		ImageView *component = new ImageView(parent);
		component->set_class(xml_element.get_attribute("class"));
		std::vector<std::string> pseudo_classes = StringHelp::split_text(xml_element.get_attribute("pseudo-class"), " ");
		for (size_t i = 0; i < pseudo_classes.size(); i++)
			component->set_pseudo_class(pseudo_classes[i], true);

		Image image(parent->get_canvas(), xml_element.get_text());
		component->set_image(image);
	}
	else if (xml_element.get_tag_name() == "pushbutton")
	{
		PushButton *component = new PushButton(parent);
		component->set_class(xml_element.get_attribute("class"));
		std::vector<std::string> pseudo_classes = StringHelp::split_text(xml_element.get_attribute("pseudo-class"), " ");
		for (size_t i = 0; i < pseudo_classes.size(); i++)
			component->set_pseudo_class(pseudo_classes[i], true);

		try
		{
			std::string text = xml_element.select_string("text/text()");
			component->set_text(text);
		}
		catch (Exception &)
		{
		}

		try
		{
			std::string image_src = xml_element.select_string("icon/text()");
			Image image(parent->get_canvas(), image_src);
			component->set_icon(image);
		}
		catch (Exception &)
		{
		}
	}
	else if (xml_element.get_tag_name() == "radiobutton")
	{
		RadioButton *component = new RadioButton(parent);
		component->set_class(xml_element.get_attribute("class"));
		std::vector<std::string> pseudo_classes = StringHelp::split_text(xml_element.get_attribute("pseudo-class"), " ");
		for (size_t i = 0; i < pseudo_classes.size(); i++)
			component->set_pseudo_class(pseudo_classes[i], true);

		try
		{
			std::string text = xml_element.select_string("text/text()");
			component->set_text(text);
		}
		catch (Exception &)
		{
		}

		try
		{
			std::string group = xml_element.select_string("group/text()");
			component->set_group_name(group);
		}
		catch (Exception &)
		{
		}
	}
	else if (xml_element.get_tag_name() == "checkbox")
	{
		CheckBox *component = new CheckBox(parent);
		component->set_class(xml_element.get_attribute("class"));
		std::vector<std::string> pseudo_classes = StringHelp::split_text(xml_element.get_attribute("pseudo-class"), " ");
		for (size_t i = 0; i < pseudo_classes.size(); i++)
			component->set_pseudo_class(pseudo_classes[i], true);
		component->set_text(xml_element.get_text());
	}		
	else if (xml_element.get_tag_name() == "lineedit")
	{
		LineEdit *component = new LineEdit(parent);
		component->set_class(xml_element.get_attribute("class"));
		std::vector<std::string> pseudo_classes = StringHelp::split_text(xml_element.get_attribute("pseudo-class"), " ");
		for (size_t i = 0; i < pseudo_classes.size(); i++)
			component->set_pseudo_class(pseudo_classes[i], true);
		component->set_text(xml_element.get_text());
	}	

	else if (xml_element.get_tag_name() == "scrollbar")
	{
		ScrollBar *component = new ScrollBar(parent);
		component->set_class(xml_element.get_attribute("class"));
		std::vector<std::string> pseudo_classes = StringHelp::split_text(xml_element.get_attribute("pseudo-class"), " ");
		for (size_t i = 0; i < pseudo_classes.size(); i++)
			component->set_pseudo_class(pseudo_classes[i], true);

		component->set_ranges(100, 200, 5, 20);
	}
	else if (xml_element.get_tag_name() == "custom")
	{
		Custom *component = new Custom(parent);
		component->set_class(xml_element.get_attribute("class"));
		std::vector<std::string> pseudo_classes = StringHelp::split_text(xml_element.get_attribute("pseudo-class"), " ");
		for (size_t i = 0; i < pseudo_classes.size(); i++)
			component->set_pseudo_class(pseudo_classes[i], true);
	}
	else if (xml_element.get_tag_name() == "slider")
	{
		Slider *component = new Slider(parent);
		component->set_class(xml_element.get_attribute("class"));
		std::vector<std::string> pseudo_classes = StringHelp::split_text(xml_element.get_attribute("pseudo-class"), " ");
		for (size_t i = 0; i < pseudo_classes.size(); i++)
			component->set_pseudo_class(pseudo_classes[i], true);

		component->set_ranges(100, 200, 5, 20);

	}	
	else
	{
		GUIComponent *component = new GUIComponent(parent, xml_element.get_tag_name());
		component->set_class(xml_element.get_attribute("class"));
		std::vector<std::string> pseudo_classes = StringHelp::split_text(xml_element.get_attribute("pseudo-class"), " ");
		for (size_t i = 0; i < pseudo_classes.size(); i++)
			component->set_pseudo_class(pseudo_classes[i], true);

		for (DomElement xml_child = xml_element.get_first_child_element(); xml_child.is_element(); xml_child = xml_child.get_next_sibling_element())
		{
			create_component(xml_child, component);
		}
	}
}

void Program::create_imageview_test(GUIComponent *root)
{
	Image image(root->get_canvas(), "../../../Examples/GUI/CommonCode/Resources/tux.png");

	GUIComponent *imagecontainer = new GUIComponent(root, "component");
	imagecontainer->set_class("imagecontainer");

	Label *label1 = new Label(imagecontainer);
	label1->set_text("Labels should probably use ellipsis text clipping");
	label1->set_class("imagelabel");

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
