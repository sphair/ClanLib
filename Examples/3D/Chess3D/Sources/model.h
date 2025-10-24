/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Mark Page
*/
#pragma once

#include "3D/orientation.h"
#include "3D/position.h"
#include "3D/light_model.h"

class Model
{
public:
	enum TextureTactic
	{
		use_wood,
		use_silver,
		read_from_file
	};

	Model(CL_GraphicContext &gc, const CL_StringRef &filename, TextureTactic texture_tactic);
	~Model();

	void set_scale(float scale) { this->scale = scale; }
	void render(CL_GraphicContext &gc, const LightModel &light_model, const Position &position, const Orientation &orientation);

private:
	void create_shader_programs(CL_GraphicContext &gc);
	void load_model(CL_GraphicContext &gc, const CL_StringRef & filename);
	void setup_light(CL_ProgramObject &program, const Material &material, const LightModel &light_model);
	CL_PixelBuffer load_image(const CL_String &filename);

	CL_ProgramObject program_object_texture;
	CL_ProgramObject program_object_no_texture;

	struct Vertex
	{
		CL_Vec3f position;
		CL_Vec3f normal;
		CL_Vec2f texcoord0;
	};

	struct MaterialRange
	{
		MaterialRange() : start(0), length(0) { }

		int start, length;
		Material material;
		CL_Texture texture;
	};

	std::map<int, MaterialRange> material_ranges;
	std::map<CL_String8, CL_Texture> textures;
	CL_VertexArrayBuffer vertices;
	float scale;

	CL_Texture wood_texture;
};
