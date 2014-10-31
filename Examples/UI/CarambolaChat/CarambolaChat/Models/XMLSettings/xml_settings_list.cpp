
#include "precomp.h"
#include "xml_settings_list.h"
#include "xml_settings.h"

XMLSettingsList::XMLSettingsList()
{
}

XMLSettingsList::XMLSettingsList(const XMLSettingsAppModel &document, const clan::DomElement &node)
: document(document), node(node)
{
}

XMLSettingsAppModel XMLSettingsList::get_document()
{
	return document;
}

XMLSettings XMLSettingsList::add()
{
	clan::DomElement element = node.get_owner_document().create_element("item");
	node.append_child(element);
	return XMLSettings(document, element);
}

XMLSettings XMLSettingsList::get(int index)
{
	int count = 0;
	clan::DomElement cur = node.get_first_child_element();
	while (!cur.is_null())
	{
		if (count == index)
			return XMLSettings(document, cur);

		count++;
		cur = cur.get_next_sibling_element();
	}
	return XMLSettings();
}

void XMLSettingsList::remove(int index)
{
	int count = 0;
	clan::DomElement cur = node.get_first_child_element();
	while (!cur.is_null())
	{
		if (count == index)
		{
			node.remove_child(cur);
			break;
		}

		count++;
		cur = cur.get_next_sibling_element();
	}
}

int XMLSettingsList::get_count() const
{
	int count = 0;
	clan::DomElement cur = node.get_first_child_element();
	while (!cur.is_null())
	{
		count++;
		cur = cur.get_next_sibling_element();
	}
	return count;
}
