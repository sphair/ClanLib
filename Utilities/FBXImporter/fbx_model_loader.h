
#pragma once

#include <fbxsdk.h>
#include "vertex_mapping.h"
#include "skinned_bone.h"

class FBXModelLoader
{
public:
	static std::shared_ptr<clan::ModelData> load(const std::string &filename);

private:
	FBXModelLoader(const std::string &filename);
	~FBXModelLoader();
	void import_scene(const std::string &filename);

	void bake_geometric_transforms(FbxNode *node = nullptr);

	void convert_node(FbxNode *node);
	void convert_mesh(FbxNode *node);
	void convert_camera(FbxNode *node);
	void convert_light(FbxNode *node);

	void convert_polygons(FbxMesh *mesh, VertexMappingVector &vertices, std::vector<VertexMapping *> &elements);
	void convert_skins(FbxNode *node, FbxMesh *mesh, VertexMappingVector &vertices);
	void convert_bones();

	clan::Vec4ub get_color(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index);
	clan::Vec3f get_normal(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index);
	clan::Vec3f get_tangent(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index);
	clan::Vec3f get_bitangent(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index);

	static clan::Vec4f to_vec4f(const FbxVector4 &v);
	static clan::Vec4ub to_vec4ub(const FbxColor &c);
	static clan::Mat4f to_mat4f(const FbxAMatrix &m);

	FbxManager *manager;
	FbxIOSettings *iosettings;
	FbxScene *scene;
	std::shared_ptr<clan::ModelData> model_data;

	std::vector<SkinnedBone> bones;
};
