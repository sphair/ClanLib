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

#pragma once

#include <memory>
#include "../../Core/Math/rect.h"
#include "../../Display/2D/color.h"

namespace clan
{
	class CanvasText;
	class PathImpl;

	enum class PathFillMode
	{
		alternate,
		winding
	};

	class Path
	{
	public:
		Path();

		void set_fill_mode(PathFillMode fill_mode);

		void move_to(const Pointf &point);
		void line_to(const Pointf &point);
		void bezier_to(const Pointf &control, const Pointf &point);
		void bezier_to(const Pointf &control1, const Pointf &control2, const Pointf &point);
		void close();
		void text(const CanvasText &text);

		static Path rect(const Rectf &box);
		static Path rect(float x, float y, float width, float height) { return Path::rect(Rectf(x, y, Sizef(width, height))); }
		static Path line(const Pointf &start, const Pointf &end);
		static Path line(float x1, float y1, float x2, float y2) { return Path::line(Pointf(x1, y1), Pointf(x2, y2)); }

		std::shared_ptr<PathImpl> get_impl() const { return impl; }

	private:
		std::shared_ptr<PathImpl> impl;
		friend class CanvasImpl;
	};
}
