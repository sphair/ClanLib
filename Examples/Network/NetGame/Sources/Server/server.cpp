#include "server.h"
#include "server_user.h"
#include "../custom_type.h"

Server::Server()
: next_user_id(1)
{
	// Connect essential signals - connecting, disconnecting and receiving events
	slots.connect(network_server.sig_client_connected(), this, &Server::on_client_connected);
	slots.connect(network_server.sig_client_disconnected(), this, &Server::on_client_disconnected);
	slots.connect(network_server.sig_event_received(), this, &Server::on_event_received);

	// Set up event dispatchers to route incoming events to functions
	login_events.func_event("Login").set(this, &Server::on_event_login);
	game_events.func_event("Game-RequestStart").set(this, &Server::on_event_game_requeststart);

	game_running = false;
}

Server::~Server()
{
}

// Server main loop
void Server::exec()
{
	network_server.start("4556");

	log_event("system", "SERVER started");

	while (true)
	{
		KeepAlive::process(10);
	}

	network_server.stop();
}

// A new client is connecting
void Server::on_client_connected(NetGameConnection *connection)
{
	log_event("network", "Client connected");

	// Create user and attach it to connection
	ServerUser *user = new ServerUser();
	user->attach_to_connection(connection);
}

// A client disconnected
void Server::on_client_disconnected(NetGameConnection *connection, const std::string &message)
{
	log_event("network", "Client disconnected");

	ServerUser *user = ServerUser::get_user(connection);
	if(user)
		delete user;

	// Shut down running game for test-purposes
	if(game_running)
		game_running = false;
}

// An event was received from a client
void Server::on_event_received(NetGameConnection *connection, const NetGameEvent &e)
{
	log_event("events", "Client sent event: %1", e.to_string());

	ServerUser *user = ServerUser::get_user(connection);
	if(user)
	{
		bool handled_event = false;

		if (user->id == 0)	// User has not logged in, so route events to login dispatcher
		{
			// Route login events
			handled_event = login_events.dispatch(e, user);
		}
		else
		{
			// Route game events
			handled_event = game_events.dispatch(e, user);
		}

		if (!handled_event)
		{
			// We received an event which we didn't hook up
			log_event("events", "Unhandled event: %1", e.to_string());
		}
	}
}

// "Login" event was received
void Server::on_event_login(const NetGameEvent &e, ServerUser *user)
{
	log_event("events", "Client requested login");

	std::string user_name = e.get_argument(0);

	if(user_name.length() == 0)
	{
		user->send_event(NetGameEvent("Login-Fail", "Missing user name"));
	}
	else
	{
		// Assign name and id to User object (created when user connected earlier)
		user->user_name = user_name;
		user->id = next_user_id++;

		user->send_event(NetGameEvent("Login-Success"));
	}
}

// "Game-RequestStartGame" event was received
void Server::on_event_game_requeststart(const NetGameEvent &e, ServerUser *user)
{
	log_event("events", "Client requested game start");

	if(game_running == false)
	{
		game_running = true;

		std::string map_name = "Map1";
		int max_players = 6;

		CustomType position(143,22,3);

		NetGameEvent loadMapEvent("Game-LoadMap");
		loadMapEvent.add_argument(map_name);
		loadMapEvent.add_argument(max_players);
		loadMapEvent.add_argument(position);
		network_server.send_event(loadMapEvent);

		network_server.send_event(NetGameEvent("Game-Start"));
	}
}
