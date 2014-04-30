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
#include "options.h"
#include <cstdlib>

const char text_shader_vertex[] =
	"#version 150\n"
	"\n"
	"in vec3 InPosition;"
	"in vec4 InColor;\n"
	"\n"
	"layout (std140) uniform ProgramUniforms\n"
	"{\n"
		"mat4 cl_ModelViewProjectionMatrix;\n"
	"};\n"
	"out vec4 Color;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	vec4 in_position = vec4(InPosition.xyz, 1.0);\n"
	"	gl_Position = cl_ModelViewProjectionMatrix * in_position;\n"
	"	Color = InColor;\n"
	"}\n"
	;

const char text_shader_fragment[] =
	"#version 120\n"
	"\n"
	"uniform sampler2D Texture0;\n"
	"in vec4 Color;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = texture2D(Texture0, gl_PointCoord) * Color;\n"
	"}\n"
	;


App::App() : quit(false)
{
}

struct ProgramUniforms
{
	clan::Mat4f cl_ModelViewProjectionMatrix;
};

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	clan::DisplayWindowDescription win_desc;
	//win_desc.set_version(3, 2, false);
	win_desc.set_allow_resize(true);
	win_desc.set_title("Point Sprite Example");
	win_desc.set_size(clan::Size( 800, 480 ), false);

	clan::DisplayWindow window(win_desc);
    clan::CallbackContainer cc;
	cc.connect(window.sig_window_close(), {this, &App::on_window_close});
	cc.connect(window.get_ic().get_keyboard().sig_key_up(), {this, &App::on_input_up});

	std::string theme;
	if (clan::FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (clan::FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw clan::Exception("No themes found");

	clan::GUIWindowManagerTexture wm(window);
	clan::GUIManager gui(wm, theme);
	
	clan::Canvas canvas(window);

	// Deleted automatically by the GUI
	Options *options = new Options(gui, clan::Rect(0, 0, canvas.get_size()));

	clan::Image image_grid(canvas, "../Blend/Resources/grid.png");
	clan::Texture2D texture_particle(canvas, "Resources/particle.png");
	float grid_width = (float) image_grid.get_width();
	float grid_height = (float) image_grid.get_height();

	grid_space = (float) (image_grid.get_width());

	setup_particles();

	clan::ShaderObject vertex_shader(canvas, clan::shadertype_vertex, text_shader_vertex);
	if(!vertex_shader.compile())
	{
		throw clan::Exception(clan::string_format("Unable to compile vertex shader object: %1", vertex_shader.get_info_log()));
	}

	clan::ShaderObject fragment_shader(canvas, clan::shadertype_fragment, text_shader_fragment);
	if(!fragment_shader.compile())
	{
		throw clan::Exception(clan::string_format("Unable to compile fragment shader object: %1", fragment_shader.get_info_log()));
	}

	clan::ProgramObject program_object(canvas);
	program_object.attach(vertex_shader);
	program_object.attach(fragment_shader);
	program_object.bind_attribute_location(0, "InPosition");
	program_object.bind_attribute_location(1, "InColor");
	if (!program_object.link())
	{
		throw clan::Exception(clan::string_format("Unable to link program object: %1", program_object.get_info_log()));
	}
	program_object.set_uniform1i("Texture0", 0);

	options->request_repaint();

	clan::BlendStateDescription blend_state_desc;
	blend_state_desc.enable_blending(true);
	blend_state_desc.set_blend_function(clan::blend_src_alpha, clan::blend_one, clan::blend_src_alpha, clan::blend_one);
	clan::BlendState blend_state(canvas, blend_state_desc);

	clan::GameTime game_time;

	while (!quit)
	{
		game_time.update();

		wm.process();
		wm.draw_windows(canvas);

		int num_particles = options->num_particles;
		if (num_particles > max_particles)
			num_particles = max_particles;

		move_particles(game_time.get_time_elapsed(), num_particles);

		const float grid_xpos = 10.0f;
		const float grid_ypos = 10.0f;

		// Draw the grid
		image_grid.draw(canvas, grid_xpos, grid_ypos);

		if (num_particles > 0)
		{
			std::vector<clan::Vec2f> positions;
			std::vector<clan::Colorf> colors;
			positions.resize(num_particles);
			colors.resize(num_particles);

			for (int cnt=0; cnt<num_particles; cnt++)
			{
				positions[cnt] = clan::Vec2f(grid_xpos + particles[cnt].xpos, grid_ypos + particles[cnt].ypos);
				switch (cnt % 3)
				{
					case 0:
						colors[cnt] = clan::Colorf(1.0f, 0.0f, 0.0f, 1.0f);
						break;
					case 1:
						colors[cnt] = clan::Colorf(0.0f, 1.0f, 0.0f, 1.0f);
						break;
					case 2:
						colors[cnt] = clan::Colorf(0.0f, 0.0f, 1.0f, 1.0f);
						break;
				}
			};

			canvas.flush();
			clan::GraphicContext gc = canvas.get_gc();

			canvas.set_blend_state(blend_state);

			clan::RasterizerStateDescription raster_state_desc;
			raster_state_desc.set_point_size(options->point_size);
			raster_state_desc.set_point_sprite_origin(clan::origin_upper_left);
			clan::RasterizerState raster_state(canvas, raster_state_desc);
			canvas.set_rasterizer_state(raster_state);

			clan::PrimitivesArray primarray(gc);

			clan::VertexArrayVector<clan::Vec2f> gpu_positions = clan::VertexArrayVector<clan::Vec2f>(gc, &positions[0], positions.size());
			clan::VertexArrayVector<clan::Colorf> gpu_colors = clan::VertexArrayVector<clan::Colorf>(gc, &colors[0], colors.size());

			primarray.set_attributes(0, gpu_positions);
			primarray.set_attributes(1, gpu_colors);

			ProgramUniforms buffer;
			buffer.cl_ModelViewProjectionMatrix = canvas.get_projection() * canvas.get_modelview();
			clan::UniformVector<ProgramUniforms> uniform_vector(gc, &buffer, 1);
			gc.set_uniform_buffer(0, uniform_vector);

			gc.set_texture(0, texture_particle);
			gc.set_program_object(program_object);
			gc.draw_primitives(clan::type_points, num_particles, primarray);
			gc.reset_program_object();
			gc.reset_texture(0);

			gc.reset_blend_state();
			gc.reset_rasterizer_state();
		}

		window.flip(1);

		clan::KeepAlive::process();
	}
	return 0;
}

// A key was pressed
void App::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}

}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

void App::setup_particles()
{
	for (int cnt=0; cnt<max_particles; cnt++)
	{
		particles[cnt].xpos = (float) (rand() % (int) grid_space);
		particles[cnt].ypos = (float) (rand() % (int) grid_space);
		particles[cnt].xspeed = (float) ((rand() & 0xFF)+0x7F);
		particles[cnt].yspeed = (float) ((rand() & 0xFF)+0x7F);
		particles[cnt].xdir = ((rand() & 1) == 0);
		particles[cnt].ydir = ((rand() & 1) == 0);
	}
}

void App::move_particles(float time_diff, int num_particles)
{
	for (int cnt=0; cnt<num_particles; cnt++)
	{
		float xdisp = (particles[cnt].xspeed * time_diff) * 0.5f;
		float ydisp = (particles[cnt].yspeed * time_diff) * 0.5f;

		if (particles[cnt].xdir)
		{
			particles[cnt].xpos += xdisp;
			if (particles[cnt].xpos >= (float) grid_space)
			{
				particles[cnt].xpos = grid_space;
				particles[cnt].xdir = false;
			}
		}
		else
		{
			particles[cnt].xpos -= xdisp;
			if (particles[cnt].xpos <= 0.0f)
			{
				particles[cnt].xpos = 0.0f;
				particles[cnt].xdir = true;
			}
		}

		if (particles[cnt].ydir)
		{
			particles[cnt].ypos += ydisp;
			if (particles[cnt].ypos >= (float) grid_space)
			{
				particles[cnt].ypos = grid_space;
				particles[cnt].ydir = false;
			}
		}
		else
		{
			particles[cnt].ypos -= ydisp;
			if (particles[cnt].ypos <= 0.0f)
			{
				particles[cnt].ypos = 0.0f;
				particles[cnt].ydir = true;
			}
		}
	}
}
