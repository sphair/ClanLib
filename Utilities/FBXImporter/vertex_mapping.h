
#pragma once

class VertexMapping
{
public:
	VertexMapping() : vertex_index(), next(nullptr), bone_selectors(255) { }
	~VertexMapping() { delete next; }

	VertexMapping *next;

	clan::Vec3f position;
	clan::Vec3f normal;
	clan::Vec3f tangent;
	clan::Vec3f bitangent;

	clan::Vec4ub color;

	clan::Vec2f diffuse_uv;
	clan::Vec2f specular_uv;
	clan::Vec2f normal_uv;
	clan::Vec2f emission_uv;

	clan::Vec4ub bone_selectors;
	clan::Vec4ub bone_weights;

	int vertex_index;
};

class VertexMappingVector : public std::vector<VertexMapping *>
{
public:
	VertexMappingVector(size_t count)
		: std::vector<VertexMapping *>(count)
	{
	}

	~VertexMappingVector()
	{
		for (size_t i = 0; i < size(); i++)
			delete (*this)[i];
	}

private:
	VertexMappingVector(const VertexMappingVector &that);
	VertexMappingVector &operator=(const VertexMappingVector &that);
};
