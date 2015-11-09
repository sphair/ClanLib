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
#include "shader.h"
#include "shader_texture.h"
#include "scene_object.h"
#include "model.h"
#include "graphic_store.h"

#if defined(_MSC_VER)
	#if !defined(_DEBUG)
		#if defined(_DLL)
			#pragma comment(lib, "assimp-static-mtdll.lib")
			#pragma comment(lib, "zlib-static-mtdll.lib")
		#else
			#pragma comment(lib, "assimp-static-mt.lib")
			#pragma comment(lib, "zlib-static-mt.lib")
		#endif
	#else
		#if defined(_DLL)
			#pragma comment(lib, "assimp-static-mtdll-debug.lib")
			#pragma comment(lib, "zlib-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "assimp-static-mt-debug.lib")
			#pragma comment(lib, "zlib-static-mt-debug.lib")
		#endif
	#endif
#endif

clan::ApplicationInstance<App> clanapp;

App::App()
{
#ifdef WIN32
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

    DisplayWindowDescription desc;

	desc.set_title("ClanLib Object 3D Example");
	desc.set_size(Size(640, 480), true);
	desc.set_multisampling(4);
	desc.set_allow_resize(true);
	desc.set_depth_size(16);

	window = DisplayWindow(desc);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::on_window_close));

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));

	canvas = Canvas(window);

	// Setup graphic store
	graphic_store = std::make_shared<GraphicStore>(canvas);
	scene.gs = graphic_store.get();

	// Prepare the display
	RasterizerStateDescription rasterizer_state_desc;
	rasterizer_state_desc.set_culled(true);
	rasterizer_state_desc.set_face_cull_mode(cull_back);
	rasterizer_state_desc.set_front_face(face_clockwise);
	raster_state = RasterizerState(canvas, rasterizer_state_desc);

	DepthStencilStateDescription depth_state_desc;
	depth_state_desc.enable_depth_write(true);
	depth_state_desc.enable_depth_test(true);
	depth_state_desc.enable_stencil_test(false);
	depth_state_desc.set_depth_compare_function(compare_lequal);
	depth_write_enabled = DepthStencilState(canvas, depth_state_desc);
	
  	create_scene(canvas);

	game_time.reset();
}

bool App::update()
{
	game_time.update();
	int time_delta_ms = game_time.get_time_elapsed_ms();

	canvas.clear(Colorf::black);
	canvas.clear_depth(1.0f);

	angle += time_delta_ms / 10.0f;
	if (angle >= 360.0f)
		angle -= 360.0f;

	scene_teapot->rotation_y.set_degrees(angle);
	scene_clanlib->rotation_y.set_degrees(angle);
	scene_tuxball->rotation_y.set_degrees(angle);

	// Render the scene using euler angles
	calculate_matricies(canvas);
	update_light(canvas);

	canvas.set_depth_stencil_state(depth_write_enabled);
	canvas.set_rasterizer_state(raster_state);
	render(canvas);

	canvas.reset_rasterizer_state();
	canvas.reset_depth_stencil_state();
		
	// Flip the display, showing on the screen what we have drawed
	// since last call to flip()
	window.flip(1);

	return !quit;
}
void App::render(GraphicContext &gc)
{
	gc.clear(Colorf(0.2f, 0.2f, 0.2f, 1.0f));

	Rect viewport_rect(0, 0, Size(gc.get_width(), gc.get_height()));
	gc.set_viewport(viewport_rect);

	gc.clear_depth(1.0f);

	Mat4f modelview_matrix = scene.gs->camera_modelview;
	scene.Draw(modelview_matrix, gc);
	gc.reset_program_object();
}

void App::create_scene(GraphicContext &gc)
{

	model_teapot = Model(gc, scene.gs, "Resources/teapot.dae", false);
	model_clanlib = Model(gc, scene.gs, "Resources/clanlib.dae", false);
	model_tuxball = Model(gc, scene.gs, "Resources/tux_ball.dae", true);

	camera = new SceneObject(scene, scene.base);
	camera->position = Vec3f(0.0f, 1.0f, 0.0f);
	camera->rotation_x = Angle(30.0f, angle_degrees);

	light_distant = new SceneObject(scene, scene.base);
	light_distant->position = Vec3f(0.0f, 32.0f, 20.0f);
	light_distant->rotation_y = Angle(45.0f, angle_degrees);
	light_distant->rotation_x = Angle(35.0f, angle_degrees);

	scene_teapot = new SceneObject(scene, scene.base);
	scene_teapot->model = model_teapot;
	scene_teapot->position = Vec3f(0.0f, 0.0f, 2.0f);
	scene_teapot->scale = Vec3f(1.0f, 1.0f, 1.0f);

	scene_clanlib = new SceneObject(scene, scene.base);
	scene_clanlib->model = model_clanlib;
	scene_clanlib->position = Vec3f(0.0f, -3.0f, 5.0f);
	scene_clanlib->scale = Vec3f(1.0f, 1.0f, 1.0f);

	scene_tuxball = new SceneObject(scene, scene.base);
	scene_tuxball->model = model_tuxball;
	scene_tuxball->position = Vec3f(0.7f, 0.5f, 2.0f);
	scene_tuxball->scale = Vec3f(0.05f, 0.05f, 0.05f);

	model_tuxball.SetMaterial(
			64,		// material_shininess
			Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// material_emission
			Vec4f(0.0f, 1.0f, 0.0f, 1.0f),	// material_ambient
			Vec4f(0.0f, 1.0f, 0.0f, 1.0f)	//material_specular
			);

	model_teapot.SetMaterial(
			64,		// material_shininess
			Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// material_emission
			Vec4f(1.0f, 1.0f, 1.0f, 1.0f),	// material_ambient
			Vec4f(1.0f, 1.0f, 1.0f, 1.0f)	//material_specular
			);

	scene.gs->LoadImages(gc);

}


void App::update_light(GraphicContext &gc)
{
	Mat4f light_modelview_matrix = Mat4f::identity();
	light_distant->GetWorldMatrix(light_modelview_matrix);

	Mat4f work_matrix = scene.gs->camera_modelview;
	work_matrix = work_matrix * light_modelview_matrix;

	// Clear translation
	work_matrix.matrix[0+3*4] = 0.0f;
	work_matrix.matrix[1+3*4] = 0.0f;
	work_matrix.matrix[2+3*4] = 0.0f;

	Vec3f light_vector = work_matrix.get_transformed_point(Vec3f(0.0f, 0.0f, -1.0f));

	Vec4f light_specular(0.5f, 0.5f, 0.5f, 1.0f);
	Vec4f light_diffuse(0.5f, 0.5f, 0.5f, 1.0f);
	Vec4f light_ambient(0.2f, 0.2f, 0.0f, 1.0f);

	scene.gs->shader.SetLight(light_vector, light_specular, light_diffuse, light_ambient);
	scene.gs->shader_texture.SetLight(light_vector, light_specular, light_diffuse, light_ambient);
}

void App::calculate_matricies(GraphicContext &gc)
{
	scene.gs->camera_projection = Mat4f::perspective(45.0f, ((float) gc.get_width()) / ((float) gc.get_height()), 0.1f, 1000.0f, handed_left, gc.get_clip_z_range());

	float ortho_size = 60.0f / 2.0f;
	scene.gs->light_projection = Mat4f::ortho(-ortho_size, ortho_size, -ortho_size, ortho_size, 0.1f, 1000.0f, handed_left, gc.get_clip_z_range());

	scene.gs->camera_modelview = Mat4f::identity();
	camera->GetWorldMatrix(scene.gs->camera_modelview);
	scene.gs->camera_modelview.inverse();

	scene.gs->light_modelview = Mat4f::identity();
	light_distant->GetWorldMatrix(scene.gs->light_modelview);
	scene.gs->light_modelview.inverse();
}

// A key was pressed
void App::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

