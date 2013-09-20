
/*! \page NetGameEngine NetGame Engine

The network library in ClanLib sports a simple event based networking engine usually referred to as the NetGame engine. In all its simplicity the engine assist in routing simple messages from a client to a server and back. 

The API consists of the following classes: 
<ul><li>\ref clan::NetGameServer </li></ul>
<ul><li>\ref clan::NetGameClient </li></ul>
<ul><li>\ref clan::NetGameConnection </li></ul>
<ul><li>\ref clan::NetGameEvent </li></ul>
<ul><li>\ref clan::NetGameEventDispatcher </li></ul>


<h2>Client and Server Connections</h2>
On the server side, the game must construct an instance of \ref clan::NetGameServer. Then for each port that the server should listen on, it must call \ref clan::NetGameServer::start(port). 
<br/>

The class also features three important signals: 
<ul><li>Client connected </li></ul>
<ul><li>Client disconnected </li></ul>
<ul><li>Event received </li></ul>

\ref clan::NetGameServer is a \ref clan::KeepAlive based class, which means that during calls to \ref clan::KeepAlive::process() the class will emit those three signals for each client that has connected/disconnected since last call, and for each game event that arrived since last. 
<br/>

It is also possible to call process_events() on the \ref clan::NetGameServer or \ref clan::NetGameClient objects directly to cause the signals to be emitted. Generally, in client applications the signals are processed automatically because the main render loop already calls the clan::KeepAlive::process() function, but for servers that to not use clanDisplay the process_events() function can be called directly. 
<br/>

The \ref clan::NetGameClient class is similar in design, except that there is only one connection (to the server), and therefore the connect/disconnect signals do not include any \ref clan::NetGameConnection objects. 
<br/>

<h2>NetGame Events</h2>

The class clan::NetGameEvent represents a game event message that is either sent or received. Each event consists of two things: an event name and list of event parameters. The name is what identifies what the event is about, while each of the parameters can one of the following types: 
<ul><li>null </li></ul>
<ul><li>int </li></ul>
<ul><li>string </li></ul>
<ul><li>bool </li></ul>
<ul><li>float </li></ul>

Lets say that the client wants to send a log on message to the server. We want to pass a username and a password as the arguments, so we do like this: 

\code
clan::NetGameClient client;
client.connect("example.net", "5555");
 
clan::NetGameEvent event("logon", "Mr. Jones", "the secret of all secrets");
client.send_event(event);
\endcode

The constructor for \ref clan::NetGameEvent exist in 6 versions, allowing you to pass anything from 0 to 5 parameters in a single line. The types of the parameters are automatically deducted from the C++ type you pass for each parameter. If you need to pass more parameters than this, then you need to use the add_argument(const \ref clan::NetGameEventValue &value) function on \ref clan::NetGameEvent. 

On the server, this game event will be emitted via the sig_event_received, and might be handled like this: 

\code
void on_event_received(clan::NetGameConnection *connection, const clan::NetGameEvent &event)
{
	MyConnectionData *data = get_connection_data(connection);
	if (event.get_name() == "logon")
	{
		if (data->logged_in)
		{
			clan::NetGameEvent response("error", "You are already logged in!");
			connection->send_event(response);
		}
		else
		{
			data->username = event.get_argument(0);
			clan::String password = event.get_argument(1);
			data->logged_in = true;
 
			clan::NetGameEvent response("motd", cl_format("Welcome %1! Please enjoy your stay!", username));
			connection->send_event(response);
		}
	}
}
\endcode

An example of a NetGame based server can be found in the ClanLib examples. 

<h2>Example Events</h2>

To illustrate how a game can be built using this system, the following listing shows what events are used in the DiceWar example while the game itself is running: 

DiceWar Game Server to Client
<br/>

<ul><li>game-is-starting()  The game is starting, client does initialization  </li></ul>
<ul><li>game-has-started()  The game has started -> ready to play!</li></ul>  
<ul><li>game-player-joined-game(int player_id, string player_name, int visual_id)  A new player has joined the active game  </li></ul>
<ul><li>game-player-left-game(int player_id)  A player has left the active game</li></ul>  
<ul><li>game-set-maparea-ownership(int map_area_id, int player_id)  A map area has a new player ownership  </li></ul>
<ul><li>game-set-maparea-army-strength(int map_area_id, int army_strength)  A map area has a new army strength</li></ul>  
<ul><li>game-invalid-attack()  An attack was invalid</li></ul>  
<ul><li>game-attacked-area(int map_area_from_id, int map_area_to_id)  A map area was attacked (marks start of an attack)  </li></ul>
<ul><li>game-attack-dice-result(int dice_result1, dice_result2, dice_result3, dice_result4, dice_result5, dice_result6, dice_result7, dice_result8)  The result of an attack  </li></ul>
<ul><li>game-defense-dice-result(int dice_result1, dice_result2, dice_result3, dice_result4, dice_result5, dice_result6, dice_result7, dice_result8)  The result of an attack also marks the end of an attack  </li></ul>
<ul><li>game-attack-done()  An attack was completed (on all clients)</li></ul>  
<ul><li>game-player-turn-started(int player_id)  A player started his turn</li></ul>  
<ul><li>game-over(string title, string message)  The game is over</li></ul>  
<ul><li>game-player-message(int player_id, string message)  A player sent a game message  </li></ul>
<ul><li>game-system-message(string message)  The system sent an system message</li></ul>  

<br/>

DiceWar Game Client to Server
<br/>

<ul><li>game-attack-area(int map_area_from_id, int map_area_to_id)  Attack an area  --> game-attacked-area </li></ul>
<ul><li>game-attack-dice-result game-defense-dice-result game-set-maparea-ownership game-set-maparea-army-strength game-attack-done --> game-invalid-attack </li></ul>
<ul><li>game-end-turn()  End player turn --? game-player-turn-started</li></ul>  
<ul><li>game-battle-view-over()  Notify battle view is done  --> None</li></ul>  
<ul><li>game-add-message(string message)  Send a chat message to game chat --> game-player-message  </li></ul>


<br/>


World of Warcraft Events
<br/>

Although World of Warcraft do not use ClanLib or the NetGame system, it does base it communication on sending events. So to illustrate that even very large scale games can be run almost entirely using such events, here's the list of events reverse engineered out of World of Warcraft: 
http://www.wowwiki.com/Events_From_Disassembly 

<br/>



<h2>Dispatching Game Events to C++ Functions</h2>

When a \ref clan::NetGameEvent is received, we can attach a callback function to the event, or event group, that will handle the event. That's where the \ref clan::NetGameEventDispatcher classes are handy. There are 4 constructors for the \ref clan::NetGameEventDispatcher class, in order to dispatch from 0 to 3 extra arguments. 
<br/>

Let's say that on the server side, we want to dissociate account operations events (sent by the client to register a new account), from the login events. 
<br/>

Account operations handlers
<br/>

Here we want to be able to handle 3 types of events : 
<ul><li>Account-Creation events </li></ul>  
<ul><li>Account-Modification events </li></ul>  
<ul><li>Account-Deletion events </li></ul>  
<br/>

We will then set each functions to handle each events : 

\code
clan::NetGameEventDispatcher_v0 account_events;
account_events.func_event("Account-Creation").set(this, &Server::on_event_account_create);
account_events.func_event("Account-Modification").set(this, &Server::on_event_account_modify);
account_events.func_event("Account-Deletion").set(this, &Server::on_event_account_delete);
\endcode

<h2>Login handlers</h2>

Here we want to handle 2 types of events : 
<ul><li>Login events </li></ul>  
<ul><li>Logout events </li></ul>  

The difference here is that we will want to be able to pass the \ref clan::NetGameConnection attached to the client on the server, to the dispatcher as an extra argument. That is why we will use \ref clan::NetGameEventDispatcher_v1< ContextParam > class. 
<br/>

Same as before, we set the handlers : 
<br/>

\code
clan::NetGameEventDispatcher_v1< clan::NetGameConnection* > login_events;
login_events.func_event("Login").set(this, &Server::on_event_login);
login_events.func_event("Logout").set(this, &Server::on_event_logout);
\endcode

<h2>Dispatching</h2>

Now that we've set our event handlers, we need to tell the system where to dispatch the events. This will be done for example in the on_event_received() method. 

\code
void Server::on_event_received(clan::NetGameConnection *connection, const clan::NetGameEvent &e)
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
\endcode

The dispatcher will invoke the callback function that we set, or print that the event hasn't been handled. 

*/
