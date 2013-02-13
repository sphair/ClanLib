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

#include "Scene/Culling/visible_object.h"
#include "Scene/Culling/clipping_frustum.h"
namespace clan
{

class PortalSector;

class PortalClipping
{
public:
	PortalClipping(const ClippingFrustum &frustum, const clan::Mat4f &world_to_projection);
	clan::Rectf intersect(clan::Rectf box, const std::vector<clan::Vec3f> &points) const;

	clan::Mat4f world_to_projection;
	clan::Mat4f projection_to_world;
	ClippingFrustum frustum;

private:
	clan::Rectf project(const std::vector<clan::Vec2f> &points) const;
	void find_edges(const std::vector<clan::Vec3f> &points, std::vector<clan::Vec2f> &out_edges) const;
	void add_triangle_edges(const std::vector<clan::Vec3f> &points, int v0, int v1, int v2, std::vector<clan::Vec2f> &out_edges) const;
};

}

