
#pragma once

class clan::NetGameConnection;
class clan::NetGameEvent;

class ServerPlayer
{
public:
	ServerPlayer(clan::NetGameConnection *connection);

	static ServerPlayer *get_player(clan::NetGameConnection *connection);

	clan::NetGameConnection *get_connection() const { return connection; }

	bool login(int player_id, const std::string &player_name);
	bool is_logged_in() const { return id == 0; }

	int get_id() const { return id; }
	std::string get_name() const { return name; }

	void send_event(const clan::NetGameEvent &game_event);

private:
	clan::NetGameConnection *connection;

	std::string name;
	int id;
};
