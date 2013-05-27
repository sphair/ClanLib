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
#include "quad_tree.h"

namespace clan
{

QuadTree::QuadTree()
: root(0), frame(0)
{
}

QuadTree::~QuadTree()
{
	delete root;
}

SceneCullProxy *QuadTree::create_proxy(SceneItem *item, const AxisAlignedBoundingBox &aabb)
{
	return 0;
}

void QuadTree::delete_proxy(SceneCullProxy *proxy)
{
}

void QuadTree::set_aabb(SceneCullProxy *proxy, const AxisAlignedBoundingBox &aabb)
{
}

AxisAlignedBoundingBox QuadTree::get_aabb(SceneCullProxy *proxy)
{
	return AxisAlignedBoundingBox();
}

std::vector<SceneItem *> QuadTree::cull(const FrustumPlanes &frustum)
{
	frame++;
	std::vector<SceneItem *> pvs;
	root->cull(frame, frustum, aabb, pvs);
	return pvs;
}

std::vector<SceneItem *> QuadTree::cull(const Vec3f &point)
{
	std::vector<SceneItem *> pvs;
	return pvs;
}

}

