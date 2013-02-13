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

#pragma once

#include "Scene3D/Culling/aabb.h"
#include "Scene3D/Culling/visible_object.h"
#include "Scene3D/Culling/clipping_frustum.h"
namespace clan
{

class OctTreeObject
{
public:
	OctTreeObject(VisibleObject *visible_object, const AxisAlignedBoundingBox &box) : ref_count(1), visible_object(visible_object), box(box) { }
	void add_ref() { ref_count++; }
	void release() { if (--ref_count == 0) delete this; }

	int ref_count;
	VisibleObject *visible_object;
	AxisAlignedBoundingBox box;
};

class OctTreeNode
{
public:
	OctTreeNode();
	~OctTreeNode();
	void insert(OctTreeObject *object, const AxisAlignedBoundingBox &aabb, int iteration = 0);
	void remove(OctTreeObject *object, const AxisAlignedBoundingBox &aabb, int iteration = 0);
	void cull(int frame, ClippingFrustum &frustum, const AxisAlignedBoundingBox &aabb, std::vector<VisibleObject *> &pvs);
	void show(int frame, std::vector<VisibleObject *> &pvs);

private:
	static AxisAlignedBoundingBox child_aabb(int index, const AxisAlignedBoundingBox &aabb);

	std::vector<OctTreeObject *> objects;
	OctTreeNode *children[8];
	static const Vec3f barycentric_weights[16];
	static const int max_iterations = 3;
};

}

