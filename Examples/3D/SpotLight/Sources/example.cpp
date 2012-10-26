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
int App::start(const std::vector<std::string> &args)
{
	quit = false;

	OpenGLWindowDescription desc;
	desc.set_title("ClanLib SpotLight Example");
	desc.set_size(Size(900, 700), true);
	desc.set_multisampling(4);
	desc.set_allow_resize(true);
	desc.set_depth_size(16);

	DisplayWindow window(desc);

	// Connect the Window close event
	Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	// Set up GUI
	std::string theme;
	if (FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw Exception("No themes found");

	GUIWindowManagerTexture wm(window);
	GUIManager gui(wm, theme);

	// Get the graphic context
	GraphicContext gc = window.get_gc();

	// Deleted automatically by the GUI
	Options *options = new Options(gui, Rect(8, 8, Size(320, 410)));
	options->request_repaint();

	// Setup graphic store
	GraphicStore graphic_store(gc);
	scene.gs = &graphic_store;

	// Prepare the display
	gc.set_map_mode(cl_user_projection);

	gc.set_culled(true);
	gc.set_face_cull_mode(cl_cull_back);
	gc.set_front_face(cl_face_side_clockwise);

	create_scene(gc);

	Font font(gc, "tahoma", 24);

	FramerateCounter framerate_counter;

	Vec3f original_spot_light_position = spot_light->position;

	unsigned int time_last = System::get_time();
	// Run until someone presses escape
	while (!quit)
	{
		framerate_counter.frame_shown();

		unsigned int time_now = System::get_time();
		time_delta = time_now - time_last;
		time_last = time_now;

		// Copy direction options
		spot_light->rotation_y = options->spot_direction_heading;
		spot_light->rotation_x = options->spot_direction_pitch;
		spot_light->rotation_z = options->spot_direction_bank;
		spot_light->position.x = original_spot_light_position.x + options->spot_light_position.x - (options->max_position_value / 2.0f);
		spot_light->position.y = original_spot_light_position.y + options->spot_light_position.y - (options->max_position_value / 2.0f);
		spot_light->position.z = original_spot_light_position.z + options->spot_light_position.z - (options->max_position_value / 2.0f);

		light_distant->rotation_y = options->distant_direction_heading;
		light_distant->rotation_x = options->distant_direction_pitch;
		light_distant->rotation_z = options->distant_direction_bank;

		calculate_matricies(gc);

		update_light(gc, options);
		update_spot_light(gc, options);

		gc.set_culled(true);
		render(gc);

		gc.set_modelview(Mat4f::identity());
		gc.set_map_mode(cl_map_2d_upper_left);
		gc.set_culled(false);
	
		std::string fps(string_format("%1 fps", framerate_counter.get_framerate()));
		font.draw_text(gc, 16-2, gc.get_height()-16-2, fps, Colorf(0.0f, 0.0f, 0.0f, 1.0f));
		font.draw_text(gc, 16, gc.get_height()-16-2, fps, Colorf(1.0f, 1.0f, 1.0f, 1.0f));

		wm.process();
		wm.draw_windows(gc);

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
	Model model_landscape(gc, scene.gs, "../Shadow/Resources/land.dae", false);
	model_landscape.SetMaterial(64.0f,	// Shiny
			Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// Emission
			Vec4f(1.0f, 1.0f, 1.0f, 1.0f),	// Ambient
			Vec4f(0.0f, 0.0f, 0.0f, 1.0f));	// Specular

	Model model_cone(gc, scene.gs, "Resources/cone.dae", true);

	Model model_teapot(gc, scene.gs, "../Clan3D/Resources/teapot.dae", true);
	model_teapot.SetMaterial(64.0f,	// Shiny
			Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// Emission
			Vec4f(1.0f, 1.0f, 1.0f, 1.0f),	// Ambient
			Vec4f(1.0f, 1.0f, 1.0f, 1.0f));	// Specular

	camera = new SceneObject(scene, scene.base);
	camera->position = Vec3f(-34.6722f, 59.575f, -41.1818f);
	camera->rotation_y = Angle(-29.5f, cl_degrees);
	camera->rotation_x = Angle(22.0f, cl_degrees);
	camera->rotation_z = Angle(-2.20f, cl_degrees);

	light_distant = new SceneObject(scene, scene.base);
	light_distant->model = model_cone;
	light_distant->position = Vec3f(-72.0535f, 48.0866f, 29.0f);
	//  Note, these are updated by the options
	light_distant->rotation_y = Angle(45.0f, cl_degrees);
	light_distant->rotation_x = Angle(35.0f, cl_degrees);

	SceneObject *object_landscape = new SceneObject(scene, scene.base);
	object_landscape->model = model_landscape;
	object_landscape->position = Vec3f(0.0f, 0.0f, 0.0f);

	spot_light = new SceneObject(scene, object_landscape);
	spot_light->model = model_cone;
	spot_light->position = Vec3f(-64.7266f, 35.0f, 29.2167f);
	//  Note, these are updated by the options
	spot_light->rotation_y = Angle(129.6f, cl_degrees);
	spot_light->rotation_x = Angle(145.4f, cl_degrees);
	spot_light->rotation_z = Angle(0.0f, cl_degrees);

	SceneObject *object_teapot = new SceneObject(scene, scene.base);
	object_teapot->model = model_teapot;
	object_teapot->position = Vec3f(-89.1235f, 14.8972f, 49.1522f);
	object_teapot->rotation_y = Angle(7.7f, cl_degrees);
	object_teapot->rotation_x = Angle(-2.2f, cl_degrees);
	object_teapot->rotation_z = Angle(2.7f, cl_degrees);
	object_teapot->scale = Vec3f(30.0f, 30.0f, 30.0f);

	scene.gs->LoadImages(gc);

}

void App::update_light(GraphicContext &gc, Options *options)
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

	Vec4f light_specular(options->distant_specular_color.r, options->distant_specular_color.g, options->distant_specular_color.b, options->distant_specular_color.a);
	Vec4f light_diffuse(options->distant_diffuse_color.r, options->distant_diffuse_color.g, options->distant_diffuse_color.b, options->distant_diffuse_color.a);

	scene.gs->shader_texture.SetLight(light_vector, light_specular, light_diffuse);
	scene.gs->shader_color.SetLight(light_vector, light_specular, light_diffuse);
}

void App::update_spot_light(GraphicContext &gc, Options *options)
{
	Mat4f light_modelview_matrix = Mat4f::identity();
	spot_light->GetWorldMatrix(light_modelview_matrix);
	light_modelview_matrix.scale_self(1.0f, 1.0f, -1.0f);

	Mat4f temp_matrix = scene.gs->camera_modelview * light_modelview_matrix;

	// Set position
	Vec3f light_position = Vec3f(
			temp_matrix.matrix[0+3*4],
			temp_matrix.matrix[1+3*4],
			temp_matrix.matrix[2+3*4]);

	// Clear translation
	temp_matrix.matrix[0+3*4] = 0.0f;
	temp_matrix.matrix[1+3*4] = 0.0f;
	temp_matrix.matrix[2+3*4] = 0.0f;

	// Get direction
	Vec3f light_vector = temp_matrix.get_transformed_point(Vec3f(0.0f, 0.0f, -1.0f));

	Vec4f spot_light_specular(options->spot_specular_color.r, options->spot_specular_color.g, options->spot_specular_color.b, options->spot_specular_color.a);
	Vec4f spot_light_diffuse(options->spot_diffuse_color.r, options->spot_diffuse_color.g, options->spot_diffuse_color.b, options->spot_diffuse_color.a);

	scene.gs->shader_texture.SetSpotLight(light_position, light_vector, spot_light_specular, spot_light_diffuse, options->spot_light_exponent, options->spot_light_cutoff);
	scene.gs->shader_color.SetSpotLight(light_position, light_vector, spot_light_specular, spot_light_diffuse, options->spot_light_exponent, options->spot_light_cutoff);
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

