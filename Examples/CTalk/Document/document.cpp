
#include "precomp.h"
#include "document.h"
#include "irc_session.h"
#include "xml_settings.h"
#include "xml_settings_document.h"
#include "xml_settings_list.h"
#include "xml_settings_map.h"
#include <algorithm>

Document::Document()
{
	try
	{
		settings = load_settings("Carambola.xmlsettings");
	}
	catch (CL_Exception &)
	{
		XMLSettingsList connections = settings.get_list("connections");

		XMLSettings connection1 = connections.add();
		connection1.set_string("connectionname", "FreeNode");
		connection1.set_string("server", "irc.freenode.net");
		connection1.set_string("comment", "Freenode provides discussion facilities for the Free and Open Source Software communities, for not-for-profit organizations and for related communities and organizations.");
		connection1.set_string("nick", "CTalk2009");
		connection1.set_string("username", "carambola");
		connection1.set_string("name", "Anonymous ClanLib Carambola User");
		connection1.set_bool("autoconnect", false);
		XMLSettingsList performlist1 = connection1.get_list("performlist");

		XMLSettings connection2 = connections.add();
		connection2.set_string("connectionname", "QuakeNet");
		connection2.set_string("server", "jubiigames.dk.quakenet.org"); /*"servercentral.il.us.quakenet.org"*/
		connection2.set_string("comment", "QuakeNet is built around the gaming community, and is an organizations that facilitates the communication of many teams and online organizations.");
		connection2.set_string("nick", "CTalk2009");
		connection2.set_string("username", "carambola");
		connection2.set_string("name", "Anonymous ClanLib Carambola User");
		connection2.set_bool("autoconnect", false);
		XMLSettingsList performlist2 = connection2.get_list("performlist");

		XMLSettings section = settings.get_section("preferences");
		section.set_bool("minimizetotray", true);
	}
}

Document::~Document()
{
	for (size_t i = 0; i < sessions.size(); i++)
		delete sessions[i];
	sessions.clear();

	save_settings("Carambola.xmlsettings", settings);
}

IRCSession *Document::create_session(const CL_String &connection_name)
{
	IRCSession *session = new IRCSession;
	session->set_connection_name(connection_name);

	sessions.push_back(session);

	cb_session_created.invoke(session);

	return session;
}

void Document::destroy_session(IRCSession *session)
{
	if (session)
	{
		std::vector<IRCSession*>::iterator it = std::find(sessions.begin(), sessions.end(), session);
		if (it == sessions.end())
			throw CL_Exception("IRCSession object is already destroyed or does not belong to this document");

		cb_session_destroyed.invoke(session);

		sessions.erase(it);
		delete session;
	}
}

XMLSettings Document::load_settings(const CL_String &settings_file)
{
	CL_String appdata_dir = CL_Directory::get_appdata("ClanLib", "Carambola", "1.0");
	XMLSettingsDocument document;
	document.load(appdata_dir + settings_file);
	return document.get_root();
}

void Document::save_settings(const CL_String &settings_file, XMLSettings settings)
{
	CL_String appdata_dir = CL_Directory::get_appdata("ClanLib", "Carambola", "1.0");
	settings.get_document().save(appdata_dir + settings_file);
}
