/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "UI/precomp.h"
#include "API/UI/Style/style.h"
#include "style_impl.h"
#include "Properties/background.h"
#include "Properties/border.h"
#include "Properties/border_image.h"
#include "Properties/box_shadow.h"
#include "Properties/content.h"
#include "Properties/flex.h"
#include "Properties/layout.h"
#include "Properties/margin.h"
#include "Properties/outline.h"
#include "Properties/padding.h"
#include "Properties/text.h"

namespace clan
{
	class StyleForceLink
	{
	public:
		StyleForceLink()
		{
			force_link_style_parser_background();
			force_link_style_parser_border();
			force_link_style_parser_border_image();
			force_link_style_parser_box_shadow();
			force_link_style_parser_content();
			force_link_style_parser_flex();
			force_link_style_parser_layout();
			force_link_style_parser_margin();
			force_link_style_parser_outline();
			force_link_style_parser_padding();
			force_link_style_parser_text();
		}
	} style_force_link;

	Style::Style() : impl(new StyleImpl())
	{
	}

	Style::~Style()
	{
	}

	void Style::set(const std::string& properties)
	{
		StyleProperty::parse(impl.get(), properties);
	}

	StyleGetValue Style::declared_value(PropertyHash hash) const
	{
		return impl->declared_value(hash);
	}
}