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

#pragma once

#include "API/Core/Math/point.h"

namespace clan
{
	class PathRenderer
	{
	public:
		virtual ~PathRenderer() { }

		virtual void begin(float x, float y);

		virtual void line(float x, float y) = 0;
		virtual void end(bool close);

		void quadratic_bezier(float cp1_x, float cp1_y, float cp2_x, float cp2_y);
		void cubic_bezier(float cp1_x, float cp1_y, float cp2_x, float cp2_y, float cp3_x, float cp3_y);

	protected:
		float start_x = 0.0f;
		float start_y = 0.0f;
		float last_x = 0.0f;
		float last_y = 0.0f;

	private:
		void subdivide_bezier(int level, float cp0_x, float cp0_y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, float cp3_x, float cp3_y, float t0, float t1);
		static clan::Pointf point_on_bezier(float cp0_x, float cp0_y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, float cp3_x, float cp3_y, float t);
	};
}
