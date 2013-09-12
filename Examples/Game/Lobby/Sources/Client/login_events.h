
#pragma once

class Client;

class LoginEvents
{
public:
	LoginEvents(Client *client);
	~LoginEvents();

	clan::NetGameEventDispatcher_v0 &get_dispatcher() { return login_events; }

private:
	void on_event_login_successful(const clan::NetGameEvent &e);
	void on_event_login_failed(const clan::NetGameEvent &e);

	clan::NetGameEventDispatcher_v0 login_events;

	Client *client;
};
