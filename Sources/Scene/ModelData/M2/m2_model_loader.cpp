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

#include "Scene/precomp.h"
#include "m2_model_loader.h"
#include "Scene/Framework/file_search_path.h"
#include <algorithm>

namespace clan
{


std::shared_ptr<ModelData> M2ModelLoader::load(const std::string &filename, const FileSearchPath &search_path)
{
	std::shared_ptr<ModelData> model(new ModelData());
	M2ModelLoader self(model.get(), filename, search_path);
	return model;
}

M2ModelLoader::M2ModelLoader(ModelData *data, const std::string &filename, const FileSearchPath &search_path)
: data(data)
{
	File file(filename);
	std::string local_path = PathHelp::get_fullpath(filename);

	char magic[5];
	magic[4] = 0;
	file.read(magic, 4);
	if (std::string(magic) != "MD20")
		throw Exception("Not a M2 model file");

	version = file.read_uint32();
	// classic    = 00 01 00 00
	// bc         = 04 01 00 00 = 260
	// wotlk      = 08 01 00 00 = 264
	// cataclysm  = 10 01 00 00 = 272

	unsigned int model_name_length = file.read_uint32();
	unsigned int model_name_offset = file.read_uint32();
	unsigned int model_flags = file.read_uint32();
	unsigned int num_global_sequences = file.read_uint32();
	unsigned int offset_global_sequences = file.read_uint32();
	unsigned int num_animations = file.read_uint32();
	unsigned int offset_animations = file.read_uint32();
	unsigned int num_animation_lookup = file.read_uint32();
	unsigned int offset_animation_lookup = file.read_uint32();
	if (version < 264)
	{
		unsigned int num_unknown0 = file.read_uint32();
		unsigned int offset_unknown0 = file.read_uint32();
	}
	unsigned int num_bones = file.read_uint32();
	unsigned int offset_bones = file.read_uint32();
	unsigned int num_skelbone_lookup = file.read_uint32();
	unsigned int offset_skelbone_lookup = file.read_uint32();
	unsigned int num_vertices = file.read_uint32();
	unsigned int offset_vertices = file.read_uint32();
	unsigned int num_lod_views = file.read_uint32();
	unsigned int offset_lod_views = version < 264 ? file.read_uint32() : 0; // views moved to .skins files in wotlk
	unsigned int num_colors = file.read_uint32();
	unsigned int offset_colors = file.read_uint32();
	unsigned int num_textures = file.read_uint32();
	unsigned int offset_textures = file.read_uint32();
	unsigned int num_transparency = file.read_uint32();
	unsigned int offset_transparency = file.read_uint32();
	if (version < 264)
	{
		unsigned int num_unknown1 = file.read_uint32();
		unsigned int offset_unknown1 = file.read_uint32();
	}
	unsigned int num_tex_anims = file.read_uint32();
	unsigned int offset_tex_anims = file.read_uint32();
	unsigned int num_tex_replace = file.read_uint32();
	unsigned int offset_tex_replace = file.read_uint32();
	unsigned int num_render_flags = file.read_uint32();
	unsigned int offset_render_flags = file.read_uint32();
	unsigned int num_bone_lookup_table = file.read_uint32();
	unsigned int offset_bone_lookup_table = file.read_uint32();
	unsigned int num_tex_lookup = file.read_uint32();
	unsigned int offset_tex_lookup = file.read_uint32();
	unsigned int num_tex_units = file.read_uint32();
	unsigned int offset_tex_units = file.read_uint32();
	unsigned int num_trans_lookup = file.read_uint32();
	unsigned int offset_trans_lookup = file.read_uint32();
	unsigned int num_tex_anim_lookup = file.read_uint32();
	unsigned int offset_tex_anim_lookup = file.read_uint32();

	Vec3f collision_sphere_min(file.read_float(), file.read_float(), file.read_float());
	Vec3f collision_sphere_max(file.read_float(), file.read_float(), file.read_float());
	float collision_sphere_radius = file.read_float();

	Vec3f bounding_sphere_min(file.read_float(), file.read_float(), file.read_float());
	Vec3f bounding_sphere_max(file.read_float(), file.read_float(), file.read_float());
	float bounding_sphere_radius = file.read_float();

	unsigned int num_bounding_triangles = file.read_uint32();
	unsigned int offset_bounding_triangles = file.read_uint32();
	unsigned int num_bounding_vertices = file.read_uint32();
	unsigned int offset_bounding_vertices = file.read_uint32();
	unsigned int num_bounding_normals = file.read_uint32();
	unsigned int offset_bounding_normals = file.read_uint32();
	unsigned int num_attachments = file.read_uint32();
	unsigned int offset_attachments = file.read_uint32();
	unsigned int num_attach_lookup = file.read_uint32();
	unsigned int offset_attach_lookup = file.read_uint32();
	unsigned int num_events = file.read_uint32();
	unsigned int offset_events = file.read_uint32();
	unsigned int num_lights = file.read_uint32();
	unsigned int offset_lights = file.read_uint32();
	unsigned int num_cameras = file.read_uint32();
	unsigned int offset_cameras = file.read_uint32();
	unsigned int num_camera_lookup = file.read_uint32();
	unsigned int offset_camera_lookup = file.read_uint32();
	unsigned int num_ribbon_emitters = file.read_uint32();
	unsigned int offset_ribbon_emitters = file.read_uint32();
	unsigned int num_particle_emitters = file.read_uint32();
	unsigned int offset_particle_emitters = file.read_uint32();

	file.seek(offset_tex_lookup);
	std::vector<unsigned short> texture_lookup;
	texture_lookup.resize(num_tex_lookup);
	if (!texture_lookup.empty())
		file.read(&texture_lookup[0], sizeof(unsigned short) * num_tex_lookup);

	std::vector<TextureDef> textures;
	file.seek(offset_textures);
	for (unsigned int i = 0; i < num_textures; i++)
	{
		TextureDef texture;
		texture.texture_type = file.read_uint32();
		texture.unknown = file.read_uint16();
		texture.flags = file.read_uint16();
		texture.filename_length = file.read_uint32();
		texture.filename_offset = file.read_uint32();
		textures.push_back(texture);
	}

	std::vector<int> texture_index;
	for (unsigned int i = 0; i < num_textures; i++)
	{
		if (textures[i].texture_type == 0)
		{
			int length = max(((int)textures[i].filename_length) - 1, 0);
			textures[i].filename.resize(length);
			file.seek(textures[i].filename_offset);
			if (!textures[i].filename.empty())
				file.read(&textures[i].filename[0], length);

			try
			{
				textures[i].filename = search_path.search(local_path, textures[i].filename);
			}
			catch (Exception &)
			{
				// Texture not found.
				// To do: log this somehow.
			}

			texture_index.push_back(data->textures.size());
			data->textures.push_back(ModelDataTexture(textures[i].filename, false));
		}
		else
		{
			texture_index.push_back(data->replaceable_textures.size());
			data->replaceable_textures.push_back(StringHelp::uint_to_text(textures[i].texture_type));
		}
	}

	std::vector<Vertex> vertices;
	vertices.reserve(num_vertices);

	file.seek(offset_vertices);
	for (unsigned int i = 0; i < num_vertices; i++)
	{
		Vec3f position(file.read_float(), file.read_float(), file.read_float());
		unsigned char bone_weights[4] = { file.read_uint8(), file.read_uint8(), file.read_uint8(), file.read_uint8() };
		unsigned char bone_indices[4] = { file.read_uint8(), file.read_uint8(), file.read_uint8(), file.read_uint8() };
		Vec3f normal(file.read_float(), file.read_float(), file.read_float());
		Vec2f texcoord(file.read_float(), file.read_float());
		Vec2f attribute_unknown(file.read_float(), file.read_float());

		Vertex vertex;
		vertex.position = position; // Vec3f(position.x, -position.z, position.y);
		vertex.normal = normal; // Vec3f(normal.x, -normal.z, normal.y);
		vertex.texcoord0 = Vec2f(texcoord.y, texcoord.x);
		vertex.bone_weight = Vec4ub(bone_weights[0], bone_weights[1], bone_weights[2], bone_weights[3]);
		vertex.bone_index = Vec4ub(bone_indices[0], bone_indices[1], bone_indices[2], bone_indices[3]);
		vertices.push_back(vertex);
	}

	file.seek(offset_render_flags);
	std::vector<RenderFlags> render_flags;
	for (unsigned int i = 0; i < num_render_flags; i++)
	{
		RenderFlags flag;
		flag.flags = file.read_uint16();
		flag.blend_mode = file.read_uint16();
		render_flags.push_back(flag);
	}

	std::vector<ViewHeader> view_headers;
	if (version < 264)
	{
		file.seek(offset_lod_views);
		for (unsigned int i = 0; i < num_lod_views; i++)
		{
			ViewHeader header;
			header.num_vertices = file.read_uint32();
			header.offset_vertices = file.read_uint32();
			header.num_elements = file.read_uint32();
			header.offset_elements = file.read_uint32();
			header.num_vertex_properties = file.read_uint32();
			header.offset_vertex_properties = file.read_uint32();
			header.num_submeshes = file.read_uint32();
			header.offset_submeshes = file.read_uint32();
			header.num_texture_bindings = file.read_uint32();
			header.offset_texture_bindings = file.read_uint32();
			header.skinned_bones = file.read_uint32();
			header.skin_file = file;
			view_headers.push_back(header);
		}
	}
	else
	{
		std::string skin_filename = PathHelp::combine(local_path, PathHelp::get_basename(filename)) + "%1.skin";
		for (unsigned int i = 0; i < num_lod_views; i++)
		{
			std::string index_str;
			if (i < 10)
				index_str = "0" + StringHelp::int_to_text(i);
			else
				index_str = StringHelp::int_to_text(i);
			File skin_file(string_format(skin_filename, index_str));

			char skin_magic[5];
			skin_magic[4] = 0;
			skin_file.read(skin_magic, 4);
			if (std::string(skin_magic) != "SKIN")
				throw Exception("Not a M2 model skin file");

			ViewHeader header;
			header.num_vertices = skin_file.read_uint32();
			header.offset_vertices = skin_file.read_uint32();
			header.num_elements = skin_file.read_uint32();
			header.offset_elements = skin_file.read_uint32();
			header.num_vertex_properties = skin_file.read_uint32();
			header.offset_vertex_properties = skin_file.read_uint32();
			header.num_submeshes = skin_file.read_uint32();
			header.offset_submeshes = skin_file.read_uint32();
			header.num_texture_bindings = skin_file.read_uint32();
			header.offset_texture_bindings = skin_file.read_uint32();
			header.skinned_bones = skin_file.read_uint32();
			header.skin_file = skin_file;
			view_headers.push_back(header);
		}
	}

	data->mesh_lods.resize(1/*view_headers.size()*/);

	ViewHeader &view = view_headers.back();

	view.skin_file.seek(view.offset_vertices);
	std::vector<unsigned short> vertex_mappings;
	vertex_mappings.resize(view.num_vertices);
	if (!vertex_mappings.empty())
		view.skin_file.read(&vertex_mappings[0], sizeof(unsigned short) * view.num_vertices);

	view.skin_file.seek(view.offset_elements);
	std::vector<unsigned short> elements;
	elements.resize(view.num_elements);
	if (!elements.empty())
		view.skin_file.read(&elements[0], sizeof(unsigned short) * view.num_elements);

	data->mesh_lods[0].meshes.resize(1);
	data->mesh_lods[0].meshes[0].elements.reserve(view.num_elements);
	data->mesh_lods[0].meshes[0].vertices.reserve(view.num_vertices);
	data->mesh_lods[0].meshes[0].normals.reserve(view.num_vertices);
	data->mesh_lods[0].meshes[0].bone_weights.reserve(view.num_vertices);
	data->mesh_lods[0].meshes[0].bone_selectors.reserve(view.num_vertices);
	data->mesh_lods[0].meshes[0].channels.resize(1);
	data->mesh_lods[0].meshes[0].channels[0].reserve(view.num_vertices);

	for (unsigned int i = 0; i < view.num_vertices; i++)
	{
		data->mesh_lods[0].meshes[0].vertices.push_back(vertices[vertex_mappings[i]].position);
		data->mesh_lods[0].meshes[0].normals.push_back(vertices[vertex_mappings[i]].normal);
		data->mesh_lods[0].meshes[0].bone_weights.push_back(vertices[vertex_mappings[i]].bone_weight);
		data->mesh_lods[0].meshes[0].bone_selectors.push_back(vertices[vertex_mappings[i]].bone_index);
		data->mesh_lods[0].meshes[0].channels[0].push_back(vertices[vertex_mappings[i]].texcoord0);
	}

	for (unsigned int i = 0; i + 2 < view.num_elements; i+=3)
	{
		data->mesh_lods[0].meshes[0].elements.push_back(elements[i+2]);
		data->mesh_lods[0].meshes[0].elements.push_back(elements[i+1]);
		data->mesh_lods[0].meshes[0].elements.push_back(elements[i]);
	}

	view.skin_file.seek(view.offset_submeshes);
	std::vector<SubMesh> submeshes;
	for (unsigned int i = 0; i < view.num_submeshes; i++)
	{
		SubMesh submesh;
		submesh.mesh_part_id = view.skin_file.read_uint32();
		submesh.start_vertex = view.skin_file.read_uint16();
		submesh.num_vertices = view.skin_file.read_uint16();
		submesh.start_element = view.skin_file.read_uint16();
		submesh.num_elements = view.skin_file.read_uint16();
		submesh.num_bones = view.skin_file.read_uint16();
		submesh.start_bone_lookup_index = view.skin_file.read_uint16();
		submesh.bone_unknown0 = view.skin_file.read_uint16();
		submesh.bone_unknown1 = view.skin_file.read_uint16();
		submesh.unknown2 = Vec3f(view.skin_file.read_float(), view.skin_file.read_float(), view.skin_file.read_float());
		if (version >= 260)
			submesh.unknown3 = Vec4f(view.skin_file.read_float(), view.skin_file.read_float(), view.skin_file.read_float(), view.skin_file.read_float());
		submeshes.push_back(submesh);
	}

	view.skin_file.seek(view.offset_texture_bindings);
	std::vector<TextureBinding> texture_bindings;
	for (unsigned int i = 0; i < view.num_texture_bindings; i++)
	{
		TextureBinding binding;
		binding.flags = view.skin_file.read_uint16();
		binding.render_order = view.skin_file.read_uint16();
		binding.submesh_index = view.skin_file.read_uint16();
		binding.submesh_index2 = view.skin_file.read_uint16();
		binding.color_index = view.skin_file.read_uint16(); // -1 if none
		binding.render_flags_table_index = view.skin_file.read_uint16();
		binding.texture_unit_number = view.skin_file.read_uint16(); // 0 or 1 - index into the texture unit lookup table
		binding.unknown = view.skin_file.read_uint16();
		binding.texture_index = view.skin_file.read_uint16();
		binding.texture_unit_number2 = view.skin_file.read_uint16();
		binding.transparency_index = view.skin_file.read_uint16();
		binding.texture_animation_index = view.skin_file.read_uint16();
		texture_bindings.push_back(binding);
	}

	for (size_t i = 0; i < submeshes.size(); i++)
	{
		for (size_t j = 0; j < texture_bindings.size(); j++)
		{
			if (texture_bindings[j].submesh_index == i)
			{
				ModelDataMaterialRange range;
				range.start_element = submeshes[i].start_element;
				range.num_elements = submeshes[i].num_elements;
				range.ambient = Vec3f(1.0f);
				range.diffuse = Vec3f(1.0f);
				range.two_sided = (render_flags[texture_bindings[j].render_flags_table_index].flags & 0x04) == 0;
				range.diffuse_map.channel = 0;
				range.diffuse_map.texture = texture_index[texture_lookup[texture_bindings[j].texture_index]];
				range.diffuse_map.replaceable_texture = textures[texture_lookup[texture_bindings[j].texture_index]].texture_type != 0;
				data->mesh_lods[0].meshes[0].material_ranges.push_back(range);
			}
		}
	}

	file.seek(offset_bones);
	std::vector<Bone> bones;
	for (unsigned int i = 0; i < num_bones; i++)
	{
		Bone bone;
		bone.animation_id = file.read_int32();
		bone.flags = file.read_uint32();
		bone.parent_bone = file.read_int16();
		bone.geo_id = file.read_uint16();
		bone.unknown = file.read_uint32();
		read_animation_block(file, version, bone.position);
		read_animation_block(file, version, bone.orientation);
		read_animation_block(file, version, bone.scale);
		bone.pivot = Vec3f(file.read_float(), file.read_float(), file.read_float());
		bones.push_back(bone);
	}
	for (unsigned int i = 0; i < num_bones; i++)
	{
		for (size_t k = 0; k < bones[i].position.timelines.size(); k++)
		{
			file.seek(bones[i].position.timelines[k].offset_values);
			for (unsigned j = 0; j < bones[i].position.timelines[k].num_values; j++)
				bones[i].position.timelines[k].values.push_back(Vec3f(file.read_float(), file.read_float(), file.read_float()));
		}

		for (size_t k = 0; k < bones[i].orientation.timelines.size(); k++)
		{
			file.seek(bones[i].orientation.timelines[k].offset_values);
			for (unsigned j = 0; j < bones[i].orientation.timelines[k].num_values; j++)
			{
				// Bug: is this the right order? Quaternionf constructor uses wxyz order, but this code seem to think it uses xyzw order
				Vec4us value(file.read_uint16(), file.read_uint16(), file.read_uint16(), file.read_uint16());
				Vec4f valuef = Vec4f(value.x, value.y, value.z, value.w) / 32767.0f - 1.0f;
				bones[i].orientation.timelines[k].values.push_back(Quaternionf(valuef.x, valuef.y, valuef.z, valuef.w));
			}
		}

		for (size_t k = 0; k < bones[i].scale.timelines.size(); k++)
		{
			file.seek(bones[i].scale.timelines[k].offset_values);
			for (unsigned j = 0; j < bones[i].scale.timelines[k].num_values; j++)
				bones[i].scale.timelines[k].values.push_back(Vec3f(file.read_float(), file.read_float(), file.read_float()));
		}
	}

	for (unsigned int i = 0; i < num_bones; i++)
	{
		ModelDataBone model_bone;
		model_bone.parent_bone = bones[i].parent_bone;
		model_bone.billboarded = (bones[i].flags & Bone::billboarded) != 0;
		model_bone.pivot = bones[i].pivot;

		if (bones[i].flags & Bone::transformed)
		{
			for (size_t k = 0; k < bones[i].position.timelines.size(); k++)
			{
				ModelDataAnimationTimeline<Vec3f> timeline;
				for (unsigned j = 0; j < bones[i].position.timelines[k].timestamps.size(); j++)
					timeline.timestamps.push_back((float)bones[i].position.timelines[k].timestamps[j]);
				for (unsigned j = 0; j < bones[i].position.timelines[k].values.size(); j++)
					timeline.values.push_back(bones[i].position.timelines[k].values[j]);
				model_bone.position.timelines.push_back(timeline);
			}

			for (size_t k = 0; k < bones[i].orientation.timelines.size(); k++)
			{
				ModelDataAnimationTimeline<Quaternionf> timeline;
				for (unsigned j = 0; j < bones[i].orientation.timelines[k].timestamps.size(); j++)
					timeline.timestamps.push_back((float)bones[i].orientation.timelines[k].timestamps[j]);
				for (unsigned j = 0; j < bones[i].orientation.timelines[k].values.size(); j++)
					timeline.values.push_back(bones[i].orientation.timelines[k].values[j]);
				model_bone.orientation.timelines.push_back(timeline);
			}

			for (size_t k = 0; k < bones[i].scale.timelines.size(); k++)
			{
				ModelDataAnimationTimeline<Vec3f> timeline;
				for (unsigned j = 0; j < bones[i].scale.timelines[k].timestamps.size(); j++)
					timeline.timestamps.push_back((float)bones[i].scale.timelines[k].timestamps[j]);
				for (unsigned j = 0; j < bones[i].scale.timelines[k].values.size(); j++)
					timeline.values.push_back(bones[i].scale.timelines[k].values[j]);
				model_bone.scale.timelines.push_back(timeline);
			}
		}

		data->bones.push_back(model_bone);
	}

	file.seek(offset_global_sequences);
	std::vector<unsigned int> global_sequences;
	for (unsigned int i = 0; i < num_global_sequences; i++)
		global_sequences.push_back(file.read_uint32());

	data->bones.resize(num_bones);
	file.seek(offset_animations);
	for (unsigned int i = 0; i < num_animations; i++)
	{
		unsigned short animation_id = file.read_uint16();
		unsigned short subanimation_id = file.read_uint16();
		unsigned int start_timestamp = version < 264 ? file.read_uint32() : 0;
		unsigned int end_timestamp = file.read_uint32();
		float moving_speed = file.read_float();
		unsigned int playback_count = file.read_uint32(); // 0 = loop
		unsigned int rarity = file.read_uint16();
		unsigned int unknown0 = file.read_uint16();
		unsigned int unknown1 = file.read_uint32();
		unsigned int unknown2 = file.read_uint32();
		unsigned int play_speed = file.read_uint32();
		Vec3f aabb_min(file.read_float(), file.read_float(), file.read_float());
		Vec3f aabb_max(file.read_float(), file.read_float(), file.read_float());
		float radius = file.read_float();
		short next_sub_animation_index = file.read_int16(); // -1 if none, points at next animation with same animation ID
		unsigned short index = file.read_uint16();

		ModelDataAnimation animation;
		animation.name = StringHelp::uint_to_text(animation_id);
		animation.playback_speed = (float)play_speed;
		animation.moving_speed = moving_speed;
		animation.loop = (playback_count == 0);
		animation.rarity = rarity;
		animation.length = (float)(end_timestamp - start_timestamp);
		data->animations.push_back(animation);
	}

	if (!data->mesh_lods.empty() && !data->mesh_lods[0].meshes.empty() && !data->mesh_lods[0].meshes[0].vertices.empty())
	{
		data->aabb_min = data->mesh_lods[0].meshes[0].vertices[0];
		data->aabb_max = data->mesh_lods[0].meshes[0].vertices[0];
		for (size_t i = 1; i < data->mesh_lods[0].meshes[0].vertices.size(); i++)
		{
			data->aabb_min.x = min(data->aabb_min.x, data->mesh_lods[0].meshes[0].vertices[i].x);
			data->aabb_min.y = min(data->aabb_min.y, data->mesh_lods[0].meshes[0].vertices[i].y);
			data->aabb_min.z = min(data->aabb_min.z, data->mesh_lods[0].meshes[0].vertices[i].z);
			data->aabb_max.x = max(data->aabb_max.x, data->mesh_lods[0].meshes[0].vertices[i].x);
			data->aabb_max.y = max(data->aabb_max.y, data->mesh_lods[0].meshes[0].vertices[i].y);
			data->aabb_max.z = max(data->aabb_max.z, data->mesh_lods[0].meshes[0].vertices[i].z);
		}
	}

	file.close();
}

Mat4f M2ModelLoader::get_bone_transform(int animation_index, const std::vector<Bone> &bones, int bone, unsigned int timestamp)
{
	Mat4f transform = Mat4f::identity();

	if (bones[bone].parent_bone != -1)
		transform = get_bone_transform(animation_index, bones, bones[bone].parent_bone, timestamp);

	if (bones[bone].flags & Bone::transformed)
	{
		if (!bones[bone].position.timelines.empty())
		{
			int timeline_index = min(animation_index, (int)bones[bone].position.timelines.size() - 1);
			if (bones[bone].position.timelines[timeline_index].num_values > 0)
			{
				size_t index, index2;
				float t = find_animation_indices(bones[bone].position.timelines[timeline_index], timestamp, index, index2);
				Vec3f pos = bones[bone].position.timelines[timeline_index].values[index] * (1.0f - t) + bones[bone].position.timelines[timeline_index].values[index2] * t;
				transform = transform * Mat4f::translate(pos);
			}
		}

		if (!bones[bone].orientation.timelines.empty())
		{
			int timeline_index = min(animation_index, (int)bones[bone].orientation.timelines.size() - 1);
			if (bones[bone].orientation.timelines[timeline_index].num_values > 0)
			{
				size_t index, index2;
				float t = find_animation_indices(bones[bone].orientation.timelines[timeline_index], timestamp, index, index2);
				transform = transform * Quaternionf::lerp(bones[bone].orientation.timelines[timeline_index].values[index],  bones[bone].orientation.timelines[timeline_index].values[index2], t).to_matrix();
			}
		}

		if (!bones[bone].scale.timelines.empty())
		{
			int timeline_index = min(animation_index, (int)bones[bone].scale.timelines.size() - 1);
			if (bones[bone].scale.timelines[timeline_index].num_values > 0)
			{
				size_t index, index2;
				float t = find_animation_indices(bones[bone].scale.timelines[timeline_index], timestamp, index, index2);
				Vec3f scale = bones[bone].scale.timelines[timeline_index].values[index] * (1.0f - t) + bones[bone].scale.timelines[timeline_index].values[index2] * t;
				transform = transform * Mat4f::scale(scale);
			}
		}
	}

	return transform;
}

template<typename ValueType>
float M2ModelLoader::find_animation_indices(const AnimationTimeline<ValueType> &timeline, unsigned int timestamp, size_t &index, size_t &index2)
{
	if (timeline.timestamps.empty())
	{
		index = 0;
		index2 = 0;
		return 0.0f;
	}

	index = 0;
	while (index + 1 < timeline.timestamps.size() && timeline.timestamps[index + 1] <= timestamp)
		index++;

	index2 = index + 1;
	if (index2 == timeline.timestamps.size())
		index2 = index;

	unsigned int start = timeline.timestamps[index];
	unsigned int end = timeline.timestamps[index2];
	if (start != end)
		return (timestamp - start) / (float)(end - start);
	else
		return 0.0f;
}

template<typename ValueType>
void M2ModelLoader::read_animation_block(IODevice &file, int version, AnimationBlock<ValueType> &block)
{
	block.interpolation_type = file.read_uint16();
	block.global_sequence_id = file.read_int16();
	if (version < 264)
	{
		unsigned int num_ranges = file.read_uint32();
		unsigned int offset_ranges = file.read_uint32();
		unsigned int num_timestamps = file.read_uint32();
		unsigned int offset_timestamps = file.read_uint32();
		unsigned int num_values = file.read_uint32();
		unsigned int offset_values = file.read_uint32();
		int cur_pos = file.get_position();

		AnimationTimeline<ValueType> timeline;
		timeline.num_values = num_values;
		timeline.offset_values = offset_values;

		if (num_ranges > 0)
		{
			file.seek(offset_ranges);
			for (unsigned int j = 0; j < num_ranges; j++)
				timeline.ranges.push_back(std::pair<int,int>(file.read_uint32(), file.read_uint32()));
		}
		if (num_timestamps > 0)
		{
			file.seek(offset_timestamps);
			for (unsigned int j = 0; j < num_timestamps; j++)
				timeline.timestamps.push_back(file.read_uint32());
		}

		block.timelines.push_back(timeline);

		file.seek(cur_pos);
	}
	else
	{
		unsigned int num_timestamp_timelines = file.read_uint32();
		unsigned int offset_timestamp_timelines = file.read_uint32();
		unsigned int num_value_timelines = file.read_uint32();
		unsigned int offset_value_timelines = file.read_uint32();

		int cur_pos = file.get_position();

		std::vector<int> timestamp_timelines;
		if (num_timestamp_timelines > 0)
		{
			file.seek(offset_timestamp_timelines);
			for (unsigned int j = 0; j < num_timestamp_timelines * 2; j++)
				timestamp_timelines.push_back(file.read_uint32());
		}

		std::vector<int> value_timelines;
		if (num_value_timelines > 0)
		{
			file.seek(offset_value_timelines);
			for (unsigned int j = 0; j < num_value_timelines * 2; j++)
				value_timelines.push_back(file.read_uint32());
		}

		if (num_timestamp_timelines != num_value_timelines)
			throw Exception("num_timestamp_timelines != num_value_timelines");
		block.timelines.resize(num_value_timelines);
		for (size_t i = 0; i < block.timelines.size(); i++)
		{
			unsigned int num_timestamps = timestamp_timelines[i * 2];
			unsigned int offset_timestamps = timestamp_timelines[i * 2 + 1];

			unsigned int num_values = value_timelines[i * 2];
			unsigned int offset_values = value_timelines[i * 2 + 1];

			if (num_timestamps > 0)
			{
				file.seek(offset_timestamps);
				for (unsigned int j = 0; j < num_timestamps; j++)
					block.timelines[i].timestamps.push_back(file.read_uint32());
			}

			block.timelines[i].num_values = num_values;
			block.timelines[i].offset_values = offset_values;
		}

		file.seek(cur_pos);
	}
}

}

