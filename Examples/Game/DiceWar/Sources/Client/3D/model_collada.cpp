
#include "precomp.h"
#include "model_collada.h"

/////////////////////////////////////////////////////////////////////////////
// ModelCollada Construction:

ModelCollada::ModelCollada(CL_GraphicContext &gc, const CL_StringRef &filename)
{
	CL_String resource_dir = "Resources/";

	CL_VirtualDirectory vd;
	program_object_texture = CL_ProgramObject::load(gc, resource_dir + "model_3ds_vertex.glsl", resource_dir + "model_3ds_fragment.glsl", vd);
	program_object_texture.bind_attribute_location(0, "in_position");
	program_object_texture.bind_attribute_location(1, "in_normal");
	program_object_texture.bind_attribute_location(2, "in_texcoord");
	if (!program_object_texture.link())
		throw CL_Exception("Unable to link 3ds model program object (texture)");
	program_object_no_texture = CL_ProgramObject::load(gc, resource_dir + "model_3ds_vertex.glsl", resource_dir + "model_3ds_fragment_no_texture.glsl", vd);
	program_object_no_texture.bind_attribute_location(0, "in_position");
	program_object_no_texture.bind_attribute_location(1, "in_normal");
	if (!program_object_no_texture.link())
		throw CL_Exception("Unable to link 3ds model program object (no texture)");

	load(filename);
	load_textures(gc);
}

ModelCollada::~ModelCollada()
{
}

/////////////////////////////////////////////////////////////////////////////
// ModelCollada Attributes:

/////////////////////////////////////////////////////////////////////////////
// ModelCollada Operations:

void ModelCollada::render(CL_GraphicContext &gc, const LightModel &light_model, const Position &position, const Orientation &orientation)
{

	CL_Mat4f modelmatrix = CL_Mat4f::identity();
	modelmatrix.multiply(position.to_matrix());
	modelmatrix.multiply(orientation.to_matrix());
	float scale = 1.5f;

	modelmatrix.multiply(CL_Mat4f::rotate(CL_Angle(-90.0f, cl_degrees), 1.0f, 0.0f, 0.0f, false));
	modelmatrix.multiply(CL_Mat4f::scale(scale, scale, scale));
	gc.push_modelview();
	gc.mult_modelview(modelmatrix);

	CL_PolygonRasterizer polygon_rasterizer;
	polygon_rasterizer.set_culled(true);
	polygon_rasterizer.set_face_cull_mode(cl_cull_back);
	polygon_rasterizer.set_front_face(cl_face_side_clockwise);
	gc.set_polygon_rasterizer(polygon_rasterizer);

	Material material;
	setup_light(program_object_texture, material, light_model);

	program_object_texture.set_uniform1i("texture1",0 );

	std::vector<CL_Collada_Triangles>::size_type num_surfaces, surface_count;
	num_surfaces = mesh.positions_surface_list.size();
	unsigned int position_offset = 0;
	unsigned int texcoords_offset = 0;
	for (surface_count=0; surface_count < num_surfaces; surface_count++)
	{
		CL_Collada_Triangles &current_position_triangle = mesh.positions_surface_list[surface_count];
		unsigned int position_size = current_position_triangle.get_count() * 3;
		if (!position_size)
			continue;

		CL_Collada_Triangles &current_texcoord_triangle = mesh.texcoord_surface_list[surface_count];
		unsigned int texcoord_size = current_texcoord_triangle.get_count() * 3;

		CL_PrimitivesArray prim_array(gc);
		prim_array.set_attributes(0, &mesh.object_positions[position_offset]);
		prim_array.set_attributes(1, &mesh.object_normals[position_offset]);

		if (texcoord_size)
		{
			prim_array.set_attributes(2, &mesh.object_texcoords[texcoords_offset]);
			gc.set_program_object(program_object_texture);

			set_texture(gc, current_texcoord_triangle);
		}
		else
		{
			gc.set_program_object(program_object_no_texture);
		}

		gc.draw_primitives(cl_triangles, position_size, prim_array);
		position_offset += position_size;
		texcoords_offset += texcoord_size;

		gc.reset_texture(0);

	}

	gc.reset_program_object();
	gc.reset_polygon_rasterizer();

	gc.pop_modelview();

}

/////////////////////////////////////////////////////////////////////////////
// ModelCollada Implementation:

void ModelCollada::setup_light(CL_ProgramObject &program, const Material &material, const LightModel &light_model)
{
	program.set_uniform4f("lightSourcePosition", light_model.light_sources[0].position);
	program.set_uniform4f("lightSourceHalfVector", light_model.light_sources[0].get_halfvector());
	program.set_uniform4f("lightSourceDiffuse", light_model.light_sources[0].diffuse);
	program.set_uniform4f("lightSourceAmbient", light_model.light_sources[0].ambient);
	program.set_uniform4f("lightSourceSpecular", light_model.light_sources[0].specular);
	program.set_uniform1f("lightSourceConstantAttenuation", light_model.light_sources[0].constant_attenuation);
	program.set_uniform1f("lightSourceLinearAttenuation", light_model.light_sources[0].constant_attenuation);
	program.set_uniform1f("lightSourceQuadraticAttenuation", light_model.light_sources[0].quadratic_attenuation);
	program.set_uniform4f("frontMaterialDiffuse", material.diffuse);
	program.set_uniform4f("frontMaterialAmbient", material.ambient);
	program.set_uniform4f("frontMaterialSpecular", material.specular);
	program.set_uniform1f("frontMaterialShininess", material.shininess);
	program.set_uniform4f("lightModelAmbient", light_model.scene_ambient);

}

void ModelCollada::load_textures(CL_GraphicContext &gc)
{
	unsigned int num_images = library_images.size();
	for (unsigned int cnt = 0; cnt < num_images; cnt++)
	{
		CL_String filename = library_images[cnt].get_filename();
		textures[filename] = CL_Texture(gc, cl_format("Resources/%1", CL_PathHelp::get_filename(filename)));
		textures[filename].set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
		textures[filename].set_min_filter(cl_filter_linear);
		textures[filename].set_mag_filter(cl_filter_linear);
	}
}

void ModelCollada::load(const CL_StringRef &filename)
{
	CL_File file(filename, CL_File::open_existing, CL_File::access_read);
	CL_DomDocument doc(file);
	CL_Collada object(doc, library_images);

	std::vector< CL_Collada_Geometry > &geometries = object.get_geometries();
	std::vector< CL_Collada_Geometry >::size_type size, cnt;
	size = geometries.size();

	for (cnt=0; cnt<size; cnt++)
		insert_object(geometries[cnt], mesh, library_images, CL_Angle(89.53f, cl_degrees));
}

void ModelCollada::insert_object(CL_Collada_Geometry &geometry, CL_ObjectMesh &model, std::vector<CL_Collada_Image> &library_images, const CL_Angle &smooth_threshold)
{
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

	std::vector<CL_Collada_Triangles> unused;
	mesh.create_vertices(&model.object_normals[dest_normals_offset], 0, "NORMAL", unused);

	if (vertex_count_texcoord)
	{
		mesh.create_vertices(&model.object_texcoords[dest_texcoords_offset], 0, "TEXCOORD", model.texcoord_surface_list);
		// Invert uvmap positions for clanlib
		invert_uvmap(&model.object_texcoords[dest_texcoords_offset], vertex_count_texcoord);
	}
	else
	{
		// We still need to fill model.texcoord_surface_list
		CL_Vec2f unused_position;
		mesh.create_vertices(&unused_position, 0, "TEXCOORD", model.texcoord_surface_list);
	}

}

void ModelCollada::scale_points(CL_Vec3f *points_ptr, int size, const CL_Vec3f &object_scale)
{
	for (; size > 0; --size, points_ptr++)
	{
		points_ptr->x *= object_scale.x;
		points_ptr->y *= object_scale.y;
		points_ptr->z *= object_scale.z;
	}
}

void ModelCollada::invert_uvmap(CL_Vec2f *points_ptr, int size)
{
	for (; size > 0; --size, points_ptr++)
	{
		points_ptr->y = 1.0f - points_ptr->y;
	}
}

void ModelCollada::translate_points(CL_Vec3f *points_ptr, int size, const CL_Vec3f &object_translation)
{
	for (; size > 0; --size, points_ptr++)
	{
		points_ptr->x += object_translation.x;
		points_ptr->y += object_translation.y;
		points_ptr->z += object_translation.z;
	}
}

void ModelCollada::set_texture(CL_GraphicContext &gc, CL_Collada_Triangles &surface)
{
	//TODO: Tidy this up

	CL_Collada_Material &material = surface.get_material();
	if (material.is_null())
		throw CL_Exception("humm, the surface material is missing");

	CL_Collada_Effect &effect = material.get_effect();
	if (effect.is_null())
		throw CL_Exception("Unexpected !");

	CL_Collada_Effect_ColorTexture color_texture = effect.get_diffuse();
	if (color_texture.is_null())
		throw CL_Exception("Unexpected !");

	CL_Collada_Effect_Texture &texture = color_texture.get_texture();
	if (texture.is_null())
		throw CL_Exception("Unexpected !");

	CL_Collada_Image &image = texture.get_image();
	if (image.is_null())
		throw CL_Exception("Unexpected !");

	CL_String &filename = image.get_filename();
	CL_Texture required_texture = textures[filename];
	if (required_texture.is_null())
		throw CL_Exception("Unexpected !");

	gc.set_texture(0, required_texture);

}
