
#include "precomp.h"
#include "model_3ds.h"
#include <lib3ds/camera.h>
#include <lib3ds/node.h>
#include <lib3ds/mesh.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>

/////////////////////////////////////////////////////////////////////////////
// Model3DS Construction:

Model3DS::Model3DS(CL_GraphicContext &gc, const CL_StringRef &filename)
{
	CL_String app_name = cl_text("3dsviewer");
	CL_String resource_dir = "Resources/"; // CL_Directory::get_resourcedata(app_name);

	CL_VirtualDirectory vd;
	program_object_texture = CL_ProgramObject::load(gc, resource_dir + cl_text("model_3ds_vertex.glsl"), resource_dir + cl_text("model_3ds_fragment.glsl"), vd);
	program_object_texture.bind_attribute_location(0, "in_position");
	program_object_texture.bind_attribute_location(1, "in_normal");
	program_object_texture.bind_attribute_location(2, "in_texcoord");
	if (!program_object_texture.link())
		throw CL_Exception("Unable to link 3ds model program object (texture)");
	program_object_no_texture = CL_ProgramObject::load(gc, resource_dir + cl_text("model_3ds_vertex.glsl"), resource_dir + cl_text("model_3ds_fragment_no_texture.glsl"), vd);
	program_object_no_texture.bind_attribute_location(0, "in_position");
	program_object_no_texture.bind_attribute_location(1, "in_normal");
	if (!program_object_no_texture.link())
		throw CL_Exception("Unable to link 3ds model program object (no texture)");

	CL_File file(filename, CL_File::open_existing, CL_File::access_read);
	model_file.load(file);
	file.close();

	load_textures(gc);
	generate_vertex_buffer(gc);
}

Model3DS::~Model3DS()
{
}

/////////////////////////////////////////////////////////////////////////////
// Model3DS Attributes:

/////////////////////////////////////////////////////////////////////////////
// Model3DS Operations:

void Model3DS::render(CL_GraphicContext &gc, const LightModel &light_model, const Position &position, const Orientation &orientation)
{
	clMatrixMode(CL_MODELVIEW);
	clPushMatrix();
	clMultMatrixf(position.to_matrix());
	clMultMatrixf(orientation.to_matrix());

	clMultMatrixf(Orientation::axis_angle(-90.0f, CL_Vec3f(1.0f, 0.0f, 0.0f)).to_matrix());
	clMultMatrixf(CL_Mat4f::scale(1.0f, -1.0f, 1.0f));
	clScaled(2.0, 2.0, 2.0);

	CL_PolygonRasterizer polygon_rasterizer;
	polygon_rasterizer.set_culled(true);
	gc.set_polygon_rasterizer(polygon_rasterizer);

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, vertices, 3, cl_type_float, &static_cast<MeshVertex*>(0)->position, sizeof(MeshVertex));
	prim_array.set_attributes(1, vertices, 3, cl_type_float, &static_cast<MeshVertex*>(0)->normal, sizeof(MeshVertex));
	prim_array.set_attributes(2, vertices, 2, cl_type_float, &static_cast<MeshVertex*>(0)->texcoord0, sizeof(MeshVertex));
	gc.set_primitives_array(prim_array);

	Lib3dsFile *file = model_file.get_file();
	Lib3dsNode *cur_frame_node = file->nodes;
	while (cur_frame_node)
	{
		render_node(gc, light_model, file, cur_frame_node);
		cur_frame_node = cur_frame_node->next;
	}

	gc.reset_primitives_array();
	gc.reset_texture(0);
	gc.reset_polygon_rasterizer();

	clMatrixMode(CL_MODELVIEW);
	clPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// Model3DS Implementation:

void Model3DS::render_node(CL_GraphicContext &gc, const LightModel &light_model, Lib3dsFile *file, Lib3dsNode *node)
{
	Lib3dsNode *cur_child_node = node->childs;
	while (cur_child_node)
	{
		render_node(gc, light_model, file, cur_child_node);
		cur_child_node = cur_child_node->next;
	}

	if (node->type != LIB3DS_OBJECT_NODE)
		return;
	if (node->name == CL_StringRef8("$$$DUMMY"))
		return;

	Lib3dsMesh *mesh = lib3ds_file_mesh_by_name(file, node->name);
	if (mesh == 0)
		return;

	std::map<Lib3dsMesh *, std::vector<MeshElementsRange> >::const_iterator it_mesh;
	it_mesh = mesh_ranges.find(mesh);
	if (it_mesh == mesh_ranges.end())
		return;
	const std::vector<MeshElementsRange> &ranges = it_mesh->second;

	CL_Mat4f node_matrix = CL_Mat4f::null();
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			node_matrix.matrix[x+y*4] = node->matrix[y][x];

	clMatrixMode(CL_MODELVIEW);
	clPushMatrix();
	clMultMatrixf(node_matrix);
	clTranslatef(-node->data.object.pivot[0], -node->data.object.pivot[1], -node->data.object.pivot[2]);
/*
	Orientation o = Orientation::quaternion(
		node->data.object.rot[0],
		node->data.object.rot[1],
		node->data.object.rot[2],
		node->data.object.rot[3]);
	clMultMatrixf(o.to_matrix());
*/
	std::vector<MeshElementsRange>::size_type index, size;
	size = ranges.size();
	for (index = 0; index < size; index++)
	{
		const Material &material = ranges[index].material;
		if (ranges[index].texture.is_null())
		{
			gc.set_program_object(program_object_no_texture);
			setup_light(program_object_no_texture, material, light_model);
		}
		else
		{
			gc.set_program_object(program_object_texture);
			gc.set_texture(0, ranges[index].texture);
			program_object_texture.set_uniform1i(cl_text("texture1"), 0);
			setup_light(program_object_texture, material, light_model);
		}
		gc.draw_primitives_array(cl_triangles, ranges[index].start, ranges[index].length);

		gc.reset_texture(0);
		gc.reset_program_object();
	}

	clMatrixMode(CL_MODELVIEW);
	clPopMatrix();
}

void Model3DS::setup_light(CL_ProgramObject &program, const Material &material, const LightModel &light_model)
{
	program.set_uniform4f(cl_text("lightSourcePosition"), light_model.light_sources[0].position);
	program.set_uniform4f(cl_text("frontLightProductAmbient"), light_model.get_light_ambient(material, light_model.light_sources[0]));
	program.set_uniform4f(cl_text("frontLightProductDiffuse"), light_model.get_light_diffuse(material, light_model.light_sources[0]));
	program.set_uniform4f(cl_text("frontLightProductSpecular"), light_model.get_light_specular(material, light_model.light_sources[0]));
	program.set_uniform1f(cl_text("frontMaterialShininess"), material.shininess);
	program.set_uniform4f(cl_text("frontLightModelProductSceneColor"), light_model.get_scene_color(material));
}

void Model3DS::load_textures(CL_GraphicContext &gc)
{
	// Load textures:
	Lib3dsFile *mfile = model_file.get_file();
	for (Lib3dsMaterial *m = mfile->materials; m != 0; m = m->next)
	{
		if (m->texture1_map.name[0])
		{
			textures[m->texture1_map.name] = CL_Texture(gc, "Resources/" + CL_StringHelp::local8_to_text(m->texture1_map.name));
			textures[m->texture1_map.name].set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);
			textures[m->texture1_map.name].set_min_filter(cl_filter_linear);
			textures[m->texture1_map.name].set_mag_filter(cl_filter_linear);
		}

		// Should we also bind texture1_mask, texture2_map and texture2_mask?
		// In which situations are those textures used?
		// Textures can also be rotated and scaled on a material. How often is this done?

		Material material;
		material.ambient = CL_Vec4f(m->ambient[0], m->ambient[1], m->ambient[2], m->ambient[3]);
		material.diffuse = CL_Vec4f(m->diffuse[0], m->diffuse[1], m->diffuse[2], m->diffuse[3]);
		material.specular = CL_Vec4f(m->specular[0], m->specular[1], m->specular[2], m->specular[3]);
		float s = pow(2, 10.0f * m->shininess);
		if (s > 128.0f)
			s = 128.0f;
		material.shininess = s;
		materials[m] = material;
	}

	// Default (null) material:
	Material material;
	material.ambient = CL_Vec4f(0.2f, 0.2f, 0.2f, 1.0f);
	material.diffuse = CL_Vec4f(0.8f, 0.8f, 0.8f, 1.0f);
	material.specular = CL_Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	materials[0] = material;
}

void Model3DS::generate_vertex_buffer(CL_GraphicContext &gc)
{
	Lib3dsFile *mfile = model_file.get_file();

	// Create vertex buffer and find mesh element ranges:
	int num_vertices = 0;
	for (Lib3dsMesh *mesh = mfile->meshes; mesh != 0; mesh = mesh->next)
		num_vertices += mesh->faces * 3;

	vertices = CL_VertexArrayBuffer(gc, sizeof(MeshVertex) * num_vertices);
	vertices.lock(cl_access_write_only);
	MeshVertex *mesh_vertices = (MeshVertex *) vertices.get_data();

	int vertex_index = 0;
	std::map<Lib3dsMaterial *, bool> mesh_materials;
	CL_DataBuffer normals_buffer;
	for (Lib3dsMesh *mesh = mfile->meshes; mesh != 0; mesh = mesh->next)
	{
		// Generate normals for mesh:
		normals_buffer.set_size(mesh->faces * 3 * sizeof(Lib3dsVector));
		Lib3dsVector *normals = (Lib3dsVector *) normals_buffer.get_data();
		lib3ds_mesh_calculate_normals(mesh, normals);

		// Count materials (yeah yeah I'm sure there are more optimal ways):
		mesh_materials.clear();
		for (unsigned int index = 0; index < mesh->faces; index++)
		{
			Lib3dsMaterial *cur_model_material = 0;
			if (mesh->faceL[index].material[0])
				cur_model_material = lib3ds_file_material_by_name(mfile, mesh->faceL[index].material);
			mesh_materials[cur_model_material] = true;
		}

		std::vector<MeshElementsRange> ranges;

		// Sort vertices based on materials (yep once again there is most likely better algorithms)
		std::map<Lib3dsMaterial *, bool>::iterator it;
		for (it = mesh_materials.begin(); it != mesh_materials.end(); ++it)
		{
			Lib3dsMaterial *material = it->first;

			MeshElementsRange range;
			range.start = vertex_index;
			range.length = 0;
			if (materials.find(material) == materials.end())
				throw CL_Exception(cl_text("Material not in materials map!"));
			range.material = materials[material];
			if (material)
			{
				if (textures.find(material->texture1_map.name) != textures.end())
					range.texture = textures[material->texture1_map.name];
			}

			for (unsigned int index = 0; index < mesh->faces; index++)
			{
				Lib3dsMaterial *cur_model_material = 0;
				if (mesh->faceL[index].material[0])
					cur_model_material = lib3ds_file_material_by_name(mfile, mesh->faceL[index].material);
				if (cur_model_material != material)
					continue;

				for (int v = 0; v < 3; v++)
				{
					int point_index = mesh->faceL[index].points[v];

					mesh_vertices[vertex_index + v].position = CL_Vec3f(
						mesh->pointL[point_index].pos[0],
						mesh->pointL[point_index].pos[1],
						mesh->pointL[point_index].pos[2]);

					mesh_vertices[vertex_index + v].normal = CL_Vec3f(
						normals[index * 3 + v][0],
						normals[index * 3 + v][1],
						normals[index * 3 + v][2]);
					mesh_vertices[vertex_index + v].normal.normalize();

					if(mesh->texelL)
						mesh_vertices[vertex_index + v].texcoord0 = CL_Vec2f(
							mesh->texelL[point_index][0],
							1.0f - mesh->texelL[point_index][1]);
				}
				vertex_index += 3;
			}

			range.length = vertex_index - range.start;
			ranges.push_back(range);
		}

		mesh_ranges[mesh] = ranges;
	}
	vertices.unlock();

	if (num_vertices != vertex_index)
		throw CL_Exception(cl_text("num_vertices != vertex_index"));
}
