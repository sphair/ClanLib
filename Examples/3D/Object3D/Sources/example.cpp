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

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	quit = false;

	CL_GL1WindowDescription desc;
	desc.set_title("ClanLib Object 3D Example");
	desc.set_size(CL_Size(640, 480), true);
	desc.set_multisampling(4);
	desc.set_depth_size(16);

	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();
	CL_GraphicContext_GL1 gc_gl1 = gc;

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

	// Set the lights
	CL_LightModel_GL1 light_model;
	light_model.enable_lighting(true);
	light_model.set_flat_shading(false);
	light_model.set_scene_ambient_light(CL_Colorf(0.2f, 0.2f, 0.2f, 1.0f));
	gc_gl1.set_light_model(light_model);

	CL_LightSource_GL1 light_distant;
	light_distant.set_spot_cutoff(180.0f);
	light_distant.set_diffuse_intensity(CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
	light_distant.set_position(CL_Vec4f(0.0f, -2.0f, 30.0f, 0.0f).normalize3());
	gc_gl1.set_light(0, light_distant);

	// Create the objects
	Model scene_model;
	std::vector<CL_Collada_Image> library_images;
	create_objects_without_texture(scene_model, library_images);

	Model scene_model2;
	create_objects(scene_model2, library_images);

	// Load the texture
	CL_Texture tux(gc, "../Clan3D/Resources/tux.png");

	float angle = 0.0f;
	// Run until someone presses escape
	while (!quit)
	{

		CL_Mat4f perp = CL_Mat4f::perspective(45.0f, ((float) gc.get_width()) / ((float) gc.get_height()), 0.1f, 100000.0f);
		gc.set_projection(perp);

		gc.clear(CL_Colorf::black);
		gc.clear_depth(1.0f);

		angle += 1.0f;
		if (angle >= 360.0f)
			angle -= 360.0f;

		// Draw object_positions_1
		CL_PrimitivesArray prim_array(gc);

		gc.push_modelview();
		gc.set_modelview(CL_Mat4f::identity());
		gc.mult_scale(1.0f,1.0f, -1.0f);	// So +'ve Z goes into the screen
		gc.mult_translate(0.0f, 0.0f, 2.0f);
		gc.mult_rotate(CL_Angle(angle, cl_degrees), 0.0f, 1.0f, 0.0f, false);

		prim_array.set_attributes(0, &scene_model.object_positions[0]);
		prim_array.set_attribute(1, CL_Colorf::white);
		prim_array.set_attributes(4, &scene_model.object_normals[0]);
		gc.draw_primitives(cl_triangles, scene_model.object_positions.size(), prim_array);

		gc.pop_modelview();

		// Draw object_positions_2

		gc.push_modelview();
		gc.set_modelview(CL_Mat4f::identity());
		gc.mult_scale(1.0f,1.0f, -1.0f);	// So +'ve Z goes into the screen
		gc.mult_translate(0.7f, 0.5f, 2.0f);
		gc.mult_rotate(CL_Angle(angle * 4.0f, cl_degrees), 0.0f, 1.0f, 0.0f, false);

		gc.set_texture(0, tux);
		prim_array.set_attributes(0, &scene_model2.object_positions[0]);
		prim_array.set_attribute(1, CL_Colorf::white);
		prim_array.set_attributes(2, &scene_model2.object_texcoords[0]);
		prim_array.set_attributes(4, &scene_model2.object_normals[0]);
		gc.draw_primitives(cl_triangles, scene_model2.object_positions.size(), prim_array);
		gc.reset_texture(0);

		gc.pop_modelview();

		// Flip the display, showing on the screen what we have drawed
		// since last call to flip()
		window.flip(1);

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

void App::create_objects_without_texture(Model &model, std::vector<CL_Collada_Image> &library_images)
{
		CL_File file;
		CL_DomDocument doc;
		CL_Collada object;

		file = CL_File("../Clan3D/Resources/clanlib.dae");
		doc = CL_DomDocument(file);
		object = CL_Collada(doc, library_images);

		CL_Vec3f logo_scale(0.5f, 0.5f, 0.5f);
		CL_Vec3f logo_translate(0.0f, -0.5f, 0.0f);
		CL_Angle logo_smooth_threshold(0.0f, cl_degrees);
		insert_object(object, "Mesh_Object",   logo_scale, logo_translate, model, library_images, logo_smooth_threshold);
		insert_object(object, "Mesh_Object_2", logo_scale, logo_translate, model, library_images, logo_smooth_threshold);
		insert_object(object, "Mesh_Object_3", logo_scale, logo_translate, model, library_images, logo_smooth_threshold);
		insert_object(object, "Mesh_Object_4", logo_scale, logo_translate, model, library_images, logo_smooth_threshold);
		insert_object(object, "Mesh_Object_5", logo_scale, logo_translate, model, library_images, logo_smooth_threshold);
		insert_object(object, "Mesh_Object_6", logo_scale, logo_translate, model, library_images, logo_smooth_threshold);
		insert_object(object, "Mesh_Object_7", logo_scale, logo_translate, model, library_images, logo_smooth_threshold);

		file = CL_File("../Clan3D/Resources/teapot.dae");
		doc = CL_DomDocument(file);
		object = CL_Collada(doc, library_images);
		insert_object(object, "Mesh_Object", CL_Vec3f(1.0f, 1.0f, 1.0f), CL_Vec3f(0.0f, 0.0f, 0.0f), model, library_images, CL_Angle(89.53f, cl_degrees));
}

void App::create_objects(Model &model, std::vector<CL_Collada_Image> &library_images)
{
		CL_File file;
		CL_DomDocument doc;

		file = CL_File("../Clan3D/Resources/tux_ball.dae");
		doc = CL_DomDocument(file);
		CL_Collada tuxball_object(doc, library_images);

		insert_object(tuxball_object, "Mesh_Object", CL_Vec3f(0.05f, 0.05f, 0.05f), CL_Vec3f(0.0f, 0.0f, 0.0f), model, library_images, CL_Angle(89.53f, cl_degrees));
}

void App::insert_object(CL_Collada &object, const CL_String &geometry_name, const CL_Vec3f &object_scale, const CL_Vec3f &object_translation, Model &model, std::vector<CL_Collada_Image> &library_images, const CL_Angle &smooth_threshold)
{
		CL_Collada_Geometry &geometry = object.get_geometry(geometry_name);
		CL_Collada_Mesh &mesh = geometry.get_mesh();

		unsigned int dest_positions_offset = model.object_positions.size();
		unsigned int dest_texcoords_offset = model.object_texcoords.size();
		unsigned int dest_normals_offset = model.object_normals.size();

		int vertex_count_positions = mesh.get_triangle_count("VERTEX") * 3;
		if (!vertex_count_positions)
			return;		// Return now, if not vertices were found - maybe throw an exception instead?

		int vertex_count_texcoord = mesh.get_triangle_count("TEXCOORD") * 3;

		model.object_positions.resize(dest_positions_offset + vertex_count_positions);
		model.object_texcoords.resize(dest_texcoords_offset + vertex_count_texcoord);
		model.object_normals.resize(dest_normals_offset + vertex_count_positions);

		mesh.create_vertices(&model.object_positions[dest_positions_offset], 0, "VERTEX", model.positions_surface_list);
		mesh.create_vertices_normal(&model.object_normals[dest_normals_offset], 0, "VERTEX", smooth_threshold);

		if (vertex_count_texcoord)
		{
			mesh.create_vertices(&model.object_texcoords[dest_texcoords_offset], 0, "TEXCOORD", model.texcoord_surface_list);
			// Invert uvmap positions for clanlib
			invert_uvmap(&model.object_texcoords[dest_texcoords_offset], vertex_count_texcoord);
		}

		scale_points(&model.object_positions[dest_positions_offset], vertex_count_positions, object_scale);
		translate_points(&model.object_positions[dest_positions_offset], vertex_count_positions, object_translation);
}

void App::scale_points(CL_Vec3f *points_ptr, int size, const CL_Vec3f &object_scale)
{
	for (; size > 0; --size, points_ptr++)
	{
		points_ptr->x *= object_scale.x;
		points_ptr->y *= object_scale.y;
		points_ptr->z *= object_scale.z;
	}
}

void App::invert_uvmap(CL_Vec2f *points_ptr, int size)
{
	for (; size > 0; --size, points_ptr++)
	{
		points_ptr->y = 1.0f - points_ptr->y;
	}
}

void App::translate_points(CL_Vec3f *points_ptr, int size, const CL_Vec3f &object_translation)
{
	for (; size > 0; --size, points_ptr++)
	{
		points_ptr->x += object_translation.x;
		points_ptr->y += object_translation.y;
		points_ptr->z += object_translation.z;
	}
}
