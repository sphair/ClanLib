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
#include "shader_depth.h"
#include "scene_object.h"
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

	DisplayWindowDescription desc;
	desc.set_title("ClanLib Shadow Example");
	desc.set_size(Size(640, 640), true);
	desc.set_multisampling(4);
	desc.set_depth_size(16);

	DisplayWindow window(desc);

	// Connect the Window close event
	Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	// Get the graphic context
	GraphicContext gc = window.get_gc();

	GraphicStore graphic_store(gc);
	scene.gs = &graphic_store;

	// Prepare the display
	gc.set_map_mode(cl_user_projection);

	gc.set_culled(true);
	gc.set_face_cull_mode(cl_cull_back);
	gc.set_front_face(cl_face_side_clockwise);

	create_scene(gc);

	FrameBuffer framebuffer(gc);

	Texture2D new_depth_texture(gc, Size(1024, 1024), cl_depth_component);
	new_depth_texture.set_wrap_mode(wrap_clamp_to_edge, wrap_clamp_to_edge, wrap_clamp_to_edge);
	framebuffer.attach_depth_buffer(new_depth_texture);

	scene.gs->texture_shadow = new_depth_texture;

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

		rotate_teapot();
		control_camera();
		update_light(gc);

		calculate_matricies(gc);

		render_from_lightsource(gc, framebuffer);
		render_from_camera(gc, framebuffer);

		gc.set_modelview(Mat4f::identity());
		gc.set_map_mode(map_2d_upper_left);

		std::string fps(string_format("%1 fps", framerate_counter.get_framerate()));
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


void App::render_from_lightsource(GraphicContext &gc, FrameBuffer &framebuffer)
{
	PrimitivesArray prim_array(gc);

	gc.set_frame_buffer(framebuffer);

	gc.set_map_mode(cl_user_projection);

	Rect viewport_rect2(0, 0, Size(scene.gs->texture_shadow.get_width(), scene.gs->texture_shadow.get_height()));
	gc.set_viewport(viewport_rect2);

	Mat4f perp = Mat4f::perspective(67.0f, 1.0f, 0.1f, 1000.0f);
	gc.set_projection(scene.gs->light_projection);

	gc.set_depth_compare_function(cl_comparefunc_lequal);

	gc.enable_depth_write(true);
	gc.enable_depth_test(true);
	gc.enable_stencil_test(false);
	gc.enable_color_write(false);

	gc.clear_depth(1.0f);

	Mat4f modelview_matrix = scene.gs->light_modelview;
	scene.Draw(modelview_matrix, gc, true);

	gc.reset_program_object();

	gc.set_modelview(Mat4f::identity());
	gc.set_map_mode(MapMode(map_2d_upper_left));

	gc.reset_frame_buffer();
}

void App::render_from_camera(GraphicContext &gc, FrameBuffer &framebuffer)
{
	gc.set_map_mode(cl_user_projection);
	Rect viewport_rect(0, 0, Size(gc.get_width(), gc.get_height()));
	gc.set_viewport(viewport_rect);

	gc.set_projection(scene.gs->camera_projection);
//	gc.set_projection(scene.gs->light_projection);

	gc.set_depth_compare_function(cl_comparefunc_lequal);
	gc.enable_depth_write(true);
	gc.enable_depth_test(true);
	gc.enable_stencil_test(false);
	gc.enable_color_write(true);

	gc.clear(Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	gc.clear_depth(1.0f);

	Mat4f modelview_matrix = scene.gs->camera_modelview;
//	Mat4f modelview_matrix = scene.gs->light_modelview;
	scene.Draw(modelview_matrix, gc, false);
	gc.reset_program_object();
}

void App::create_scene(GraphicContext &gc)
{
	Model model_teapot(gc, scene.gs, "../Clan3D/Resources/teapot.dae", true);
	Model model_landscape(gc, scene.gs, "Resources/land.dae", false);
	Model model_tree(gc, scene.gs, "Resources/tree.dae", false);
	Model model_gear(gc, scene.gs, "../Clan3D/Resources/gear.dae", false);

	model_teapot.SetMaterial(
		32.0f,	// shininess
		Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// emission
		Vec4f(1.5f, 1.5f, 0.5f, 1.0f),	// ambient
		Vec4f(0.5f, 0.5f, 0.5f, 1.0f) );	// specular

	camera = new SceneObject(scene, scene.base);
	camera->position = Vec3f(0.0f, 50.0f, -20.0f);
	camera->rotation_y = Angle(0.0f, angle_degrees);
	camera->scale = Vec3f(1.0f, 1.0f, 1.0f);

	light = new SceneObject(scene, scene.base);
	light->position = Vec3f(-20.4732f, 48.7872f, -20.5439f);
	light->rotation_y = Angle(45.0f, angle_degrees);
	light->rotation_x = Angle(35.0f, angle_degrees);
	light->scale = Vec3f(1.0f, 1.0f, 1.0f);

	SceneObject *object_landscape = new SceneObject(scene, scene.base);
	object_landscape->model = model_landscape;
	object_landscape->position = Vec3f(0.0f, 0.0f, 0.0f);

	object_teapot1 = new SceneObject(scene, object_landscape);
	object_teapot1->model = model_teapot;
	object_teapot1->position = Vec3f(-8.2f, 37.3f, 8.0f);
	object_teapot1->scale = Vec3f(8.0f, 8.0f, 8.0f);

	object_teapot2 = new SceneObject(scene, object_landscape);
	object_teapot2->model = model_teapot;
	object_teapot2->position = Vec3f(2.0f, 40.0f, 1.0f);
	object_teapot2->scale = Vec3f(8.0f, 8.0f, 8.0f);

	SceneObject *object_tree = new SceneObject(scene, object_landscape);
	object_tree->model = model_tree;
	object_tree->position = Vec3f(-7.2701f, 25.58f, -9.0932f);

	object_tree = new SceneObject(scene, object_landscape);
	object_tree->model = model_tree;
	object_tree->position = Vec3f(-11.829f, 25.8125f, 0.0f);

	object_tree = new SceneObject(scene, object_landscape);
	object_tree->model = model_tree;
	object_tree->position = Vec3f(-5.0f, 27.6963f, 0.0f);

	object_tree = new SceneObject(scene, object_landscape);
	object_tree->model = model_tree;
	object_tree->position = Vec3f(0.0f, 29.4237f, 0.0f);

	object_gear = new SceneObject(scene, object_landscape);
	object_gear->model = model_gear;
	object_gear->position = Vec3f(10.0f, 40.58f, 10.0);
	object_gear->scale = Vec3f(3.0f, 3.0f, 3.0f);

	scene.gs->LoadImages(gc);

}

void App::control_camera()
{
	camera_angle += ((float) time_delta) / 20.0f;

	if (camera_angle > 360.0f)
	{
		camera_angle -= 360.0f;
	}

	float camera_angle_rad = (camera_angle * PI) / 180.0f;

	float radius = 50.0f;
	float xpos = cos(camera_angle_rad) * radius;
	float zpos = sin(camera_angle_rad) * radius;

	camera->position = Vec3f(xpos, 50.0f, zpos);

	camera->rotation_x = Angle(20.0f, angle_degrees);
	camera->rotation_y = Angle(-(camera_angle+90.0f), angle_degrees);

}

void App::update_light(GraphicContext &gc)
{
	Mat4f light_modelview_matrix = Mat4f::identity();
	light->GetWorldMatrix(light_modelview_matrix);

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
	light->GetWorldMatrix(scene.gs->light_modelview);
	scene.gs->light_modelview.scale_self(1.0f, 1.0f, -1.0f);
	scene.gs->light_modelview.inverse();

	scene.gs->shadow_matrix = scene.gs->light_projection * scene.gs->light_modelview;
}

void App::rotate_teapot()
{
	float delta = ((float) time_delta) / 5.0f;

	object_teapot1->rotation_y = Angle(object_teapot1->rotation_y.to_degrees() + delta, angle_degrees);

	object_teapot2->rotation_y = Angle(object_teapot2->rotation_y.to_degrees() - delta, angle_degrees);

	object_gear->rotation_z = Angle(object_gear->rotation_z.to_degrees() + delta, angle_degrees);


}
