/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include <cstdlib>

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
    SlotContainer cc; 
	DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Vertex Buffer Object Example");
	win_desc.set_depth_size(16);

	win_desc.set_size(Size( 800, 600 ), false);

	DisplayWindow window(win_desc);
    cc.connect(window.sig_window_close(), std::function<void()>(this, &App::on_window_close));
	cc.connect(window.get_ic().get_keyboard().sig_key_up(), std::function<void(const clan::InputEvent&)>(this, &App::on_input_up));

	Canvas canvas(window);

	Shader shader(canvas);

	// Prepare the display
	RasterizerStateDescription rasterizer_state_desc;
	rasterizer_state_desc.set_culled(true);
	rasterizer_state_desc.set_face_cull_mode(cull_back);
	rasterizer_state_desc.set_front_face(face_clockwise);
	RasterizerState raster_state(canvas, rasterizer_state_desc);

	DepthStencilStateDescription depth_state_desc;
	depth_state_desc.enable_depth_write(true);
	depth_state_desc.enable_depth_test(true);
	depth_state_desc.enable_stencil_test(false);
	depth_state_desc.set_depth_compare_function(compare_lequal);
	DepthStencilState depth_write_enabled(canvas, depth_state_desc);

	std::vector<Vec3f> object_positions;
	std::vector<Vec3f> object_normals;
	std::vector<Vec4f> object_material_ambient;

	const int num_cubes = 200;
	object_positions.reserve(num_cubes * 6 * 6);	// 6 faces, and 6 vertices per face
	object_normals.reserve(num_cubes * 6 * 6);
	object_material_ambient.reserve(num_cubes * 6 * 6);

	for (int cnt=0; cnt < num_cubes; cnt++)
	{
		create_cube(object_positions, object_normals, object_material_ambient);
	}

	VertexArrayBuffer vb_positions(canvas, sizeof(Vec3f) * object_positions.size());
	vb_positions.upload_data(canvas, 0, &object_positions[0], sizeof(Vec3f) * object_positions.size()/2);
	vb_positions.upload_data(canvas, sizeof(Vec3f) * object_positions.size()/2, &object_positions[object_positions.size()/2], sizeof(Vec3f) * object_positions.size()/2);

	VertexArrayBuffer vb_normals(canvas, &object_normals[0], sizeof(Vec3f) * object_normals.size());
	VertexArrayBuffer vb_material_ambient(canvas, &object_material_ambient[0], sizeof(Vec4f) * object_material_ambient.size());

	// ** Note, at this point "object_positions, object_normals and object_material_ambient" can be destroyed.

	clan::Font fps_font(canvas, "tahoma", 20);

	GameTime game_time;

	float angle = 0.0f;

	while (!quit)
	{
		game_time.update();

		canvas.clear(Colorf(0.0f, 0.0f, 0.0f, 1.0f));
		canvas.clear_depth(1.0f);

		std::string fps = string_format("%1 fps", clan::StringHelp::float_to_text(game_time.get_updates_per_second(), 1));
		fps_font.draw_text(canvas, canvas.get_width() - 100, 30, fps);
		std::string info = string_format("%1 vertices", (int) object_positions.size());
		fps_font.draw_text(canvas, 30, 30, info);

		Mat4f perspective_matrix = Mat4f::perspective(45.0f, ((float) canvas.get_width()) / ((float) canvas.get_height()), 0.1f, 10000.0f, handed_left, canvas.get_gc().get_clip_z_range() );

		angle += game_time.get_time_elapsed() * 50.0f;
		if (angle >= 360.0f)
			angle -= 360.0f;

		Mat4f modelview_matrix = Mat4f::identity();
		modelview_matrix = modelview_matrix.translate(0.0f, 0.0f, 800.0f);
		modelview_matrix = modelview_matrix * Mat4f::rotate(Angle(angle*2.0f, angle_degrees), 0.0f, 1.0f, 0.0f, false);
		modelview_matrix = modelview_matrix * Mat4f::rotate(Angle(angle, angle_degrees), 1.0f, 0.0f, 0.0f, false);


		canvas.set_depth_stencil_state(depth_write_enabled);
		canvas.set_rasterizer_state(raster_state);

		PrimitivesArray prim_array(canvas);

		prim_array.set_attributes(0, vb_positions, 3, type_float, 0);
		prim_array.set_attributes(1, vb_normals, 3, type_float, 0);
		prim_array.set_attributes(2, vb_material_ambient, 4, type_float, 0);

		Mat4f matrix_modelview_projection = perspective_matrix *  modelview_matrix;
		Mat3f normal_matrix = Mat3f(modelview_matrix);
		normal_matrix.inverse();
		normal_matrix.transpose();

		shader.Use(canvas, modelview_matrix, matrix_modelview_projection, Mat4f(normal_matrix));

		canvas.get_gc().draw_primitives(type_triangles, object_positions.size(), prim_array);
		canvas.get_gc().reset_program_object();
		canvas.reset_rasterizer_state();
		canvas.reset_depth_stencil_state();

		window.flip(1);
		KeepAlive::process();
	}

	return 0;
}

// A key was pressed
void App::on_input_up(const InputEvent &key)
{
	switch (key.id)
	{
		case keycode_escape:
			quit = true;
			break;
	}

}

// The window was closed
void App::on_window_close()
{
	quit = true;
}


void App::create_cube( std::vector<Vec3f> &object_positions, std::vector<Vec3f> &object_normals, std::vector<Vec4f> &object_material_ambient )
{
	Vec3f position;
	position.x = (float) ((rand() & 0x1FF) - 0xFF);
	position.y = (float) ((rand() & 0x1FF) - 0xFF);
	position.z = (float) ((rand() & 0x1FF) - 0xFF);

	Vec4f material_ambient;
	material_ambient.r = ((float) (rand() & 0xFF)) / 255.0f;
	material_ambient.g = ((float) (rand() & 0xFF)) / 255.0f;
	material_ambient.b = ((float) (rand() & 0xFF)) / 255.0f;
	material_ambient.a = 1.0f;

	Vec3f size(10.0f, 10.0f, 10.0f);

	object_positions.push_back(Vec3f(-size.x, size.y, size.z) +position);
	object_positions.push_back(Vec3f(-size.x, size.y, -size.z) +position);
	object_positions.push_back(Vec3f(-size.x, -size.y, -size.z) +position);
	object_positions.push_back(Vec3f(-size.x, -size.y, -size.z) +position);
	object_positions.push_back(Vec3f(-size.x, -size.y, size.z) +position);
	object_positions.push_back(Vec3f(-size.x, size.y, size.z) +position);

	object_positions.push_back(Vec3f(-size.x, size.y, size.z) +position);
	object_positions.push_back(Vec3f(-size.x, -size.y, size.z) +position);
	object_positions.push_back(Vec3f(size.x, -size.y, size.z) +position);
	object_positions.push_back(Vec3f(size.x, -size.y, size.z) +position);
	object_positions.push_back(Vec3f(size.x, size.y, size.z) +position);
	object_positions.push_back(Vec3f(-size.x, size.y, size.z) +position);

	object_positions.push_back(Vec3f(-size.x, size.y, size.z) +position);
	object_positions.push_back(Vec3f(size.x, size.y, size.z) +position);
	object_positions.push_back(Vec3f(size.x, size.y, -size.z) +position);
	object_positions.push_back(Vec3f(size.x, size.y, -size.z) +position);
	object_positions.push_back(Vec3f(-size.x, size.y, -size.z) +position);
	object_positions.push_back(Vec3f(-size.x, size.y, size.z) +position);

	object_positions.push_back(Vec3f(size.x, -size.y, -size.z) +position);
	object_positions.push_back(Vec3f(-size.x, -size.y, -size.z) +position);
	object_positions.push_back(Vec3f(-size.x, size.y, -size.z) +position);
	object_positions.push_back(Vec3f(-size.x, size.y, -size.z) +position);
	object_positions.push_back(Vec3f(size.x, size.y, -size.z) +position);
	object_positions.push_back(Vec3f(size.x, -size.y, -size.z) +position);

	object_positions.push_back(Vec3f(size.x, -size.y, -size.z) +position);
	object_positions.push_back(Vec3f(size.x, -size.y, size.z) +position);
	object_positions.push_back(Vec3f(-size.x, -size.y, size.z) +position);
	object_positions.push_back(Vec3f(-size.x, -size.y, size.z) +position);
	object_positions.push_back(Vec3f(-size.x, -size.y, -size.z) +position);
	object_positions.push_back(Vec3f(size.x, -size.y, -size.z) +position);

	object_positions.push_back(Vec3f(size.x, -size.y, -size.z) +position);
	object_positions.push_back(Vec3f(size.x, size.y, -size.z) +position);
	object_positions.push_back(Vec3f(size.x, size.y, size.z) +position);
	object_positions.push_back(Vec3f(size.x, size.y, size.z) +position);
	object_positions.push_back(Vec3f(size.x, -size.y, size.z) +position);
	object_positions.push_back(Vec3f(size.x, -size.y, -size.z) +position);

	for (int cnt=0; cnt<6; cnt++)
		object_normals.push_back(Vec3f(-1.0f, 0.0f, 0.0f));
	for (int cnt=0; cnt<6; cnt++)
		object_normals.push_back(Vec3f(0.0f, 0.0f, 1.0f));
	for (int cnt=0; cnt<6; cnt++)
		object_normals.push_back(Vec3f(0.0f, 1.0f, 0.0f));
	for (int cnt=0; cnt<6; cnt++)
		object_normals.push_back(Vec3f(0.0f, 0.0f, -1.0f));
	for (int cnt=0; cnt<6; cnt++)
		object_normals.push_back(Vec3f(0.0f, -1.0f, 0.0f));
	for (int cnt=0; cnt<6; cnt++)
		object_normals.push_back(Vec3f(1.0f, 0.0f, 0.0f));

	for (int cnt=0; cnt<36; cnt++)
		object_material_ambient.push_back(material_ambient);
}

