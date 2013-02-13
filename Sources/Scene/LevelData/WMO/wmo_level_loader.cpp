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
#include "wmo_level_loader.h"
#include "Scene/ModelData/model_data.h"
#include "Scene/Framework/file_search_path.h"

namespace clan
{


std::shared_ptr<LevelData> WMOLevelLoader::load(const std::string &filename, const FileSearchPath &search_path)
{
	std::shared_ptr<LevelData> portal_map(new LevelData());
	WMOLevelLoader self(portal_map.get(), filename, search_path);
	return portal_map;
}

WMOLevelLoader::WMOLevelLoader(LevelData *data, const std::string &filename, const FileSearchPath &search_path)
: data(data)
{
	File file(filename);
	std::string local_path = PathHelp::get_fullpath(filename);

	unsigned int num_materials = 0;
	unsigned int num_groups = 0;
	unsigned int num_portals = 0;
	unsigned int num_lights = 0;
	unsigned int num_models = 0;
	unsigned int num_doodads = 0;
	unsigned int num_sets = 0;

	DataBuffer motx_buffer;
	std::vector<BaseMaterial> base_materials;

	std::vector<Vec3f> portal_points;

	while (true)
	{
		IFFHeader iff = read_iff_header(file);
		if (iff.size == 0)
			break;

		if (iff.name == "MVER")
		{
			unsigned int version = file.read_uint32();
			if (version != 17)
				return;
		}
		else if (iff.name == "MOHD")
		{
			num_materials = file.read_uint32();
			num_groups = file.read_uint32();
			num_portals = file.read_uint32();
			num_lights = file.read_uint32();
			num_models = file.read_uint32();
			num_doodads = file.read_uint32();
			num_sets = file.read_uint32();
		}
		else if (iff.name == "MOTX")
		{
			// zero padded, zero terminated strings of textures used (BLP filenames)
			motx_buffer = DataBuffer(iff.size + 1);
			file.read(motx_buffer.get_data(), motx_buffer.get_size());
			motx_buffer.get_data()[iff.size] = 0;
		}
		else if (iff.name == "MOMT" && !motx_buffer.is_null())
		{
			for (unsigned int i = 0; i < num_materials; i++)
			{
				BaseMaterial material;
				material.flags = file.read_uint32();
				material.specular_mode = file.read_uint32();
				material.blend_mode = file.read_uint32();
				material.offset_texture0 = file.read_uint32();
				material.color_texture0 = file.read_uint32();
				material.flags_texture0 = file.read_uint32();
				material.offset_texture1 = file.read_uint32();
				material.color_texture1 = file.read_uint32();
				material.flags_texture1 = file.read_uint32();
				material.unknown0 = file.read_uint32();
				material.unknown1 = file.read_uint32();
				material.unknown2 = file.read_uint32();
				material.unknown3 = file.read_uint32();
				material.unknown4 = file.read_uint32();
				material.unknown5 = file.read_uint32();
				material.unknown6 = file.read_uint32();
				material.texture0 = motx_buffer.get_data() + min(material.offset_texture0, (unsigned int)motx_buffer.get_size()-1);
				material.texture1 = motx_buffer.get_data() + min(material.offset_texture1, (unsigned int)motx_buffer.get_size()-1);
				base_materials.push_back(material);
			}
		}
		else if (iff.name == "MOGN")
		{
			// null terminated list of group names, "antiportal" supposedly has special meaning
		}
		else if (iff.name == "MOGI")
		{
			for (unsigned int i = 0; i < num_groups; i++)
			{
				unsigned int flags = file.read_uint32();
				Vec3f aabb_min(file.read_float(), file.read_float(), file.read_float());
				Vec3f aabb_max(file.read_float(), file.read_float(), file.read_float());
				unsigned int group_name_index = file.read_uint32();
			}
		}
		else if (iff.name == "MOSB")
		{
			// null terminated string for the skybox model to be used
			// std::string skybox = file.read_string_nul();
		}
		else if (iff.name == "MOPV")
		{
			unsigned int num_portal_vertices = iff.size / (sizeof(float) * 3);
			for (unsigned int i = 0; i < num_portal_vertices; i++)
				portal_points.push_back(Vec3f(file.read_float(), file.read_float(), file.read_float()));
		}
		else if (iff.name == "MOPT")
		{
			for (unsigned int i = 0; i < num_portals; i++)
			{
				unsigned int start_vertex = file.read_uint32();
				unsigned int num_vertices = file.read_uint32();
				Vec3f normal(file.read_float(), file.read_float(), file.read_float());
				unsigned int unknown = file.read_uint32();
			}
		}
		else if (iff.name == "MOPR")
		{
			for (unsigned int i = 0; i < num_portals * 2; i++)
			{
				unsigned short portal_index = file.read_uint16();
				unsigned short group_index = file.read_uint16();
				int side = file.read_int16(); // 1 or -1
				unsigned short unknown = file.read_uint16();
			}
		}
		else if (iff.name == "MODS")
		{
			for (unsigned int i = 0; i < num_sets; i++)
			{
				char name[21];
				name[20] = 0;
				file.read(name, 20);
				unsigned int modd_start_index = file.read_uint32();
				unsigned int num_doodads = file.read_uint32();
				unsigned int unknown = file.read_uint32();
			}
		}
		else if (iff.name == "MODN")
		{
			// zero padded, zero terminated, filenames for .mdx models
		}
		else if (iff.name == "MODD")
		{
			unsigned int num_instances = iff.size / 40;
			for (unsigned int i = 0; i < num_instances; i++)
			{
				unsigned int modn_filename_offset = file.read_uint32();
				Vec3f position(file.read_float(), file.read_float(), file.read_float());
				Quaternionf orientation(file.read_float(), file.read_float(), file.read_float(), file.read_float());
				float scale = file.read_float();
				unsigned int color = file.read_uint32();
			}
		}

		if (!file.seek(iff.next_position))
			break;
	}
	file.close();

#ifdef SECTOR_MESH_LOADING_CODE
	for (unsigned int group = 0; group < num_groups; group++)
	{
		std::string postfix;
		if (group < 10)
			postfix = string_format("_00%1.wmo", group);
		else if (group < 100)
			postfix = string_format("_0%1.wmo", group);
		else
			postfix = string_format("_%1.wmo", group);
		std::string group_filename = PathHelp::combine(PathHelp::get_fullpath(filename), PathHelp::get_basename(filename) + postfix);
		File file(group_filename);

		PortalSector sector;
		sector.model = std::shared_ptr<ModelData>(new ModelData());
		sector.collision_model = std::shared_ptr<ModelData>(new ModelData());

		bool is_outdoors = false;
		while (true)
		{
			IFFHeader iff = read_iff_header(file);
			if (iff.size == 0)
				break;

			if (iff.name == "MVER")
			{
				unsigned int version = file.read_uint32();
				if (version != 17)
					return;
			}
			else if (iff.name == "MOGP")
			{
				unsigned int mogn_group_name_offset = file.read_uint32();
				unsigned int mogn_group_description_offset = file.read_uint32();
				unsigned int flags = file.read_uint32();
				Vec3f aabb_min(file.read_float(), file.read_float(), file.read_float());
				Vec3f aabb_max(file.read_float(), file.read_float(), file.read_float());
				unsigned short mopr_portal_start = file.read_uint16();
				unsigned short mopr_portal_count = file.read_uint16();
				unsigned short unknown0 = file.read_uint16();
				unsigned short unknown1 = file.read_uint16();
				unsigned int batch_count = file.read_uint32();
				unsigned int liquid_type = file.read_uint32();
				unsigned int wmo_group_id = file.read_uint32();
				unsigned int unknown2 = file.read_uint32();
				unsigned int unknown3 = file.read_uint32();
				unsigned int unknown4 = file.read_uint32();
				sector.model->aabb_min = Vec4f(aabb_min, 1.0f);
				sector.model->aabb_max = Vec4f(aabb_max, 1.0f);
				sector.collision_model->aabb_min = Vec4f(aabb_min, 1.0f);
				sector.collision_model->aabb_max = Vec4f(aabb_max, 1.0f);

				is_outdoors = (flags & 0x8) != 0;

				continue; // bit of a hack. All remaining chunks are subchunks and this statement allows us to ignore that.
			}
			else if (iff.name == "MOPY")
			{
				unsigned int num_material_references = iff.size / 2;
				int start_material_index = -1;
				int start_element = 0;

				for (unsigned int i = 0; i < num_material_references; i++)
				{
					unsigned char flags = file.read_uint8();
					unsigned char material_index = file.read_uint8();

					if (start_material_index != -1 && material_index != start_material_index)
					{
						if (start_material_index != 255) // material 255 is their collision mesh
						{
							ModelData::Material material;
							material.shininess = 120.0f;
							material.surface_color = Vec3f(1.0f, 1.0f, 1.0f);
							try
							{
								material.texture_filename = search_path.search(local_path, base_materials[start_material_index].texture0);
							}
							catch (Exception &)
							{
							}
							material.two_sided = ((base_materials[start_material_index].flags_texture0 & 0x04) == 0);
							sector.model->materials.push_back(material);

							ModelData::MeshObject *mesh = new ModelData::MeshObject();
							mesh->material_index = sector.model->materials.size() - 1;
							mesh->indices_index = start_element * 3;
							mesh->indices_count = (i - start_element) * 3;
							sector.model->meshes.push_back(mesh);
						}
						else
						{
							ModelData::MeshObject *mesh = new ModelData::MeshObject();
							mesh->material_index = 0;
							mesh->indices_index = start_element * 3;
							mesh->indices_count = (i - start_element) * 3;
							sector.collision_model->meshes.push_back(mesh);
						}
						start_element = i;
					}
					start_material_index = material_index;
				}

				if (start_material_index != -1 && start_material_index != 255)
				{
					ModelData::Material material;
					material.shininess = 120.0f;
					material.surface_color = Vec3f(1.0f, 1.0f, 1.0f);
					try
					{
						material.texture_filename = search_path.search(local_path, base_materials[start_material_index].texture0);
					}
					catch (Exception &)
					{
					}
					material.two_sided = ((base_materials[start_material_index].flags_texture0 & 0x04) == 0);
					sector.model->materials.push_back(material);

					ModelData::MeshObject *mesh = new ModelData::MeshObject();
					mesh->material_index = sector.model->materials.size() - 1;
					mesh->indices_index = (start_element * 3);
					mesh->indices_count = (num_material_references - start_element) * 3;
					sector.model->meshes.push_back(mesh);
				}
				else
				{
					ModelData::MeshObject *mesh = new ModelData::MeshObject();
					mesh->material_index = 0;
					mesh->indices_index = (start_element * 3);
					mesh->indices_count = (num_material_references - start_element) * 3;
					sector.collision_model->meshes.push_back(mesh);
				}
			}
			else if (iff.name == "MOVI")
			{
				unsigned int num_elements = iff.size / 2;
				std::vector<unsigned short> elements;
				elements.resize(num_elements);
				if (num_elements > 0)
					file.read(&elements[0], sizeof(unsigned short) * num_elements);
				for (size_t i = 0; i + 2 < elements.size(); i += 3)
				{
					unsigned short tmp = elements[i];
					elements[i] = elements[i + 2];
					elements[i + 2] = tmp;
				}
				sector.model->indices.insert(sector.model->indices.end(), elements.begin(), elements.end());
			}
			else if (iff.name == "MOVT")
			{
				unsigned int num_vertices = iff.size / (sizeof(float) * 3);
				sector.model->vertices.resize(num_vertices);
				for (unsigned int i = 0; i < num_vertices; i++)
				{
					Vec3f position(file.read_float(), file.read_float(), file.read_float());
					sector.model->vertices[i].position = position;
					sector.model->vertices[i].bone_weight = Vec4f(1.0f, 0.0f, 0.0f, 0.0f);
				}
			}
			else if (iff.name == "MONR")
			{
				unsigned int num_normals = iff.size / (sizeof(float) * 3);
				sector.model->vertices.resize(num_normals);
				for (unsigned int i = 0; i < num_normals; i++)
					sector.model->vertices[i].normal = Vec3f(file.read_float(), file.read_float(), file.read_float());
			}
			else if (iff.name == "MOTV")
			{
				unsigned int num_texcoords = iff.size / (sizeof(float) * 2);
				sector.model->vertices.resize(max(sector.model->vertices.size(), num_texcoords));
				for (unsigned int i = 0; i < num_texcoords; i++)
				{
					Vec2f texcoord(file.read_float(), file.read_float());
					sector.model->vertices[i].texcoord0 = Vec2f(texcoord.y, texcoord.x);
				}
			}
			else if (iff.name == "MOBA")
			{
				/*
				unsigned int num_batches = iff.size / 24;
				std::vector<int> tmp;
				for (unsigned int i = 0; i < num_batches; i++)
				{
					unsigned int unknown0 = file.read_uint32();
					unsigned int unknown1 = file.read_uint32();
					unsigned int unknown2 = file.read_uint32();
					unsigned int start_element = file.read_uint32();
					unsigned short num_elements = file.read_uint16();
					unsigned short start_vertex = file.read_uint16();
					unsigned short end_vertex = file.read_uint16();
					unsigned char unknown3 = file.read_uint8();
					unsigned char mopy_index = file.read_uint8();

					ModelData::MeshObject *mesh = new ModelData::MeshObject();
					mesh->material_index = mopy_index;
					mesh->indices_index = start_element;
					mesh->indices_count = num_elements;
					sector.model->meshes.push_back(mesh);
				}
				*/
			}
			else if (iff.name == "MODR")
			{
				unsigned int num_doodad_references = iff.size / 2;
				std::vector<unsigned short> modd_references;
				modd_references.resize(num_doodad_references);
				if (num_doodad_references > 0)
					file.read(&modd_references[0], sizeof(unsigned short) * num_doodad_references);
			}

			if (!file.seek(iff.next_position))
				break;
		}
		file.close();

		/*if (group + 1 == num_groups) // Small hack to render portals:
		{
			ModelData::MeshObject *mesh = new ModelData::MeshObject();
			mesh->material_index = sector.model->materials.size();
			mesh->indices_index = sector.model->indices.size();
			mesh->indices_count = portal_points.size() / 4 * 6;
			sector.model->meshes.push_back(mesh);
			ModelData::Material m;
			m.two_sided = true;
			m.surface_color = Vec4f(0.5f, 0.5f, 0.0f, 1.0f);
			m.shininess = 125.0f;
			sector.model->materials.push_back(m);
			for (size_t i = 0; i + 3 < portal_points.size(); i += 4)
			{
				sector.model->indices.push_back(sector.model->vertices.size());
				sector.model->indices.push_back(sector.model->vertices.size()+1);
				sector.model->indices.push_back(sector.model->vertices.size()+2);
				sector.model->indices.push_back(sector.model->vertices.size()+0);
				sector.model->indices.push_back(sector.model->vertices.size()+2);
				sector.model->indices.push_back(sector.model->vertices.size()+3);
				Vec3f n = Vec3f::cross(portal_points[i+1] - portal_points[i], portal_points[i+2] - portal_points[i]);
				n.normalize();
				ModelData::Vertex v;
				v.normal = n;
				v.bone_weight = Vec4f(1.0f, 0.0f, 0.0f, 0.0f);
				v.position = portal_points[i];
				sector.model->vertices.push_back(v);
				v.position = portal_points[i+1];
				sector.model->vertices.push_back(v);
				v.position = portal_points[i+2];
				sector.model->vertices.push_back(v);
				v.position = portal_points[i+3];
				sector.model->vertices.push_back(v);
			}
		}*/

		ModelData::Bone bone;
		ModelData::BoneAnimationStep step;
		step.transform = Mat4f::identity();
		bone.steps.push_back(step);
		sector.model->bones.push_back(bone);

		ModelData::Animation animation;
		sector.model->animations.push_back(animation);

		/*if (!sector.collision_model->meshes.empty())
		{
			sector.collision_model->vertices = sector.model->vertices;
			for (size_t i = 0; i < sector.collision_model->meshes.size(); i++)
			{
				unsigned int start = sector.collision_model->meshes[i]->indices_index;
				for (unsigned int j = 0; j < sector.collision_model->meshes[i]->indices_count; j++)
				{
					sector.collision_model->indices.push_back(sector.model->indices[start + j]);
				}
			}
			sector.collision_model->meshes.clear();
		}*/
		sector.collision_model->meshes.clear();
		sector.collision_model->vertices = sector.model->vertices;
		sector.collision_model->indices = sector.model->indices;

		if (is_outdoors)
			data->sectors.push_back(sector);
	}
#endif
}

WMOLevelLoader::IFFHeader WMOLevelLoader::read_iff_header(File &file)
{
	char iff_name_reverse[4];
	int received = file.read(iff_name_reverse, 4, false);
	if (received != 4)
		return IFFHeader();
	unsigned int iff_size = file.read_uint32();
	unsigned int next_position = file.get_position() + iff_size;
	std::string iff_name;
	for (int i = 0; i < 4; i++)
		iff_name.push_back(iff_name_reverse[3-i]);
	return IFFHeader(iff_name, iff_size, next_position);
}

}

