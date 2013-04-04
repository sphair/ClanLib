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

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"

namespace clan
{

class GraphicContext;
class FontMetrics;
class Colorf;
class Point;
class Size;
class Rect;
class Canvas;

/// \brief Interface for implementing a Font target.
class CL_API_DISPLAY FontProvider
{
/// \name Construction
/// \{

public:
	virtual ~FontProvider() { return; }

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns information about the current font.
	virtual FontMetrics get_font_metrics() = 0;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Print text on gc.
	///
	/// Function does not need to support multiline text (Font splits it)
	///
	/// \param gc = Graphic Context
	/// \param x = X position
	/// \param y = Y position
	/// \param text = The text to draw
	/// \param color = The text color
	virtual void draw_text(Canvas &canvas, float x, float y, const std::string &text, const Colorf &color) = 0;

	/// \brief Calculate size of text string.
	///
	/// Function does not need to support multiline text (Font splits it)
	virtual Size get_text_size(GraphicContext &gc, const std::string &text) = 0;

	/// \brief Get the character index at a specified point
	///
	/// Function should support multiline text (seperated by /n)
	///
	/// \param gc = The Graphic Context
	/// \param text = The string
	/// \param point = The point
	/// \return The character index. -1 = Not at specified point
	virtual int get_character_index(GraphicContext &gc, const std::string &text, const Point &point) = 0;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

}

/// \}
