
#include "precomp.h"
#include "xml_settings_map.h"
#include "xml_settings.h"

XMLSettingsMap::XMLSettingsMap()
{
}

XMLSettingsMap::XMLSettingsMap(const XMLSettingsDocument &document, const CL_DomElement &node)
: document(document), node(node)
{
}

XMLSettingsDocument XMLSettingsMap::get_document()
{
	return document;
}

XMLSettings XMLSettingsMap::get(const CL_String &key)
{
	CL_DomElement cur = node.get_first_child_element();
	while (!cur.is_null())
	{
		if (cur.get_attribute(L"key") == key)
			return XMLSettings(document, cur);

		cur = cur.get_next_sibling_element();
	}

	cur = node.get_owner_document().create_element(L"item");
	cur.set_attribute(L"key", key);
	node.append_child(cur);
	return XMLSettings(document, cur);
}

void XMLSettingsMap::remove(const CL_String &key)
{
	CL_DomElement cur = node.get_first_child_element();
	while (!cur.is_null())
	{
		if (cur.get_attribute(L"key") == key)
		{
			node.remove_child(cur);
			break;
		}

		cur = cur.get_next_sibling_element();
	}
}
