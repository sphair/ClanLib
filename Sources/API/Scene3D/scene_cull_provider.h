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
#include "../Core/Math/vec3.h"
#include "../Core/Math/mat4.h"
#include <vector>

namespace clan
{
/// \addtogroup clanScene_Scene clanScene Scene
/// \{

class SceneItem
{
public:
	virtual ~SceneItem() { }
};

class SceneCullProxy
{
};

class AxisAlignedBoundingBox;
class FrustumPlanes;

/// \brief Interface for finding the potential visible set (PVS) for a set of objects
class CL_API_SCENE SceneCullProvider
{
public:
	virtual ~SceneCullProvider() { }

	virtual SceneCullProxy *create_proxy(SceneItem *item, const AxisAlignedBoundingBox &aabb) = 0;
	virtual void delete_proxy(SceneCullProxy *proxy) = 0;

	virtual void set_aabb(SceneCullProxy *proxy, const AxisAlignedBoundingBox &aabb) = 0;
	virtual AxisAlignedBoundingBox get_aabb(SceneCullProxy *proxy) = 0;

	virtual std::vector<SceneItem *> cull(const FrustumPlanes &frustum) = 0;
	virtual std::vector<SceneItem *> cull(const Vec3f &point) = 0;
};

}

/// \}
