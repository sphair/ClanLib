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

/// \addtogroup clanScene_Scene clanScene Scene
/// \{

#pragma once

#include "api_scene3d.h"
#include "../Core/Math/vec3.h"
#include "../Core/Math/quaternion.h"
#include <memory>

namespace clan
{

class Scene;
class SceneCamera_Impl;

class CL_API_SCENE SceneCamera
{
public:
	SceneCamera();
	SceneCamera(Scene &scene);

	bool is_null() const;

	Vec3f get_position() const;
	Quaternionf get_orientation() const;

	float get_field_of_view() const;

	void set_position(const Vec3f &position);
	void set_orientation(const Quaternionf &orientation);

	void set_field_of_view(float fov);

private:
	std::shared_ptr<SceneCamera_Impl> impl;

	friend class Scene;
	friend class Scene_Impl;
};

}

/// \}
