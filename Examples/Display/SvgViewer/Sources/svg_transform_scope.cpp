/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "precomp.h"
#include "svg_transform_scope.h"
#include "svg_attribute_reader.h"

SvgTransformScope::SvgTransformScope(clan::Canvas &canvas, clan::DomElement &e) : canvas(canvas)
{
	old_transform = canvas.get_transform();
	canvas.mult_transform(parse_transform(e, transform_active));
}

SvgTransformScope::SvgTransformScope(clan::Canvas &canvas, const clan::Mat4f &transform, bool transform_active) : canvas(canvas), transform_active(transform_active)
{
	old_transform = canvas.get_transform();
	canvas.mult_transform(transform);
}

SvgTransformScope::~SvgTransformScope()
{
	if (transform_active)
	{
		canvas.set_transform(old_transform);
	}
}

clan::Mat4f SvgTransformScope::parse_transform(clan::DomElement &e, bool &transform_active)
{
	try
	{
		clan::Mat4d mat = clan::Mat4d::identity();

		SvgAttributeReader transform(e, "transform");
		while (!transform.is_end())
		{
			clan::Mat4d t;
			if (transform.is_keyword("matrix"))
			{
				transform.get_keyword("matrix");
				transform.get_operator("(");
				t = clan::Mat4d::identity();
				t.matrix[0] = transform.get_number();
				t.matrix[1] = transform.get_sequence_number();
				t.matrix[4] = transform.get_sequence_number();
				t.matrix[5] = transform.get_sequence_number();
				t.matrix[12] = transform.get_sequence_number();
				t.matrix[13] = transform.get_sequence_number();
				transform.get_operator(")");
			}
			else if (transform.is_keyword("translate"))
			{
				transform.get_keyword("translate");
				transform.get_operator("(");
				double tx = transform.get_number();
				if (transform.is_operator(",")) transform.get_operator(",");
				double ty = transform.is_number() ? transform.get_number() : 0.0;
				transform.get_operator(")");

				t = clan::Mat4d::translate(tx, ty, 0.0);
			}
			else if (transform.is_keyword("scale"))
			{
				transform.get_keyword("scale");
				transform.get_operator("(");
				double sx = transform.get_number();
				if (transform.is_operator(",")) transform.get_operator(",");
				double sy = transform.is_number() ? transform.get_number() : sx;
				transform.get_operator(")");

				t = clan::Mat4d::scale(sx, sy, 0.0);
			}
			else if (transform.is_keyword("rotate"))
			{
				transform.get_keyword("rotate");
				transform.get_operator("(");
				double a = transform.get_number() * clan::PI / 180.0;

				t = clan::Mat4d::identity();
				t.matrix[0] = std::cos(a);
				t.matrix[1] = std::sin(a);
				t.matrix[4] = -t.matrix[1];
				t.matrix[5] = -t.matrix[0];

				if (transform.is_operator(",")) transform.get_operator(",");
				if (transform.is_number())
				{
					double tx = transform.get_number();
					if (transform.is_operator(",")) transform.get_operator(",");
					double ty = transform.is_number() ? transform.get_number() : 0.0;
					t = clan::Mat4d::translate(tx, ty, 0.0) * t * clan::Mat4d::translate(-tx, -ty, 0.0);
				}

				transform.get_operator(")");
			}
			else if (transform.is_keyword("skewX"))
			{
				transform.get_keyword("skewX");
				transform.get_operator("(");
				double a = transform.get_number() * clan::PI / 180.0;
				transform.get_operator(")");

				t = clan::Mat4d::identity();
				t.matrix[4] = std::tan(a);
			}
			else if (transform.is_keyword("skewY"))
			{
				transform.get_keyword("skewY");
				transform.get_operator("(");
				double a = transform.get_number() * clan::PI / 180.0;
				transform.get_operator(")");

				t = clan::Mat4d::identity();
				t.matrix[1] = std::tan(a);
			}
			else
			{
				transform.parse_error("expected transform keyword");
			}

			mat = mat * t;

			if (transform.is_operator(","))
				transform.get_operator(",");
		}

		transform_active = true;
		return clan::Mat4f(mat);
	}
	catch (clan::Exception &)
	{
		transform_active = false;
		return clan::Mat4f::identity();
	}
}
