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
*/

#include "UI/precomp.h"
#include "API/UI/StandardViews/listbox_view.h"
#include "API/UI/StandardViews/label_view.h"
#include "listbox_view_impl.h"

namespace clan
{
	ListBoxView::ListBoxView() : impl(new ListBoxViewImpl())
	{
		impl->listbox = this;
		content_view()->style()->set("flex-direction: column");
		
		set_focus_policy(FocusPolicy::accept);

		slots.connect(sig_key_press(), impl.get(), &ListBoxViewImpl::on_key_press);
		slots.connect(content_view()->sig_pointer_press(), impl.get(), &ListBoxViewImpl::on_pointer_press);
		slots.connect(content_view()->sig_pointer_release(), impl.get(), &ListBoxViewImpl::on_pointer_release);

	}

	ListBoxView::~ListBoxView()
	{
	}
	
	std::function<void()> &ListBoxView::func_selection_changed()
	{
		return impl->func_selection_changed;
	}
	
	void ListBoxView::set_items(const std::vector<std::shared_ptr<View>> &items)
	{
		impl->selected_item = -1;
		
		auto views = content_view()->children();
		while (!views.empty())
			views.back()->remove_from_parent();
		
		for (auto &item : items)
		{
			content_view()->add_child(item);
			slots.connect(item->sig_pointer_enter(), impl.get(), &ListBoxViewImpl::on_pointer_enter);
			slots.connect(item->sig_pointer_leave(), impl.get(), &ListBoxViewImpl::on_pointer_leave);

		}

	}
	
	int ListBoxView::selected_item() const
	{
		return impl->selected_item;
	}
	
	void ListBoxView::set_selected_item(int index)
	{
		if (index == impl->selected_item)
			return;
		
		if (index < -1 || index >= (int) content_view()->children().size())
			throw Exception("Listbox index out of bounds");

		if (impl->selected_item != -1)
			content_view()->children().at(impl->selected_item)->set_state("selected", false);
		
		if (index != -1)
		{
			if (impl->hot_item == index)
				impl->set_hot_item(-1);

			auto new_selected_item = content_view()->children().at(index);
			new_selected_item->set_state("selected", true);
			
			// To do: call set_content_offset() if new_selected_item is not within range (maybe add a helper on ScrollView for this?)
		}
		
		impl->selected_item = index;
	}
}
