
#include "precomp.h"
#include "login_events.h"
#include "client.h"

LoginEvents::LoginEvents(Client *client)
: client(client)
{
	login_events.func_event("login-successful").set(this, &LoginEvents::on_event_login_successful);
	login_events.func_event("login-failed").set(this, &LoginEvents::on_event_login_failed);
}

LoginEvents::~LoginEvents()
{
}

void LoginEvents::on_event_login_successful(const CL_NetGameEvent &e)
{
	int player_id = e.get_argument(0);

	client->set_state(Client::in_lobby);
	client->set_player_id(player_id);

	client->get_network_client()->send_event(CL_NetGameEvent("lobby-get-available-games"));
}

void LoginEvents::on_event_login_failed(const CL_NetGameEvent &e)
{
	CL_String fail_reason = e.get_argument(0);

	// TODO: Handle failed login more gracefully
	throw CL_Exception("Login failed: " + fail_reason);
}
