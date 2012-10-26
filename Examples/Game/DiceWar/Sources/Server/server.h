
#pragma once

class ServerPlayer;
class ServerGameEvents;
class ServerLobbyEvents;
class ServerLoginEvents;
class ServerLobbyModel;

class Server
{
public:
	Server();
	~Server();

	void exec(Event &stop_event);

	NetGameServer *get_network_server() { return &network_server; }

	void login(ServerPlayer *player, std::string player_name);
	ServerPlayer *create_ai_player(const std::string &player_name);

	void handle_event(ServerPlayer *player, const NetGameEvent &e);

private:
	void on_client_connected(NetGameConnection *connection);
	void on_client_disconnected(NetGameConnection *connection);

	void on_event_received(NetGameConnection *connection, const NetGameEvent &e);

	int next_player_id;

	NetGameServer network_server;

	std::auto_ptr<ServerLoginEvents> login_events;
	std::auto_ptr<ServerLobbyEvents> lobby_events;
	std::auto_ptr<ServerGameEvents> game_events;

	std::auto_ptr<ServerLobbyModel> lobby_model;

	SlotContainer slots;
};
