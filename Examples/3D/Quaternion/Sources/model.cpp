/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
	void SetMaterial(float new_material_shininess, const Vec4f &new_material_emission, const Vec4f &new_material_ambient, const Vec4f &new_material_specular);

private:
	int count_vertices(const struct aiScene* sc, const struct aiNode* nd);
	int insert_vbo(GraphicContext &gc, int vertex_count, const struct aiScene* sc, const struct aiNode* nd);

	VertexArrayVector<Vec3f> vbo_positions;
	VertexArrayVector<Vec3f> vbo_normals;
	int vbo_size;
	float material_shininess;
	Vec4f material_emission;
	Vec4f material_ambient;
	Vec4f material_specular;

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

void Model::SetMaterial(float new_material_shininess, const Vec4f &new_material_emission, const Vec4f &new_material_ambient, const Vec4f &new_material_specular)
{
	impl->SetMaterial(new_material_shininess, new_material_emission, new_material_ambient, new_material_specular);
}

void Model_Impl::SetMaterial(float new_material_shininess, const Vec4f &new_material_emission, const Vec4f &new_material_ambient, const Vec4f &new_material_specular)
{
	material_shininess = new_material_shininess;
	material_emission = new_material_emission;
	material_ambient = new_material_ambient;
	material_specular = new_material_specular;
}

void Model::Draw(GraphicContext &gc, GraphicStore *gs, const Mat4f &modelview_matrix)
{
	impl->Draw(gc, gs, modelview_matrix);
}

Model_Impl::Model_Impl()
{
	vbo_size = 0;
	material_shininess = 64.0f;
	material_emission = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	material_ambient =  Vec4f(0.9f, 0.2f, 0.2f, 1.0f);
	material_specular = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
}

void Model_Impl::Load(GraphicContext &gc, GraphicStore *gs, const char *filename)
{
#if defined(I_LOVE_ASSIMP_AND_PRECOMPILED_IT)
	const struct aiScene* scene = aiImportFileExWithProperties(filename,aiProcessPreset_TargetRealtime_MaxQuality, NULL, gs->store);
	if (!scene)
		throw Exception("Cannot load a model");

	try
	{
		vbo_size = count_vertices(scene, scene->mRootNode);
		if (!vbo_size)
			throw Exception("No vertices found in the model");
	
		vbo_positions = VertexArrayVector<Vec3f>(gc, vbo_size);
		vbo_normals = VertexArrayVector<Vec3f>(gc, vbo_size);
		insert_vbo(gc, 0, scene, scene->mRootNode);
	}catch(...)
	{
		aiReleaseImport(scene);
		throw;
	}

	aiReleaseImport(scene);
#else
	Vec3f position;

	Vec3f size(0.5f, 0.5f, 0.5f);

	std::vector<Vec3f> object_normals;
	std::vector<Vec3f> object_positions;
	object_normals.reserve(6 * 6);
	object_positions.reserve(6 * 6);

	object_positions.push_back(Vec3f(-size.x, size.y, size.z) + position);
	object_positions.push_back(Vec3f(-size.x, size.y, -size.z) + position);
	object_positions.push_back(Vec3f(-size.x, -size.y, -size.z) + position);
	object_positions.push_back(Vec3f(-size.x, -size.y, -size.z) + position);
	object_positions.push_back(Vec3f(-size.x, -size.y, size.z) + position);
	object_positions.push_back(Vec3f(-size.x, size.y, size.z) + position);

	object_positions.push_back(Vec3f(-size.x, size.y, size.z) + position);
	object_positions.push_back(Vec3f(-size.x, -size.y, size.z) + position);
	object_positions.push_back(Vec3f(size.x, -size.y, size.z) + position);
	object_positions.push_back(Vec3f(size.x, -size.y, size.z) + position);
	object_positions.push_back(Vec3f(size.x, size.y, size.z) + position);
	object_positions.push_back(Vec3f(-size.x, size.y, size.z) + position);

	object_positions.push_back(Vec3f(-size.x, size.y, size.z) + position);
	object_positions.push_back(Vec3f(size.x, size.y, size.z) + position);
	object_positions.push_back(Vec3f(size.x, size.y, -size.z) + position);
	object_positions.push_back(Vec3f(size.x, size.y, -size.z) + position);
	object_positions.push_back(Vec3f(-size.x, size.y, -size.z) + position);
	object_positions.push_back(Vec3f(-size.x, size.y, size.z) + position);

	object_positions.push_back(Vec3f(size.x, -size.y, -size.z) + position);
	object_positions.push_back(Vec3f(-size.x, -size.y, -size.z) + position);
	object_positions.push_back(Vec3f(-size.x, size.y, -size.z) + position);
	object_positions.push_back(Vec3f(-size.x, size.y, -size.z) + position);
	object_positions.push_back(Vec3f(size.x, size.y, -size.z) + position);
	object_positions.push_back(Vec3f(size.x, -size.y, -size.z) + position);

	object_positions.push_back(Vec3f(size.x, -size.y, -size.z) + position);
	object_positions.push_back(Vec3f(size.x, -size.y, size.z) + position);
	object_positions.push_back(Vec3f(-size.x, -size.y, size.z) + position);
	object_positions.push_back(Vec3f(-size.x, -size.y, size.z) + position);
	object_positions.push_back(Vec3f(-size.x, -size.y, -size.z) + position);
	object_positions.push_back(Vec3f(size.x, -size.y, -size.z) + position);

	object_positions.push_back(Vec3f(size.x, -size.y, -size.z) + position);
	object_positions.push_back(Vec3f(size.x, size.y, -size.z) + position);
	object_positions.push_back(Vec3f(size.x, size.y, size.z) + position);
	object_positions.push_back(Vec3f(size.x, size.y, size.z) + position);
	object_positions.push_back(Vec3f(size.x, -size.y, size.z) + position);
	object_positions.push_back(Vec3f(size.x, -size.y, -size.z) + position);

	for (int cnt = 0; cnt<6; cnt++)
		object_normals.push_back(Vec3f(-1.0f, 0.0f, 0.0f));
	for (int cnt = 0; cnt<6; cnt++)
		object_normals.push_back(Vec3f(0.0f, 0.0f, 1.0f));
	for (int cnt = 0; cnt<6; cnt++)
		object_normals.push_back(Vec3f(0.0f, 1.0f, 0.0f));
	for (int cnt = 0; cnt<6; cnt++)
		object_normals.push_back(Vec3f(0.0f, 0.0f, -1.0f));
	for (int cnt = 0; cnt<6; cnt++)
		object_normals.push_back(Vec3f(0.0f, -1.0f, 0.0f));
	for (int cnt = 0; cnt<6; cnt++)
		object_normals.push_back(Vec3f(1.0f, 0.0f, 0.0f));

	vbo_positions = VertexArrayVector<Vec3f>(gc, object_positions.size());
	vbo_normals = VertexArrayVector<Vec3f>(gc, object_normals.size());
	vbo_positions.upload_data(gc, 0, &object_positions[0], object_positions.size());
	vbo_normals.upload_data(gc, 0, &object_normals[0], object_normals.size());
	vbo_size = object_positions.size();

#endif

}

int Model_Impl::count_vertices(const struct aiScene* sc, const struct aiNode* nd)
{
#if defined(I_LOVE_ASSIMP_AND_PRECOMPILED_IT)
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
#else
	return 0;
#endif
}

int Model_Impl::insert_vbo(GraphicContext &gc, int vertex_count, const struct aiScene* sc, const struct aiNode* nd)
{
#if defined(I_LOVE_ASSIMP_AND_PRECOMPILED_IT)
	int i;
	unsigned int n = 0, t;

	// All meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
		int num_vertex = mesh->mNumFaces * 3;
		if (!num_vertex)
			continue;

		std::vector<Vec3f> normals;
		std::vector<Vec3f> vertices;

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

		vbo_positions.upload_data(gc, vertex_count, &vertices[0], num_vertex);
		vbo_normals.upload_data(gc, vertex_count, &normals[0], num_vertex);

		vertex_count += num_vertex;
	}

	// All children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		vertex_count = insert_vbo(gc, vertex_count, sc, nd->mChildren[n]);
	}
	return vertex_count;
#else
	return 0;
#endif
}


void Model_Impl::Draw(GraphicContext &gc, GraphicStore *gs, const Mat4f &modelview_matrix)
{
	Mat4f matrix_modelview_projection = gs->camera_projection *  modelview_matrix;
	Mat3f normal_matrix = Mat3f(modelview_matrix);
	normal_matrix.inverse();
	normal_matrix.transpose();

	PrimitivesArray prim_array(gc);
	
	prim_array.set_attributes(0, vbo_positions, 3, VertexAttributeDataType::type_float, 0);
	prim_array.set_attributes(1, vbo_normals, 3, VertexAttributeDataType::type_float, 0);
	gs->shader_color.SetMaterial(material_shininess, material_emission, material_ambient, material_specular);
	gs->shader_color.Use(gc, modelview_matrix, matrix_modelview_projection, Mat4f(normal_matrix));
	gc.draw_primitives(PrimitivesType::triangles, vbo_size, prim_array);
}
