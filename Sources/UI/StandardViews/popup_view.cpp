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

#include "UI/precomp.h"
#include "API/UI/StandardViews/popup_view.h"
#include "API/Display/Window/display_window_description.h"

namespace clan
{
	class PopupViewImpl
	{
	public:
		static DisplayWindowDescription get_window_desc()
		{
			DisplayWindowDescription desc;
			desc.set_type(WindowType::popup);
			desc.set_visible(false);
			desc.set_topmost(true);
			desc.set_no_activate(true);
			return desc;
		}
	};

	PopupView::PopupView() : WindowView(PopupViewImpl::get_window_desc())
	{
		get_display_window().enable_alpha_channel(Rect(-1, -1, 0, 0));
	}

	void PopupView::set_root_hidden(bool value)
	{
		if (value != hidden())
		{
			if (!value)
				show(WindowShowType::show_no_activate);
			else
				hide();
		}
	}
}
