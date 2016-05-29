/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Artem Khomenko (add sticky property)
*/

#include "UI/precomp.h"
#include "API/UI/StandardViews/button_view.h"
#include "API/UI/StandardViews/image_view.h"
#include "button_view_impl.h"

namespace clan
{

	ButtonView::ButtonView() : impl(new ButtonViewImpl())
	{
		impl->button = this;
		style()->set("flex-direction: row");

		impl->image_view = std::make_shared<ImageView>();
		impl->image_view->style()->set("flex: 0 0 auto");
		add_child(impl->image_view);

		impl->label = std::make_shared<LabelView>();
		impl->label->style()->set("margin: auto");
		impl->label->style()->set("flex: 1 1 auto");
		add_child(impl->label);

		slots.connect(sig_pointer_press(), impl.get(), &ButtonViewImpl::on_pointer_press);
		slots.connect(sig_pointer_release(), impl.get(), &ButtonViewImpl::on_pointer_release);

		slots.connect(sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_hot = true;  impl->update_state(); });
		slots.connect(sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_hot = false;  impl->update_state(); });
		slots.connect(impl->label->sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_hot = true;  impl->update_state(); });
		slots.connect(impl->label->sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_hot = false;  impl->update_state(); });
		slots.connect(impl->image_view->sig_pointer_enter(), [&](PointerEvent &e) {impl->_state_hot = true;  impl->update_state(); });
		slots.connect(impl->image_view->sig_pointer_leave(), [&](PointerEvent &e) {impl->_state_hot = false;  impl->update_state(); });
	}

	ButtonView::~ButtonView()
	{
	}

	std::function<void()> &ButtonView::func_clicked()
	{
		return impl->_func_clicked;
	}

	bool ButtonView::disabled() const
	{
		return impl->_state_disabled;
	}

	void ButtonView::set_disabled()
	{
		if (!impl->_state_disabled)
		{
			impl->_state_disabled = true;
			impl->update_state();
		}
	}
	void ButtonView::set_enabled()
	{
		if (impl->_state_disabled)
		{
			impl->_state_disabled = false;
			impl->update_state();
		}
	}

	std::shared_ptr<LabelView> ButtonView::label()
	{
		return impl->label;
	}

	std::shared_ptr<ImageView> ButtonView::image_view()
	{
		return impl->image_view;
	}

	void ButtonView::move_label_before_image()
	{
		impl->label->remove_from_parent();
		impl->image_view->remove_from_parent();

		add_child(impl->label);
		add_child(impl->image_view);
	}

	void ButtonView::move_label_after_image()
	{
		impl->label->remove_from_parent();
		impl->image_view->remove_from_parent();

		add_child(impl->image_view);
		add_child(impl->label);
	}

	void ButtonView::set_sticky(bool is_sticky)
	{
		impl->_sticky = is_sticky;
	}

	bool ButtonView::sticky()
	{
		return impl->_sticky;
	}

	bool ButtonView::pressed()
	{
		return impl->_state_pressed;
	}

	void ButtonView::set_pressed(bool is_pressed)
	{
		if (impl->_state_pressed != is_pressed)
		{
			impl->_state_pressed = is_pressed;
			impl->_state_pressed_previous = !is_pressed;
			impl->update_state();
		}
	}
}

