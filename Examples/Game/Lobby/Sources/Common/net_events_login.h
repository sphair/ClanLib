#pragma once

// Server to Client (STC) Events

// Login was successful
// Sent-to: Single player
#define STC_LOGIN_SUCCESSFUL	"login-successful"	// (int player_id)

// Login failed
// Sent-to: Single player
#define STC_LOGIN_FAILED		"login-failed"		// (string reason)


// Client to Server (CTS) Events

// Login to server
// Response: login-successful, login-failed
#define CTS_LOGIN				"login"				// (string name)
