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
*/

#include "precomp.h"
#include "app.h"

int App::start(const std::vector<std::string> &args)
{
	OpenGLWindowDescription description;
	description.set_title("UniformBlock Shader");
	//description.set_version(3, 1, false);
	description.set_size(Size(1024, 768), true);

	DisplayWindow window(description);
	InputDevice keyboard = window.get_ic().get_keyboard();
	GraphicContext gc = window.get_gc();

	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	Slot slot_window_close = window.sig_window_close().connect(this, &App::window_close);

	// Load and link shaders
	ProgramObject shader = ProgramObject::load(gc, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	if (!shader.link())
		throw Exception("Unable to link shader program: Error:" + shader.get_info_log());

	int block_size = shader.get_uniform_block_size("TestBlock");
	const int num_blocks = 16;
	ProgramUniformBlock block(gc, block_size * num_blocks);

	std::vector<float> data_to_upload;
	data_to_upload.resize((num_blocks * block_size) / sizeof(float) );

	float test_colour = 1.0f;
	for (int cnt=0; cnt<num_blocks; cnt++)
	{

		int offset = cnt * block_size / sizeof(float) ;
		data_to_upload[offset + 0] = test_colour;
		data_to_upload[offset + 1] = 0.5f;
		data_to_upload[offset + 2] = 1.0f;
		test_colour -= 0.05f;
	}

	block.upload_data(0, &data_to_upload[0], block_size * num_blocks);

	quit = false;

	Font font(gc, "tahoma", 32);

	unsigned int startTime = System::get_time();



		//// Test code
		//GLuint uniform_index = -1;
		//const char *names[] = {"src_color"};

		//glGetUniformIndices(handle, 1, names, &uniform_index);
		//if (uniform_index >=0)
		//{
		//		GLint offset;
		//		GLint singleSize;
		//		GLint uniform_type;
		//		glGetActiveUniformsiv(handle, 1, &uniform_index, GL_UNIFORM_TYPE, &uniform_type);
		//		glGetActiveUniformsiv(handle, 1, &uniform_index, GL_UNIFORM_OFFSET, &offset);
		//		glGetActiveUniformsiv(handle, 1, &uniform_index, GL_UNIFORM_SIZE, &singleSize);
		//		if ((uniform_type != GL_FLOAT_VEC3) || (offset !=0) || (singleSize != 1))
		//		{
		//			throw Exception("well it seems it does not work");
		//		}
		//}


	while (!quit)
	{
		gc.clear(Colorf(0.1f, 0.1f, 0.2f));
		OpenGL::check_error();


		for (int test_run_y=0; test_run_y < 16; test_run_y++)
		{
			shader.set_uniform_block("TestBlock", block, test_run_y*block_size);
			for (int test_run_x=0; test_run_x < 16; test_run_x++)
			{
				Vec2f positions[3];
				float size = 32.0f;
				positions[0].x = test_run_x * size;
				positions[0].y = test_run_y * size + size;
				positions[1].x = test_run_x * size + size;
				positions[1].y = test_run_y * size + size;
				positions[2].x = test_run_x * size + size;
				positions[2].y = test_run_y * size;

				PrimitivesArray prim_array(gc);
				prim_array.set_attributes(0, positions);
				gc.set_program_object(shader, cl_program_matrix_modelview_projection);
				gc.draw_primitives(cl_triangles, 3, prim_array);
				gc.reset_program_object();
			}
		}
		font.draw_text(gc, 32, 200, "Hello World");
		window.flip(1);

		KeepAlive::process();
	}

	return 0;
}


void App::window_close()
{
	quit = true;
}

void App::on_input_up(const InputEvent &key, const InputState &state)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}


}
