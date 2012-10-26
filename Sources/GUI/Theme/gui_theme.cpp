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
**    Kenneth Gangstoe
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_theme.h"
#include "API/GUI/Providers/gui_theme_provider.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Math/rect.h"

namespace clan
{

class GUITheme_Impl
{
public:
	GUITheme_Impl()
	: provider(0)
	{
	}

	~GUITheme_Impl()
	{
		if (provider)
			delete provider;
	}

	GUIThemeProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// GUITheme Construction:

GUITheme::GUITheme(GUIThemeProvider *provider)
: impl(new GUITheme_Impl)
{
	impl->provider = provider;
}

GUITheme::~GUITheme()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUITheme Attributes:

bool GUITheme::is_null() const
{
	return !impl;
}

GUIThemeProvider *GUITheme::get_provider() const
{
	if (!impl)
		return 0;
	return impl->provider;
}

std::string GUITheme::get_property(
	const GUIThemePart &part,
	const std::string &name,
	const std::string &css_value)
{
	return impl->provider->get_property(part, name, css_value);
}

ResourceManager GUITheme::get_resources() const
{
	return impl->provider->get_resources();
}

/////////////////////////////////////////////////////////////////////////////
// GUITheme Operations:

void GUITheme::set_resources(ResourceManager resources)
{
	impl->provider->set_resources(resources);
}

void GUITheme::add_resources(ResourceManager resources)
{
	impl->provider->add_resources(resources);
}

void GUITheme::render_box(
	Canvas &canvas,
	GUIThemePart &part,
	const Rect &rect,
	const Rect &clip_rect)
{
	impl->provider->render_box(canvas, part, rect, clip_rect);
}

Rect GUITheme::render_text(
   Canvas &canvas,
   GUIThemePart &part,
   Font &font,
   const std::string &text,
   const Rect &content_rect,
   const Rect &clip_rect)
{
	return impl->provider->render_text(canvas, part, font, text, content_rect, clip_rect);
}

void GUITheme::component_destroyed(GUIComponent *component)
{
	impl->provider->component_destroyed(component);
}

/////////////////////////////////////////////////////////////////////////////
// GUITheme Implementation:

}
