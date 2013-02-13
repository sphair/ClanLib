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

#pragma once

#include "Scene/ModelData/model_data.h"
namespace clan
{

class FileSearchPath;

class M2ModelLoader
{
public:
	static std::shared_ptr<ModelData> load(const std::string &filename, const FileSearchPath &search_path);

private:
	M2ModelLoader(ModelData *data, const std::string &filename, const FileSearchPath &search_path);
	ModelData *data;
	int version;

	struct Vertex
	{
		clan::Vec3f position;
		clan::Vec3f normal;
		clan::Vec2f texcoord0;
		clan::Vec4ub bone_weight;
		clan::Vec4ub bone_index;
	};

	struct ViewHeader
	{
		unsigned int num_vertices;
		unsigned int offset_vertices;
		unsigned int num_elements;
		unsigned int offset_elements;
		unsigned int num_vertex_properties;
		unsigned int offset_vertex_properties;
		unsigned int num_submeshes;
		unsigned int offset_submeshes;
		unsigned int num_texture_bindings;
		unsigned int offset_texture_bindings;
		unsigned int skinned_bones;
		clan::File skin_file;
	};

	struct SubMesh
	{
		unsigned int mesh_part_id;
		unsigned short start_vertex;
		unsigned short num_vertices;
		unsigned short start_element;
		unsigned short num_elements;
		unsigned short num_bones;
		unsigned short start_bone_lookup_index;
		unsigned short bone_unknown0;
		unsigned short bone_unknown1;
		clan::Vec3f unknown2;
		clan::Vec4f unknown3;
	};

	struct TextureDef
	{
		unsigned int texture_type;
		unsigned short unknown;
		enum Flags { wrap_x = 1, wrap_y = 2 };
		unsigned short flags;
		unsigned int filename_length;
		unsigned int filename_offset;
		std::string filename;
	};

	struct RenderFlags
	{
		unsigned short flags;
		unsigned short blend_mode;
	};

	struct TextureBinding
	{
		unsigned short flags;
		unsigned short render_order;
		unsigned short submesh_index;
		unsigned short submesh_index2;
		unsigned short color_index; // -1 if none
		unsigned short render_flags_table_index;
		unsigned short texture_unit_number;
		unsigned short unknown;
		unsigned short texture_index;
		unsigned short texture_unit_number2;
		unsigned short transparency_index;
		unsigned short texture_animation_index;
	};

	template<typename ValueType>
	struct AnimationTimeline
	{
		AnimationTimeline() : num_values(0), offset_values(0) { }

		unsigned int num_values;
		unsigned int offset_values;

		std::vector<std::pair<int,int>> ranges;
		std::vector<unsigned int> timestamps;
		std::vector<ValueType> values;
	};

	template<typename ValueType>
	struct AnimationBlock
	{
		unsigned short interpolation_type;
		short global_sequence_id;

		std::vector<AnimationTimeline<ValueType>> timelines;
	};

	typedef AnimationBlock<clan::Vec3f> BonePosition;
	typedef AnimationBlock<clan::Quaternionf> BoneOrientation;
	typedef AnimationBlock<clan::Vec3f> BoneScale;

	struct Bone
	{
		enum BoneFlags { billboarded = 0x008, transformed = 0x200 };
		int animation_id;
		unsigned int flags;
		short parent_bone;
		unsigned short geo_id;
		unsigned int unknown;
		BonePosition position;
		BoneOrientation orientation;
		BoneScale scale;
		clan::Vec3f pivot;
	};

	template<typename ValueType>
	void read_animation_block(clan::IODevice &file, int version, AnimationBlock<ValueType> &out_block);

	clan::Mat4f get_bone_transform(int animation_index, const std::vector<Bone> &bones, int bone, unsigned int timestamp);

	template<typename ValueType>
	float find_animation_indices(const AnimationTimeline<ValueType> &timeline, unsigned int timestamp, size_t &out_index, size_t &out_index2);
};

}

