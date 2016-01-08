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

#include "Display/precomp.h"
#include "path_stroke_renderer.h"

namespace clan
{
	PathStrokeRenderer::PathStrokeRenderer(GraphicContext &gc)
	{
	}

	void PathStrokeRenderer::set_pen(Canvas &canvas, const Pen &pen)
	{
	}

	void PathStrokeRenderer::line(float x, float y)
	{
		last_x = x;
		last_y = y;

		// https://www.mapbox.com/blog/drawing-antialiased-lines/
	}

	void PathStrokeRenderer::end(bool close)
	{
	}
}
