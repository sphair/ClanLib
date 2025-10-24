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
#include "model.h"
#include "graphic_store.h"

class Model_Impl
{
public:
	Model_Impl();

	void Load(const char *filename, std::vector<CL_Collada_Image> &library_images);
	void Load(CL_DomDocument &doc, const char *geometry_name, std::vector<CL_Collada_Image> &library_images);

	void Draw(CL_GraphicContext &gc, GraphicStore *gs, const CL_Mat4f &modelview_matrix, bool use_geometry_shader);
	void SetMaterial(float new_material_shininess, const CL_Vec4f &new_material_emission, const CL_Vec4f &new_material_ambient, const CL_Vec4f &new_material_specular);

private:
	void invert_uvmap(CL_Vec2f *points_ptr, int size);
	void insert_object(CL_Collada &object, const CL_String &geometry_name, std::vector<CL_Collada_Image> &library_images, const CL_Angle &smooth_threshold);
	void invert_winding_rule(CL_Vec2f *points_ptr, int size);
	void invert_winding_rule(CL_Vec3f *points_ptr, int size);

	std::vector<CL_Collada_Triangles> positions_surface_list;
	std::vector<CL_Vec3f> object_positions;
	std::vector<CL_Vec3f> object_normals;

	std::vector<CL_Collada_Triangles> texcoord_surface_list;
	std::vector<CL_Vec2f> object_texcoords;

	float material_shininess;
	CL_Vec4f material_emission;
	CL_Vec4f material_ambient;
	CL_Vec4f material_specular;

	bool update_vbo;

	CL_VertexArrayBuffer object_positions_vbo;
	CL_VertexArrayBuffer object_normals_vbo;
	CL_VertexArrayBuffer object_texcoords_vbo;
};


Model::Model()
{
}

Model::Model(const char *filename, std::vector<CL_Collada_Image> &library_images): impl(new Model_Impl())
{
	impl->Load(filename, library_images);
}

Model::Model(CL_DomDocument &doc, const char *geometry_name, std::vector<CL_Collada_Image> &library_images): impl(new Model_Impl())
{
	impl->Load(doc, geometry_name, library_images);
}

bool Model::is_null()
{
	return impl.is_null();
}

void Model::Draw(CL_GraphicContext &gc, GraphicStore *gs, const CL_Mat4f &modelview_matrix, bool use_geometry_shader)
{
	impl->Draw(gc, gs, modelview_matrix, use_geometry_shader);
}

Model_Impl::Model_Impl()
{
	update_vbo = true;
	material_shininess = 64.0f;
	material_emission = CL_Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	material_ambient =  CL_Vec4f(0.9f, 0.2f, 0.2f, 1.0f);
	material_specular = CL_Vec4f(0.0f, 0.0f, 0.0f, 1.0f);

}

void Model::SetMaterial(float new_material_shininess, const CL_Vec4f &new_material_emission, const CL_Vec4f &new_material_ambient, const CL_Vec4f &new_material_specular)
{
	impl->SetMaterial(new_material_shininess, new_material_emission, new_material_ambient, new_material_specular);
}

void Model_Impl::SetMaterial(float new_material_shininess, const CL_Vec4f &new_material_emission, const CL_Vec4f &new_material_ambient, const CL_Vec4f &new_material_specular)
{
	material_shininess = new_material_shininess;
	material_emission = new_material_emission;
	material_ambient = new_material_ambient;
	material_specular = new_material_specular;
}

void Model_Impl::Load(const char *filename, std::vector<CL_Collada_Image> &library_images)
{
	CL_File file;
	CL_DomDocument doc;

	file = CL_File(filename);
	doc = CL_DomDocument(file);

	Load(doc, "Mesh_Object", library_images);
}

void Model_Impl::Load(CL_DomDocument &doc, const char *geometry_name, std::vector<CL_Collada_Image> &library_images)
{
	CL_Collada object;
	object = CL_Collada(doc, library_images);
	insert_object(object, geometry_name, library_images, CL_Angle(89.53f, cl_degrees));

	update_vbo = true;
}

void Model_Impl::insert_object(CL_Collada &object, const CL_String &geometry_name, std::vector<CL_Collada_Image> &library_images, const CL_Angle &smooth_threshold)
{
	CL_Collada_Geometry &geometry = object.get_geometry(geometry_name);
	CL_Collada_Mesh &mesh = geometry.get_mesh();

	unsigned int dest_positions_offset = object_positions.size();
	unsigned int dest_texcoords_offset = object_texcoords.size();
	unsigned int dest_normals_offset = object_normals.size();

	int vertex_count_positions = mesh.get_triangle_count("VERTEX") * 3;
	if (!vertex_count_positions)
		return;		// Return now, if not vertices were found - maybe throw an exception instead?

	int vertex_count_texcoord = mesh.get_triangle_count("TEXCOORD") * 3;

	object_positions.resize(dest_positions_offset + vertex_count_positions);
	object_texcoords.resize(dest_texcoords_offset + vertex_count_texcoord);
	object_normals.resize(dest_normals_offset + vertex_count_positions);

	mesh.create_vertices(&object_positions[dest_positions_offset], 0, "VERTEX", positions_surface_list);
	mesh.create_vertices_normal(&object_normals[dest_normals_offset], 0, "VERTEX", smooth_threshold);

	invert_winding_rule(&object_positions[dest_positions_offset], vertex_count_positions);
	invert_winding_rule(&object_normals[dest_normals_offset], vertex_count_positions);

	if (vertex_count_texcoord)
	{
		mesh.create_vertices(&object_texcoords[dest_texcoords_offset], 0, "TEXCOORD", texcoord_surface_list);
		// Invert uvmap positions for clanlib
		invert_uvmap(&object_texcoords[dest_texcoords_offset], vertex_count_texcoord);

		invert_winding_rule(&object_texcoords[dest_texcoords_offset], vertex_count_texcoord);
	}
}

void Model_Impl::invert_winding_rule(CL_Vec3f *points_ptr, int size)
{
	size/=3;
	for (; size > 0; --size, points_ptr+=3)
	{
		CL_Vec3f temp_point = points_ptr[0];
		points_ptr[0] = points_ptr[2];
		points_ptr[2] = temp_point;

		points_ptr[0].z = -points_ptr[0].z;
		points_ptr[1].z = -points_ptr[1].z;
		points_ptr[2].z = -points_ptr[2].z;
	}
}

void Model_Impl::invert_winding_rule(CL_Vec2f *points_ptr, int size)
{
	size/=3;
	for (; size > 0; --size, points_ptr+=3)
	{
		CL_Vec2f temp_point = points_ptr[0];
		points_ptr[0] = points_ptr[2];
		points_ptr[2] = temp_point;
	}
}
void Model_Impl::invert_uvmap(CL_Vec2f *points_ptr, int size)
{
	for (; size > 0; --size, points_ptr++)
	{
		points_ptr->y = 1.0f - points_ptr->y;
	}
}

void Model_Impl::Draw(CL_GraphicContext &gc, GraphicStore *gs, const CL_Mat4f &modelview_matrix, bool use_geometry_shader)
{
	if (!object_positions.size())
		return;

	if (update_vbo)
	{
		update_vbo = false;
		if (object_texcoords.size())
		{
			object_texcoords_vbo = CL_VertexArrayBuffer(gc, &object_texcoords[0], sizeof(CL_Vec2f) * object_texcoords.size());
		}
		object_positions_vbo = CL_VertexArrayBuffer(gc, &object_positions[0], sizeof(CL_Vec3f) * object_positions.size());
		object_normals_vbo = CL_VertexArrayBuffer(gc, &object_normals[0], sizeof(CL_Vec3f) * object_normals.size());

	}

	gc.set_modelview(modelview_matrix);

	CL_PrimitivesArray prim_array(gc);

	prim_array.set_attributes(0, object_positions_vbo, 3, cl_type_float, (void *) 0);
	prim_array.set_attributes(1, object_normals_vbo, 3, cl_type_float, (void *) 0);

	if (!use_geometry_shader)
	{
		if (object_texcoords.size())
		{
			prim_array.set_attributes(2, object_texcoords_vbo, 2, cl_type_float, (void *) 0);
			gc.set_texture(0, gs->texture_brick);
			gs->shader_texture.SetMaterial(material_shininess, material_emission, material_ambient, material_specular);
			gs->shader_texture.Use(gc);
		}
		else
		{
			gs->shader_color.SetMaterial(material_shininess, material_emission, material_ambient, material_specular);
			gs->shader_color.Use(gc);
		}
	}
	else
	{
		if (object_texcoords.size())
			throw CL_Exception("Shader not supported");

		gs->shader_color_geometry.SetMaterial(material_shininess, material_emission, material_ambient, material_specular);
		gs->shader_color_geometry.Use(gc);

		clDisable(CL_CULL_FACE);	// For for example, so you can see inside the teapot

	}

	gc.draw_primitives(cl_triangles, object_positions.size(), prim_array);

	if (use_geometry_shader)
		clEnable(CL_CULL_FACE);

	if (object_texcoords.size())
	{
		gc.reset_texture(0);
	}
}
