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

#include "API/Scene3D/scene_cull_provider.h"
#include "API/Core/Math/aabb.h"
#include "rtree_root.h"

namespace clan
{

class AxisAlignedBoundingBox;

class RTreeObject : public SceneCullProxy
{
public:
	RTreeObject(SceneItem *visible_object, const AxisAlignedBoundingBox &box) : ref_count(1), visible_object(visible_object), box(box), rendered_frame(-1) { }
	void add_ref() { ref_count++; }
	void release() { if (--ref_count == 0) delete this; }

	int ref_count;
	SceneItem *visible_object;
	AxisAlignedBoundingBox box;

	int rendered_frame;

	void add(int frame, std::vector<SceneItem *> &pvs)
	{
		if (rendered_frame != frame)
		{
			rendered_frame = frame;
			pvs.push_back(visible_object);
		}
	}
};

class RTree : public SceneCullProvider
{
public:
	RTree();
	~RTree();

	SceneCullProxy *create_proxy(SceneItem *item, const AxisAlignedBoundingBox &aabb);
	void delete_proxy(SceneCullProxy *proxy);

	void set_aabb(SceneCullProxy *proxy, const AxisAlignedBoundingBox &aabb);
	AxisAlignedBoundingBox get_aabb(SceneCullProxy *proxy);

	std::vector<SceneItem *> cull(const FrustumPlanes &frustum);
	std::vector<SceneItem *> cull(const Vec3f &point);

private:
	RTreeRoot<RTreeObject*, float, 3, float> root;
	int frame;
};

}

