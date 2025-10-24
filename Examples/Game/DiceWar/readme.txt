Demonstrates the following in ClanLib:
- ClanDisplay
  - 2D
  - 3D
  - Shaders
- ClanGL
- ClanGUI
  - Custom theme
- ClanNetwork

LINUX:
To build dicewar_client:
	make -f MakeClient
To build dicewar_server:
	make -f MakeServer

To easily start the server, add a -debug command parameter when running it.

Models:
http://www.3dtotal.com//ffa/meshes_v2/low_poly/rendb01/model_main.htm#

To get an overview of the network events between server and clients,
check out Sources\Lib\net_events_*.h files.

TODO
----------------------------------------------------------
CODE POLISH:
- Cleanup GUI init
- Some better solution to GamePlayer/LobbyPlayer mess
- Event sending from AI
- Other TODOs around code

INGAME FEATURES:
- Add "quit to lobby" functionality
- Quit to lobby after End Game
- Battle - show on maps
- Multiple graphic detail levels

INGAME BUGS:
- Don't update map with result of battle before battleview is done
- Random crash on shutdown?
- Highlight colors on map are displayed wrong

INGAME POLISH:
- Improve army 3d y-placements
- GameView GUI subviews
- Beautify Island map

LOBBY FEATURES:
- SelectMap combobox - be able to select which map the game is played on
- Skin scrollbar
- Skin combobox
- Show a loading screen when starting game
- Select graphic detail level - Low, Medium, High 
  (selects between different heightmap textures)

LOBBY POLISH:
- Fix theme regarding to fonts and listview
- Split lobby_view into separate gui component classes

CLANLIB BUGS:
- ComboBox crashes when you click an item
- Slow uniforms
