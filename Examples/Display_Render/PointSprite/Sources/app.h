/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Mark Page
*/

#pragma once


typedef struct
{
	float xpos;
	float ypos;
	float xspeed;
	float yspeed;
	bool xdir;
	bool ydir;
} ParticlePosition;

class Options;

class App : public clan::Application
{
public:
	App();
	bool update() override;

private:
	void on_input_up(const clan::InputEvent &key);
	void on_window_close();
	void setup_particles();
	void move_particles(float time_diff, int num_particles);
private:
	clan::DisplayWindow window;
	clan::SlotContainer sc;
	clan::Canvas canvas;
	clan::BlendState blend_state;
	clan::ProgramObject program_object;
	clan::Image image_grid;
	clan::Texture2D texture_particle;
	clan::UIThread ui_thread;
	std::shared_ptr<Options> options;

	bool quit = false;
	float grid_space;
	float ball_alpha;

	clan::GameTime game_time;
	static const int max_particles = 1000;
	ParticlePosition particles[max_particles];

};
