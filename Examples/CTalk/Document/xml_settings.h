
#pragma once

#include "xml_settings_document.h"

class XMLSettingsList;
class XMLSettingsMap;

class XMLSettings
{
public:
	XMLSettings();
	XMLSettings(const XMLSettingsDocument &document, const CL_DomElement &node);

	XMLSettingsDocument get_document();

	bool get_bool(const CL_String &key, bool default_value = false) const;
	int get_int(const CL_String &key, int default_value = 0) const;
	CL_String get_string(const CL_String &key, const CL_String &default_value = CL_String()) const;

	void set_bool(const CL_String &key, bool value);
	void set_int(const CL_String &key, int value);
	void set_string(const CL_String &key, const CL_String &value);

	XMLSettingsList get_list(const CL_String &key);
	XMLSettingsMap get_map(const CL_String &key);
	XMLSettings get_section(const CL_String &key);

private:
	XMLSettingsDocument document;
	CL_DomElement node;
};
