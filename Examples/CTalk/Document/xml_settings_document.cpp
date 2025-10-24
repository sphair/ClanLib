
#include "precomp.h"
#include "xml_settings_document.h"
#include "xml_settings.h"

XMLSettingsDocument::XMLSettingsDocument()
{
}

void XMLSettingsDocument::load(const CL_String &filename)
{
	CL_File file(filename, CL_File::open_existing, CL_File::access_read, CL_File::share_read);
	load(file);
}

void XMLSettingsDocument::load(CL_IODevice device)
{
	document.load(device, false);
}

void XMLSettingsDocument::save(const CL_String &filename)
{
	CL_File file(filename, CL_File::create_always, CL_File::access_write, 0);
	save(file);
}

void XMLSettingsDocument::save(CL_IODevice device)
{
	document.save(device, false);
}

XMLSettings XMLSettingsDocument::get_root()
{
	CL_DomElement element = document.get_document_element();
	if (element.is_null())
	{
		element = document.create_element("settings");
		document.append_child(element);
	}
	return XMLSettings(*this, element);
}
