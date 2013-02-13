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
#include "NGPCore/p3dhli.h"
namespace clan
{

class NGPlantLoader
{
public:
	static std::shared_ptr<ModelData> load(const std::string &filename);
	static std::shared_ptr<ModelData> load(const P3DPlantModel *plant_model, const std::string &base_path);

private:
	struct Vertex
	{
		clan::Vec3f pos;
		clan::Vec3f normal;
		clan::Vec3f texcoord0;
		clan::Vec3f tangent;
		clan::Vec3f binormal;
		clan::Vec3f billboard_pos;
	};

	struct Material
	{
		enum BillboardType
		{
			type_none,
			type_spherical,
			type_cylindrical
		};

		Material() : billboard_mode(type_none) { }

		BillboardType billboard_mode;
		std::string filename;

		bool is_two_sided;
	};

	NGPlantLoader(const P3DPlantModel *plant_model, const std::string &base_path);
	Material load_material(P3DHLIPlantTemplate &plant_template, int group_index);

	std::string base_path;
	std::shared_ptr<ModelData> data;
	std::vector<Material> materials;
	std::vector<int> group_lengths;
};

}

