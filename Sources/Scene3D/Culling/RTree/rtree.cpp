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

#include "Scene3D/precomp.h"
#include "rtree.h"

namespace clan
{


RTree::RTree()
: frame(0)
{
}

RTree::~RTree()
{
}

SceneCullProxy *RTree::create_proxy(SceneItem *object, const AxisAlignedBoundingBox &box)
{
	RTreeObject *tree_object = new RTreeObject(object, box);
	float a_min[3] = { box.aabb_min.x, box.aabb_min.y, box.aabb_min.z };
	float a_max[3] = { box.aabb_max.x, box.aabb_max.y, box.aabb_max.z };
	root.Insert(a_min, a_max, tree_object);
	return tree_object;
}

void RTree::delete_proxy(SceneCullProxy *proxy)
{
	RTreeObject *tree_object = static_cast<RTreeObject*>(proxy);

	float a_old_min[3] = { tree_object->box.aabb_min.x, tree_object->box.aabb_min.y, tree_object->box.aabb_min.z };
	float a_old_max[3] = { tree_object->box.aabb_max.x, tree_object->box.aabb_max.y, tree_object->box.aabb_max.z };

	root.Remove(a_old_min, a_old_max, tree_object);
	tree_object->release();
}

void RTree::set_aabb(SceneCullProxy *proxy, const AxisAlignedBoundingBox &box)
{
	RTreeObject *tree_object = static_cast<RTreeObject*>(proxy);

	float a_old_min[3] = { tree_object->box.aabb_min.x, tree_object->box.aabb_min.y, tree_object->box.aabb_min.z };
	float a_old_max[3] = { tree_object->box.aabb_max.x, tree_object->box.aabb_max.y, tree_object->box.aabb_max.z };

	float a_new_min[3] = { box.aabb_min.x, box.aabb_min.y, box.aabb_min.z };
	float a_new_max[3] = { box.aabb_max.x, box.aabb_max.y, box.aabb_max.z };

	root.Remove(a_old_min, a_old_max, tree_object);
	tree_object->box = box;
	root.Insert(a_new_min, a_new_max, tree_object);
}

AxisAlignedBoundingBox RTree::get_aabb(SceneCullProxy *proxy)
{
	RTreeObject *tree_object = static_cast<RTreeObject*>(proxy);
	return tree_object->box;
}

std::vector<SceneItem *> RTree::cull(const FrustumPlanes &frustum)
{
	std::vector<SceneItem *> pvs;
	//root->cull(frame++, frustum, aabb, pvs);
	return pvs;
}

}

