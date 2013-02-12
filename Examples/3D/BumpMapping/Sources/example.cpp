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
#include "shader_bumpmap.h"
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

	DisplayWindowDescription desc;
	desc.set_title("ClanLib BumpMap Example");
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
	Options *options = new Options(gui, Rect(8, 8, Size(340, 600)));
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

	graphic_store.image_grid = Image(gc, "../../Display_Render/Blend/Resources/grid.png");

	FramerateCounter framerate_counter;

	unsigned int time_last = System::get_time();
	// Run until someone presses escape
	while (!quit)
	{
		framerate_counter.frame_shown();

		unsigned int time_now = System::get_time();
		time_delta = time_now - time_last;
		time_last = time_now;

		// Copy direction options
		light_distant->rotation_y = options->light_direction_heading;
		light_distant->rotation_x = options->light_direction_pitch;

		// Set material options

		float shininess = options->material_shininess;

		// Convert shininess from a percentage, using Lightwave 3d's method
		shininess = shininess / 100.0f;
		shininess = pow(2, (10.0f * shininess) + 2);

		teapot->model.SetMaterial(
			shininess,		// material_shininess
			Vec4f(options->material_emission_color.r, options->material_emission_color.g, options->material_emission_color.b, options->material_emission_color.a),	// material_emission
			Vec4f(options->material_ambient_color.r, options->material_ambient_color.g, options->material_ambient_color.b, options->material_ambient_color.a),	// material_ambient
			Vec4f(options->material_specular_color.r, options->material_specular_color.g, options->material_specular_color.b, options->material_specular_color.a)	//material_specular
			);

		rotate_teapot();

		calculate_matricies(gc);

		update_light(gc, options);

		gc.set_culled(true);
		render(gc);

		gc.set_modelview(Mat4f::identity());
		gc.set_map_mode(map_2d_upper_left);
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

	scene.gs->image_grid.draw(gc, 420.0f, 120.0f);	// Draw a grid in the backgound

	gc.set_map_mode(cl_user_projection);
	Rect viewport_rect(0, 0, Size(gc.get_width(), gc.get_height()));
	gc.set_viewport(viewport_rect);

	gc.set_projection(scene.gs->camera_projection);

	gc.set_depth_compare_function(cl_comparefunc_lequal);
	gc.enable_depth_write(true);
	gc.enable_depth_test(true);
	gc.enable_stencil_test(false);
	gc.enable_color_write(true);

	gc.clear_depth(1.0f);

	Mat4f modelview_matrix = scene.gs->camera_modelview;
	scene.Draw(modelview_matrix, gc);
	gc.reset_program_object();
}

void App::create_scene(GraphicContext &gc)
{
	Model model_teapot(gc, scene.gs, "../Clan3D/Resources/teapot.dae", true);

	camera = new SceneObject(scene, scene.base);
	camera->position = Vec3f(-20.0f, 40.0f, -60.0f);
	camera->rotation_x = Angle(20.0f, angle_degrees);

	light_distant = new SceneObject(scene, scene.base);
	//light_distant->model = model_cone;
	//light_distant->position = Vec3f(0.0f, 32.0f, 20.0f);
	//  Note, these are updated by the options
	light_distant->rotation_y = Angle(45.0f, angle_degrees);
	light_distant->rotation_x = Angle(35.0f, angle_degrees);

	teapot = new SceneObject(scene, scene.base);
	teapot->model = model_teapot;
	teapot->position = Vec3f(-2.5f, 0.0f, 20.0f);
	teapot->scale = Vec3f(50.0f, 50.0f, 50.0f);

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

	Vec4f light_specular(options->light_specular_color.r, options->light_specular_color.g, options->light_specular_color.b, options->light_specular_color.a);
	Vec4f light_diffuse(options->light_diffuse_color.r, options->light_diffuse_color.g, options->light_diffuse_color.b, options->light_diffuse_color.a);
	Vec4f light_ambient(options->light_ambient_color.r, options->light_ambient_color.g, options->light_ambient_color.b, options->light_ambient_color.a);

	scene.gs->shader_bumpmap.SetLight(light_vector, light_specular, light_diffuse, light_ambient);
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


void App::rotate_teapot()
{
	float delta = ((float) time_delta) / 10.0f;

	teapot->rotation_y = Angle(teapot->rotation_y.to_degrees() + delta, angle_degrees);

}
