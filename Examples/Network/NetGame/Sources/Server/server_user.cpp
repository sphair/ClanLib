#include <ClanLib/core.h>
#include <ClanLib/network.h>
#include "server_user.h"

ServerUser::ServerUser()
: connection(0), id(0)
{
}

void ServerUser::attach_to_connection(CL_NetGameConnection *connection)
{
	this->connection = connection;

	if(connection)
		connection->set_data("serveruser", this);
}

ServerUser *ServerUser::get_user(CL_NetGameConnection* connection)
{
	if(connection)
		return reinterpret_cast<ServerUser*>(connection->get_data("serveruser"));
	else
		return 0;
}

void ServerUser::send_event(const CL_NetGameEvent &gameEvent)
{
	if(connection)
		connection->send_event(gameEvent);
}
