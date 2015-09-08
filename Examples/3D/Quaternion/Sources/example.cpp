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
#include "shader_color.h"
#include "scene_object.h"
#include "model.h"
#include "graphic_store.h"
#include "options.h"

#if defined(I_LOVE_ASSIMP_AND_PRECOMPILED_IT)
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
#endif

clan::ApplicationInstance<App> clanapp;

App::App()
{
	// We support all display targets, in order listed here
	clan::D3DTarget::set_current();
	clan::OpenGLTarget::set_current();

	DisplayWindowDescription desc;
	desc.set_title("ClanLib Quaternion's Example");
	desc.set_size(Size(900, 700), true);
	desc.set_multisampling(4);
	desc.set_allow_resize(true);
	desc.set_depth_size(16);

	window = DisplayWindow(desc);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::on_window_close));

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));

	canvas = Canvas(window);

	FileResourceDocument doc(FileSystem("../../ThemeAero"));
	ResourceManager resources = FileResourceManager::create(doc);
	ui_thread = UIThread(resources);

	options_view = std::make_shared<Options>(canvas);
	options_view->set_always_render();
	options_view->set_window(window);

	// Setup graphic store
	graphic_store = std::make_shared<GraphicStore>(canvas);
	scene.gs = graphic_store.get();

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

	font = clan::Font("tahoma", 24);

	time_last = System::get_time();
	time_start = time_last;
}

bool App::update()
{
	framerate_counter.frame_shown();

	// Calculate time since last frame
	uint64_t time_now = System::get_time();
	current_time = time_now - time_start;
	time_delta = (int)(time_now - time_last);
	time_last = time_now;

	// Control the target options
	control_target(options_view.get());

	// Use the euler angle options
	rotation_euler_a->rotation_y = options_view->rotation_y;
	rotation_euler_b->rotation_x = options_view->rotation_x;
	rotation_euler_c->rotation_z = options_view->rotation_z;

	teapot_euler->rotation_x = options_view->rotation_x;
	teapot_euler->rotation_y = options_view->rotation_y;
	teapot_euler->rotation_z = options_view->rotation_z;

	// Use the target angle options
	rotation_target_a->rotation_y = options_view->target_y;
	rotation_target_b->rotation_x = options_view->target_x;
	rotation_target_c->rotation_z = options_view->target_z;

	teapot_target->rotation_x = options_view->target_x;
	teapot_target->rotation_y = options_view->target_y;
	teapot_target->rotation_z = options_view->target_z;

	// Render the scene using euler angles
	calculate_matricies(canvas);
	update_light(canvas);

	canvas.set_depth_stencil_state(depth_write_enabled);
	canvas.set_rasterizer_state(raster_state);
	render(canvas);

	// Show the quaternion teapot
	Mat4f modelview_matrix = scene.gs->camera_modelview;
	modelview_matrix.translate_self(0.0f, 0.0f, 0.0f);
	modelview_matrix = modelview_matrix * options_view->quaternion.to_matrix();
	modelview_matrix.scale_self(5.0f, 5.0f, 5.0f);
	model_teapot.Draw(canvas, scene.gs, modelview_matrix);

	// Draw information boxes
	canvas.reset_rasterizer_state();
	canvas.reset_depth_stencil_state();

	options_view->set_viewport(Rectf(8.0f, 8.0f, canvas.get_width() - 16.0f, 170.0f));
	options_view->update();
	
	std::string fps(string_format("%1 fps", framerate_counter.get_framerate()));
	font.draw_text(canvas, 16-2, canvas.get_height()-16-2, fps, Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	font.draw_text(canvas, 16, canvas.get_height()-16-2, fps, Colorf(1.0f, 1.0f, 1.0f, 1.0f));

	font.draw_text(canvas, 60, 250, "Euler Orientation");
	font.draw_text(canvas, 330, 250, "Quaternion Orientation");
	font.draw_text(canvas, 600, 250, "Target Euler Orientation");
	font.draw_text(canvas, 16, 630, "(Using YXZ rotation order)");

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
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

void App::render(GraphicContext &gc)
{
	gc.clear(Colorf(0.0f, 0.0f, 0.0f, 1.0f));

	Rect viewport_rect(0, 0, Size(gc.get_width(), gc.get_height()));
	gc.set_viewport(viewport_rect);

	gc.clear_depth(1.0f);

	Mat4f modelview_matrix = scene.gs->camera_modelview;
	scene.Draw(modelview_matrix, gc);
	gc.reset_program_object();
}

void App::create_scene(GraphicContext &gc)
{

	model_teapot = Model(gc, scene.gs, "../Object3D/Resources/teapot.dae");
	Model model_ring_a(gc, scene.gs, "Resources/ring.dae");
	Model model_ring_b(gc, scene.gs, "Resources/ring.dae");
	Model model_ring_c(gc, scene.gs, "Resources/ring.dae");

	camera = new SceneObject(scene, scene.base);
	camera->position = Vec3f(0.0f, 18.0f, -25.0f);
	camera->rotation_x = Angle(30.0f, angle_degrees);

	light_distant = new SceneObject(scene, scene.base);
	light_distant->position = Vec3f(0.0f, 32.0f, 20.0f);
	light_distant->rotation_y = Angle(45.0f, angle_degrees);
	light_distant->rotation_x = Angle(35.0f, angle_degrees);

	// Set left euler angle teapot
	SceneObject *ring;

	teapot_euler = new SceneObject(scene, scene.base);
	teapot_euler->model = model_teapot;
	teapot_euler->position = Vec3f(-9.0f, 0.0f, 0.0f);
	teapot_euler->scale = Vec3f(5.0f, 5.0f, 5.0f);

	rotation_euler_a = new SceneObject(scene, scene.base);
	rotation_euler_a->position = teapot_euler->position;
	ring = new SceneObject(scene, rotation_euler_a);
	ring->rotation_y = Angle(-90.0f, angle_degrees);
	ring->model = model_ring_a;

	rotation_euler_b = new SceneObject(scene, rotation_euler_a);
	ring = new SceneObject(scene, rotation_euler_b);
	ring->model = model_ring_b;
	ring->rotation_x = Angle(90.0f, angle_degrees);

	rotation_euler_c = new SceneObject(scene, rotation_euler_b);
	ring = new SceneObject(scene, rotation_euler_c);
	ring->model = model_ring_c;
	ring->rotation_x = Angle(180.0f, angle_degrees);
	ring->rotation_z = Angle(90.0f, angle_degrees);

	// Set right target angle teapot
	teapot_target = new SceneObject(scene, scene.base);
	teapot_target->model = model_teapot;
	teapot_target->position = Vec3f(9.0f, 0.0f, 0.0f);
	teapot_target->scale = Vec3f(5.0f, 5.0f, 5.0f);

	rotation_target_a = new SceneObject(scene, scene.base);
	rotation_target_a->position = teapot_target->position;
	ring = new SceneObject(scene, rotation_target_a);
	ring->rotation_y = Angle(-90.0f, angle_degrees);
	ring->model = model_ring_a;

	rotation_target_b = new SceneObject(scene, rotation_target_a);
	ring = new SceneObject(scene, rotation_target_b);
	ring->model = model_ring_b;
	ring->rotation_x = Angle(90.0f, angle_degrees);

	rotation_target_c = new SceneObject(scene, rotation_target_b);
	ring = new SceneObject(scene, rotation_target_c);
	ring->model = model_ring_c;
	ring->rotation_x = Angle(180.0f, angle_degrees);
	ring->rotation_z = Angle(90.0f, angle_degrees);

	// Set ring colours

	model_ring_a.SetMaterial(
			64,		// material_shininess
			Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// material_emission
			Vec4f(1.0f, 0.0f, 0.0f, 1.0f),	// material_ambient
			Vec4f(1.0f, 0.0f, 0.0f, 1.0f)	//material_specular
			);
	model_ring_b.SetMaterial(
			64,		// material_shininess
			Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// material_emission
			Vec4f(1.0f, 1.0f, 0.0f, 1.0f),	// material_ambient
			Vec4f(1.0f, 1.0f, 0.0f, 1.0f)	//material_specular
			);
	model_ring_c.SetMaterial(
			64,		// material_shininess
			Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// material_emission
			Vec4f(0.0f, 1.0f, 0.0f, 1.0f),	// material_ambient
			Vec4f(0.0f, 1.0f, 0.0f, 1.0f)	//material_specular
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

	scene.gs->shader_color.SetLight(light_vector, light_specular, light_diffuse, light_ambient);
}

void App::calculate_matricies(GraphicContext &gc)
{
	scene.gs->camera_projection = Mat4f::perspective(45.0f, ((float) gc.get_width()) / ((float) gc.get_height()), 0.1f, 1000.0f, handed_left, clip_negative_positive_w);

	float ortho_size = 60.0f / 2.0f;
	scene.gs->light_projection = Mat4f::ortho(-ortho_size, ortho_size, -ortho_size, ortho_size, 0.1f, 1000.0f, handed_left, clip_negative_positive_w);

	scene.gs->camera_modelview = Mat4f::identity();
	camera->GetWorldMatrix(scene.gs->camera_modelview);
	scene.gs->camera_modelview.inverse();

	scene.gs->light_modelview = Mat4f::identity();
	light_distant->GetWorldMatrix(scene.gs->light_modelview);
	scene.gs->light_modelview.inverse();
}

void App::control_target(Options *options)
{
	if ((options->button_lerp_clicked) || (options->button_slerp_clicked))
	{
		if (options->button_lerp_clicked)
			active_lerp = true;

		if (options->button_slerp_clicked)
			active_slerp = true;

		options->button_lerp->set_disabled();
		options->button_lerp_clicked = false;
		options->button_slerp->set_disabled();
		options->button_slerp_clicked = false;

		time_made_active = current_time;
		initial_quaternion = options->quaternion;
		final_quaternion = Quaternionf(options->target_x, options->target_y, options->target_z, order_YXZ);
	}

	if (options->button_rotate_clicked)
	{
		Quaternionf quat = Quaternionf::multiply(Quaternionf(options->target_x, options->target_y, options->target_z, order_YXZ), options->quaternion);
		options->set_new_quaternion(quat);
		options->button_rotate_clicked = false;
	}

	if (active_lerp || active_slerp)
	{
		float time_active = ((float) (current_time - time_made_active)) / 2000.0f;
		if (time_active > 1.0f)
		{
			time_active = 1.0f;
		}

		if (active_lerp)
		{
			Quaternionf quat = Quaternionf::lerp(initial_quaternion, final_quaternion, time_active);
			options->set_new_quaternion(quat);
		}
		else if (active_slerp)
		{
			Quaternionf quat = Quaternionf::slerp(initial_quaternion, final_quaternion, time_active);
			options->set_new_quaternion(quat);
		}

		if (time_active >= 1.0f)
		{
			options->button_lerp->set_enabled();
			options->button_slerp->set_enabled();
			active_lerp = false;
			active_slerp = false;
		}
	}
}
