
#include "precomp.h"
#include "xml_settings_document.h"
#include "xml_settings.h"

XMLSettingsAppModel::XMLSettingsAppModel()
{
}

void XMLSettingsAppModel::load(const std::string &filename)
{
	clan::File file(filename, clan::File::open_existing, clan::File::access_read, clan::File::share_read);
	load(file);
}

void XMLSettingsAppModel::load(clan::IODevice device)
{
	document.load(device, false);
}

void XMLSettingsAppModel::save(const std::string &filename)
{
	clan::File file(filename, clan::File::create_always, clan::File::access_write, 0);
	save(file);
}

void XMLSettingsAppModel::save(clan::IODevice device)
{
	document.save(device, false);
}

XMLSettings XMLSettingsAppModel::get_root()
{
	clan::DomElement element = document.get_document_element();
	if (element.is_null())
	{
		element = document.create_element("settings");
		document.append_child(element);
	}
	return XMLSettings(*this, element);
}
