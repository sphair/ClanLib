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
#include "test.h"
#include "object.h"

const float g_CubeRadius = 8.0f;
const int g_CubeSegments = 12;

Test::Test() : quit(false)
{
}

void Test::on_input_up(const clan::InputEvent &key)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}
}

void Test::on_window_close()
{
	quit = true;
}

// The start of the Application
int Test::start(const std::vector<std::string> &args)
{
	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(false);
	win_desc.set_title("The fantastic Cube Test");
	win_desc.set_depth_size(16);
	clan::DisplayWindow window(win_desc);

 	clan::GraphicContext_GL1 gc = window.get_gc();
	clan::Canvas canvas(gc);
	// Connect the Window close event
	clan::Slot slot_quit = window.sig_window_close().connect(this, &Test::on_window_close);

	// Connect a keyboard handler to on_key_up()
	clan::Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Test::on_input_up);

	clan::Texture2D texture_image(gc, "Resources/tux.png");
	texture_image.set_wrap_mode(clan::cl_wrap_repeat, clan::cl_wrap_repeat);
	texture_image.set_min_filter(clan::cl_filter_linear);
	texture_image.set_mag_filter(clan::cl_filter_linear);

	float fov = 45.0f;
	float aspect = gc.get_width()/(float) gc.get_height();
	canvas.set_map_mode(clan::cl_user_projection);
	canvas.set_projection(clan::Mat4f::perspective(fov, aspect, 0.1f, 100.0f));

//FIXME	gc.set_culled(true);
//FIXME	gc.set_face_cull_mode(cl_cull_back);

//FIXME	gc.enable_depth_test(true);
//FIXME	gc.set_depth_compare_function(cl_comparefunc_lequal);
//FIXME	gc.enable_depth_write(true);

	Object cube;
	cube.CreateCube(g_CubeRadius * 2.0f, g_CubeSegments);
	cube.CalcPolygonNormal();

	float angle = 0.0f;

	clan::LightModel_GL1 light_model;
	light_model.enable_lighting(true);
	light_model.set_flat_shading(false);
	light_model.set_scene_ambient_light(clan::Colorf(0.2f, 0.2f, 0.2f, 1.0f));
	gc.set_light_model(light_model);

	clan::LightSource_GL1 light0;
	light0.set_spot_cutoff(135.0f);
	light0.set_spot_exponent(40.0f);
	light0.set_constant_attenuation(1.0f);
	light0.set_linear_attenuation(0.0f);
	light0.set_quadratic_attenuation(0.0f);
	light0.set_spot_direction(clan::Vec3f(0.0f, 0.0f, -1.0f));
	light0.set_diffuse_intensity(clan::Colorf(1.0f, 0.0f, 0.0f, 1.0f));
	light0.set_position(clan::Vec4f(0.0f, -2.0f, -30.0f, 1.0f));
	gc.set_light(0, light0);

	clan::LightSource_GL1 light1;
	light1.set_spot_cutoff(135.0f);
	light1.set_spot_exponent(40.0f);
	light1.set_constant_attenuation(1.0f);
	light1.set_linear_attenuation(0.0f);
	light1.set_quadratic_attenuation(0.0f);
	light1.set_spot_direction(clan::Vec3f(0.2f, 0.2f, -1.0f));
	light1.set_diffuse_intensity(clan::Colorf(0.0f, 1.0f, 0.0f, 1.0f));
	light1.set_position(clan::Vec4f(-5.0f, 2.0f, -30.0f, 1.0f));
	gc.set_light(1, light1);

	clan::LightSource_GL1 light2;
	light2.set_spot_cutoff(135.0f);
	light2.set_spot_exponent(40.0f);
	light2.set_constant_attenuation(1.0f);
	light2.set_linear_attenuation(0.0f);
	light2.set_quadratic_attenuation(0.0f);
	light2.set_spot_direction(clan::Vec3f(-0.2f, -0.2f, -1.0f));
	light2.set_diffuse_intensity(clan::Colorf(0.0f, 0.0f, 1.0f, 1.0f));
	light2.set_position(clan::Vec4f(5.0f, 2.0f, -30.0f, 1.0f));
	gc.set_light(2, light2);

//FIXME	cl1Enable(GL_NORMALIZE);

	while (!quit)
	{
		gc.clear(clan::Colorf(0.0f,0.0f,0.56f));

		gc.clear_depth(1.0f);

		clan::Mat4f matrix1 = clan::Mat4f::identity();
		canvas.set_modelview(matrix1);

		canvas.push_modelview();

		canvas.mult_translate(0.0f,0.0f,-40.0f);
		canvas.mult_rotate(clan::Angle(angle, clan::cl_degrees), 0.0f,1.0f,0.0f);
		canvas.mult_rotate(clan::Angle(angle, clan::cl_degrees),1.0f,0.0f,0.0f);
		angle+=0.5f;

		cube.Draw(gc, texture_image);

		canvas.pop_modelview();
		canvas.flush();

		window.flip(1);

		clan::KeepAlive::process();

	}
	return 0;
}
