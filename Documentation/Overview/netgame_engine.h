
/*! \page NetGameEngine NetGame Engine

MainDocs:NetGame Engine

From ClanLib Game SDK
 

The network library in ClanLib sports a simple event based networking engine usually referred to as the NetGame engine. In all its simplicity the engine assist in routing simple messages from a client to a server and back. 

The API consists of the following classes: 
• CL_NetGameServer 
• CL_NetGameClient 
• CL_NetGameConnection 
• CL_NetGameEvent 
• CL_NetGameEventDispatcher 






Contents
•1 Client and Server Connections
•2 NetGame Events
•3 Example Events ?3.1 DiceWar Game Server to Client
?3.2 DiceWar Game Client to Server
?3.3 World of Warcraft Events

•4 Dispatching Game Events to C++ Functions ?4.1 Account operations handlers
?4.2 Login handlers
?4.3 Dispatching

 

Client and Server Connections

On the server side, the game must construct an instance of CL_NetGameServer. Then for each port that the server should listen on, it must call CL_NetGameServer::start(port). 

The class also features three important signals: 
• Client connected 
• Client disconnected 
• Event received 

CL_NetGameServer is a CL_KeepAlive based class, which means that during calls to CL_KeepAlive::process() the class will emit those three signals for each client that has connected/disconnected since last call, and for each game event that arrived since last. 

It is also possible to call process_events() on the CL_NetGameServer or CL_NetGameClient objects directly to cause the signals to be emitted. Generally, in client applications the signals are processed automatically because the main render loop already calls the CL_KeepAlive::process() function, but for servers that to not use clanDisplay the process_events() function can be called directly. 

The CL_NetGameClient class is similar in design, except that there is only one connection (to the server), and therefore the connect/disconnect signals do not include any CL_NetGameConnection objects. 




NetGame Events

The class CL_NetGameEvent represents a game event message that is either sent or received. Each event consists of two things: an event name and list of event parameters. The name is what identifies what the event is about, while each of the parameters can one of the following types: 
• null 
• int 
• string 
• bool 
• float 

Lets say that the client wants to send a log on message to the server. We want to pass a username and a password as the arguments, so we do like this: 


CL_NetGameClient client;
client.connect("example.net", "5555");
 
CL_NetGameEvent event("logon", "Mr. Jones", "the secret of all secrets");
client.send_event(event);

The constructor for CL_NetGameEvent exist in 6 versions, allowing you to pass anything from 0 to 5 parameters in a single line. The types of the parameters are automatically deducted from the C++ type you pass for each parameter. If you need to pass more parameters than this, then you need to use the add_argument(const CL_NetGameEventValue &value) function on CL_NetGameEvent. 

On the server, this game event will be emitted via the sig_event_received, and might be handled like this: 


void on_event_received(CL_NetGameConnection *connection, const CL_NetGameEvent &event)
{
	MyConnectionData *data = get_connection_data(connection);
	if (event.get_name() == "logon")
	{
		if (data->logged_in)
		{
			CL_NetGameEvent response("error", "You are already logged in!");
			connection->send_event(response);
		}
		else
		{
			data->username = event.get_argument(0);
			CL_String password = event.get_argument(1);
			data->logged_in = true;
 
			CL_NetGameEvent response("motd", cl_format("Welcome %1! Please enjoy your stay!", username));
			connection->send_event(response);
		}
	}
}

An example of a NetGame based server can be found in the ClanLib examples. 

Example Events

To illustrate how a game can be built using this system, the following listing shows what events are used in the DiceWar example while the game itself is running: 




DiceWar Game Server to Client

NetGame Event  Description  
game-is-starting()  The game is starting, client does initialization  
game-has-started()  The game has started -> ready to play!  
game-player-joined-game(int player_id, string player_name, int visual_id)  A new player has joined the active game  
game-player-left-game(int player_id)  A player has left the active game  
game-set-maparea-ownership(int map_area_id, int player_id)  A map area has a new player ownership  
game-set-maparea-army-strength(int map_area_id, int army_strength)  A map area has a new army strength  
game-invalid-attack()  An attack was invalid  
game-attacked-area(int map_area_from_id, int map_area_to_id)  A map area was attacked (marks start of an attack)  
game-attack-dice-result(int dice_result1, dice_result2, dice_result3, dice_result4, dice_result5, dice_result6, dice_result7, dice_result8)  The result of an attack  
game-defense-dice-result(int dice_result1, dice_result2, dice_result3, dice_result4, dice_result5, dice_result6, dice_result7, dice_result8)  The result of an attack also marks the end of an attack  
game-attack-done()  An attack was completed (on all clients)  
game-player-turn-started(int player_id)  A player started his turn  
game-over(string title, string message)  The game is over  
game-player-message(int player_id, string message)  A player sent a game message  
game-system-message(string message)  The system sent an system message  




DiceWar Game Client to Server

NetGame Event  Description  Responses  
game-attack-area(int map_area_from_id, int map_area_to_id)  Attack an area  game-attacked-area 
game-attack-dice-result game-defense-dice-result game-set-maparea-ownership game-set-maparea-army-strength game-attack-done game-invalid-attack 
 
game-end-turn()  End player turn  game-player-turn-started  
game-battle-view-over()  Notify battle view is done  None  
game-add-message(string message)  Send a chat message to game chat  game-player-message  




World of Warcraft Events

Although World of Warcraft do not use ClanLib or the NetGame system, it does base it communication on sending events. So to illustrate that even very large scale games can be run almost entirely using such events, here's the list of events reverse engineered out of World of Warcraft: 
•http://www.wowwiki.com/Events_From_Disassembly 




Dispatching Game Events to C++ Functions

When a CL_NetGameEvent is received, we can attach a callback function to the event, or event group, that will handle the event. That's where the CL_NetGameEventDispatcher classes are handy. There are 4 constructors for the CL_NetGameEventDispatcher class, in order to dispatch from 0 to 3 extra arguments. 

Let's say that on the server side, we want to dissociate account operations events (sent by the client to register a new account), from the login events. 

Account operations handlers

Here we want to be able to handle 3 types of events : 
• Account-Creation events 
• Account-Modification events 
• Account-Deletion events 

We will then set each functions to handle each events : 


CL_NetGameEventDispatcher_v0 account_events;
account_events.func_event("Account-Creation").set(this, &Server::on_event_account_create);
account_events.func_event("Account-Modification").set(this, &Server::on_event_account_modify);
account_events.func_event("Account-Deletion").set(this, &Server::on_event_account_delete);

Login handlers

Here we want to handle 2 types of events : 
• Login events 
• Logout events 

The difference here is that we will want to be able to pass the CL_NetGameConnection attached to the client on the server, to the dispatcher as an extra argument. That is why we will use CL_NetGameEventDispatcher_v1< ContextParam > class. 

Same as before, we set the handlers : 


CL_NetGameEventDispatcher_v1< CL_NetGameConnection* > login_events;
login_events.func_event("Login").set(this, &Server::on_event_login);
login_events.func_event("Logout").set(this, &Server::on_event_logout);

Dispatching

Now that we've set our event handlers, we need to tell the system where to dispatch the events. This will be done for example in the on_event_received() method. 


void Server::on_event_received(CL_NetGameConnection *connection, const CL_NetGameEvent &e)
{
	cl_log_event("events", "Client sent event: %1", e.to_string());
 
	ServerUser *user = ServerUser::get_user(connection);
        //If a user is attached to the connection
	if(user)
	{
		bool handled_event = false;
 
		if (user->id == 0)	// User has not logged in, so route events to login dispatcher
		{
			// Route login events
			handled_event = login_events.dispatch(e, connection);
		}
		else
		{
			// Route account events
			handled_event = account_events.dispatch(e);
		}
 
		if (!handled_event)
		{
			// We received an event which we didn't hook up
			cl_log_event("events", "Unhandled event: %1", e.to_string());
		}
	}
}

The dispatcher will invoke the callback function that we set, or print that the event hasn't been handled. 

*/
