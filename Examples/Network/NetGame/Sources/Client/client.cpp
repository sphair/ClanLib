#include "client.h"
#include "../custom_type.h"

Client::Client()
{
	// Connect essential signals - connecting, disconnecting and receiving events
	slots.connect(network_client.sig_event_received(), this, &Client::on_event_received);
	slots.connect(network_client.sig_connected(), this, &Client::on_connected);
	slots.connect(network_client.sig_disconnected(), this, &Client::on_disconnected);

	// Set up event dispatchers to route incoming events to functions
	login_events.func_event("Login-Success").set(this, &Client::on_event_login_success);
	login_events.func_event("Login-Fail").set(this, &Client::on_event_login_fail);
	game_events.func_event("Game-LoadMap").set(this, &Client::on_event_game_loadmap);
	game_events.func_event("Game-Start").set(this, &Client::on_event_game_startgame);

	quit = false;
	logged_in = false;
}

Client::~Client()
{
}

void Client::exec()
{
	log_event("system", "CLIENT started");

	connect_to_server();

	while (!quit)
	{
		KeepAlive::process(10);
	}
}

// Connect to localhost server
void Client::connect_to_server()
{
	try
	{
		network_client.connect("localhost", "4556");
	}
	catch(const Exception &e)
	{
		log_event("error", e.message);
	}
}

// Successfully connected to server
void Client::on_connected()
{
	log_event("network", "Connected to server");

	// For demonstration purposes, lets fail a login
	network_client.send_event(NetGameEvent("Login", "")); // We will receive an error event for this, as we don't send a proper user name

	// Properly login
	network_client.send_event(NetGameEvent("Login", "my user name"));
}

// Disconnected from server
void Client::on_disconnected()
{
	log_event("network", "Disconnected from server");
	quit = true;
}

// An event was received from server
void Client::on_event_received(const NetGameEvent &e) 
{
	log_event("events", "Server sent event: %1", e.to_string());

	bool handled_event = false;
	
	if(!logged_in)
	{
		// We haven't logged in, so route events to login dispatcher
		handled_event = login_events.dispatch(e);
	}
	else
	{
		// We have logged in, so route events to game dispatcher
		handled_event = game_events.dispatch(e);
	}

	if(!handled_event)
	{
		// We received an event which we didn't hook up
		log_event("events", "Unhandled event: %1", e.to_string());
	}
}

// "Login-Success" event was received
void Client::on_event_login_success(const NetGameEvent &e)
{
	log_event("events", "Login success");

	logged_in = true;

	network_client.send_event(NetGameEvent("Game-RequestStart"));
}

// "Login-Fail" event was received
void Client::on_event_login_fail(const NetGameEvent &e)
{
	std::string fail_reason = e.get_argument(0);

	log_event("events", "Login failed: %1", fail_reason);
}

// "Game-LoadMap" event was received
void Client::on_event_game_loadmap(const NetGameEvent &e) 
{
	std::string map_name = e.get_argument(0);
	int max_players = e.get_argument(1);
	CustomType position = e.get_argument(2);

	log_event("events", "Loading map: %1 with %2 players, Position %3,%4,%5", map_name, max_players, position.get_x(), position.get_y(), position.get_z());
}

// "Game-Start" event was received
void Client::on_event_game_startgame(const NetGameEvent &e) 
{
	log_event("events", "Starting game!");
}
