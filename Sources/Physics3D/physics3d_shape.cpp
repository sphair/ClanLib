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
*/

#include "Physics3D/precomp.h"
#include "API/Physics3D/physics3d_shape.h"
#include "API/Core/System/exception.h"
#include "API/Core/Math/vec3.h"
#include "API/Core/Math/quaternion.h"
#include "API/Scene3D/ModelData/model_data.h"
#include "Physics3D/Bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "Physics3D/Bullet/BulletCollision/Gimpact/btGImpactShape.h"
#include "physics3d_shape_impl.h"

namespace clan
{

Physics3DShape::Physics3DShape()
{
}

bool Physics3DShape::is_null() const
{
	return !impl;
}

Physics3DShape Physics3DShape::box(const clan::Vec3f &size)
{
	Physics3DShape shape;
	shape.impl = std::shared_ptr<Physics3DShape_Impl>(new Physics3DShape_Impl());
	shape.impl->shape.reset(new btBoxShape(btVector3(size.x, size.y, size.z)));
	shape.impl->shape->setUserPointer(shape.impl.get());
	return shape;
}

Physics3DShape Physics3DShape::capsule(float radius, float height)
{
	Physics3DShape shape;
	shape.impl = std::shared_ptr<Physics3DShape_Impl>(new Physics3DShape_Impl());
	shape.impl->shape.reset(new btCapsuleShape(radius, height));
	shape.impl->shape->setUserPointer(shape.impl.get());
	return shape;
}

Physics3DShape Physics3DShape::sphere(float radius)
{
	Physics3DShape shape;
	shape.impl = std::shared_ptr<Physics3DShape_Impl>(new Physics3DShape_Impl());
	shape.impl->shape.reset(new btSphereShape(radius));
	shape.impl->shape->setUserPointer(shape.impl.get());
	return shape;
}

Physics3DShape Physics3DShape::model(const std::shared_ptr<clan::ModelData> &model_data)
{
	Physics3DShape shape;
	shape.impl = std::shared_ptr<Physics3DShape_Impl>(new Physics3DShape_Impl());

	unsigned int vertex_offset = 0;
	for (size_t i = 0; i < model_data->mesh_lods[0].meshes.size(); i++)
	{
		shape.impl->model_vertices.insert(shape.impl->model_vertices.end(), model_data->mesh_lods[0].meshes[i].vertices.begin(), model_data->mesh_lods[0].meshes[i].vertices.end());
		for (size_t j = 0; j < model_data->mesh_lods[0].meshes[i].elements.size(); j++)
			shape.impl->model_elements.push_back(vertex_offset + model_data->mesh_lods[0].meshes[i].elements[j]);
		vertex_offset += model_data->mesh_lods[0].meshes[i].vertices.size();
	}

	btIndexedMesh submesh;
	submesh.m_numTriangles = (int)shape.impl->model_elements.size() / 3;
	submesh.m_triangleIndexBase = reinterpret_cast<const unsigned char *>(&shape.impl->model_elements[0]);
	submesh.m_triangleIndexStride = sizeof(int)*3;
	submesh.m_vertexType = PHY_FLOAT;
	submesh.m_vertexBase = reinterpret_cast<const unsigned char *>(&shape.impl->model_vertices[0]);
	submesh.m_vertexStride = sizeof(Vec3f);
	submesh.m_numVertices = (int)shape.impl->model_vertices.size();

	shape.impl->vertex_array.reset(new btTriangleIndexVertexArray());
	shape.impl->vertex_array->addIndexedMesh(submesh, PHY_INTEGER);

	bool useQuantizedAabbCompression = true;

	// if create from memory:
	btBvhTriangleMeshShape *mesh = new btBvhTriangleMeshShape(shape.impl->vertex_array.get(), useQuantizedAabbCompression);
	shape.impl->shape.reset(mesh);
	shape.impl->shape->setUserPointer(shape.impl.get());
/*
	// if save to disk:

	int max_buffer_size = 1024*1024*5;
	std::unique_ptr<btDefaultSerializer> serializer(new btDefaultSerializer(max_buffer_size));
	serializer->startSerialization();
	mesh->serializeSingleBvh(serializer.get()); // or maybe use serializeSingleShape(serializer.get()) ?
	serializer->finishSerialization();
	File file("model.bullet", File::create_always, File::access_read_write);
	file.write(serializer->getBufferPointer(), serializer->getCurrentBufferSize());
	file.close();

	// if load from disk:

	btBulletWorldImporter import(0);
	if (import.loadFile("model.bullet"))
	{
		std::unique_ptr<btTriangleIndexVertexArray> vertex_array;

		int num_bvh = import.getNumBvhs();
		if (num_bvh)
		{
			btOptimizedBvh* bvh = import.getBvhByIndex(0);
			btBvhTriangleMeshShape *mesh = new btBvhTriangleMeshShape(shape.impl->vertex_array, useQuantizedAabbCompression, aabb_min, aabb_max, false);
			shape.impl->shape = mesh;
			shape->setOptimizedBvh(bvh);
		}
	}
	else
	{
		throw Exception("Bullet mesh not found");
	}
*/

	return shape;
}

Physics3DShape Physics3DShape::scale_model(const Physics3DShape &base_model, clan::Vec3f scale)
{
	btBvhTriangleMeshShape *base_shape = dynamic_cast<btBvhTriangleMeshShape*>(base_model.impl->shape.get());
	if (base_shape == 0)
		throw Exception("Physics3DShape is not a model shape!");

	Physics3DShape shape;
	shape.impl = std::shared_ptr<Physics3DShape_Impl>(new Physics3DShape_Impl());
	shape.impl->base_model = base_model.impl;
	shape.impl->shape.reset(new btScaledBvhTriangleMeshShape(base_shape, btVector3(scale.x, scale.y, scale.z)));
	shape.impl->shape->setUserPointer(shape.impl.get());
	return shape;
}

Physics3DShape Physics3DShape::gimpact_model(const std::shared_ptr<clan::ModelData> &model_data)
{
	Physics3DShape shape;
	shape.impl = std::shared_ptr<Physics3DShape_Impl>(new Physics3DShape_Impl());

	unsigned int vertex_offset = 0;
	for (size_t i = 0; i < model_data->mesh_lods[0].meshes.size(); i++)
	{
		shape.impl->model_vertices.insert(shape.impl->model_vertices.end(), model_data->mesh_lods[0].meshes[i].vertices.begin(), model_data->mesh_lods[0].meshes[i].vertices.end());
		for (size_t j = 0; j < model_data->mesh_lods[0].meshes[i].elements.size(); j++)
			shape.impl->model_elements.push_back(vertex_offset + model_data->mesh_lods[0].meshes[i].elements[j]);
		vertex_offset += model_data->mesh_lods[0].meshes[i].vertices.size();
	}

	btIndexedMesh submesh;
	submesh.m_numTriangles = (int)shape.impl->model_elements.size() / 3;
	submesh.m_triangleIndexBase = reinterpret_cast<const unsigned char *>(&shape.impl->model_elements[0]);
	submesh.m_triangleIndexStride = sizeof(int)*3;
	submesh.m_vertexType = PHY_FLOAT;
	submesh.m_vertexBase = reinterpret_cast<const unsigned char *>(&shape.impl->model_vertices[0]);
	submesh.m_vertexStride = sizeof(Vec3f);
	submesh.m_numVertices = (int)shape.impl->model_vertices.size();

	shape.impl->vertex_array.reset(new btTriangleIndexVertexArray());
	shape.impl->vertex_array->addIndexedMesh(submesh, PHY_INTEGER);

	shape.impl->shape.reset(new btGImpactMeshShape(shape.impl->vertex_array.get()));
	shape.impl->shape->setUserPointer(shape.impl.get());

	return shape;
}

Physics3DShape Physics3DShape::terrain(const std::shared_ptr<clan::TerrainData> &terrain_data, clan::Mat4f &out_transform)
{
	throw Exception("Physics3DShape::terrain not implemented");
/*
	Physics3DShape shape;
	shape.impl = std::shared_ptr<Physics3DShape_Impl>(new Physics3DShape_Impl());
	shape.impl->heights = terrain_data->heights();

	float min_height = shape.impl->heights[0];
	float max_height = shape.impl->heights[0];
	for (size_t i = 1; i < shape.impl->heights.size(); i++)
	{
		min_height = min(min_height, shape.impl->heights[i]);
		max_height = max(max_height, shape.impl->heights[i]);
	}

	int up_axis = 1; // y is up
	shape.impl->shape.reset(new btHeightfieldTerrainShape(tile.width(), tile.height(), &shape.impl->heights[0], 1.0f, min_height, max_height, up_axis, PHY_FLOAT, false));
	shape.impl->shape->setUserPointer(shape.impl.get());
	terrain->setUseDiamondSubdivision(true);

	out_transform = Mat4f::translate(tile.x() + tile.width() * 0.5f, min_height + (max_height - min_height) * 0.5f, tile.y() + tile.height() * 0.5f);
	return shape;
*/
}

/*
Physics3DShape Physics3DShape::terrain_with_holes(const std::shared_ptr<clan::TerrainData> &terrain_data, Mat4f &out_transform)
{
	out_transform = Mat4f::translate((float)tile.x(), 0.0f, (float)tile.y());

	Physics3DShape shape;
	shape.impl = std::shared_ptr<Physics3DShape_Impl>(new Physics3DShape_Impl());

	const TerrainVector<unsigned char> &flags = tile.flags();
	const TerrainVector<float> &heights = tile.heights();

	std::shared_ptr<ModelData> model_data(new ModelData());
	for (int y = 0; y < tile.height(); y++)
	{
		for (int x = 0;  x < tile.width(); x++)
		{
			shape.impl->model_vertices.push_back(Vec3f((float)x, (float)y, heights[x + y * tile.width()]));
		}

		if (y + 1 != tile.height())
		{
			for (int x = 0;  x < tile.width() - 1; x++)
			{
				int offset = x + y * tile.width();
				float height = 0.25f * (heights[offset] + heights[offset + 1] + heights[offset + tile.width()] + heights[offset + 1 + tile.width()]);
				shape.impl->model_vertices.push_back(Vec3f((float)x + 0.5f, (float)y + 0.5f, height));
			}
		}
	}

	int width = tile.width() - 1;
	int height = tile.height() - 1;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0;  x < width; x++)
		{
			if ((flags[x + y * tile.width()] & ContinentBlock::flag_hole) == 0)
			{
				int index_top_left = x + y * (width * 2 + 1);
				int index_top_right = index_top_left + 1;
				int index_bottom_left = index_top_left + width * 2 + 1;
				int index_bottom_right = index_bottom_left + 1;
				int index_center = index_top_left + width + 1;

				shape.impl->model_elements.push_back(index_top_left);
				shape.impl->model_elements.push_back(index_top_right);
				shape.impl->model_elements.push_back(index_center);

				shape.impl->model_elements.push_back(index_center);
				shape.impl->model_elements.push_back(index_bottom_left);
				shape.impl->model_elements.push_back(index_top_left);

				shape.impl->model_elements.push_back(index_center);
				shape.impl->model_elements.push_back(index_bottom_right);
				shape.impl->model_elements.push_back(index_bottom_left);

				shape.impl->model_elements.push_back(index_top_right);
				shape.impl->model_elements.push_back(index_bottom_right);
				shape.impl->model_elements.push_back(index_center);
			}
		}
	}

	btIndexedMesh submesh;
	submesh.m_numTriangles = (int)shape.impl->model_elements.size() / 3;
	submesh.m_triangleIndexBase = reinterpret_cast<const unsigned char *>(&shape.impl->model_elements[0]);
	submesh.m_triangleIndexStride = sizeof(int)*3;
	submesh.m_vertexType = PHY_FLOAT;
	submesh.m_vertexBase = reinterpret_cast<const unsigned char *>(&shape.impl->model_vertices[0]);
	submesh.m_vertexStride = sizeof(Vec3f);
	submesh.m_numVertices = (int)shape.impl->model_vertices.size();

	shape.impl->vertex_array = new btTriangleIndexVertexArray();
	shape.impl->vertex_array->addIndexedMesh(submesh, PHY_INTEGER);

	bool useQuantizedAabbCompression = true;
	shape.impl->shape.reset(new btBvhTriangleMeshShape(shape.impl->vertex_array, useQuantizedAabbCompression));
	shape.impl->shape->setUserPointer(shape.impl.get());
	return shape;
}
*/

/////////////////////////////////////////////////////////////////////////////

Physics3DShape_Impl::Physics3DShape_Impl()
{
}

Physics3DShape_Impl::~Physics3DShape_Impl()
{
}

}
