
#include "precomp.h"
#include "property_manager.h"

using namespace clan;

PropertyManager::PropertyManager()
: listview(0)
{
	set_title("Property Manager");
	set_dockable_window_geometry(Rect(Point(50, 50), Size(300, 400)));

	listview = new ListView(this);
	listview->set_class("borderless");
	listview->get_header()->set_class("hidden");
	listview->get_header()->append(listview->get_header()->create_column("name", "Name")).set_width(110);
}

void PropertyManager::on_resized()
{
	listview->set_geometry(get_size());
}
