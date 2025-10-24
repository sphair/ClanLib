/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "listview.h"	// Required for the "enum CL_ListViewDisplayMode", as gcc does not like the incomplete type: "enum CL_ListViewDisplayMode;"

class CL_ListViewIconList_Impl;
class CL_ListViewIcon_Impl;

/// \brief ListView icon.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ListViewIcon
{
/// \name Construction
/// \{

public:
	CL_ListViewIcon();

	~CL_ListViewIcon();


/// \}
/// \name Attributes
/// \{

public:
	CL_Sprite get_sprite(CL_ListViewDisplayMode mode);

	CL_PixelBuffer get_pixel_buffer(CL_ListViewDisplayMode mode);

	CL_Point get_offset(CL_ListViewDisplayMode mode);

	bool is_scalable() const;


/// \}
/// \name Operations
/// \{

public:
	void set_sprite(const CL_Sprite &sprite, CL_ListViewDisplayMode mode);

	// Offset the drawing position of the icon. Use for positioning overlay icons.
	void set_offset(const CL_Point &offset, CL_ListViewDisplayMode mode);

	void set_pixel_buffer(CL_PixelBuffer &pbuf, CL_ListViewDisplayMode mode);

	void draw(CL_GraphicContext &gc, const CL_Rect &rect, CL_ListViewDisplayMode mode, const CL_Colorf &color);

	void set_scalable(bool value);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ListViewIcon_Impl> impl;
/// \}
};


/// \}
