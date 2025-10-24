/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "model.h"

class ShaderDepth;
class ShaderBumpMap;
class Options;

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<CL_String> &args);

private:
	void on_input_up(const CL_InputEvent &key, const CL_InputState &state);
	void on_window_close();
	void wm_repaint();

private:
	void render(CL_GraphicContext &gc);
	void create_scene(CL_GraphicContext &gc);
	void update_light(CL_GraphicContext &gc, Options *options);
	void calculate_matricies(CL_GraphicContext &gc);
	void control_target(Options *options);
private:
	Scene scene;

	Model model_teapot;

	SceneObject *camera;
	SceneObject *light_distant;
	SceneObject *teapot_euler;
	SceneObject *rotation_euler_a;
	SceneObject *rotation_euler_b;
	SceneObject *rotation_euler_c;
	SceneObject *teapot_target;
	SceneObject *rotation_target_a;
	SceneObject *rotation_target_b;
	SceneObject *rotation_target_c;


	CL_Quaternionf initial_quaternion;
	CL_Quaternionf final_quaternion;

	int time_made_active;
	bool active_lerp;
	bool active_slerp;

	int current_time;
	int time_delta;
	bool quit;
};


