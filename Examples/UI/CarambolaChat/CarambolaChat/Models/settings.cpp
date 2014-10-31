
#include "precomp.h"
#include "settings.h"
#include "XMLSettings/xml_settings_list.h"

Settings::Settings()
{
	try
	{
		xml_settings = load_settings("Carambola.xmlsettings");
	}
	catch (clan::Exception &)
	{
		XMLSettingsList connections = xml_settings.get_list("connections");

		XMLSettings connection1 = connections.add();
		connection1.set_string("connectionname", "FreeNode");
		connection1.set_string("server", "irc.freenode.net");
		connection1.set_string("comment", "Freenode provides discussion facilities for the Free and Open Source Software communities, for not-for-profit organizations and for related communities and organizations.");
		connection1.set_string("nick", "CTalk2011");
		connection1.set_string("altnick", "_CTalk2011");
		connection1.set_string("username", "carambola");
		connection1.set_string("name", "Anonymous ClanLib Carambola User");
		connection1.set_bool("autoconnect", false);
		XMLSettingsList performlist1 = connection1.get_list("performlist");

		XMLSettings connection2 = connections.add();
		connection2.set_string("connectionname", "QuakeNet");
		connection2.set_string("server", "jubiigames.dk.quakenet.org"); /*"servercentral.il.us.quakenet.org"*/
		connection2.set_string("comment", "QuakeNet is built around the gaming community, and is an organizations that facilitates the communication of many teams and online organizations.");
		connection2.set_string("nick", "CTalk2011");
		connection2.set_string("altnick", "_CTalk2011");
		connection2.set_string("username", "carambola");
		connection2.set_string("name", "Anonymous ClanLib Carambola User");
		connection2.set_bool("autoconnect", false);
		XMLSettingsList performlist2 = connection2.get_list("performlist");

		set_minimize_to_tray(true);
	}
}

Settings::~Settings()
{
	save_settings("Carambola.xmlsettings", xml_settings);
}

clan::Rect Settings::get_window_geometry()
{
	XMLSettings section = xml_settings.get_section("window-geometry");
	clan::Rect box;
	box.left = section.get_int("x");
	box.top = section.get_int("y");
	box.right = box.left + section.get_int("width");
	box.bottom = box.top + section.get_int("height");
	return box;
}

void Settings::set_window_geometry(const clan::Rect &rect)
{
	XMLSettings section = xml_settings.get_section("window-geometry");
	section.set_int("x", rect.left);
	section.set_int("y", rect.top);
	section.set_int("width", rect.get_width());
	section.set_int("height", rect.get_height());
}

bool Settings::get_minimize_to_tray()
{
	XMLSettings section = xml_settings.get_section("preferences");
	return section.get_bool("minimizetotray", true);
}

void Settings::set_minimize_to_tray(bool enable_minimize_to_tray)
{
	XMLSettings section = xml_settings.get_section("preferences");
	section.set_bool("minimizetotray", enable_minimize_to_tray);
}

std::string Settings::get_dcc_port()
{
	XMLSettings section = xml_settings.get_section("preferences");
	return section.get_string("dccport", "5000");
}

void Settings::set_dcc_port(const std::string &port)
{
	XMLSettings section = xml_settings.get_section("preferences");
	section.set_string("dccport", port);
}

XMLSettings Settings::load_settings(const std::string &settings_file)
{
	std::string appdata_dir = clan::Directory::get_appdata("ClanLib", "Carambola", "1.0");
	XMLSettingsAppModel document;
	document.load(appdata_dir + settings_file);
	return document.get_root();
}

void Settings::save_settings(const std::string &settings_file, XMLSettings settings)
{
	std::string appdata_dir = clan::Directory::get_appdata("ClanLib", "Carambola", "1.0");
	settings.get_document().save(appdata_dir + settings_file);
}
