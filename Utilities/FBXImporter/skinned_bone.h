
#pragma once

#include <fbxsdk.h>

class SkinnedBone
{
public:
	SkinnedBone() : bone_node(nullptr) { }

	clan::Mat4f bind_mesh_to_world;
	clan::Mat4f bind_bone_to_world;
	clan::Mat4f mesh_to_world;
	FbxNode *bone_node;
};
