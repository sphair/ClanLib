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
	desc.set_title("ClanLib SpotLight Example");
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
	Options *options = new Options(gui, CL_Rect(8, 8, CL_Size(320, 410)));
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

	CL_Vec3f original_spot_light_position = spot_light->position;

	unsigned int time_last = CL_System::get_time();
	// Run until someone presses escape
	while (!quit)
	{
		framerate_counter.frame_shown();

		unsigned int time_now = CL_System::get_time();
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

		polygon_rasterizer.set_culled(true);
		gc.set_polygon_rasterizer(polygon_rasterizer);
		render(gc);

		gc.set_modelview(CL_Mat4f::identity());
		gc.set_map_mode(cl_map_2d_upper_left);
		polygon_rasterizer.set_culled(false);
		gc.set_polygon_rasterizer(polygon_rasterizer);

		CL_String fps(cl_format("%1 fps", framerate_counter.get_framerate()));
		font.draw_text(gc, 16-2, gc.get_height()-16-2, fps, CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f));
		font.draw_text(gc, 16, gc.get_height()-16-2, fps, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));

		wm.process();
		wm.draw_windows(gc);

		// Use flip(1) to lock the fps
		window.flip(0);

		// This call processes user input and other events
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

	gc.clear(CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	gc.clear_depth(1.0f);

	CL_Mat4f modelview_matrix = scene.gs->camera_modelview;
	scene.Draw(modelview_matrix, gc);
	gc.reset_program_object();
}

void App::create_scene(CL_GraphicContext &gc)
{
	Model model_landscape(gc, "../Shadow/Resources/land.dae", false);
	model_landscape.SetMaterial(64.0f,	// Shiny
			CL_Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// Emission
			CL_Vec4f(1.0f, 1.0f, 1.0f, 1.0f),	// Ambient
			CL_Vec4f(0.0f, 0.0f, 0.0f, 1.0f));	// Specular

	Model model_cone(gc, "Resources/cone.dae", true);

	Model model_teapot(gc, "../Clan3D/Resources/teapot.dae", true);
	model_teapot.SetMaterial(64.0f,	// Shiny
			CL_Vec4f(0.0f, 0.0f, 0.0f, 1.0f),	// Emission
			CL_Vec4f(1.0f, 1.0f, 1.0f, 1.0f),	// Ambient
			CL_Vec4f(1.0f, 1.0f, 1.0f, 1.0f));	// Specular

	camera = new SceneObject(scene, scene.base);
	camera->position = CL_Vec3f(-34.6722f, 59.575f, -41.1818f);
	camera->rotation_y = CL_Angle(-29.5f, cl_degrees);
	camera->rotation_x = CL_Angle(22.0f, cl_degrees);
	camera->rotation_z = CL_Angle(-2.20f, cl_degrees);

	light_distant = new SceneObject(scene, scene.base);
	light_distant->model = model_cone;
	light_distant->position = CL_Vec3f(-72.0535f, 48.0866f, 29.0f);
	//  Note, these are updated by the options
	light_distant->rotation_y = CL_Angle(45.0f, cl_degrees);
	light_distant->rotation_x = CL_Angle(35.0f, cl_degrees);

	SceneObject *object_landscape = new SceneObject(scene, scene.base);
	object_landscape->model = model_landscape;
	object_landscape->position = CL_Vec3f(0.0f, 0.0f, 0.0f);

	spot_light = new SceneObject(scene, object_landscape);
	spot_light->model = model_cone;
	spot_light->position = CL_Vec3f(-64.7266f, 35.0f, 29.2167f);
	//  Note, these are updated by the options
	spot_light->rotation_y = CL_Angle(129.6f, cl_degrees);
	spot_light->rotation_x = CL_Angle(145.4f, cl_degrees);
	spot_light->rotation_z = CL_Angle(0.0f, cl_degrees);

	SceneObject *object_teapot = new SceneObject(scene, scene.base);
	object_teapot->model = model_teapot;
	object_teapot->position = CL_Vec3f(-89.1235f, 14.8972f, 49.1522f);
	object_teapot->rotation_y = CL_Angle(7.7f, cl_degrees);
	object_teapot->rotation_x = CL_Angle(-2.2f, cl_degrees);
	object_teapot->rotation_z = CL_Angle(2.7f, cl_degrees);
	object_teapot->scale = CL_Vec3f(30.0f, 30.0f, 30.0f);

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

	CL_Vec4f light_specular(options->distant_specular_color.r, options->distant_specular_color.g, options->distant_specular_color.b, options->distant_specular_color.a);
	CL_Vec4f light_diffuse(options->distant_diffuse_color.r, options->distant_diffuse_color.g, options->distant_diffuse_color.b, options->distant_diffuse_color.a);

	scene.gs->shader_texture.SetLight(light_vector, light_specular, light_diffuse);
	scene.gs->shader_color.SetLight(light_vector, light_specular, light_diffuse);
}

void App::update_spot_light(CL_GraphicContext &gc, Options *options)
{
	CL_Mat4f light_modelview_matrix = CL_Mat4f::identity();
	spot_light->GetWorldMatrix(light_modelview_matrix);
	light_modelview_matrix.scale_self(1.0f, 1.0f, -1.0f);

	CL_Mat4f temp_matrix = light_modelview_matrix * scene.gs->camera_modelview;

	// Set position
	CL_Vec3f light_position = CL_Vec3f(
			temp_matrix.matrix[0+3*4],
			temp_matrix.matrix[1+3*4],
			temp_matrix.matrix[2+3*4]);

	// Clear translation
	temp_matrix.matrix[0+3*4] = 0.0f;
	temp_matrix.matrix[1+3*4] = 0.0f;
	temp_matrix.matrix[2+3*4] = 0.0f;

	// Get direction
	CL_Vec4f light_vector = temp_matrix.get_transformed_point(CL_Vec3f(0.0f, 0.0f, -1.0f));

	CL_Vec4f spot_light_specular(options->spot_specular_color.r, options->spot_specular_color.g, options->spot_specular_color.b, options->spot_specular_color.a);
	CL_Vec4f spot_light_diffuse(options->spot_diffuse_color.r, options->spot_diffuse_color.g, options->spot_diffuse_color.b, options->spot_diffuse_color.a);

	scene.gs->shader_texture.SetSpotLight(light_position, light_vector, spot_light_specular, spot_light_diffuse, options->spot_light_exponent, options->spot_light_cutoff);
	scene.gs->shader_color.SetSpotLight(light_position, light_vector, spot_light_specular, spot_light_diffuse, options->spot_light_exponent, options->spot_light_cutoff);
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

