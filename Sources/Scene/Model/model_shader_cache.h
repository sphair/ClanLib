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

#include "model_shader_description.h"
#include "Scene/ModelData/model_data.h"
namespace clan
{

class Model;

class ModelShaderCache
{
public:
	void create_gbuffer_commands(clan::GraphicContext &gc, Model *model, int level);
	void create_transparency_commands(clan::GraphicContext &gc, Model *model, int level);
	void create_shadow_commands(clan::GraphicContext &gc, Model *model, int level);

private:
	struct Shaderset
	{
		clan::ProgramObject gbuffer;
		clan::ProgramObject transparency;
	};

	Shaderset get_shader(clan::GraphicContext &gc, const ModelShaderDescription &description);
	clan::ProgramObject create_gbuffer_program(clan::GraphicContext &gc, const ModelShaderDescription &description);
	clan::ProgramObject create_transparency_program(clan::GraphicContext &gc, const ModelShaderDescription &description);
	clan::ProgramObject get_shadow_program(clan::GraphicContext &gc, bool uses_bones);
	void create_states(clan::GraphicContext &gc);
	static clan::TextureWrapMode to_wrap_mode(ModelDataTextureMap::WrapMode mode);

	std::string base_path;
	std::map<ModelShaderDescription, Shaderset> shaders;
	clan::ProgramObject shadow_program, shadow_bones_program;
	clan::RasterizerState rasterizer_state;
	clan::RasterizerState two_sided_rasterizer_state;
};

}

