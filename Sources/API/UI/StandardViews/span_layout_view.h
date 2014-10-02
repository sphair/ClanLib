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
*/

#pragma once

#include "../View/view.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/color.h"
#include "label_view.h"

namespace clan
{
	class SpanLayoutViewImpl;

	class SpanLayoutView : public View
	{
	public:
		SpanLayoutView();

		void set_text_alignment(TextAlignment alignment);

		void clear();
		void add_text(const std::string &text, const Font &font, const Colorf &color = Colorf(0, 0, 0));
		void add_subview(const std::shared_ptr<View> &view, float baseline_offset = 0.0f);

		void render_content(Canvas &canvas) override;
		float get_preferred_width() override;
		float get_preferred_height(float width) override;
		float get_first_baseline_offset(float width) override;
		float get_last_baseline_offset(float width) override;
		void layout_subviews() override;

	protected:
		void subview_added(const std::shared_ptr<View> &view) override;
		void subview_removed(const std::shared_ptr<View> &view) override;

	private:
		std::shared_ptr<SpanLayoutViewImpl> impl;
	};
}
