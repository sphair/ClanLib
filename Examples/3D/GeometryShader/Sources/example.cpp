/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "precomp.h"
#include "example.h"
#include "scene_object.h"
#include "particle_object.h"
#include "graphic_store.h"

clan::ApplicationInstance<App> clanapp;

App::App()
{
	clan::OpenGLTarget::set_current();

	quit = false;

	DisplayWindowDescription desc;
	desc.set_title("ClanLib Geometry Shader Example");
	desc.set_size(Size(900, 700), true);
	desc.set_multisampling(0);
	desc.set_allow_resize(false);
	desc.set_depth_size(16);

	window = DisplayWindow(desc);
 
	// Connect the Window close event
	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::on_window_close));

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));

	canvas = Canvas(window);

	// Setup graphic store
	graphic_store = std::make_shared<GraphicStore>(canvas);
	scene.gs = graphic_store.get();

	RasterizerStateDescription rasterizer_state_desc;
	rasterizer_state_desc.set_culled(false);
	rasterizer_state_desc.set_face_cull_mode(cull_back);
	rasterizer_state_desc.set_front_face(face_clockwise);
	raster_state = RasterizerState(canvas, rasterizer_state_desc);

	DepthStencilStateDescription depth_state_desc;
	depth_state_desc.enable_depth_write(true);
	depth_state_desc.enable_depth_test(true);
	depth_state_desc.enable_stencil_test(false);
	depth_state_desc.set_depth_compare_function(compare_lequal);

	depth_write_enabled = DepthStencilState(canvas, depth_state_desc);

	BlendStateDescription blend_state_desc;
	blend_state_desc.enable_color_write(false, false, false, false);
	blend_disable_color_write = BlendState(canvas, blend_state_desc);

	create_scene(canvas);

	font = clan::Font("tahoma", 24);

	game_time.reset();
}

bool App::update()
{
	game_time.update();
	time_delta = game_time.get_time_elapsed_ms();

	control_camera();
	calculate_matricies(canvas);

	canvas.clear_depth(1.0f);

	canvas.set_depth_stencil_state(depth_write_enabled);
	canvas.set_rasterizer_state(raster_state);

	if (enable_dual_pass)
	{
		canvas.set_blend_state(blend_disable_color_write);
		graphic_store->texture_ball = graphic_store->texture_alpha;
		render_depth_buffer(canvas);	// Render to depth buffer first, to fake sorting the particles
		canvas.reset_blend_state();
	}
	else
	{
		graphic_store->texture_ball = graphic_store->texture_solid;
	}

	render(canvas);	// Render scene
	canvas.reset_rasterizer_state();
	canvas.reset_depth_stencil_state();

	std::string fps(string_format("fps = %1", clan::StringHelp::float_to_text(game_time.get_updates_per_second(), 1)));
	font.draw_text(canvas, 16-2, canvas.get_height()-16-2, fps, Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	font.draw_text(canvas, 16, canvas.get_height()-16-2, fps, Colorf(1.0f, 1.0f, 1.0f, 1.0f));

	std::string info(string_format("Drawing %1 particles", ParticleObject::num_points));
	font.draw_text(canvas, 16, 30, info);

	font.draw_text(canvas, 16, 64, enable_dual_pass ? "Using 2 Render Passes (Press Space to toggle)" : "Using Single Pass (Press Space to toggle)");

	// Use flip(1) to lock the fps
	window.flip(0);

	return !quit;
}

// A key was pressed
void App::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
	if (key.id == keycode_space)
		enable_dual_pass = !enable_dual_pass;
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

void App::render_depth_buffer(GraphicContext &gc)
{
	gc.set_viewport(scene.gs->texture_depth.get_size());
	Mat4f modelview_matrix = scene.gs->camera_modelview;
	scene.Draw(modelview_matrix, gc);
	gc.reset_program_object();
}

void App::render(GraphicContext &gc)
{
	gc.set_viewport(gc.get_size());

	gc.clear(Colorf(0.0f, 0.0f, 0.0f, 1.0f));

	Mat4f modelview_matrix = scene.gs->camera_modelview;
	scene.Draw(modelview_matrix, gc);
	gc.reset_program_object();
}

void App::create_scene(GraphicContext &gc)
{
	camera = new SceneObject(scene, scene.base);
	camera->position = Vec3f(0.0f, 50.0f, -20.0f);
	camera->rotation_y = Angle(0.0f, angle_degrees);

	object_particles = new ParticleObject(gc, scene, scene.base);
	object_particles->position = Vec3f(-110.0f, 54.0f, -5.0f);
	object_particles->rotation_y = Angle(0.0f, angle_degrees);
	object_particles->rotation_x = Angle(0.0f, angle_degrees);
	object_particles->rotation_z = Angle(0.0f, angle_degrees);
	object_particles->scale = Vec3f(1.0f, 1.0f, 1.0f);
}

void App::calculate_matricies(GraphicContext &gc)
{
	scene.gs->camera_projection = Mat4f::perspective(45.0f, ((float) gc.get_width()) / ((float) gc.get_height()), 0.1f, 1000.0f, handed_left, clip_negative_positive_w);

	scene.gs->camera_modelview = Mat4f::identity();
	camera->GetWorldMatrix(scene.gs->camera_modelview);
	scene.gs->camera_modelview.inverse();

}

void App::control_camera()
{
	camera_angle += ((float) time_delta) / 20.0f;

	if (camera_angle > 360.0f)
	{
		camera_angle -= 360.0f;
	}

	float camera_angle_rad = (camera_angle * PI) / 180.0f;

	float radius = 100.0f;
	float xpos = cos(camera_angle_rad) * radius;
	float zpos = sin(camera_angle_rad) * radius;

	xpos -= 100.0f;

	camera->position = Vec3f(xpos, 100.0f, zpos);

	camera->rotation_x = Angle(20.0f, angle_degrees);
	camera->rotation_y = Angle(-(camera_angle+90.0f), angle_degrees);

}
