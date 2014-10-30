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

#include "UI/precomp.h"
#include "API/UI/StandardViews/button_view.h"

namespace clan
{
	class ButtonViewImpl
	{
	public:
		std::shared_ptr<LabelView> label;
		std::shared_ptr<ImageView> image_view;
	};

	ButtonView::ButtonView() : impl(new ButtonViewImpl())
	{
		box_style.set_layout_hbox();

		impl->label = std::make_shared<LabelView>();
		impl->label->box_style.set_flex(1.0f, 1.0f);
		add_subview(impl->label);

		//impl->image_view = std::make_shared<ImageView>();
		//add_subview(impl->image_view);
	}

	ButtonView::~ButtonView()
	{
	}

	std::shared_ptr<LabelView> ButtonView::label()
	{
		return impl->label;
	}

	std::shared_ptr<ImageView> ButtonView::image_view()
	{
		return impl->image_view;
	}
}
