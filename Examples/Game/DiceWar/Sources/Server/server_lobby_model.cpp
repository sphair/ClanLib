
#include "precomp.h"
#include "server_lobby_model.h"

ServerLobbyModel::ServerLobbyModel(Server *server)
: players(server), games(server)
{
}

ServerLobbyModel::~ServerLobbyModel()
{
}
