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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/2D/path.h"
#include "path_impl.h"

namespace clan
{
	Path::Path() : impl(new PathImpl)
	{
		impl->subpaths.resize(1);
	}

	void Path::set_fill_mode(PathFillMode fill_mode)
	{
		impl->fill_mode = fill_mode;
	}

	void Path::move_to(const Pointf &point)
	{
		if (!impl->subpaths.back().commands.empty())
			impl->subpaths.push_back(CanvasSubpath());

		impl->subpaths.back().points.front() = point;
	}

	void Path::line_to(const Pointf &point)
	{
		impl->subpaths.back().points.push_back(point);
		impl->subpaths.back().commands.push_back(PathCommand::line);
	}

	void Path::bezier_to(const Pointf &control, const Pointf &point)
	{
		impl->subpaths.back().points.push_back(control);
		impl->subpaths.back().points.push_back(point);
		impl->subpaths.back().commands.push_back(PathCommand::quadradic);
	}

	void Path::bezier_to(const Pointf &control1, const Pointf &control2, const Pointf &point)
	{
		impl->subpaths.back().points.push_back(control1);
		impl->subpaths.back().points.push_back(control2);
		impl->subpaths.back().points.push_back(point);
		impl->subpaths.back().commands.push_back(PathCommand::cubic);
	}

	void Path::close()
	{
		if (!impl->subpaths.back().commands.empty())
		{
			impl->subpaths.back().closed = true;
			impl->subpaths.push_back(CanvasSubpath());
		}
	}

	void Path::text(const CanvasText &text)
	{
	}

	Path Path::rect(const Rectf &box)
	{
		Path path;
		path.move_to(box.get_top_left());
		path.line_to(Pointf(box.get_top_right()));
		path.line_to(Pointf(box.get_bottom_right()));
		path.line_to(Pointf(box.get_bottom_left()));
		path.close();
		return path;
	}

	Path Path::line(const Pointf &start, const Pointf &end)
	{
		Path path;
		path.move_to(start);
		path.line_to(end);
		return path;
	}

	Path Path::ellipse(const Pointf &center, const Sizef &radius)
	{
		float offset_x = 0;
		float offset_y = 0;

		int max_radius = max(radius.width, radius.height);

		int rotationcount = max(5, (max_radius - 3));
		float halfpi = 1.5707963267948966192313216916398f;
		float turn = halfpi / rotationcount;

		offset_x = center.x;
		offset_y = -center.y;

		Path path;

		path.move_to(center.x + radius.width, center.y);

		rotationcount *= 4;

		std::vector<Pointf> points;
		points.resize(rotationcount-1);

		for (int i = 1; i < rotationcount; i++)
		{
			float pos1 = radius.width * cos(i * turn);
			float pos2 = radius.height * sin(i * turn);

			path.line_to(center.x + pos1, center.y + pos2);
		}

		path.close();

		return path;
	}
}
