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
#include "oct_tree_node.h"
#include "Scene3D/Culling/intersection_test.h"
#include <algorithm>

namespace clan
{


OctTreeNode::OctTreeNode()
{
	for (int i = 0; i < 8; i++)
		children[i] = 0;
}

OctTreeNode::~OctTreeNode()
{
	for (size_t i = 0; i < objects.size(); i++)
		objects[i]->release();

	for (int i = 0; i < 8; i++)
		delete children[i];
}

void OctTreeNode::insert(OctTreeObject *object, const AxisAlignedBoundingBox &aabb, int iteration)
{
	if (iteration == max_iterations)
	{
		objects.push_back(object);
		object->add_ref();
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			AxisAlignedBoundingBox child_box = child_aabb(i, aabb);
			if (IntersectionTest::aabb(object->box, child_box) == IntersectionTest::overlap)
			{
				if (children[i] == 0)
					children[i] = new OctTreeNode();
				children[i]->insert(object, child_box, iteration + 1);
			}
		}
	}
}

void OctTreeNode::remove(OctTreeObject *object, const AxisAlignedBoundingBox &aabb, int iteration)
{
	if (iteration == max_iterations)
	{
		objects.erase(std::find(objects.begin(), objects.end(), object));
		object->release();
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			AxisAlignedBoundingBox child_box = child_aabb(i, aabb);
			if (IntersectionTest::aabb(object->box, child_box) == IntersectionTest::overlap)
			{
				children[i]->remove(object, child_box, iteration + 1);

				bool empty = children[i]->objects.empty();
				for (int j = 0; j < 8; j++)
					empty = empty && children[i]->children[j] == 0;
				if (empty)
				{
					delete children[i];
					children[i] = 0;
				}
			}
		}
	}
}

void OctTreeNode::cull(int frame, const FrustumPlanes &frustum, const AxisAlignedBoundingBox &aabb, std::vector<SceneItem *> &pvs)
{
	IntersectionTest::Result result = IntersectionTest::frustum_aabb(frustum, aabb);
	if (result == IntersectionTest::inside)
	{
		show(frame, pvs);
	}
	else if (result == IntersectionTest::intersecting)
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (IntersectionTest::aabb(objects[i]->box, aabb) == IntersectionTest::overlap)
				objects[i]->add(frame, pvs);
		}

		for (int i = 0; i < 8; i++)
		{
			if (children[i])
				children[i]->cull(frame, frustum, child_aabb(i, aabb), pvs);
		}
	}
}

void OctTreeNode::show(int frame, std::vector<SceneItem *> &pvs)
{
	for (size_t i = 0; i < objects.size(); i++)
		objects[i]->add(frame, pvs);

	for (int i = 0; i < 8; i++)
	{
		if (children[i])
			children[i]->show(frame, pvs);
	}
}

AxisAlignedBoundingBox OctTreeNode::child_aabb(int index, const AxisAlignedBoundingBox &aabb)
{
	return AxisAlignedBoundingBox(aabb, barycentric_weights[index * 2], barycentric_weights[index * 2 + 1]);
}

const Vec3f OctTreeNode::barycentric_weights[16] =
{
	Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.5f, 0.5f, 0.5f),
	Vec3f(0.5f, 0.0f, 0.0f), Vec3f(1.0f, 0.5f, 0.5f),
	Vec3f(0.0f, 0.0f, 0.5f), Vec3f(0.5f, 0.5f, 1.0f),
	Vec3f(0.5f, 0.0f, 0.5f), Vec3f(1.0f, 0.5f, 1.0f),
	Vec3f(0.0f, 0.5f, 0.0f), Vec3f(0.5f, 1.0f, 0.5f),
	Vec3f(0.5f, 0.5f, 0.0f), Vec3f(1.0f, 1.0f, 0.5f),
	Vec3f(0.0f, 0.5f, 0.5f), Vec3f(0.5f, 1.0f, 1.0f),
	Vec3f(0.5f, 0.5f, 0.5f), Vec3f(1.0f, 1.0f, 1.0f)
};

}

