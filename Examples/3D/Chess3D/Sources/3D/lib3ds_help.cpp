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
**    Magnus Norddahl
**    Mark Page
*/
#include "precomp.h"
#include "lib3ds_help.h"

/////////////////////////////////////////////////////////////////////////////

Lib3dsIo::Lib3dsIo(IODevice device)
: device(device)
{
	io.self = this;
	io.log_func = &Lib3dsIo::io_log_func;
	io.seek_func = &Lib3dsIo::io_seek_func;
	io.tell_func = &Lib3dsIo::io_tell_func;
	io.read_func = &Lib3dsIo::io_read_func;
	io.write_func = &Lib3dsIo::io_write_func;
}

Lib3dsIo::~Lib3dsIo()
{
}

void Lib3dsIo::io_log_func(void *self, Lib3dsLogLevel level, int indent, const char *msg)
{
}

long Lib3dsIo::io_seek_func(void *_self, long offset, Lib3dsIoSeek origin)
{
	Lib3dsIo *self = (Lib3dsIo *) _self;
	switch (origin)
	{
	case LIB3DS_SEEK_SET:
		return self->device.seek(offset, IODevice::seek_set);
	case LIB3DS_SEEK_CUR:
		return self->device.seek(offset, IODevice::seek_cur);
	case LIB3DS_SEEK_END:
		return self->device.seek(offset, IODevice::seek_end);
	}
	return 0;
}

long Lib3dsIo::io_tell_func(void *_self)
{
	Lib3dsIo *self = (Lib3dsIo *) _self;
	return self->device.get_position();
}

size_t Lib3dsIo::io_read_func(void *_self, void *buffer, size_t size)
{
	Lib3dsIo *self = (Lib3dsIo *) _self;
	return self->device.read(buffer, size, false);
}

size_t Lib3dsIo::io_write_func(void *_self, const void *buffer, size_t size)
{
	Lib3dsIo *self = (Lib3dsIo *) _self;
	return self->device.write(buffer, size, false);
}

/////////////////////////////////////////////////////////////////////////////

Lib3dsFile::Lib3dsFile()
: file(0)
{
	file = lib3ds_file_new();
}

Lib3dsFile::~Lib3dsFile()
{
	lib3ds_file_free(file);
}

void Lib3dsFile::load(IODevice device)
{
	Lib3dsIo io(device);
	int result = lib3ds_file_read(file, io);
	if (!result)
		throw Exception("Unable to load 3ds file");
}

void Lib3dsFile::save(IODevice device)
{
	Lib3dsIo io(device);
	int result = lib3ds_file_write(file, io);
	if (!result)
		throw Exception("Unable to save 3ds file");
}

std::vector<Lib3dsMesh> Lib3dsFile::export_meshes(Lib3dsNode *node)
{
	if (node == 0)
	{
		lib3ds_file_eval(file, 0.0f);
		node = file->nodes;
	}

	std::vector<Lib3dsMesh> meshes;
	for (; node; node = node->next)
	{
		if (node->type == LIB3DS_NODE_MESH_INSTANCE)
		{
			Lib3dsMeshInstanceNode *instance_node = (Lib3dsMeshInstanceNode *) node;
			Lib3dsMesh *mesh = lib3ds_file_mesh_for_node(file, (Lib3dsNode*)node);
			if (mesh && mesh->vertices)
			{
				float inv_matrix[4][4], M[4][4];
				lib3ds_matrix_copy(M, instance_node->base.matrix);
				lib3ds_matrix_translate(M, -instance_node->pivot[0], -instance_node->pivot[1], -instance_node->pivot[2]);
				lib3ds_matrix_copy(inv_matrix, mesh->matrix);
				lib3ds_matrix_inv(inv_matrix);
				lib3ds_matrix_mult(M, M, inv_matrix);

				std::vector<Vec3f> positions;
				for (int i = 0; i < mesh->nvertices; ++i)
				{
					float position[3];
					lib3ds_vector_transform(position, M, mesh->vertices[i]);
					positions.push_back(Vec3f(position[0], position[1], position[2]));
				}

				std::vector<Vec2f> texcoords;
				if (mesh->texcos)
				{
					for (int i = 0; i < mesh->nvertices; ++i)
					{
						float tx = mesh->texcos[i][0];
						float ty = mesh->texcos[i][1];
						texcoords.push_back(Vec2f(tx, ty));
					}
				}

				std::vector<Vec3f> normals;
				if (mesh->faces && mesh->nfaces > 0)
				{
			        float (*normals_array)[3] = (float(*)[3])malloc(sizeof(float) * 9 * mesh->nfaces);
					lib3ds_mesh_calculate_vertex_normals(mesh, normals_array);
					for (int i = 0; i < 3 * mesh->nfaces; ++i)
					{
						normals.push_back(Vec3f(normals_array[i][0], normals_array[i][1], normals_array[i][2]));
					}
					free(normals_array);
				}

				Lib3dsMesh mesh_object;
				for (int i = 0; i < mesh->nfaces; ++i)
				{
					int material_index = mesh->faces[i].material;
					mesh_object.face_materials.push_back(material_index);

					for (int j = 0; j < 3; ++j)
					{
						int vertex_index = mesh->faces[i].index[j];
						mesh_object.positions.push_back(positions[vertex_index]);
						if (!texcoords.empty())
							mesh_object.texcooords.push_back(texcoords[vertex_index]);
						if (!normals.empty())
							mesh_object.normals.push_back(normals[i*3+j]);
					}
				}

				meshes.push_back(mesh_object);
			}

			if (node->childs)
			{
				std::vector<Lib3dsMesh> child_meshes = export_meshes(node->childs);
				meshes.insert(meshes.end(), child_meshes.begin(), child_meshes.end());
			}
		}
	}

	return meshes;
}