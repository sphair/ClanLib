
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
	void triangulate_scene();
	void bake_geometric_transforms(FbxNode *node = nullptr);

	void convert_node(FbxNode *node);
	void convert_mesh(FbxNode *node);
	void convert_camera(FbxNode *node);
	void convert_light(FbxNode *node);

	void convert_polygons(FbxMesh *mesh, VertexMappingVector &vertices, std::vector<VertexMapping *> &elements, const clan::Mat4f &mesh_to_world, const clan::Mat3f &normal_mesh_to_world);
	void convert_skins(FbxNode *node, FbxMesh *mesh, VertexMappingVector &vertices);
	void convert_bones();

	clan::ModelDataDrawRange create_draw_range(size_t start_element, size_t num_elements, FbxSurfaceMaterial *material);

	clan::Vec4ub get_color(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index);
	clan::Vec3f get_normal(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index);
	clan::Vec3f get_tangent(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index);
	clan::Vec3f get_bitangent(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index);
	clan::Vec2f get_uv(FbxMesh *mesh, int polygon, int point, int control_index, int vertex_index, int uv_channel_index);

	static clan::Vec2f to_vec2f(const FbxVector2 &v);
	static clan::Vec3f to_vec3f(const FbxDouble3 &d);
	static clan::Vec4f to_vec4f(const FbxVector4 &v);
	static clan::Vec4ub to_vec4ub(const FbxColor &c);
	static clan::Mat4f to_mat4f(const FbxAMatrix &m);

	std::string base_path;

	FbxManager *manager;
	FbxIOSettings *iosettings;
	FbxScene *scene;
	std::shared_ptr<clan::ModelData> model_data;

	std::vector<SkinnedBone> bones;
};
