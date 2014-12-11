/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "../../Display/Window/display_window.h"
#include "../../Display/Window/keys.h"

namespace clan
{
	class InputEvent;
	class TextureView_Impl;

	class TextureView : public View
	{
	public:
		TextureView(Canvas &canvas);

		/// \brief Set position and size within the canvas
		///
		/// \param rect = position and size.
		void set_rect(const Rect &rect);

		/// \brief Returns the position and size within the canvas
		Rect get_rect() const;

		void set_needs_render() override;
		bool local_root() override;
		void layout_local() override;

		void update();

		Pointf to_screen_pos(const Pointf &pos) override;
		Pointf from_screen_pos(const Pointf &pos) override;

	private:
		std::shared_ptr<TextureView_Impl> impl;
	};
}
