
#pragma once

#include "Document/xml_settings.h"
#include "Document/irc_session.h"
#include <vector>

class IRCSession;

class Document
{
public:
	Document();
	~Document();

	XMLSettings settings;

	IRCSession *create_session(const CL_String &connection_name);
	void destroy_session(IRCSession *session);

	CL_Signal_v1<IRCSession *> cb_session_created;
	CL_Signal_v1<IRCSession *> cb_session_destroyed;

private:
	XMLSettings load_settings(const CL_String &settings_file);
	void save_settings(const CL_String &settings_file, XMLSettings settings);

	std::vector<IRCSession *> sessions;
};
