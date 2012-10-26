
#include "precomp.h"
#include "server_login_events.h"
#include "server.h"
#include "../Lib/net_events_login.h"

ServerLoginEvents::ServerLoginEvents(Server *server)
: server(server)
{
	login_events.func_event(CTS_LOGIN).set(this, &ServerLoginEvents::on_event_login);
}

ServerLoginEvents::~ServerLoginEvents()
{
}

void ServerLoginEvents::on_event_login(const NetGameEvent &e, ServerPlayer *player)
{
	std::string player_name = e.get_argument(0);

	server->login(player, player_name);
}
