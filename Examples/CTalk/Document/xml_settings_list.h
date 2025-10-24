
#pragma once

#include "xml_settings_document.h"

class XMLSettings;

class XMLSettingsList
{
public:
	XMLSettingsList();
	XMLSettingsList(const XMLSettingsDocument &document, const CL_DomElement &node);

	XMLSettingsDocument get_document();

	XMLSettings add();
	XMLSettings get(int index);
	void remove(int index);
	int get_count() const;

private:
	XMLSettingsDocument document;
	CL_DomElement node;
};
