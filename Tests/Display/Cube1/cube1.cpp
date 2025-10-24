
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#ifdef WIN32
#include <windows.h>
#endif

void link_shader(CL_ProgramObject program_object);

static CL_Vec3d positions400[6*4*400];
static CL_Vec3d normals400[6*4*400];
static CL_Vec2d texcoords400[6*4*400];
static CL_Vec4d colors400[6*4*400];
static CL_Vec1d k_attributes400[6*4*400];

#define attribute_bind_position_k 15

static CL_Vec3d positions[6*4] = 
{
	CL_Vec3d(-0.5, -0.5,  0.5),
	CL_Vec3d( 0.5, -0.5,  0.5),
	CL_Vec3d( 0.5,  0.5,  0.5),
	CL_Vec3d(-0.5,  0.5,  0.5),

	CL_Vec3d(-0.5, -0.5, -0.5),
	CL_Vec3d(-0.5,  0.5, -0.5),
	CL_Vec3d( 0.5,  0.5, -0.5),
	CL_Vec3d( 0.5, -0.5, -0.5),

	CL_Vec3d(-0.5,  0.5, -0.5),
	CL_Vec3d(-0.5,  0.5,  0.5),
	CL_Vec3d( 0.5,  0.5,  0.5),
	CL_Vec3d( 0.5,  0.5, -0.5),

	CL_Vec3d(-0.5, -0.5, -0.5),
	CL_Vec3d( 0.5, -0.5, -0.5),
	CL_Vec3d( 0.5, -0.5,  0.5),
	CL_Vec3d(-0.5, -0.5,  0.5),

	CL_Vec3d( 0.5, -0.5, -0.5),
	CL_Vec3d( 0.5,  0.5, -0.5),
	CL_Vec3d( 0.5,  0.5,  0.5),
	CL_Vec3d( 0.5, -0.5,  0.5),

	CL_Vec3d(-0.5, -0.5, -0.5),
	CL_Vec3d(-0.5, -0.5,  0.5),
	CL_Vec3d(-0.5,  0.5,  0.5),
	CL_Vec3d(-0.5,  0.5, -0.5)
};

static CL_Vec3d normals[6*4] =
{
	 CL_Vec3d(0.0, 0.0, 1.0),
	 CL_Vec3d(0.0, 0.0, 1.0),
	 CL_Vec3d(0.0, 0.0, 1.0),
	 CL_Vec3d(0.0, 0.0, 1.0),

	 CL_Vec3d(0.0, 0.0,-1.0),
	 CL_Vec3d(0.0, 0.0,-1.0),
	 CL_Vec3d(0.0, 0.0,-1.0),
	 CL_Vec3d(0.0, 0.0,-1.0),

	 CL_Vec3d(0.0, 1.0, 0.0),
	 CL_Vec3d(0.0, 1.0, 0.0),
	 CL_Vec3d(0.0, 1.0, 0.0),
	 CL_Vec3d(0.0, 1.0, 0.0),

	 CL_Vec3d(0.0,-1.0, 0.0),
	 CL_Vec3d(0.0,-1.0, 0.0),
	 CL_Vec3d(0.0,-1.0, 0.0),
	 CL_Vec3d(0.0,-1.0, 0.0),

	 CL_Vec3d(1.0, 0.0, 0.0),
	 CL_Vec3d(1.0, 0.0, 0.0),
	 CL_Vec3d(1.0, 0.0, 0.0),
	 CL_Vec3d(1.0, 0.0, 0.0),

	 CL_Vec3d(-1.0, 0.0, 0.0),
	 CL_Vec3d(-1.0, 0.0, 0.0),
	 CL_Vec3d(-1.0, 0.0, 0.0),
	 CL_Vec3d(-1.0, 0.0, 0.0)
};

static CL_Vec2d texcoords[6*4] =
{
	CL_Vec2d(0.0, 0.0),
	CL_Vec2d(1.0, 0.0),
	CL_Vec2d(1.0, 1.0),
	CL_Vec2d(0.0, 1.0),

	CL_Vec2d(0.0, 0.0),
	CL_Vec2d(1.0, 0.0),
	CL_Vec2d(1.0, 1.0),
	CL_Vec2d(0.0, 1.0),

	CL_Vec2d(0.0, 0.0),
	CL_Vec2d(1.0, 0.0),
	CL_Vec2d(1.0, 1.0),
	CL_Vec2d(0.0, 1.0),

	CL_Vec2d(0.0, 0.0),
	CL_Vec2d(1.0, 0.0),
	CL_Vec2d(1.0, 1.0),
	CL_Vec2d(0.0, 1.0),

	CL_Vec2d(0.0, 0.0),
	CL_Vec2d(1.0, 0.0),
	CL_Vec2d(1.0, 1.0),
	CL_Vec2d(0.0, 1.0),

	CL_Vec2d(0.0, 0.0),
	CL_Vec2d(1.0, 0.0),
	CL_Vec2d(1.0, 1.0),
	CL_Vec2d(0.0, 1.0)
};

static CL_Vec4d colors[6*4];

static CL_Vec1d k_attributes[6*4];

#ifdef WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(void)
#endif
{
	try
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;

		CL_OpenGLWindowDescription description;
		description.set_title(cl_text("Cube1 Test"));
		description.set_size(CL_Size(1024, 768), true);
		description.set_depth_size(16);

		CL_DisplayWindow window(description);
		CL_InputContext ic = window.get_ic();
		CL_InputDevice keyboard = ic.get_keyboard();
		CL_GraphicContext gc = window.get_gc();

		CL_PixelBuffer image = CL_ImageProviderFactory::load(cl_text("image-p2.png"));
		CL_Texture texture1(gc, image.get_width(), image.get_height());
		texture1.set_generate_mipmap(true);
		texture1.set_subimage(0, 0, image);
		texture1.set_min_filter(cl_filter_linear_mipmap_linear);
		texture1.set_mag_filter(cl_filter_linear_mipmap_linear);

		double fov = 45.0;
		double aspect = gc.get_width()/(double) gc.get_height();
		gc.set_map_mode(cl_user_projection);
		gc.set_projection(CL_Mat4f::perspective(fov, aspect, 0.1, 100.0));

		CL_PolygonRasterizer polygon_rasterizer;
		polygon_rasterizer.set_culled(true);
		polygon_rasterizer.set_face_cull_mode(cl_cull_front);
		gc.set_polygon_rasterizer(polygon_rasterizer);

		CL_BufferControl buffer_control;
		buffer_control.enable_depth_test(true);
		buffer_control.set_depth_compare_function(cl_comparefunc_lequal);
		buffer_control.enable_depth_write(true);
		gc.set_buffer_control(buffer_control);

		CL_VirtualDirectory vd;

		CL_ProgramObject cube_group1_shader = CL_ProgramObject::load(gc, cl_text("cube_group1.glsl"), cl_text("cube_group1_fragment.glsl"), vd);
		cube_group1_shader.bind_attribute_location(attribute_bind_position_k, cl_text("k"));
		link_shader(cube_group1_shader);

		CL_ProgramObject cube_group2_shader = CL_ProgramObject::load(gc, cl_text("cube_group2.glsl"), cl_text("cube_group1_fragment.glsl"), vd);
		cube_group2_shader.bind_attribute_location(attribute_bind_position_k, cl_text("k"));
		link_shader(cube_group2_shader);

		CL_ProgramObject cube_group3_shader = CL_ProgramObject::load(gc, cl_text("cube_group3.glsl"), cl_text("cube_group1_fragment.glsl"), vd);
		cube_group3_shader.bind_attribute_location(attribute_bind_position_k, cl_text("k"));
		link_shader(cube_group3_shader);

		CL_ProgramObject cube_group4_shader = CL_ProgramObject::load(gc, cl_text("cube_group4.glsl"), cl_text("cube_group1_fragment.glsl"), vd);
		cube_group4_shader.bind_attribute_location(attribute_bind_position_k, cl_text("k"));
		link_shader(cube_group4_shader);

		CL_ProgramObject cube_group5_shader = CL_ProgramObject::load(gc, cl_text("cube_group5.glsl"), cl_text("cube_group1_fragment.glsl"), vd);
		cube_group5_shader.bind_attribute_location(attribute_bind_position_k, cl_text("k"));
		link_shader(cube_group5_shader);

		CL_ProgramObject cube_group6_shader = CL_ProgramObject::load(gc, cl_text("cube_group6.glsl"), cl_text("cube_group1_fragment.glsl"), vd);
		cube_group6_shader.bind_attribute_location(attribute_bind_position_k, cl_text("k"));
		link_shader(cube_group6_shader);

		for (int l = 0; l < 400; l++)
		{
			for (int j = 0; j < 6*4; j++)
				positions400[l*6*4+j] = positions[j];
			for (int j = 0; j < 6*4; j++)
				normals400[l*6*4+j] = normals[j];
			for (int j = 0; j < 6*4; j++)
				texcoords400[l*6*4+j] = texcoords[j];
			for (int j = 0; j < 6*4; j++)
			{
				colors400[l*6*4+j].r = 1.0;
				colors400[l*6*4+j].g = 1.0;
				colors400[l*6*4+j].b = 1.0;
				colors400[l*6*4+j].a = 1.0;
//				colors400[l*6*4+j] = colors[j];
			}
			for (int j = 0; j < 6*4; j++)
				k_attributes400[l*6*4+j].x = l;
		}
/*
		unsigned int vertex_elements[400*6*4];
		for (int i = 0; i < 400; i++)
		{
			for (int j = 0; j < 6*4; j++)
				vertex_elements[i*6*4+j] = j;
		}
*/
		double spinner0 = 0.0;
		double spinner1 = 0.0;
		double spinner2 = 0.0;

		double s0 = 0;
		double deltas0 = 0.25;

		CL_Mat4f identity_matrix = CL_Mat4f::identity();

		int start_time = CL_System::get_time();
		int cur_frame = 0;

		while (!keyboard.get_keycode(CL_KEY_ESCAPE))
		{
			cur_frame++;

			gc.clear(CL_Colorf(0.05f, 0.05f, 0.05f, 1.0f));
			gc.clear_depth(1.0);

			double sin0 = (sin(s0) + 1.0) / 2.0;

			CL_LightModel light_model;
			light_model.enable_lighting(true);
			light_model.set_flat_shading(false);
			light_model.set_scene_ambient_light(CL_Colorf(0.5f, 0.5f, 0.8f, 1.0f));
			gc.set_light_model(light_model);

			CL_LightSource light0;
			light0.set_diffuse_intensity(CL_Colorf(1.0f, 1.0f, 0.8f, 1.0f));
			light0.set_position(CL_Vector(-10.0, -10.0, -10.0, 1.0));

			gc.set_modelview(CL_Matrix4x4::identity());
			gc.set_light(0, light0);

			CL_Material material;
			material.set_specular(CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
			material.set_diffuse(CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
			material.set_shininess(25.0);
			gc.set_material(material);

			gc.set_color_material(cl_color_material_ambient_and_diffuse);
			gc.set_modelview(CL_Matrix4x4::identity());

			CL_PrimitivesArray box_primitive(gc);
			box_primitive.set_positions(positions400);
			box_primitive.set_normals(normals400);
			box_primitive.set_primary_colors(colors400);
			box_primitive.set_tex_coords(0, texcoords400);
			box_primitive.set_attributes(attribute_bind_position_k, k_attributes400);

			gc.set_texture(0, texture1);

			gc.set_primitives_array(box_primitive);
			gc.set_program_object(cube_group1_shader);
			cube_group1_shader.set_uniform(cl_text("spinner0"), (float) spinner0);
			cube_group1_shader.set_uniform(cl_text("spinner1"), (float) spinner1);
			cube_group1_shader.set_uniform(cl_text("sin0"), (float) sin0);
			cube_group1_shader.set_uniform(cl_text("texture1"), 0);
			gc.draw_primitives_array(cl_quads, 400*6*4);
//			gc.draw_primitives_array(cl_quads, vertex_elements, 400*6*4);
			gc.reset_program_object();

			gc.set_program_object(cube_group2_shader);
			cube_group2_shader.set_uniform(cl_text("spinner0"), (float) spinner0);
			cube_group2_shader.set_uniform(cl_text("sin0"), (float) sin0);
			cube_group2_shader.set_uniform(cl_text("texture1"), 0);
			gc.draw_primitives_array(cl_quads, 400*6*4);
//			gc.draw_primitives_array(cl_quads, vertex_elements, 400*6*4);
			gc.reset_program_object();

			gc.set_program_object(cube_group3_shader);
			cube_group3_shader.set_uniform(cl_text("spinner0"), (float) spinner0);
			cube_group3_shader.set_uniform(cl_text("spinner1"), (float) spinner1);
			cube_group3_shader.set_uniform(cl_text("sin0"), (float) sin0);
			cube_group3_shader.set_uniform(cl_text("texture1"), 0);
			gc.draw_primitives_array(cl_quads, 400*6*4);
//			gc.draw_primitives_array(cl_quads, vertex_elements, 400*6*4);
			gc.reset_program_object();

			gc.set_program_object(cube_group4_shader);
			cube_group4_shader.set_uniform(cl_text("spinner0"), (float) spinner0);
			cube_group4_shader.set_uniform(cl_text("spinner1"), (float) spinner1);
			cube_group4_shader.set_uniform(cl_text("sin0"), (float) sin0);
			cube_group4_shader.set_uniform(cl_text("texture1"), 0);
			gc.draw_primitives_array(cl_quads, 400*6*4);
//			gc.draw_primitives_array(cl_quads, vertex_elements, 400*6*4);
			gc.reset_program_object();

			gc.set_program_object(cube_group5_shader);
			cube_group5_shader.set_uniform(cl_text("spinner0"), (float) spinner0);
			cube_group5_shader.set_uniform(cl_text("spinner1"), (float) spinner1);
			cube_group5_shader.set_uniform(cl_text("sin0"), (float) sin0);
			cube_group5_shader.set_uniform(cl_text("texture1"), 0);
			gc.draw_primitives_array(cl_quads, 400*6*4);
//			gc.draw_primitives_array(cl_quads, vertex_elements, 400*6*4);
			gc.reset_program_object();

			gc.set_program_object(cube_group6_shader);
			cube_group6_shader.set_uniform(cl_text("spinner0"), (float) spinner0);
			cube_group6_shader.set_uniform(cl_text("sin0"), (float) sin0);
			cube_group6_shader.set_uniform(cl_text("texture1"), 0);
			gc.draw_primitives_array(cl_quads, 400*6*4);
//			gc.draw_primitives_array(cl_quads, vertex_elements, 400*6*4);
			gc.reset_program_object();
			gc.reset_primitives_array();

			gc.reset_texture(0);

			gc.reset_material();
			gc.reset_light(0);
			gc.reset_light_model();

			s0 += deltas0;
			if(s0 > 30.0f && deltas0 > 0.0f)
				deltas0 -= 0.001f;

			spinner0 += 0.025f;
			spinner1 += 0.025f;

			window.flip(0);

			if(CL_DisplayMessageQueue::has_messages())
				CL_DisplayMessageQueue::process();
		}
		float delta_time = (CL_System::get_time()-start_time)/(float) 1000;

		CL_ConsoleWindow console(cl_text("Console"));
		CL_Console::write_line( "Frames per second: %1", cur_frame / delta_time );
		console.display_close_message();
	}
	catch (CL_Exception e)
	{
		CL_ConsoleWindow console(cl_text("Console"));
		CL_Console::write_line(e.message);
		console.display_close_message();

		return -1;
	}

	return 0;
}

void link_shader(CL_ProgramObject program_object)
{
	if(!program_object.link())
		throw CL_Exception(cl_format(cl_text("Unable to link shader program: %1"), program_object.get_info_log()));
}
