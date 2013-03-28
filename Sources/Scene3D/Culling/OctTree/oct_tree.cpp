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

#include "Scene3D/precomp.h"
#include "oct_tree.h"

namespace clan
{


OctTree::OctTree()
: aabb(Vec3f(-300.0f), Vec3f(300.0f)), root(new OctTreeNode())
{
}

OctTree::~OctTree()
{
	delete root;
}

OctTreeObject *OctTree::add_object(VisibleObject *object, const AxisAlignedBoundingBox &box)
{
	OctTreeObject *tree_object = new OctTreeObject(object, box);
	root->insert(tree_object, aabb);
	return tree_object;
}

void OctTree::move_object(OctTreeObject *tree_object, const AxisAlignedBoundingBox &box)
{
	root->remove(tree_object, aabb);
	tree_object->box = box;
	root->insert(tree_object, aabb);
}

void OctTree::remove_object(OctTreeObject *tree_object)
{
	root->remove(tree_object, aabb);
	tree_object->release();
}

std::vector<VisibleObject *> OctTree::cull(int frame, ClippingFrustum &frustum)
{
	std::vector<VisibleObject *> pvs;
	root->cull(frame, frustum, aabb, pvs);
	return pvs;
}

}

