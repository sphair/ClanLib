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

#include "precomp.h"
#include "example.h"
#include "shader_bumpmap.h"
#include "scene_object.h"
#include "model.h"
#include "graphic_store.h"
#include "framerate_counter.h"
#include "options.h"

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	quit = false;

	CL_OpenGLWindowDescription desc;
	desc.set_title("ClanLib BumpMap Example");
	desc.set_size(CL_Size(900, 700), true);
	desc.set_multisampling(4);
	desc.set_allow_resize(true);
	desc.set_depth_size(16);

	CL_DisplayWindow window(desc);

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
	wm.func_repaint().set(this, &App::wm_repaint);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	// Deleted automatically by the GUI
	Options *options = new Options(gui, CL_Rect(8, 8, CL_Size(340, 600)));
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

	graphic_store.image_grid = CL_Image(gc, "../../Display_Render/Blend/Resources/grid.png");

	FramerateCounter framerate_counter;

	unsigned int time_last = CL_System::get_time();
	// Run until someone presses escape
	while (!quit)
	{
		framerate_counter.frame_shown();

		unsigned int time_now = CL_System::get_time();
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
			CL_Vec4f(options->material_emission_color.r, options->material_emission_color.g, options->material_emission_color.b, options->material_emission_color.a),	// material_emission
			CL_Vec4f(options->material_ambient_color.r, options->material_ambient_color.g, options->material_ambient_color.b, options->material_ambient_color.a),	// material_ambient
			CL_Vec4f(options->material_specular_color.r, options->material_specular_color.g, options->material_specular_color.b, options->material_specular_color.a)	//material_specular
			);

		rotate_teapot();

		calculate_matricies(gc);

		update_light(gc, options);

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

		gui.exec(false);
		wm.draw_windows(gc);

		// Use flip(1) to lock the fps
		window.flip(0);

		// This call processes user input and other events
		CL_KeepAlive::process();
	}

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

	scene.gs->image_grid.draw(gc, 420.0f, 120.0f);	// Draw a grid in the backgound

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
	std::vector<CL_Collada_Image> library_images;

	Model model_cone("../SpotLight/Resources/cone.dae", library_images);
	Model model_teapot("../Clan3D/Resources/teapot.dae", library_images, true);


	camera = new SceneObject(scene, scene.base);
	camera->position = CL_Vec3f(-20.0f, 40.0f, -60.0f);
	camera->rotation_x = CL_Angle(20.0f, cl_degrees);

	light_distant = new SceneObject(scene, scene.base);
	light_distant->model = model_cone;
	light_distant->position = CL_Vec3f(0.0f, 32.0f, 20.0f);
	//  Note, these are updated by the options
	light_distant->rotation_y = CL_Angle(45.0f, cl_degrees);
	light_distant->rotation_x = CL_Angle(35.0f, cl_degrees);

	teapot = new SceneObject(scene, scene.base);
	teapot->model = model_teapot;
	teapot->position = CL_Vec3f(-2.5f, 0.0f, 20.0f);
	teapot->scale = CL_Vec3f(50.0f, 50.0f, 50.0f);

	scene.gs->LoadImages(gc, library_images);

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

	CL_Vec4f light_specular(options->light_specular_color.r, options->light_specular_color.g, options->light_specular_color.b, options->light_specular_color.a);
	CL_Vec4f light_diffuse(options->light_diffuse_color.r, options->light_diffuse_color.g, options->light_diffuse_color.b, options->light_diffuse_color.a);
	CL_Vec4f light_ambient(options->light_ambient_color.r, options->light_ambient_color.g, options->light_ambient_color.b, options->light_ambient_color.a);

	scene.gs->shader_bumpmap.SetLight(light_vector, light_specular, light_diffuse, light_ambient);
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

void App::wm_repaint()
{
}

void App::rotate_teapot()
{
	float delta = ((float) time_delta) / 10.0f;

	teapot->rotation_y = CL_Angle(teapot->rotation_y.to_degrees() + delta, cl_degrees);

}
