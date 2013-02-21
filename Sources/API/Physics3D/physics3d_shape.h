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

/// \addtogroup clanPhysics3D_World clanPhysics3D World
/// \{

#pragma once

#include "api_physics3d.h"
#include "../Core/Math/vec3.h"
#include "../Core/Math/mat4.h"
#include <string>
#include <memory>

namespace clan
{

class ModelData;
class TerrainData;
class Physics3DWorld;
class Physics3DShape_Impl;

class CL_API_PHYSICS3D Physics3DShape
{
public:
	Physics3DShape();

	bool is_null() const;

	static Physics3DShape box(const Vec3f &size);
	static Physics3DShape capsule(float radius, float height);
	static Physics3DShape sphere(float radius);
	static Physics3DShape terrain(const std::shared_ptr<TerrainData> &terrain_data, Mat4f &out_transform);
	static Physics3DShape model(const std::shared_ptr<ModelData> &model_data);
	static Physics3DShape scale_model(const Physics3DShape &base_model, Vec3f scale);
	static Physics3DShape gimpact_model(const std::shared_ptr<ModelData> &model_data);

private:
	std::shared_ptr<Physics3DShape_Impl> impl;

	friend class Physics3DObject;
};

}

/// \}
