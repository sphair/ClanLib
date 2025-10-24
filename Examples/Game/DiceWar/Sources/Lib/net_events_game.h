#pragma once

// Server to Client (STC) Events

// The game is starting, client does initialization 
#define STC_GAME_IS_STARTING				"game-is-starting"				// ()	

// A new map should be loaded and initialized
#define STC_GAME_LOAD_MAP					"game-load-map"					// (string mapname)

// The game has started -> ready to play!
#define STC_GAME_HAS_STARTED				"game-has-started"				// ()

// A new player has joined the active game. Sent-to: All lobby players
#define STC_GAME_PLAYER_JOINED_GAME			"game-player-joined-game"		// (int player_id, string player_name, int visual_id)

// A player has left the active game. Sent-to: All lobby players
#define STC_GAME_PLAYER_LEFT_GAME			"game-player-left-game"			// (int player_id)

// A map area has a new player ownership
#define STC_GAME_SET_MAPAREA_OWNERSHIP		"game-set-maparea-ownership"	// (int map_area_id, int player_id)

// A map area has a new army strength
#define STC_GAME_SET_MAPAREA_ARMY_STRENGTH	"game-set-maparea-army-strength"// (int map_area_id, int army_strength)

// An attack was invalid
#define STC_GAME_INVALID_ATTACK				"game-invalid-attack"			// ()

// A map area was attacked (marks start of an attack)
#define STC_GAME_ATTACKED_AREA				"game-attacked-area"			// (int map_area_from_id, int map_area_to_id)

// The result of an attack
#define STC_GAME_ATTACK_DICE_RESULT			"game-attack-dice-result"		// (int dice_result1, dice_result2, dice_result3, dice_result4, dice_result5, dice_result6, dice_result7, dice_result8)

// The result of an defense - also marks the end of an attack
#define STC_GAME_DEFENSE_DICE_RESULT		"game-defense-dice-result"		// (int dice_result1, dice_result2, dice_result3, dice_result4, dice_result5, dice_result6, dice_result7, dice_result8)

// An attack was completed (on all clients)
#define STC_GAME_ATTACK_DONE				"game-attack-done"				// ()

// A player started his turn
#define STC_GAME_PLAYER_TURN_STARTED		"game-player-turn-started"		// (int player_id)

// The game is over
#define STC_GAME_OVER						"game-over"						// (string title, string message)

// A player sent a game message
#define STC_GAME_PLAYER_MESSAGE				"game-player-message"			// (int player_id, string message)

// The system sent an system message
#define STC_GAME_SYSTEM_MESSAGE				"game-system-message"			// (string message)


// Client to Server (CTS) Events

// Attack an area
// Response: game-attacked-area, game-attack-dice-result, game-defense-dice-result, game-set-maparea-ownership, game-set-maparea-army-strength, game-attack-done, game-invalid-attack
#define CTS_GAME_ATTACK_AREA				"game-attack-area"				// (int map_area_from_id, int map_area_to_id)

// End player turn
// Response: game-player-turn-started
#define CTS_GAME_END_TURN					"game-end-turn"					// ()

// Notify battle view is done
// Response: None
#define CTS_GAME_BATTLE_VIEW_OVER			"game-battle-view-over"			// ()

// Send a chat message to game chat
// Response: game-player-message
#define CTS_GAME_ADD_MESSAGE				"game-add-message"				// (string message)

