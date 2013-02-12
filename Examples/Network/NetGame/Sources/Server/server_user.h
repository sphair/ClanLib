#pragma once

#include <ClanLib/core.h>
using namespace clan;

class clan::NetGameConnection;
class clan::NetGameEvent;

class ServerUser
{
public:
	static ServerUser *get_user(NetGameConnection* connection);

public:
	ServerUser();

	void attach_to_connection(NetGameConnection *connection);

	void send_event(const NetGameEvent &gameEvent);

public:
	int id;
	std::string user_name;

private:
	NetGameConnection *connection;
};
