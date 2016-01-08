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

#include "../state.h"
#include "../LinearParticle/L_ParticleSystem.h"
#include "framerate_counter.h"

class DemoMSmall : public DemoScreen
{
public:
	DemoMSmall(clan::DisplayWindow &window);

	bool update() override;

private:
	void on_key_up(const clan::InputEvent& key);
	void on_window_close();

private:
	clan::DisplayWindow window;
	clan::SlotContainer sc;
	clan::Canvas canvas;
	clan::Sprite surface;
	clan::Font font;
	FramerateCounter frameratecounter;
	bool quit = false;
	bool show_menu = true;
	L_MotionController motion_ctrl;
	std::unique_ptr<L_ExplosionEffect> effect;
	std::unique_ptr<L_Particle> particle;
	uint64_t last_time;

};
