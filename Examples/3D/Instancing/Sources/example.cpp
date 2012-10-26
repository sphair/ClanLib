/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "shader_color.h"
#include "scene_object.h"
#include "instanced_object.h"
#include "model.h"
#include "graphic_store.h"
#include "framerate_counter.h"

#if defined(_MSC_VER)
	#if !defined(_DEBUG)
		#if defined(_DLL)
			#pragma comment(lib, "assimp-static-mtdll.lib")
		#else
			#pragma comment(lib, "assimp-static-mt.lib")
		#endif
	#else
		#if defined(_DLL)
			#pragma comment(lib, "assimp-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "assimp-static-mt-debug.lib")
		#endif
	#endif
#endif

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	quit = false;

	OpenGLWindowDescription desc;
	desc.set_title("ClanLib Instancing Example");
	desc.set_size(Size(900, 700), true);
	desc.set_multisampling(4);
	desc.set_allow_resize(true);
	desc.set_depth_size(16);

	DisplayWindow window(desc);

	// Connect the Window close event
	Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	// Get the graphic context
	GraphicContext gc = window.get_gc();

	// Setup graphic store
	GraphicStore graphic_store(gc);
	scene.gs = &graphic_store;

	// Prepare the display
	gc.set_map_mode(cl_user_projection);

	gc.set_culled(true);
	gc.set_face_cull_mode(cl_cull_back);
	gc.set_front_face(cl_face_side_clockwise);

	create_scene(gc);

	FrameBuffer framebuffer(gc);

	camera_angle = 0.0f;

	Font font(gc, "tahoma", 24);

	FramerateCounter framerate_counter;

	unsigned int time_last = System::get_time();
	// Run until someone presses escape
	while (!quit)
	{
		framerate_counter.frame_shown();

		unsigned int time_now = System::get_time();
		time_delta = time_now - time_last;
		time_last = time_now;

		control_camera();
		update_light(gc);
		calculate_matricies(gc);

		gc.set_culled(true);
		render(gc);

		gc.set_modelview(Mat4f::identity());
		gc.set_map_mode(cl_map_2d_upper_left);
		gc.set_culled(false);
	
		std::string fps(string_format("Drawing %1 teapots (containing 2882 triangles). fps = %2", (int) object_teapot->centers.size(), framerate_counter.get_framerate()));
		font.draw_text(gc, 16-2, gc.get_height()-16-2, fps, Colorf(0.0f, 0.0f, 0.0f, 1.0f));
		font.draw_text(gc, 16, gc.get_height()-16-2, fps, Colorf(1.0f, 1.0f, 1.0f, 1.0f));

		// Use flip(1) to lock the fps
		window.flip(0);

		// This call processes user input and other events
		KeepAlive::process();
	}

	return 0;
}

// A key was pressed
void App::on_input_up(const InputEvent &key)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

void App::render(GraphicContext &gc)
{
	gc.set_map_mode(cl_user_projection);
	Rect viewport_rect(0, 0, Size(gc.get_width(), gc.get_height()));
	gc.set_viewport(viewport_rect);

	gc.set_projection(scene.gs->camera_projection);

	gc.set_depth_compare_function(cl_comparefunc_lequal);
	gc.enable_depth_write(true);
	gc.enable_depth_test(true);
	gc.enable_stencil_test(false);
	gc.enable_color_write(true);

	gc.clear(Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	gc.clear_depth(1.0f);

	Mat4f modelview_matrix = scene.gs->camera_modelview;
	scene.Draw(modelview_matrix, gc);
	gc.reset_program_object();
}

void App::create_scene(GraphicContext &gc)
{
	Model model_landscape(gc, scene.gs, "../Shadow/Resources/land.dae");
	model_landscape.SetMaterial(64.0f,	// Shiny
			Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// Emission
			Vec4f(1.0f, 1.0f, 1.0f, 1.0f),	// Ambient
			Vec4f(0.0f, 0.0f, 0.0f, 1.0f));	// Specular

	Model model_teapot(gc, scene.gs, "../Clan3D/Resources/teapot.dae");
	model_teapot.SetMaterial(64.0f,	// Shiny
			Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// Emission
			Vec4f(1.0f, 1.0f, 1.0f, 1.0f),	// Ambient
			Vec4f(1.0f, 1.0f, 1.0f, 1.0f));	// Specular

	camera = new SceneObject(scene, scene.base);
	camera->position = Vec3f(0.0f, 50.0f, -20.0f);
	camera->rotation_y = Angle(0.0f, cl_degrees);

	light_distant = new SceneObject(scene, scene.base);
	light_distant->rotation_y = Angle(45.0f, cl_degrees);
	light_distant->rotation_x = Angle(35.0f, cl_degrees);

	SceneObject *object_landscape = new SceneObject(scene, scene.base);
	object_landscape->model = model_landscape;
	object_landscape->position = Vec3f(0.0f, 0.0f, 0.0f);

	object_teapot = new InstancedObject(scene, object_landscape);
	object_teapot->model = model_teapot;
	object_teapot->position = Vec3f(0.0f, 50.0f, 0.0f);
	object_teapot->rotation_y = Angle(0.0f, cl_degrees);
	object_teapot->rotation_x = Angle(0.0f, cl_degrees);
	object_teapot->rotation_z = Angle(0.0f, cl_degrees);
	object_teapot->scale = Vec3f(4.0f, 4.0f, 4.0f);

	// Note - When changing the number of teapots here, remember to change this value in the shader
	// but check opengl capabilities first
	// http://www.opengl.org/registry/specs/ARB/uniform_buffer_object.txt
	// Ideally, the center and colors should be stored in a texture buffer
	// http://www.opengl.org/registry/specs/ARB/texture_buffer_object.txt
	// That is now part of the opengl 3.1 standard
	const int num_teapots = 256;

	const float teapot_scale_x = 24.0f;
	const float teapot_scale_y = 128.0f;
	const float teapot_scale_z = 24.0f;
	object_teapot->centers.reserve(num_teapots);
	for (int cnt=0; cnt < num_teapots; cnt++)
	{
		object_teapot->centers.push_back( Vec3f(
			((rand() & 0x1FF) - 0xFF) / teapot_scale_x,
			((rand() & 0x1FF) - 0xFF) / teapot_scale_y,
			((rand() & 0x1FF) - 0xFF) / teapot_scale_z ) );

		object_teapot->colors.push_back( Vec4f(
			(rand() & 0xFF) / 256.0f,
			(rand() & 0xFF) / 256.0f,
			(rand() & 0xFF) / 256.0f,
			1.0f));
	}

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

	scene.gs->shader_texture.SetLight(light_vector, light_specular, light_diffuse);
	scene.gs->shader_color.SetLight(light_vector, light_specular, light_diffuse);
}

void App::calculate_matricies(GraphicContext &gc)
{
	scene.gs->camera_projection = Mat4f::perspective(45.0f, ((float) gc.get_width()) / ((float) gc.get_height()), 0.1f, 1000.0f);

	float ortho_size = 60.0f / 2.0f;
	scene.gs->light_projection = Mat4f::ortho(-ortho_size, ortho_size, -ortho_size, ortho_size, 0.1f, 1000.0f);

	scene.gs->camera_modelview = Mat4f::identity();
	camera->GetWorldMatrix(scene.gs->camera_modelview);
	scene.gs->camera_modelview.scale_self(1.0f, 1.0f, -1.0f);
	scene.gs->camera_modelview.inverse();

	scene.gs->light_modelview = Mat4f::identity();
	light_distant->GetWorldMatrix(scene.gs->light_modelview);
	scene.gs->light_modelview.scale_self(1.0f, 1.0f, -1.0f);
	scene.gs->light_modelview.inverse();
}

void App::wm_repaint()
{
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

	camera->position = Vec3f(xpos, 70.0f, zpos);

	camera->rotation_x = Angle(20.0f, cl_degrees);
	camera->rotation_y = Angle(-(camera_angle+90.0f), cl_degrees);

}
