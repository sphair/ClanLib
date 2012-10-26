
#pragma once

class Client;

class LoginEvents
{
public:
	LoginEvents(Client *client);
	~LoginEvents();

	NetGameEventDispatcher_v0 &get_dispatcher() { return login_events; }

private:
	void on_event_login_successful(const NetGameEvent &e);
	void on_event_login_failed(const NetGameEvent &e);

	NetGameEventDispatcher_v0 login_events;

	Client *client;
};
