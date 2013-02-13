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
#include <memory>

namespace clan
{

class Scene_Impl;

class CL_API_SCENE Scene
{
public:
	Scene();
	Scene(GraphicContext &gc);

	bool is_null() const;

	void set_viewport(const Rect &box);
	void set_camera(const Vec3f &position, const Quaternionf &orientation);
	void set_camera_position(const Vec3f &position);
	void set_camera_orientation(const Quaternionf &orientation);
	void set_camera_field_of_view(float fov);

	void render(GraphicContext &gc);

	void update(GraphicContext &gc, float time_elapsed);

private:
	std::shared_ptr<Scene_Impl> impl;
};

}

/// \}
