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

class Scene;
class SceneLight_Impl;
class OctTreeObject;
class AxisAlignedBoundingBox;
class VSMShadowMapPassLightData;

class CL_API_SCENE SceneLight
{
public:
	SceneLight();
	SceneLight(Scene *scene);
	bool is_null() const { return !impl; }

	enum Type
	{
		type_omni,
		type_directional,
		type_spot,
		type_sun // directional light with parallel split shadow maps
	};

	Type get_type() const;
	Vec3f get_position() const;
	Quaternionf get_orientation() const;

	Vec3f get_color() const;
	float get_falloff() const;
	float get_hotspot() const;
	float get_attenuation_start() const;
	float get_attenuation_end() const;
	bool is_rectangle_shape() const;
	float get_aspect_ratio() const;

	bool is_shadow_caster() const;
	bool is_light_caster() const;
	SceneLight get_shadow_source() const;

	void set_type(Type type);
	void set_position(const Vec3f &position);
	void set_orientation(const Quaternionf &orientation);

	void set_color(Vec3f color);
	void set_falloff(float falloff);
	void set_hotspot(float hotspot);
	void set_attenuation_start(float attenuation_start);
	void set_attenuation_end(float attenuation_end);
	void set_rectangle_shape(bool rectangle);
	void set_aspect_ratio(float aspect);

	void set_shadow_caster(bool casts_shadows);
	void set_light_caster(bool casts_light);
	void set_shadow_source(SceneLight light);

private:
	std::shared_ptr<SceneLight_Impl> impl;
};

}

/// \}
