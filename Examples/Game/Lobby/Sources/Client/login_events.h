
#pragma once

class Client;

class LoginEvents
{
public:
	LoginEvents(Client *client);
	~LoginEvents();

	CL_NetGameEventDispatcher_v0 &get_dispatcher() { return login_events; }

private:
	void on_event_login_successful(const CL_NetGameEvent &e);
	void on_event_login_failed(const CL_NetGameEvent &e);

	CL_NetGameEventDispatcher_v0 login_events;

	Client *client;
};
