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
**    Artem Khomenko
*/

#include "UI/precomp.h"
#include "API/UI/StandardViews/listbox_view.h"
#include "API/UI/StandardViews/text_field_view.h"
#include "API/UI/Events/pointer_event.h"
#include "API/UI/Events/key_event.h"
#include "listbox_view_impl.h"

namespace clan
{
	void ListBoxViewImpl::on_key_press(KeyEvent &e)
	{
		if (listbox->content_view()->children().empty())
			return;

		if (e.key() == Key::up)
		{
			set_selected_item(clan::max(selected_item - 1, 0));
			if (func_selection_changed)
				func_selection_changed();
		}
		else if (e.key() == Key::down)
		{
			set_selected_item(clan::min(selected_item + 1, (int)listbox->content_view()->children().size() - 1));
			if (func_selection_changed)
				func_selection_changed();
		}
	}

	void ListBoxViewImpl::on_pointer_press(PointerEvent &e)
	{
		if (e.button() != PointerButton::left)
			return;

		last_selected_item = selected_item;

		int index = get_selection_index(e);
		set_selected_item(index);
	}

	void ListBoxViewImpl::on_pointer_release(PointerEvent &e)
	{
		if (e.phase() != EventUIPhase::bubbling || e.button() != PointerButton::left)
			return;

		int index = get_selection_index(e);

		// Only allow click when mouse released over component
		if (index == selected_item)
		{
			if (func_selection_changed)
				func_selection_changed();
		}
		else
		{
			set_selected_item(last_selected_item);
		}
	}

	int ListBoxViewImpl::get_selection_index(PointerEvent &e)
	{
		int index = 0;
		for (auto &view : listbox->content_view()->children())
		{
			if (view->geometry().border_box().contains(e.pos(listbox->content_view())))
				return index;
			index++;
		}
		return -1;
	}

	void ListBoxViewImpl::set_hot_item(int index)
	{
		// Selected item state has priority.
		if (index == hot_item || index == selected_item && index != -1)
			return;

		if (index < -1 || index >= (int) listbox->content_view()->children().size())
			throw Exception("Listbox index out of bounds");

		if (hot_item != -1)
			listbox->content_view()->children().at(hot_item)->set_state("hot", false);

		if (index != -1)
		{
			auto new_selected_item = listbox->content_view()->children().at(index);
			new_selected_item->set_state("hot", true);
		}

		hot_item = index;

		// Drawing changes.
		listbox->children().front()->draw_without_layout();
	}

	void ListBoxViewImpl::set_selected_item(int index)
	{
		listbox->set_selected_item(index);
		listbox->set_needs_layout();
	}

		void ListBoxViewImpl::on_pointer_enter(PointerEvent &e)
	{
		set_hot_item(get_selection_index(e));
	}

	void ListBoxViewImpl::on_pointer_leave(PointerEvent &e)
	{
		set_hot_item(-1);
	}

}
