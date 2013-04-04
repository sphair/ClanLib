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
**    Harry Storbacka
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{

#pragma once

#include "../api_gui.h"
#include "../../Display/2D/sprite.h"

#include "listview.h"	// Required for the "enum ListViewDisplayMode", as gcc does not like the incomplete type: "enum ListViewDisplayMode;"

namespace clan
{

class ListViewIconList_Impl;
class ListViewIcon_Impl;

/// \brief ListView icon.
class CL_API_GUI ListViewIcon
{
/// \name Construction
/// \{

public:
	ListViewIcon();

	~ListViewIcon();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get sprite
	///
	/// \param mode = List View Display Mode
	///
	/// \return Sprite
	Sprite get_sprite(ListViewDisplayMode mode);

	/// \brief Get pixel buffer
	///
	/// \param mode = List View Display Mode
	///
	/// \return Pixel Buffer
	PixelBuffer get_pixel_buffer(ListViewDisplayMode mode);

	/// \brief Get offset
	///
	/// \param mode = List View Display Mode
	///
	/// \return Point
	Point get_offset(ListViewDisplayMode mode);

	/// \brief Is Scalable
	///
	/// \return true = scalable
	bool is_scalable() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Set sprite
	///
	/// \param sprite = Sprite
	/// \param mode = List View Display Mode
	void set_sprite(const Sprite &sprite, ListViewDisplayMode mode);

	// Offset the drawing position of the icon. Use for positioning overlay icons.
	void set_offset(const Point &offset, ListViewDisplayMode mode);

	/// \brief Set pixel buffer
	///
	/// \param pbuf = Pixel Buffer
	/// \param mode = List View Display Mode
	void set_pixel_buffer(PixelBuffer &pbuf, ListViewDisplayMode mode);

	/// \brief Draw
	///
	/// \param canvas = Canvas
	/// \param rect = Rect
	/// \param mode = List View Display Mode
	/// \param color = Colorf
	void draw(Canvas &canvas, const Rect &rect, ListViewDisplayMode mode, const Colorf &color);

	/// \brief Set scalable
	///
	/// \param value = bool
	void set_scalable(bool value);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<ListViewIcon_Impl> impl;
/// \}
};

}

/// \}
