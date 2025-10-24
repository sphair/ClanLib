#pragma once

#include <ClanLib/core.h>

class CL_NetGameConnection;
class CL_NetGameEvent;

class ServerUser
{
public:
	static ServerUser *get_user(CL_NetGameConnection* connection);

public:
	ServerUser();

	void attach_to_connection(CL_NetGameConnection *connection);

	void send_event(const CL_NetGameEvent &gameEvent);

public:
	int id;
	CL_String user_name;

private:
	CL_NetGameConnection *connection;
};
