
#include "precomp.h"
#include "xml_settings.h"
#include "xml_settings_list.h"
#include "xml_settings_map.h"

XMLSettings::XMLSettings()
{
	*this = document.get_root();
}

XMLSettings::XMLSettings(const XMLSettingsDocument &document, const CL_DomElement &node)
: document(document), node(node)
{
}

XMLSettingsDocument XMLSettings::get_document()
{
	return document;
}

bool XMLSettings::get_bool(const CL_String &key, bool default_value) const
{
	return node.get_child_bool(key, default_value);
}

int XMLSettings::get_int(const CL_String &key, int default_value) const
{
	return node.get_child_int(key, default_value);
}

CL_String XMLSettings::get_string(const CL_String &key, const CL_String &default_value) const
{
	return node.get_child_string(key, default_value);
}

void XMLSettings::set_bool(const CL_String &key, bool value)
{
	node.set_child_bool(key, value);
}

void XMLSettings::set_int(const CL_String &key, int value)
{
	node.set_child_int(key, value);
}

void XMLSettings::set_string(const CL_String &key, const CL_String &value)
{
	node.set_child_string(key, value);
}

XMLSettingsList XMLSettings::get_list(const CL_String &key)
{
	CL_DomNode value_node = node.named_item(key);
	if (value_node.is_null())
	{
		value_node = node.get_owner_document().create_element(key);
		node.append_child(value_node);
	}
	return XMLSettingsList(document, value_node.to_element());
}

XMLSettingsMap XMLSettings::get_map(const CL_String &key)
{
	CL_DomNode value_node = node.named_item(key);
	if (value_node.is_null())
	{
		value_node = node.get_owner_document().create_element(key);
		node.append_child(value_node);
	}
	return XMLSettingsMap(document, value_node.to_element());
}

XMLSettings XMLSettings::get_section(const CL_String &key)
{
	CL_DomNode value_node = node.named_item(key);
	if (value_node.is_null())
	{
		value_node = node.get_owner_document().create_element(key);
		node.append_child(value_node);
	}
	return XMLSettings(document, value_node.to_element());
}
