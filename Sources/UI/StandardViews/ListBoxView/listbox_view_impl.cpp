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
**    Mark Page
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
		if (items.empty())
			return;
		
		if (e.key() == Key::up)
		{
			listbox->set_selected_item(std::max(selected_item - 1, 0));
			if (func_selection_changed)
				func_selection_changed();
		}
		else if (e.key() == Key::down)
		{
			listbox->set_selected_item(std::min(selected_item + 1, (int)items.size() - 1));
			if (func_selection_changed)
				func_selection_changed();
		}
	}
	
	void ListBoxViewImpl::on_pointer_press(PointerEvent &e)
	{
		listbox->set_focus();
	}
	
	void ListBoxViewImpl::on_pointer_release(PointerEvent &e)
	{
		if (e.phase() != EventUIPhase::bubbling)
			return;
		
		auto target = e.target();
		int index = 0;
		for (auto &view : listbox->content_view()->subviews())
		{
			if (view == target)
			{
				listbox->set_selected_item(index);
				if (func_selection_changed)
					func_selection_changed();
				break;
			}
			index++;
		}
	}
}
