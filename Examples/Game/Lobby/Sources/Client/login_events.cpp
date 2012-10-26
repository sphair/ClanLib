
#include "precomp.h"
#include "login_events.h"
#include "client.h"
#include "../Common/net_events_login.h"
#include "../Lobby/server_lobby_net_events.h"

LoginEvents::LoginEvents(Client *client)
: client(client)
{
	login_events.func_event(STC_LOGIN_SUCCESSFUL).set(this, &LoginEvents::on_event_login_successful);
	login_events.func_event(STC_LOGIN_FAILED).set(this, &LoginEvents::on_event_login_failed);
}

LoginEvents::~LoginEvents()
{
}

void LoginEvents::on_event_login_successful(const CL_NetGameEvent &e)
{
	int player_id = e.get_argument(0);

	client->set_state(Client::in_lobby);
	client->set_player_id(player_id);

	// TODO; move this to lobby code
	client->get_network_client()->send_event(CL_NetGameEvent(CTS_LOBBY_GET_AVAILABLE_GAMES));
}

void LoginEvents::on_event_login_failed(const CL_NetGameEvent &e)
{
	CL_String fail_reason = e.get_argument(0);

	// TODO: Handle failed login more gracefully
	throw CL_Exception("Login failed: " + fail_reason);
}
