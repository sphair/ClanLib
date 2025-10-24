
#pragma once

#include "xml_settings_document.h"

class XMLSettings;

class XMLSettingsMap
{
public:
	XMLSettingsMap();
	XMLSettingsMap(const XMLSettingsDocument &document, const CL_DomElement &node);

	XMLSettingsDocument get_document();

	XMLSettings get(const CL_String &key);
	void remove(const CL_String &key);

private:
	XMLSettingsDocument document;
	CL_DomElement node;
};
