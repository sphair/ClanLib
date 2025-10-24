/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_theme.h"
#include "API/GUI/Providers/gui_theme_provider.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Math/rect.h"

class CL_GUITheme_Impl
{
public:
	CL_GUITheme_Impl()
	: provider(0)
	{
	}

	~CL_GUITheme_Impl()
	{
		if (provider)
			provider->destroy();
	}

	CL_GUIThemeProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GUITheme Construction:

CL_GUITheme::CL_GUITheme(CL_GUIThemeProvider *provider)
: impl(new CL_GUITheme_Impl)
{
	impl->provider = provider;
}

CL_GUITheme::~CL_GUITheme()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUITheme Attributes:

bool CL_GUITheme::is_null()
{
	return impl.is_null();
}

CL_GUIThemeProvider *CL_GUITheme::get_provider() const
{
	if (impl.is_null())
		return 0;
	return impl->provider;
}

CL_StringRef CL_GUITheme::get_property(
	const CL_GUIThemePart &part,
	const CL_StringRef &name,
	const CL_StringRef &css_value)
{
	return impl->provider->get_property(part, name, css_value);
}

CL_ResourceManager CL_GUITheme::get_resources() const
{
	return impl->provider->get_resources();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUITheme Operations:

void CL_GUITheme::set_resources(CL_ResourceManager resources)
{
	impl->provider->set_resources(resources);
}

void CL_GUITheme::render_box(
	CL_GraphicContext &gc,
	CL_GUIThemePart &part,
	const CL_Rect &rect,
	const CL_Rect &clip_rect)
{
	impl->provider->render_box(gc, part, rect, clip_rect);
}

CL_Rect CL_GUITheme::render_text(
   CL_GraphicContext &gc,
   CL_GUIThemePart &part,
   CL_Font &font,
   const CL_StringRef &text,
   const CL_Rect &content_rect,
   const CL_Rect &clip_rect)
{
	return impl->provider->render_text(gc, part, font, text, content_rect, clip_rect);
}

void CL_GUITheme::component_destroyed(CL_GUIComponent *component)
{
	impl->provider->component_destroyed(component);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUITheme Implementation:
