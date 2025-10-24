/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "framerate_counter.h"
#include "options.h"

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
int App::start(const std::vector<CL_String> &args)
{
	quit = false;

	CL_OpenGLWindowDescription desc;
	desc.set_title("ClanLib Quaternion's Example");
	desc.set_size(CL_Size(900, 700), true);
	desc.set_multisampling(4);
	desc.set_allow_resize(true);
	desc.set_depth_size(16);

	CL_DisplayWindow window(desc);

#ifdef _DEBUG
	//struct aiLogStream stream;
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	//aiAttachLogStream(&stream);
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	//aiAttachLogStream(&stream);
#endif
	aiSetImportPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE,89.53f);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	// Set up GUI
	CL_String theme;
	if (CL_FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (CL_FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw CL_Exception("No themes found");

	CL_GUIWindowManagerTexture wm(window);
	CL_GUIManager gui(wm, theme);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	// Deleted automatically by the GUI
	Options *options = new Options(gui, CL_Rect(8, 8, CL_Size(gc.get_width()-16, 170)));
	options->request_repaint();

	// Setup graphic store
	GraphicStore graphic_store(gc);
	scene.gs = &graphic_store;

	// Prepare the display
	gc.set_map_mode(cl_user_projection);

	CL_PolygonRasterizer polygon_rasterizer;
	polygon_rasterizer.set_culled(true);
	polygon_rasterizer.set_face_cull_mode(cl_cull_back);
	polygon_rasterizer.set_front_face(cl_face_side_clockwise);
	gc.set_polygon_rasterizer(polygon_rasterizer);

	create_scene(gc);

	CL_Font font(gc, "tahoma", 24);

	FramerateCounter framerate_counter;

	active_lerp = false;
	unsigned int time_last = CL_System::get_time();
	unsigned int time_start = time_last;

	// Run until someone presses escape
	while (!quit)
	{
		framerate_counter.frame_shown();

		// Calculate time since last frame
		unsigned int time_now = CL_System::get_time();
		current_time = time_now - time_start;
		time_delta = time_now - time_last;
		time_last = time_now;

		// Control the target options
		control_target(options);

		// Use the euler angle options
		rotation_euler_a->rotation_y = options->rotation_y;
		rotation_euler_b->rotation_x = options->rotation_x;
		rotation_euler_c->rotation_z = options->rotation_z;

		teapot_euler->rotation_x = options->rotation_x;
		teapot_euler->rotation_y = options->rotation_y;
		teapot_euler->rotation_z = options->rotation_z;

		// Use the target angle options
		rotation_target_a->rotation_y = options->target_y;
		rotation_target_b->rotation_x = options->target_x;
		rotation_target_c->rotation_z = options->target_z;

		teapot_target->rotation_x = options->target_x;
		teapot_target->rotation_y = options->target_y;
		teapot_target->rotation_z = options->target_z;

		// Render the scene using euler angles
		calculate_matricies(gc);
		update_light(gc, options);
		polygon_rasterizer.set_culled(true);
		gc.set_polygon_rasterizer(polygon_rasterizer);
		render(gc);

		// Show the quaternion teapot
		CL_Mat4f modelview_matrix = scene.gs->camera_modelview;
		modelview_matrix.translate_self(0.0f, 0.0f, 0.0f);
		modelview_matrix = options->quaternion.to_matrix() * modelview_matrix;
		modelview_matrix.scale_self(5.0f, 5.0f, 5.0f);
		model_teapot.Draw(gc, scene.gs, modelview_matrix);

		// Draw information boxes
		gc.set_modelview(CL_Mat4f::identity());
		gc.set_map_mode(cl_map_2d_upper_left);
		polygon_rasterizer.set_culled(false);
		gc.set_polygon_rasterizer(polygon_rasterizer);

		CL_String fps(cl_format("%1 fps", framerate_counter.get_framerate()));
		font.draw_text(gc, 16-2, gc.get_height()-16-2, fps, CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f));
		font.draw_text(gc, 16, gc.get_height()-16-2, fps, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));

		font.draw_text(gc, 60, 250, "Euler Orientation");
		font.draw_text(gc, 330, 250, "Quaternion Orientation");
		font.draw_text(gc, 600, 250, "Target Euler Orientation");
		font.draw_text(gc, 16, 630, "(Using YXZ rotation order)");

		wm.process();
		wm.draw_windows(gc);

		// Use flip(1) to lock the fps
		window.flip(0);

		CL_KeepAlive::process();
	}
	aiDetachAllLogStreams();
	return 0;
}

// A key was pressed
void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

void App::render(CL_GraphicContext &gc)
{
	gc.clear(CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f));

	gc.set_map_mode(cl_user_projection);
	CL_Rect viewport_rect(0, 0, CL_Size(gc.get_width(), gc.get_height()));
	gc.set_viewport(viewport_rect);

	gc.set_projection(scene.gs->camera_projection);

	CL_BufferControl buffer_control;
	buffer_control.set_depth_compare_function(cl_comparefunc_lequal);
	buffer_control.enable_depth_write(true);
	buffer_control.enable_depth_test(true);
	buffer_control.enable_stencil_test(false);
	buffer_control.enable_color_write(true);
	gc.set_buffer_control(buffer_control);

	gc.clear_depth(1.0f);

	CL_Mat4f modelview_matrix = scene.gs->camera_modelview;
	scene.Draw(modelview_matrix, gc);
	gc.reset_program_object();
}

void App::create_scene(CL_GraphicContext &gc)
{

	model_teapot = Model(gc, "../Clan3D/Resources/teapot.dae");
	Model model_ring_a(gc, "Resources/ring.dae");
	Model model_ring_b(gc, "Resources/ring.dae");
	Model model_ring_c(gc, "Resources/ring.dae");

	camera = new SceneObject(scene, scene.base);
	camera->position = CL_Vec3f(0.0f, 18.0f, -25.0f);
	camera->rotation_x = CL_Angle(30.0f, cl_degrees);

	light_distant = new SceneObject(scene, scene.base);
	light_distant->position = CL_Vec3f(0.0f, 32.0f, 20.0f);
	light_distant->rotation_y = CL_Angle(45.0f, cl_degrees);
	light_distant->rotation_x = CL_Angle(35.0f, cl_degrees);

	// Set left euler angle teapot
	SceneObject *ring;

	teapot_euler = new SceneObject(scene, scene.base);
	teapot_euler->model = model_teapot;
	teapot_euler->position = CL_Vec3f(-9.0f, 0.0f, 0.0f);
	teapot_euler->scale = CL_Vec3f(5.0f, 5.0f, 5.0f);

	rotation_euler_a = new SceneObject(scene, scene.base);
	rotation_euler_a->position = teapot_euler->position;
	ring = new SceneObject(scene, rotation_euler_a);
	ring->rotation_y = CL_Angle(-90.0f, cl_degrees);
	ring->model = model_ring_a;

	rotation_euler_b = new SceneObject(scene, rotation_euler_a);
	ring = new SceneObject(scene, rotation_euler_b);
	ring->model = model_ring_b;
	ring->rotation_x = CL_Angle(90.0f, cl_degrees);

	rotation_euler_c = new SceneObject(scene, rotation_euler_b);
	ring = new SceneObject(scene, rotation_euler_c);
	ring->model = model_ring_c;
	ring->rotation_x = CL_Angle(180.0f, cl_degrees);
	ring->rotation_z = CL_Angle(90.0f, cl_degrees);

	// Set right target angle teapot
	teapot_target = new SceneObject(scene, scene.base);
	teapot_target->model = model_teapot;
	teapot_target->position = CL_Vec3f(9.0f, 0.0f, 0.0f);
	teapot_target->scale = CL_Vec3f(5.0f, 5.0f, 5.0f);

	rotation_target_a = new SceneObject(scene, scene.base);
	rotation_target_a->position = teapot_target->position;
	ring = new SceneObject(scene, rotation_target_a);
	ring->rotation_y = CL_Angle(-90.0f, cl_degrees);
	ring->model = model_ring_a;

	rotation_target_b = new SceneObject(scene, rotation_target_a);
	ring = new SceneObject(scene, rotation_target_b);
	ring->model = model_ring_b;
	ring->rotation_x = CL_Angle(90.0f, cl_degrees);

	rotation_target_c = new SceneObject(scene, rotation_target_b);
	ring = new SceneObject(scene, rotation_target_c);
	ring->model = model_ring_c;
	ring->rotation_x = CL_Angle(180.0f, cl_degrees);
	ring->rotation_z = CL_Angle(90.0f, cl_degrees);

	// Set ring colours

	model_ring_a.SetMaterial(
			64,		// material_shininess
			CL_Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// material_emission
			CL_Vec4f(1.0f, 0.0f, 0.0f, 1.0f),	// material_ambient
			CL_Vec4f(1.0f, 0.0f, 0.0f, 1.0f)	//material_specular
			);
	model_ring_b.SetMaterial(
			64,		// material_shininess
			CL_Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// material_emission
			CL_Vec4f(1.0f, 1.0f, 0.0f, 1.0f),	// material_ambient
			CL_Vec4f(1.0f, 1.0f, 0.0f, 1.0f)	//material_specular
			);
	model_ring_c.SetMaterial(
			64,		// material_shininess
			CL_Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// material_emission
			CL_Vec4f(0.0f, 1.0f, 0.0f, 1.0f),	// material_ambient
			CL_Vec4f(0.0f, 1.0f, 0.0f, 1.0f)	//material_specular
			);
	scene.gs->LoadImages(gc);

}


void App::update_light(CL_GraphicContext &gc, Options *options)
{
	CL_Mat4f light_modelview_matrix = CL_Mat4f::identity();
	light_distant->GetWorldMatrix(light_modelview_matrix);

	CL_Mat4f work_matrix = scene.gs->camera_modelview;
	work_matrix.multiply(light_modelview_matrix);

	// Clear translation
	work_matrix.matrix[0+3*4] = 0.0f;
	work_matrix.matrix[1+3*4] = 0.0f;
	work_matrix.matrix[2+3*4] = 0.0f;

	CL_Vec4f light_vector = work_matrix.get_transformed_point(CL_Vec3f(0.0f, 0.0f, -1.0f));

	CL_Vec4f light_specular(0.5f, 0.5f, 0.5f, 1.0f);
	CL_Vec4f light_diffuse(0.5f, 0.5f, 0.5f, 1.0f);
	CL_Vec4f light_ambient(0.0f, 0.0f, 0.0f, 1.0f);

	scene.gs->shader_color.SetLight(light_vector, light_specular, light_diffuse, light_ambient);
}

void App::calculate_matricies(CL_GraphicContext &gc)
{
	scene.gs->camera_projection = CL_Mat4f::perspective(45.0f, ((float) gc.get_width()) / ((float) gc.get_height()), 0.1f, 1000.0f);

	float ortho_size = 60.0f / 2.0f;
	scene.gs->light_projection = CL_Mat4f::ortho(-ortho_size, ortho_size, -ortho_size, ortho_size, 0.1f, 1000.0f);

	scene.gs->camera_modelview = CL_Mat4f::identity();
	camera->GetWorldMatrix(scene.gs->camera_modelview);
	scene.gs->camera_modelview.scale_self(1.0f, 1.0f, -1.0f);
	scene.gs->camera_modelview.inverse();

	scene.gs->light_modelview = CL_Mat4f::identity();
	light_distant->GetWorldMatrix(scene.gs->light_modelview);
	scene.gs->light_modelview.scale_self(1.0f, 1.0f, -1.0f);
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

		options->button_lerp->set_enabled(false);
		options->button_lerp_clicked = false;
		options->button_slerp->set_enabled(false);
		options->button_slerp_clicked = false;

		time_made_active = current_time;
		initial_quaternion = options->quaternion;
		final_quaternion = CL_Quaternionf(options->target_x, options->target_y, options->target_z, cl_YXZ);
	}

	if (options->button_rotate_clicked)
	{
		CL_Quaternionf quat = CL_Quaternionf::multiply(CL_Quaternionf(options->target_x, options->target_y, options->target_z, cl_YXZ), options->quaternion);
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
			CL_Quaternionf quat = CL_Quaternionf::lerp(initial_quaternion, final_quaternion, time_active);
			options->set_new_quaternion(quat);
		}
		else if (active_slerp)
		{
			CL_Quaternionf quat = CL_Quaternionf::slerp(initial_quaternion, final_quaternion, time_active);
			options->set_new_quaternion(quat);
		}

		if (time_active >= 1.0f)
		{
			options->button_lerp->set_enabled(true);
			options->button_slerp->set_enabled(true);
			active_lerp = false;
			active_slerp = false;
		}
	}

}
