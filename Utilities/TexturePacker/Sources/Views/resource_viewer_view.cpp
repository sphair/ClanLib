#include "precomp.h"
#include "resource_viewer_view.h"
#include "../sprite_component.h"
#include "../texture_packer.h"

ResourceViewerView::ResourceViewerView(CL_GUIComponent *parent, MainWindow *mainwindow)
: View(parent, mainwindow, "Resource viewer")
{
	func_resized().set(this, &ResourceViewerView::on_resized);

	label_selected_resource_file = new CL_Label(this);
	label_selected_resource_file->set_text("Resource file:");

	lineedit_selected_resource_file = new CL_LineEdit(this);

	button_browse_resource_file = new CL_PushButton(this);
	button_browse_resource_file->set_text("...");
	button_browse_resource_file->func_clicked().set(this, &ResourceViewerView::on_button_browse_resource_file);

	resource_list = new CL_ListView(this);
	CL_ListViewColumnHeader col = resource_list->get_header()->append(resource_list->get_header()->create_column("Resource", "Resource"));
	col.set_width(200);
	resource_list->get_header()->append(resource_list->get_header()->create_column("Message", "Message"));

	CL_ListViewIconList icon_list = resource_list->get_icon_list();
	CL_ListViewIcon icon_disabled;
	icon_disabled.set_sprite(CL_Sprite(get_gc(), "Resources/sweetie-16-em-cross.png"), listview_mode_details);
	CL_ListViewIcon icon_enabled;
	icon_enabled.set_sprite(CL_Sprite(get_gc(), "Resources/sweetie-16-em-check.png"), listview_mode_details);
	icon_list.set_icon(1, icon_disabled);
	icon_list.set_icon(2, icon_enabled);

	resource_list->func_selection_changed().set(this, &ResourceViewerView::on_selection_changed);

	sprite_component = new SpriteComponent(this);

	on_resized();
}

ResourceViewerView::~ResourceViewerView()
{
}

void ResourceViewerView::on_resized()
{
	CL_Size size = get_size();

	label_selected_resource_file->set_geometry(CL_Rect(10, 13, 80, 30));
	lineedit_selected_resource_file->set_geometry(CL_Rect(80, 10, size.width - 20 - 20, 30));
	button_browse_resource_file->set_geometry(CL_Rect(10 + size.width - 20 - 30, 10, 10 + size.width - 20, 30));

	resource_list->set_geometry(CL_Rect(10, 40, 10 + size.width - 20, 200));

	sprite_component->set_geometry(CL_Rect(10, 210, 10 + size.width - 20, 210 + size.height - 50));
}

void ResourceViewerView::on_button_browse_resource_file()
{
	CL_OpenFileDialog dlg(this);
	dlg.add_filter("Resource files", "*.xml", true);
	dlg.add_filter("All files", "*.*");

	dlg.set_initial_directory(CL_System::get_exe_path());

	if(dlg.show())
	{
		CL_String filename = dlg.get_filename();

		try
		{
			load_resource_file(filename);
			lineedit_selected_resource_file->set_text(filename);
		}
		catch(CL_Exception error)
		{
//			generation_result->set_text(cl_format("Error: %1", error.message));
		}
	}
}

void ResourceViewerView::on_selection_changed(CL_ListViewSelection selection)
{
	CL_UnknownSharedPtr ptr =  selection.get_first().get_item().get_userdata();
	ResourceItem *resource_item = (ResourceItem *)ptr.get();
	if(resource_item)
		show_resource(resource_item);
}

void ResourceViewerView::show_resource(ResourceItem *resource_item)
{
	SpriteResourceItem *sprite_item = dynamic_cast<SpriteResourceItem *>(resource_item);
	if(sprite_item)
	{
		sprite_component->set_sprite(&sprite_item->sprite);
	}
}

void ResourceViewerView::load_resource_file(const CL_String &file)
{
	packer.load_resources(get_gc(), file);

	std::vector<ResourceItem *> &items = packer.get_resource_items();
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
