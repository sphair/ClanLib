
#include "precomp.h"
#include "xml_settings_map.h"
#include "xml_settings.h"

XMLSettingsMap::XMLSettingsMap()
{
}

XMLSettingsMap::XMLSettingsMap(const XMLSettingsAppModel &document, const clan::DomElement &node)
: document(document), node(node)
{
}

XMLSettingsAppModel XMLSettingsMap::get_document()
{
	return document;
}

XMLSettings XMLSettingsMap::get(const std::string &key)
{
	clan::DomElement cur = node.get_first_child_element();
	while (!cur.is_null())
	{
		if (cur.get_attribute("key") == key)
			return XMLSettings(document, cur);

		cur = cur.get_next_sibling_element();
	}

	cur = node.get_owner_document().create_element("item");
	cur.set_attribute("key", key);
	node.append_child(cur);
	return XMLSettings(document, cur);
}

void XMLSettingsMap::remove(const std::string &key)
{
	clan::DomElement cur = node.get_first_child_element();
	while (!cur.is_null())
	{
		if (cur.get_attribute("key") == key)
		{
			node.remove_child(cur);
			break;
		}

		cur = cur.get_next_sibling_element();
	}
}
