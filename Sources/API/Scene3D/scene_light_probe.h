/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "api_scene3d.h"
#include <memory>

namespace clan
{
/// \addtogroup clanScene_Scene clanScene Scene
/// \{

class Scene;
class SceneLightProbe_Impl;

class CL_API_SCENE SceneLightProbe
{
public:
	SceneLightProbe();
	SceneLightProbe(Scene &scene);
	bool is_null() const { return !impl; }

	Vec3f get_position() const;
	float get_radius() const;
	Vec3f get_color() const;

	void set_position(const Vec3f &position);
	void set_radius(float radius);
	void set_color(const Vec3f &color);

private:
	std::shared_ptr<SceneLightProbe_Impl> impl;
};

}

/// \}
