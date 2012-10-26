#pragma once

// Server to Client (STC) Events

// New player logged in to server
// Sent-to: All server players
#define STC_LOBBY_PLAYER_LOGGED_IN			"lobby-player-logged-in"		// (int player_id, string name)

// Player logged out from server
// Sent-to: All server players
#define STC_LOBBY_PLAYER_LOGGED_OUT			"lobby-player-logged-out"		// (int player_id, string reason)

// A game was created
// Sent-to: Game creator
#define STC_LOBBY_GAME_CREATED				"lobby-game-created"			// (int game_id)

// A game was removed
// Sent-to: All lobby players
#define STC_LOBBY_GAME_REMOVED				"lobby-game-removed"			// (int game_id)

// Information about an existing game
// Sent-to: Single player, All lobby players
#define STC_LOBBY_GAME_INFO					"lobby-game-info"				// (int game_id, string name, string map_name, int max_players, int state)

// No games are available (if a list was requested)
// Sent-to: Single player
#define STC_LOBBY_NO_GAMES_AVAILABLE		"lobby-no-games-available"		// ()

// A new player has joined the game
// Sent-to: All lobby players
#define STC_LOBBY_PLAYER_JOINED_GAME		"lobby-player-joined-game"		// (int player_id, int game_id)

// A player has left the game
// Sent-to: All lobby players
#define STC_LOBBY_PLAYER_LEFT_GAME			"lobby-player-left-game"		// (int player_id, int game_id)

// A lobby game was started
// Sent-to: All lobby players
#define STC_LOBBY_GAME_STARTED				"lobby-game-started"			// (int game_id)

// A player sent a lobby message
#define STC_LOBBY_PLAYER_MESSAGE			"lobby-player-message"			// (int player_id, string message)

// The system sent an error message
#define STC_LOBBY_ERROR_MESSAGE				"lobby-error-message"			// (string message)


// Client to Server (CTS) Events

// Create a new game 
// Response: lobby-game-created, lobby-game-info, lobby-player-joined-game
#define CTS_LOBBY_CREATE_GAME				"lobby-create-game"				// ()

// Change game settings
#define CTS_LOBBY_CHANGE_GAME_SETTINGS		"lobby-change-game-settings"	// (string game_name, string map_name, int max_players)

// Request all available games 
// Response: lobby-game-info, lobby-no-games-available
#define CTS_LOBBY_GET_AVAILABLE_GAMES		"lobby-get-available-games"		// ()

// Join a game
// Response: lobby-player-joined-game
#define CTS_LOBBY_JOIN_GAME					"lobby-join-game"				// (int game_id)

// Leave a game
// Response: lobby-player-left-game, lobby-game-removed
#define CTS_LOBBY_LEAVE_GAME				"lobby-leave-game"				// ()

// Start a self-created game
// Response: game-is-starting, game-load-map, game-set-maparea-ownership, game-set-maparea-army-strength
#define CTS_LOBBY_START_GAME				"lobby-start-game"				// ()

// Send a chat message to lobby chat
// Response: lobby-player-message
#define CTS_LOBBY_ADD_MESSAGE				"lobby-add-message"				// (string message)
