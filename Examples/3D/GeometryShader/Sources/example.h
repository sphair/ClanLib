/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "scene.h"
#include "graphic_store.h"

class ShaderDepth;
class ShaderColor;
class ParticleObject;
class Options;

class Model;

class App : public clan::Application
{
public:
	App();
	bool update() override;
private:
	void on_input_up(const InputEvent &key);
	void on_window_close();

private:
	void render(GraphicContext &gc);
	void render_depth_buffer(GraphicContext &gc);
	void create_scene(GraphicContext &gc);
	void calculate_matricies(GraphicContext &gc);
	void control_camera();
private:
	DisplayWindow window;
	SlotContainer sc;
	Canvas canvas;
	BlendState blend_disable_color_write;
	DepthStencilState depth_write_enabled;
	RasterizerState raster_state;
	Scene scene;

	SceneObject *camera;
	float camera_angle = 0.0f;

	ParticleObject *object_particles;

	bool enable_dual_pass = false;

	int time_delta;
	bool quit = false;
	clan::GameTime game_time;
	std::shared_ptr<GraphicStore> graphic_store;
	clan::Font font;
};

