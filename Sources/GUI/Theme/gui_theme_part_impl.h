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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#pragma once

#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Math/rect.h"
#include "API/CSSLayout/css_property.h"
#include "API/CSSLayout/css_document.h"
#include "API/CSSLayout/css_token.h"
#include "API/Core/Crypto/hash_functions.h"
#include "../gui_manager_impl.h"
#include "../gui_css_strings.h"
#include <map>

namespace clan
{

class GUIThemePart_Impl
{
public:
	GUIThemePart_Impl(GUIComponent *component);

	GUIManager get_manager() const;

	GUIComponent *component;

	mutable bool has_cached_content_box_shrink_rect;
	mutable Rect cached_content_box_shrink_rect;
	mutable bool font_loaded;
};

inline GUIThemePart_Impl::GUIThemePart_Impl(GUIComponent *component)
: component(component), has_cached_content_box_shrink_rect(false), font_loaded(false)
{
}

inline GUIManager GUIThemePart_Impl::get_manager() const
{
	return component->get_gui_manager();
}

}
