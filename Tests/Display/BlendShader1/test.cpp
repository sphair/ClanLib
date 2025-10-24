#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class RectTexturePrimitive : public CL_PrimitivesArray
{
public:
	RectTexturePrimitive()
	: texunits(texcoords)
	{
		CL_Colorf c = CL_Colorf::white;
		color[0] = c.get_red();
		color[1] = c.get_green();
		color[2] = c.get_blue();
		color[3] = c.get_alpha();
		set_vertex(0,   0.0,   0.0, 0.0, 1.0);
		set_vertex(1, 640.0,   0.0, 1.0, 1.0);
		set_vertex(2,   0.0, 800.0, 0.0, 0.0);
		set_vertex(3,   0.0, 800.0, 0.0, 0.0);
		set_vertex(4, 640.0,   0.0, 1.0, 1.0);
		set_vertex(5, 640.0, 800.0, 1.0, 0.0);
	}

	void get_subarray(int index, PrimitivesSubarray &v) const
	{
		v.num_vertices = 6;
		v.vertex = vertex;
		v.normal = 0;
		v.primary_color = color;
		v.secondary_color = 0;
		v.edge_flag = 0;
		v.fog_coord = 0;
		v.num_tex_units = 1;
		v.tex_coords = &texunits;
		v.num_attribs = 0;
		v.use_primary_color_array = false;
		v.use_secondary_color_array = false;
	}

	int get_subarray_length() const { return 1; }

	void set_vertex(int i, double x, double y, double s, double t)
	{
		vertex[i*4 + 0] = x;
		vertex[i*4 + 1] = y;
		vertex[i*4 + 2] = 0.0;
		vertex[i*4 + 3] = 1.0;
		texcoords[i*4 + 0] = s;
		texcoords[i*4 + 1] = t;
		texcoords[i*4 + 2] = 0.0;
		texcoords[i*4 + 3] = 1.0;
	}

	double vertex[6*4];
	double *texunits;
	double texcoords[6*4];
	double color[4];
};

class App : public CL_ClanApplication
{
	int main(int argc, char** argv)
	{
		CL_ConsoleWindow console("Console");

		try
		{
			CL_SetupCore setup_core; 
			CL_SetupDisplay setup_disp;
			CL_SetupGL setup_gl;

			const char *blendhorz_source =
				"uniform sampler2D my_texture;"
				"uniform float distance;"
				"void main(void)"
				"{"
				"vec4 texval1 = texture2D(my_texture, vec2(gl_TexCoord[0]) - vec2(0, distance*2.0))*0.05;"
				"vec4 texval2 = texture2D(my_texture, vec2(gl_TexCoord[0]) - vec2(0, distance))*0.10;"
				"vec4 texval3 = texture2D(my_texture, vec2(gl_TexCoord[0]))*0.7;"
				"vec4 texval4 = texture2D(my_texture, vec2(gl_TexCoord[0]) + vec2(0, distance))*0.10;"
				"vec4 texval5 = texture2D(my_texture, vec2(gl_TexCoord[0]) + vec2(0, distance*2.0))*0.05;"
				"vec4 texsum = (texval1 + texval2 + texval3 + texval4 + texval5);"
				"gl_FragColor = vec4(texsum.x, texsum.y, texsum.z, 1.0);"
				"}";

			const char *blendvert_source =
				"uniform sampler2D my_texture;"
				"uniform float distance;"
				"void main(void)"
				"{"
				"vec4 texval1 = texture2D(my_texture, vec2(gl_TexCoord[0]) - vec2(distance*2.0, 0))*0.05;"
				"vec4 texval2 = texture2D(my_texture, vec2(gl_TexCoord[0]) - vec2(distance, 0))*0.10;"
				"vec4 texval3 = texture2D(my_texture, vec2(gl_TexCoord[0]))*0.7;"
				"vec4 texval4 = texture2D(my_texture, vec2(gl_TexCoord[0]) + vec2(distance, 0))*0.10;"
				"vec4 texval5 = texture2D(my_texture, vec2(gl_TexCoord[0]) + vec2(distance*2.0, 0))*0.05;"
				"vec4 texsum = (texval1 + texval2 + texval3 + texval4 + texval5);"
				"gl_FragColor = vec4(texsum.x, texsum.y, texsum.z, 1.0);"
				"}";

			CL_DisplayWindow w("BlendShader1", 640, 800);
			CL_GraphicContext gc = w.get_gc();
			gc.clear(CL_Colorf::white);

			CL_ShaderObject blendhorz_shader(cl_shadertype_fragment, blendhorz_source, gc);
			if (!blendhorz_shader.get_compile_status())
			{
				CL_Console::write_line("Blend Horizontal shader compile failed:");
				CL_Console::write_line(blendhorz_shader.get_info_log());
			}
			CL_ProgramObject blendhorz_program(gc);
			blendhorz_program.attach(blendhorz_shader);
			blendhorz_program.link();
			if (!blendhorz_program.get_link_status())
			{
				CL_Console::write_line("Blend Horizontal program link failed:");
				CL_Console::write_line(blendhorz_program.get_info_log());
			}

			CL_ShaderObject blendvert_shader(cl_shadertype_fragment, blendvert_source, gc);
			if (!blendvert_shader.get_compile_status())
			{
				CL_Console::write_line("Blend Horizontal shader compile failed:");
				CL_Console::write_line(blendvert_shader.get_info_log());
			}
			CL_ProgramObject blendvert_program(gc);
			blendvert_program.attach(blendvert_shader);
			blendvert_program.link();
			if (!blendvert_program.get_link_status())
			{
				CL_Console::write_line("Blend Horizontal program link failed:");
				CL_Console::write_line(blendvert_program.get_info_log());
			}

			CL_Sprite sprite("image.png", gc);

			CL_Texture texture(gc, 640, 800, 1, 0);

			CL_Font f2(gc, "Tahoma", 32, 0, 0.0, 0.0, 0, false, true, false, false);
			CL_Font f3(gc, "Tahoma", 55, 0, 0.0, 0.0, 0, false, true, false, false);
			gc.set_map_mode(cl_map_2d_upper_left);
			int i = 10;
			int dir = 0;
			while (!w.get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE))
			{
				// Draw light source:
				gc.clear(CL_Colorf::whitesmoke);

				// Draw scene objects:
				CL_BlendMode blend_mode;
				blend_mode.enable_blending(true);
				gc.set_texture(0, CL_Texture());
				sprite.draw(gc, 0.0, 0.0);
				gc.set_texture(0, CL_Texture());
				gc.set_blend_mode(blend_mode);
				gc.set_font(f2);
				gc.draw_text(100, 140, "Blend Fragment Shader Test #1", CL_Colorf::black);
				gc.set_font(f3);
				gc.draw_text(100, 240, "To blend or not to blend!", CL_Colorf::darkgoldenrod);

				i += dir;
				if (i == 99)
					dir = -1;
				if (i == 10)
					dir = 1;
//				if ((i/10)%2)
				{
					//: Draw Blend horizontal:
					texture.copy_image_from(0, 0, 640, 800, 0, 0, CL_RGB, &gc);
					texture.set_min_filter(cl_filter_nearest);
					texture.set_mag_filter(cl_filter_nearest);
					gc.set_texture(0, texture);
					gc.set_program_object(blendhorz_program);
					blendhorz_program.set_uniform("my_texture", 0);
					blendhorz_program.set_uniform("distance", i/1000.0f);
					gc.draw_primitives(cl_triangles, RectTexturePrimitive());
					gc.reset_program_object();
					gc.reset_texture(0);

					//: Draw Blend vertical:
					texture.copy_image_from(0, 0, 640, 800, 0, 0, CL_RGB, &gc);
					texture.set_min_filter(cl_filter_nearest);
					texture.set_mag_filter(cl_filter_nearest);
					gc.set_texture(0, texture);
					gc.set_program_object(blendvert_program);
					blendvert_program.set_uniform("my_texture", 0);
					blendvert_program.set_uniform("distance", i/1000.0f);
					gc.draw_primitives(cl_triangles, RectTexturePrimitive());
					gc.reset_program_object();
					gc.reset_texture(0);
				}

				w.flip(1);
				CL_DisplayMessageQueue::process();
			}
			return 0;
		}
		catch(CL_Exception error)
		{
			CL_Console::write_line("Exception caught:");
			CL_Console::write_line(error.message);
			console.display_close_message();

			return -1;
		}

		return 0;
	}
} app;
