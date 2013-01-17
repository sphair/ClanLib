/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Arkadiusz Kalinowski
*/

#pragma once
#include "Precomp.h"
#include <list>

class Gameobject;
class Player;
class Map;
class Message;

class Game
{
//_____________________________________________________________________________________
//																C O N S T R U C T O R S
public:
	Game();
	~Game();
	
//_________________________________________________________________________________
//																A T T R I B U T E S

	void get_screen_resolution(int &xRes, int &yRes);
	int get_width() { return screen_x_res; }
	int get_height() { return screen_y_res; }
	GraphicContext &get_gc();
	PhysicsContext &get_pc();
	ResourceManager &get_resources();

	Signal_v1<Canvas &>  get_draw_sig() { return draw_signal; }
	Signal_v1<int> get_update_sig() { return update_signal; }

//_________________________________________________________________________________
//																O P E R A T I O N S
public:

	void on_quit();
	void run();
	void update_game(int time_elapsed_ms);

	void on_key_down	(const InputEvent &key);
	void on_key_up		(const InputEvent &key);
	void on_pointer_move(const InputEvent &key);
	void on_pointer_down(const InputEvent &key);
	void on_pointer_up	(const InputEvent &key);

	void add_for_deletion(Gameobject *gameobject);
	void mute_music();
	void play_sample(int ID);

//_________________________________________________________________________________________
//																I M P L E M E N T A T I O N
private:

	//Core
	Player *playerP;
	Map *mapP;

	Signal_v1<Canvas &> draw_signal;
	Signal_v1<int> update_signal;

	PhysicsContext pc;
	GraphicContext gc;
	ResourceManager *resources_;
	std::list<Gameobject *> objects_for_deletion; 
	
	bool quit;
	int screen_x_res;
	int screen_y_res;
	float time_elapsed_since_last_enemy;

	//Sounds
	std::vector<SoundBuffer> samples;
	SoundBuffer_Session *music_session_;
	SoundBuffer_Session sound_session1;
	SoundBuffer_Session sound_session2;
	SoundBuffer_Session sound_session3;

	bool is_music_muted;
	int total_channels;
	int current_channel;
	int total_samples;
	


};