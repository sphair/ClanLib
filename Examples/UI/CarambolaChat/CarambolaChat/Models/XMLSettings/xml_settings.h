
#pragma once

#include "xml_settings_document.h"

class XMLSettingsList;
class XMLSettingsMap;

class XMLSettings
{
public:
	XMLSettings();
	XMLSettings(const XMLSettingsAppModel &document, const clan::DomElement &node);

	XMLSettingsAppModel get_document();

	bool get_bool(const std::string &key, bool default_value = false) const;
	int get_int(const std::string &key, int default_value = 0) const;
	std::string get_string(const std::string &key, const std::string &default_value = std::string()) const;

	void set_bool(const std::string &key, bool value);
	void set_int(const std::string &key, int value);
	void set_string(const std::string &key, const std::string &value);

	XMLSettingsList get_list(const std::string &key);
	XMLSettingsMap get_map(const std::string &key);
	XMLSettings get_section(const std::string &key);

private:
	XMLSettingsAppModel document;
	clan::DomElement node;
};
