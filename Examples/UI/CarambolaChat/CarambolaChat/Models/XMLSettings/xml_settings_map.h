
#pragma once

#include "xml_settings_document.h"

class XMLSettings;

class XMLSettingsMap
{
public:
	XMLSettingsMap();
	XMLSettingsMap(const XMLSettingsAppModel &document, const clan::DomElement &node);

	XMLSettingsAppModel get_document();

	XMLSettings get(const std::string &key);
	void remove(const std::string &key);

private:
	XMLSettingsAppModel document;
	clan::DomElement node;
};
