
#pragma once

class CL_NetGameConnection;
class CL_NetGameEvent;

class ServerPlayer
{
public:
	ServerPlayer(CL_NetGameConnection *connection);

	static ServerPlayer *get_player(CL_NetGameConnection *connection);

	CL_NetGameConnection *get_connection() const { return connection; }

	bool login(int player_id, const CL_String &player_name);
	bool is_logged_in() const { return id == 0; }

	int get_id() const { return id; }
	CL_String get_name() const { return name; }

	void send_event(const CL_NetGameEvent &game_event);

private:
	CL_NetGameConnection *connection;

	CL_String name;
	int id;
};
