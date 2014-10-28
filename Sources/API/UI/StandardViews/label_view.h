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
#include "../../Display/Font/font.h"
#include "../../Display/2D/color.h"

namespace clan
{
	enum class TextAlignment
	{
		left,
		center,
		right,
		justified,
		natural
	};

	enum class LineBreakMode
	{
		//word_wrapping,
		//char_wrapping,
		clipping,
		truncating_head,
		truncating_tail,
		truncating_middle
	};

	class LabelViewImpl;

	class LabelView : public View
	{
	public:
		LabelView();

		std::string text() const;
		void set_text(const std::string &value);

		Font font() const;
		void set_font(const Font &value);

		Colorf text_color() const;
		void set_text_color(const Colorf &value);

		TextAlignment text_alignment() const;
		void set_text_alignment(TextAlignment value);

		LineBreakMode line_break_mode() const;
		void set_line_break_mode(LineBreakMode value);

		bool enabled() const;
		void set_enabled(bool value);

		int number_of_lines() const;
		void set_number_of_lines(int value);

		Colorf highlighted_color() const;
		void set_highlighted_color(const Colorf &value);

		bool highlighted() const;
		void set_highlighted(bool value);

		Colorf shadow_color() const;
		void set_shadow_color(const Colorf &value);

		void render_content(Canvas &canvas) override;
		float get_preferred_width() override;
		float get_preferred_height(float width) override;
		float get_first_baseline_offset(float width) override;
		float get_last_baseline_offset(float width) override;

	private:
		std::shared_ptr<LabelViewImpl> impl;
	};

}
