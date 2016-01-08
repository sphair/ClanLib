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
**    Magnus Norddahl
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/Math/quad.h"
#include "API/Core/Math/rect.h"

namespace clan
{
	template<typename Type>
	Type Quadx<Type>::get_width() const
	{
		return max(max(max(p.x, q.x), r.x), s.x)
			- min(min(min(p.x, q.x), r.x), s.x);
	}

	template<typename Type>
	Type Quadx<Type>::get_height() const
	{
		return max(max(max(p.y, q.y), r.y), s.y)
			- min(min(min(p.y, q.y), r.y), s.y);
	}

	template<typename Type>
	Rect Quadx<Type>::get_bounds() const
	{
		return Rect(
			min(min(min(p.x, q.x), r.x), s.x),
			min(min(min(p.y, q.y), r.y), s.y),
			max(max(max(p.x, q.x), r.x), s.x),
			max(max(max(p.y, q.y), r.y), s.y));
	}

	template<typename Type>
	Quadx<Type> &Quadx<Type>::rotate(const Vec2<Type> &hotspot, const Angle &angle)
	{
		p.rotate(hotspot, angle);
		q.rotate(hotspot, angle);
		r.rotate(hotspot, angle);
		s.rotate(hotspot, angle);
		return *this;
	}

	template<typename Type>
	Quadx<Type> &Quadx<Type>::scale(const Vec2<Type> &hotspot, float sx, float sy)
	{
		p.x = (Type)((p.x - hotspot.x)*sx + hotspot.x); p.y = (Type)((p.y - hotspot.y)*sy + hotspot.y);
		q.x = (Type)((q.x - hotspot.x)*sx + hotspot.x); q.y = (Type)((q.y - hotspot.y)*sy + hotspot.y);
		r.x = (Type)((r.x - hotspot.x)*sx + hotspot.x); r.y = (Type)((r.y - hotspot.y)*sy + hotspot.y);
		s.x = (Type)((s.x - hotspot.x)*sx + hotspot.x); s.y = (Type)((s.y - hotspot.y)*sy + hotspot.y);
		return *this;
	}

	template<typename Type>
	Quadx<Type> &Quadx<Type>::scale(float sx, float sy)
	{
		p.x = (Type)(p.x*sx); p.y = (Type)(p.y*sy);
		q.x = (Type)(q.x*sx); q.y = (Type)(q.y*sy);
		r.x = (Type)(r.x*sx); r.y = (Type)(r.y*sy);
		s.x = (Type)(s.x*sx); s.y = (Type)(s.y*sy);
		return *this;
	}

	template<typename Type>
	Vec2<Type> Quadx<Type>::center() const
	{
		Rect r = get_bounds();
		return Vec2<Type>((r.left + r.right) / 2, (r.top + r.bottom) / 2);
	}

	template<typename Type>
	Quadx<Type> &Quadx<Type>::apply_alignment(Origin origin, Type x, Type y)
	{
		Vec2<Type> offset = Vec2<Type>::calc_origin(origin, get_size());
		offset.x -= x;
		offset.y -= y;

		p.x += offset.x;
		p.y += offset.y;
		q.x += offset.x;
		q.y += offset.y;
		r.x += offset.x;
		r.y += offset.y;
		s.x += offset.x;
		s.y += offset.y;
		return *this;
	}

	template<typename Type>
	bool Quadx<Type>::is_inside(const Vec2<Type> &point) const
	{
		const int num_points = 4;
		const Vec2<Type> *vertices[num_points] = { &p, &q, &r, &s };

		// Ray cast to the right to determine if the point is inside the polygon:
		// An uneven count of crossed lines means we are inside the polygon.

		bool inside = false;

		int prev = num_points - 1;
		for (int current = 0; current < num_points; current++)
		{
			bool vertically_in_range = (vertices[current]->y >= point.y) != (vertices[prev]->y > point.y);
			if (vertically_in_range)
			{
				// Find intersection point between line and our ray:
				Type line_range_x = vertices[prev]->x - vertices[current]->x;
				Type line_range_y = vertices[prev]->y - vertices[current]->y;
				Type delta_y = point.y - vertices[current]->y;
				Type intersect_x = line_range_x * delta_y / line_range_y + vertices[current]->x;

				if (point.x < intersect_x)
				{
					inside = !inside;
				}
			}

			prev = current;
		}

		return inside;
	}

	// Explicit instantiate the versions we use:
	template class Quadx<int>;
	template class Quadx<float>;
	template class Quadx<double>;
}
