#include "precomp.h"
#include "main_window.h"
#include "application.h"
#include "texture_packer.h"
#include "sprite_component.h"
#include "texturegroup_component.h"

MainWindow::MainWindow(Application *application)
: CL_Window(get_startup_geometry(), application->get_gui(), get_startup_description()),
  application(application)
{
	set_id_name("editor");
	func_close().set(this, &MainWindow::on_close);
	func_resized().set(this, &MainWindow::on_resized);

	create_components();
	on_resized();

	packer = new TexturePacker(get_gc());

	load_resources();
}

MainWindow::~MainWindow()
{
	// We do not have to free the GUI components here because the CL_GUIComponent
	// destructor deletes all child components.
}

CL_Rect MainWindow::get_startup_geometry()
{
	return CL_Rect(200, 200, 1024, 768);
}

CL_GUITopLevelDescription MainWindow::get_startup_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title(cl_text("ClanLib Texture Packer"));
	desc.set_allow_resize(true);
	return desc;
}

void MainWindow::create_components()
{
	tab = new CL_Tab(this);

	// TAB 1
	tab_page_1 = tab->add_page("Resource viewer");

	resource_list = new CL_ListView(tab_page_1);
	CL_ListViewColumnHeader col = resource_list->get_header()->append(resource_list->get_header()->create_column("Resource", "Resource"));
	col.set_width(200);
	resource_list->get_header()->append(resource_list->get_header()->create_column("Message", "Message"));

	CL_ListViewIconList icon_list = resource_list->get_icon_list();
	CL_ListViewIcon icon_disabled;
	icon_disabled.set_sprite(CL_Sprite("Resources/sweetie-16-em-cross.png", get_gc()), listview_mode_details);
	CL_ListViewIcon icon_enabled;
	icon_enabled.set_sprite(CL_Sprite("Resources/sweetie-16-em-check.png", get_gc()), listview_mode_details);
	icon_list.set_icon(1, icon_disabled);
	icon_list.set_icon(2, icon_enabled);

	resource_list->func_selection_changed().set(this, &MainWindow::on_selection_changed);

	sprite_component = new SpriteComponent(tab_page_1);

	// TAB 2
	tab_page_2 = tab->add_page("Texture packer");

	radio_texture32 = new CL_RadioButton(tab_page_2);
	radio_texture32->set_geometry(CL_Rect(10, 10, CL_Size(80, 21)));
	radio_texture32->set_text("32x32");
	radio_texture32->set_group_name("TextureSize");

	radio_texture64 = new CL_RadioButton(tab_page_2);
	radio_texture64->set_geometry(CL_Rect(80+10, 10, CL_Size(80, 21)));
	radio_texture64->set_text("64x64");
	radio_texture64->set_group_name("TextureSize");

	radio_texture128 = new CL_RadioButton(tab_page_2);
	radio_texture128->set_geometry(CL_Rect(80+80+10, 10, CL_Size(80, 21)));
	radio_texture128->set_text("128x128");
	radio_texture128->set_group_name("TextureSize");

	radio_texture256 = new CL_RadioButton(tab_page_2);
	radio_texture256->set_geometry(CL_Rect(80+80+80+10, 10, CL_Size(80, 21)));
	radio_texture256->set_text("256x256");
	radio_texture256->set_group_name("TextureSize");

	radio_texture512 = new CL_RadioButton(tab_page_2);
	radio_texture512->set_geometry(CL_Rect(80+80+80+80+10, 10, CL_Size(80, 21)));
	radio_texture512->set_text("512x512");
	radio_texture512->set_group_name("TextureSize");

	radio_texture1024 = new CL_RadioButton(tab_page_2);
	radio_texture1024->set_geometry(CL_Rect(80+80+80+80+80+10, 10, CL_Size(80, 21)));
	radio_texture1024->set_text("1024x1024");
	radio_texture1024->set_group_name("TextureSize");

	radio_texture2048 = new CL_RadioButton(tab_page_2);
	radio_texture2048->set_geometry(CL_Rect(80+80+80+80+80+80+10, 10, CL_Size(80, 21)));
	radio_texture2048->set_text("2048x2048");
	radio_texture2048->set_group_name("TextureSize");
	radio_texture2048->set_selected(true);

	radio_texture512x256 = new CL_RadioButton(tab_page_2);
	radio_texture512x256->set_geometry(CL_Rect(80+80+80+80+80+80+80+10, 10, CL_Size(80, 21)));
	radio_texture512x256->set_text("512x256");
	radio_texture512x256->set_group_name("TextureSize");
	radio_texture512x256->set_selected(true);

	spin_border = new CL_Spin(tab_page_2);
	spin_border->set_geometry(CL_Rect(650, 10, CL_Size(50, 21)));
	spin_border->set_value(1);
	spin_border->set_step_size(1);
	spin_border->set_ranges(0, 8);
	spin_border->set_floating_point_mode(false);

	label_border = new CL_Label(tab_page_2);
	label_border->set_geometry(CL_Rect(710, 10, CL_Size(100, 21)));
	label_border->set_text("Pixel Border");

	button_pack = new CL_PushButton(tab_page_2);
	button_pack->set_text("Generate textures");
	button_pack->func_clicked().set(this, &MainWindow::on_button_generate_textures);

	button_save = new CL_PushButton(tab_page_2);
	button_save->set_text("Save resourcefile");
	button_save->func_clicked().set(this, &MainWindow::on_button_save_resources);

	generation_result = new CL_Label(tab_page_2);

	texturegroup_component = new TextureGroupComponent(tab_page_2);
}

void MainWindow::on_button_save_resources()
{
	try
	{
		CL_String filename = "resources_new.xml";
		packer->save_resources(filename);
		generation_result->set_text(cl_format("Saved resources: %1", filename));
	}
	catch(CL_Exception error)
	{
		generation_result->set_text(cl_format("Save error: %1", error.message));
	}
}

void MainWindow::on_button_generate_textures()
{
	CL_Size texture_size;
	if(radio_texture32->is_selected())
		texture_size = CL_Size(32, 23);
	if(radio_texture64->is_selected())
		texture_size = CL_Size(64, 64);
	if(radio_texture128->is_selected())
		texture_size = CL_Size(128, 128);
	if(radio_texture256->is_selected())
		texture_size = CL_Size(256, 256);
	if(radio_texture512->is_selected())
		texture_size = CL_Size(512, 512);
	if(radio_texture512x256->is_selected())
		texture_size = CL_Size(512, 256);
	if(radio_texture1024->is_selected())
		texture_size = CL_Size(1024, 1024);
	if(radio_texture2048->is_selected())
		texture_size = CL_Size(2048, 2048);

	try
	{
		CL_TextureGroup *group = packer->pack(texture_size, spin_border->get_value());
		texturegroup_component->set_texturegroup(group);
		texturegroup_component->invalidate_rect();

		generation_result->set_text(cl_format("Textures generated: %1", group->get_texture_count()));
	}
	catch(CL_Exception error)
	{
		generation_result->set_text(cl_format("Error: %1", error.message));
	}
}

void MainWindow::on_selection_changed(CL_ListViewSelection selection)
{
	CL_UnknownSharedPtr ptr =  selection.get_first().get_item().get_userdata();
	ResourceItem *resource_item = (ResourceItem *)ptr.get();
	if(resource_item)
		show_resource(resource_item);
}

void MainWindow::on_close()
{
	exit_with_code(0);
}

void MainWindow::on_resized()
{
	tab->set_geometry(CL_Rect(10, 10, get_width() - 10, get_height() - 10));

	resource_list->set_geometry(CL_Rect(10, 10, tab_page_1->get_width() - 10, 200));

	sprite_component->set_geometry(CL_Rect(10, 210, tab_page_1->get_width() - 10, tab_page_1->get_height() - 10));

	button_pack->set_geometry(CL_Rect(10, 40, CL_Size(120, 25)));
	button_save->set_geometry(CL_Rect(tab_page_2->get_width() - 130, 40, CL_Size(120, 25)));
	generation_result->set_geometry(CL_Rect(170, 44, tab_page_2->get_width() - 130, 60));
	texturegroup_component->set_geometry(CL_Rect(10, 70, tab_page_2->get_width() - 10, tab_page_2->get_height() - 10));
}

void MainWindow::show_resource(ResourceItem *resource_item)
{
	SpriteResourceItem *sprite_item = dynamic_cast<SpriteResourceItem *>(resource_item);
	if(sprite_item)
	{
		sprite_component->set_sprite(&sprite_item->sprite);
	}
}

void MainWindow::load_resources()
{
//	packer->load_resources("resources.xml");
//	packer->load_resources("../../Resources/GUIThemeLuna/resources_sprites.xml");
//	packer->load_resources("../../Resources/GUIThemeLuna/resources.xml");
	packer->load_resources("../../Resources/GUIThemeAero/resources.xml");

	std::vector<ResourceItem *> &items = packer->get_resource_items();
	std::vector<ResourceItem *>::iterator it;
	for(it = items.begin(); it != items.end(); ++it)
	{
		ResourceItem *resource_item = (ResourceItem *)(*it);

		CL_ListViewItem item = resource_list->create_item();
		item.set_column_text("Resource", resource_item->resource.get_name());

		CL_SharedPtr<ResourceItem> userdata(resource_item);
		item.set_userdata(userdata);

		NotSupportedResourceItem *not_supported_item = dynamic_cast<NotSupportedResourceItem *>(resource_item);
		if(not_supported_item)
		{
			item.set_icon(1);
			item.set_column_text("Message", not_supported_item->error);
		}
		else
		{
			item.set_icon(2);
		}

		resource_list->get_document_item().append_child(item);
	}
}
