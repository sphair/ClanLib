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
#include "app.h"
#include "shader.h"
#include "framerate_counter.h"
#include <cstdlib>

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Vertex Buffer Object Example");
	win_desc.set_depth_size(16);

	win_desc.set_size(CL_Size( 800, 600 ), false);

	CL_DisplayWindow window(win_desc);
	CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	CL_GraphicContext gc = window.get_gc();

	Shader shader(gc);

	// Prepare the display
	gc.set_map_mode(cl_user_projection);

	CL_PolygonRasterizer polygon_rasterizer;
	polygon_rasterizer.set_culled(true);
	polygon_rasterizer.set_face_cull_mode(cl_cull_back);
	polygon_rasterizer.set_front_face(cl_face_side_clockwise);
	gc.set_polygon_rasterizer(polygon_rasterizer);

	CL_BufferControl buffer_control;
	buffer_control.enable_depth_test(true);
	buffer_control.set_depth_compare_function(cl_comparefunc_lequal);
	buffer_control.enable_depth_write(true);
	gc.set_buffer_control(buffer_control);

	std::vector<CL_Vec3f> object_positions;
	std::vector<CL_Vec3f> object_normals;
	std::vector<CL_Vec4f> object_material_ambient;

	const int num_cubes = 20000;
	object_positions.reserve(num_cubes * 6 * 6);	// 6 faces, and 6 vertices per face
	object_normals.reserve(num_cubes * 6 * 6);
	object_material_ambient.reserve(num_cubes * 6 * 6);

	for (int cnt=0; cnt < num_cubes; cnt++)
	{
		create_cube(object_positions, object_normals, object_material_ambient);
	}

	CL_VertexArrayBuffer vb_positions(gc, &object_positions[0], sizeof(CL_Vec3f) * object_positions.size());
	CL_VertexArrayBuffer vb_normals(gc, &object_normals[0], sizeof(CL_Vec3f) * object_normals.size());
	CL_VertexArrayBuffer vb_material_ambient(gc, &object_material_ambient[0], sizeof(CL_Vec4f) * object_material_ambient.size());

	// ** Note, at this point "object_positions, object_normals and object_material_ambient"
	// ** can be destroyed. But for the purpose of this example, is it kept

	CL_Font fps_font(gc, "tahoma", 20);

	FramerateCounter frameratecounter;
	unsigned int time_last = CL_System::get_time();

	float angle = 0.0f;
	is_vertex_buffer_on = true;

	while (!quit)
	{
		unsigned int time_now = CL_System::get_time();
		float time_diff = (float) (time_now - time_last);
		time_last = time_now;

		gc.clear(CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f));
		gc.clear_depth(1.0f);

		gc.set_map_mode(cl_map_2d_upper_left);
		CL_String fps = cl_format("%1 fps", frameratecounter.get_framerate());
		fps_font.draw_text(gc, gc.get_width() - 100, 30, fps);
		CL_String info = cl_format("%1 vertices", (int) object_positions.size());
		fps_font.draw_text(gc, 30, 30, info);

		fps_font.draw_text(gc, 30, gc.get_height() - 8, "Press any key to toggle the Vertex Buffer option");

		if (is_vertex_buffer_on)
		{
			fps_font.draw_text(gc, 200, 30, "Vertex Buffer = ON");
		}
		else
		{
			fps_font.draw_text(gc, 200, 30, "Vertex Buffer = OFF");
		}

		gc.set_map_mode(cl_user_projection);
		CL_Mat4f perp = CL_Mat4f::perspective(45.0f, ((float) gc.get_width()) / ((float) gc.get_height()), 0.1f, 100000.0f);
		gc.set_projection(perp);

		angle += time_diff / 20.0f;
		if (angle >= 360.0f)
			angle -= 360.0f;

		gc.push_modelview();
		gc.set_modelview(CL_Mat4f::identity());
		gc.mult_scale(1.0f,1.0f, -1.0f);	// So +'ve Z goes into the screen
		gc.mult_translate(0.0f, 0.0f, 800.0f);
		gc.mult_rotate(CL_Angle(angle*2.0f, cl_degrees), 0.0f, 1.0f, 0.0f, false);
		gc.mult_rotate(CL_Angle(angle, cl_degrees), 1.0f, 0.0f, 0.0f, false);

		shader.Set(gc);
		shader.Use(gc);

		CL_PrimitivesArray prim_array(gc);

		if (is_vertex_buffer_on)
		{
			prim_array.set_attributes(0, vb_positions, 3, cl_type_float, (void *) 0);
			prim_array.set_attributes(1, vb_normals, 3, cl_type_float, (void *) 0);
			prim_array.set_attributes(2, vb_material_ambient, 4, cl_type_float, (void *) 0);
		}
		else
		{
			prim_array.set_attributes(0, &object_positions[0]);
			prim_array.set_attributes(1, &object_normals[0]);
			prim_array.set_attributes(2, &object_material_ambient[0]);
		}
		gc.draw_primitives(cl_triangles, object_positions.size(), prim_array);

		gc.pop_modelview();

		gc.reset_program_object();

		window.flip(0);
		frameratecounter.frame_shown();

		CL_KeepAlive::process();
	}

	return 0;
}

// A key was pressed
void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	switch (key.id)
	{
		case CL_KEY_ESCAPE:
			quit = true;
			break;
	}

	is_vertex_buffer_on = !is_vertex_buffer_on;

}

// The window was closed
void App::on_window_close()
{
	quit = true;
}


void App::create_cube( std::vector<CL_Vec3f> &object_positions, std::vector<CL_Vec3f> &object_normals, std::vector<CL_Vec4f> &object_material_ambient )
{
	CL_Vec3f position;
	position.x = (float) ((rand() & 0x1FF) - 0xFF);
	position.y = (float) ((rand() & 0x1FF) - 0xFF);
	position.z = (float) ((rand() & 0x1FF) - 0xFF);

	CL_Vec4f material_ambient;
	material_ambient.r = ((float) (rand() & 0xFF)) / 255.0f;
	material_ambient.g = ((float) (rand() & 0xFF)) / 255.0f;
	material_ambient.b = ((float) (rand() & 0xFF)) / 255.0f;
	material_ambient.a = 1.0f;

	CL_Vec3f size(5.0f, 5.0f, 5.0f);

	object_positions.push_back(CL_Vec3f(-size.x, size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, -size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, -size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, -size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, size.y, size.z) +position);

	object_positions.push_back(CL_Vec3f(-size.x, size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, -size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, -size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, -size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, size.y, size.z) +position);

	object_positions.push_back(CL_Vec3f(-size.x, size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, size.y, size.z) +position);

	object_positions.push_back(CL_Vec3f(size.x, -size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, -size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, -size.y, -size.z) +position);

	object_positions.push_back(CL_Vec3f(size.x, -size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, -size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, -size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, -size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(-size.x, -size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, -size.y, -size.z) +position);

	object_positions.push_back(CL_Vec3f(size.x, -size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, size.y, -size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, -size.y, size.z) +position);
	object_positions.push_back(CL_Vec3f(size.x, -size.y, -size.z) +position);

	for (int cnt=0; cnt<6; cnt++)
		object_normals.push_back(CL_Vec3f(-1.0f, 0.0f, 0.0f));
	for (int cnt=0; cnt<6; cnt++)
		object_normals.push_back(CL_Vec3f(0.0f, 0.0f, 1.0f));
	for (int cnt=0; cnt<6; cnt++)
		object_normals.push_back(CL_Vec3f(0.0f, 1.0f, 0.0f));
	for (int cnt=0; cnt<6; cnt++)
		object_normals.push_back(CL_Vec3f(0.0f, 0.0f, -1.0f));
	for (int cnt=0; cnt<6; cnt++)
		object_normals.push_back(CL_Vec3f(0.0f, -1.0f, 0.0f));
	for (int cnt=0; cnt<6; cnt++)
		object_normals.push_back(CL_Vec3f(1.0f, 0.0f, 0.0f));

	for (int cnt=0; cnt<36; cnt++)
		object_material_ambient.push_back(material_ambient);
}

