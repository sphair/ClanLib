/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Harry Storbacka
**    Magnus Norddahl
**    Kenneth Gangstoe
**    Mark Page
*/

#include "precomp.h"
#include <vector>
#include "../CollisionAPI/outline_circle.h"
#include "../CollisionAPI/contour.h"

using namespace clan;

class Contour_Impl
{
public:
	Contour_Impl() : is_inside_contour(false) {};

	std::vector<Pointf> points;

	bool is_inside_contour;

	std::vector<OutlineCircle> sub_circles;
};

Contour::Contour() : impl(std::make_shared<Contour_Impl>()) {};

Contour::~Contour() {};

std::vector<Pointf> &Contour::get_points() { return impl->points; }
const std::vector<Pointf> &Contour::get_points() const { return impl->points; }

bool Contour::is_inside_contour() const { return impl->is_inside_contour; }

void Contour::set_inside_contour(bool is_inside) { impl->is_inside_contour = is_inside; }

std::vector<OutlineCircle> &Contour::get_sub_circles() { return impl->sub_circles; }
const std::vector<OutlineCircle> &Contour::get_sub_circles() const { return impl->sub_circles; }

bool Contour::operator==(const Contour &other) const { return impl==other.impl; }

bool Contour::operator!=(const Contour &other) const { return impl!=other.impl; }

bool Contour::operator<(const Contour &other) const { return impl < other.impl; }

Contour Contour::clone() const
{
	Contour copy;
	copy.impl->points = impl->points;
	copy.impl->is_inside_contour = impl->is_inside_contour;
	copy.impl->sub_circles = impl->sub_circles;
	return copy;
}

