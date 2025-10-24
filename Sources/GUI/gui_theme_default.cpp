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
**    Kenneth Gangstoe
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_theme_default.h"
#include "API/Core/Text/string_help.h"
#include "gui_theme_default_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIThemeDefault Construction:

CL_GUIThemeDefault::CL_GUIThemeDefault()
: CL_GUITheme(), impl(new CL_GUIThemeDefault_Impl)
{
}

CL_GUIThemeDefault::~CL_GUIThemeDefault()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIThemeDefault Attributes:

CL_StringRef CL_GUIThemeDefault::get_property(
	const CL_GUIThemePart &part,
	const CL_StringRef &name,
	const CL_StringRef &css_value)
{
	return impl->get_property(part,name,css_value);
}

CL_ResourceManager CL_GUIThemeDefault::get_resources() const
{
	return impl->resources;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIThemeDefault Operations:

void CL_GUIThemeDefault::set_resources(CL_ResourceManager resources)
{
	impl->resources = resources;
}

void CL_GUIThemeDefault::render_box(
	CL_GraphicContext &gc,
	CL_GUIThemePart &part,
	const CL_Rect &rect,
	const CL_Rect &clip_rect)
{
	impl->render_box(gc, part,rect,clip_rect);
}

CL_Rect CL_GUIThemeDefault::render_text(
   CL_GraphicContext &gc,
   CL_GUIThemePart &part,
   CL_Font &font,
   const CL_StringRef &text,
   const CL_Rect &content_rect,
   const CL_Rect &clip_rect)
{
	return impl->render_text(gc, part, font, text, content_rect, clip_rect);
}

void CL_GUIThemeDefault::component_destroyed(CL_GUIComponent *component)
{
	impl->component_destroyed(component);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIThemeDefault Implementation:
