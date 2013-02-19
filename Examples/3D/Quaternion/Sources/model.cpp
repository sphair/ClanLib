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
#include "model.h"
#include "graphic_store.h"

class Model_Impl
{
public:
	Model_Impl();

	void Load(GraphicContext &gc, GraphicStore *gs, const char *filename);

	void Draw(GraphicContext &gc, GraphicStore *gs, const Mat4f &modelview_matrix);
	
private:
	int count_vertices(const struct aiScene* sc, const struct aiNode* nd);
	void insert_vbo(GraphicContext &gc, int vertex_count, const struct aiScene* sc, const struct aiNode* nd);

	VertexArrayBuffer vbo_positions;
	VertexArrayBuffer vbo_normals;
	VertexArrayBuffer vbo_texcoords;
	int vbo_size;

	bool generate_texture_coords;

};


Model::Model()
{
}

Model::Model(GraphicContext &gc, GraphicStore *gs, const char *filename): impl(new Model_Impl())
{
	impl->Load(gc, gs, filename);
}

bool Model::is_null()
{
	return !impl;
}

void Model::Draw(GraphicContext &gc, GraphicStore *gs, const Mat4f &modelview_matrix)
{
	impl->Draw(gc, gs, modelview_matrix);
}

Model_Impl::Model_Impl()
{
	vbo_size = 0;
}

void Model_Impl::Load(GraphicContext &gc, GraphicStore *gs, const char *filename)
{
	generate_texture_coords = true;

	const struct aiScene* scene = aiImportFileExWithProperties(filename,aiProcessPreset_TargetRealtime_MaxQuality, NULL, gs->store);
	if (!scene)
		throw Exception("Cannot load a model");

	try
	{
		vbo_size = count_vertices(scene, scene->mRootNode);
		if (!vbo_size)
			throw Exception("No vertices found in the model");
	
		vbo_positions = VertexArrayBuffer(gc, vbo_size * sizeof(Vec3f));
		vbo_normals = VertexArrayBuffer(gc, vbo_size * sizeof(Vec3f));
		if (generate_texture_coords)
			vbo_texcoords = VertexArrayBuffer(gc, vbo_size * sizeof(Vec2f));
		insert_vbo(gc, 0, scene, scene->mRootNode);
	}catch(...)
	{
		aiReleaseImport(scene);
		throw;
	}

	aiReleaseImport(scene);
}

int Model_Impl::count_vertices(const struct aiScene* sc, const struct aiNode* nd)
{
	int vertex_count = 0;
	unsigned int n = 0, t;

	// All meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
		int num_vertex = mesh->mNumFaces * 3;
		if (!num_vertex)
			continue;

		vertex_count += num_vertex;
		for (t = 0; t < mesh->mNumFaces; ++t)
		{
			if (mesh->mFaces[t].mNumIndices != 3)
					throw Exception("This example only supports triangles");
		}
	}

	// All children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		vertex_count += count_vertices(sc, nd->mChildren[n]);
	}

	return vertex_count;
}

void Model_Impl::insert_vbo(GraphicContext &gc, int vertex_count, const struct aiScene* sc, const struct aiNode* nd)
{
	int i;
	unsigned int n = 0, t;

	bool use_texcoords = !vbo_texcoords.is_null();

	// All meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
		int num_vertex = mesh->mNumFaces * 3;
		if (!num_vertex)
			continue;

		std::vector<Vec3f> normals;
		std::vector<Vec3f> vertices;
		std::vector<Vec2f> tex_coords;

		normals.reserve(num_vertex);
		vertices.reserve(num_vertex);

		for (t = 0; t < mesh->mNumFaces; ++t)
		{
			const struct aiFace* face = &mesh->mFaces[t];
			if (face->mNumIndices != 3)
					throw Exception("This example only supports triangles");

			for(i = 0; i < face->mNumIndices; i++)
			{
				int index = face->mIndices[i];
				normals.push_back(Vec3f(&mesh->mNormals[index].x));
				vertices.push_back(Vec3f( &mesh->mVertices[index].x));
			}
		}

//FIXME
//		vbo_positions.upload_data(gc, vertex_count * sizeof(Vec3f), &vertices[0], num_vertex * sizeof(Vec3f));
//		vbo_normals.upload_data(gc, vertex_count * sizeof(Vec3f), &normals[0], num_vertex * sizeof(Vec3f));
		vbo_positions.upload_data(gc, &vertices[0], num_vertex * sizeof(Vec3f));
		vbo_normals.upload_data(gc, &normals[0], num_vertex * sizeof(Vec3f));

		vertex_count += num_vertex;
	}

	// All children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		insert_vbo(gc, vertex_count, sc, nd->mChildren[n]);
	}

}


void Model_Impl::Draw(GraphicContext &gc, GraphicStore *gs, const Mat4f &modelview_matrix)
{
	Mat4f matrix_modelview_projection = gs->camera_projection *  modelview_matrix;
	Mat4f normal_matrix = Mat4f::identity();

	PrimitivesArray prim_array(gc);
	
	prim_array.set_attributes(0, vbo_positions, 3, type_float, 0);
	prim_array.set_attributes(1, vbo_normals, 3, type_float, 0);
	gs->shader_color.Use(gc, modelview_matrix, matrix_modelview_projection, normal_matrix);
	gc.draw_primitives(type_triangles, vbo_size, prim_array);
}
