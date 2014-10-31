
#pragma once

#include "xml_settings_document.h"

class XMLSettings;

class XMLSettingsList
{
public:
	XMLSettingsList();
	XMLSettingsList(const XMLSettingsAppModel &document, const clan::DomElement &node);

	XMLSettingsAppModel get_document();

	XMLSettings add();
	XMLSettings get(int index);
	void remove(int index);
	int get_count() const;

private:
	XMLSettingsAppModel document;
	clan::DomElement node;
};
