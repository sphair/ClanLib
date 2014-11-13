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

#include "API/UI/View/view.h"
#include "API/UI/View/focus_policy.h"
#include "API/Display/Window/cursor.h"
#include "API/Display/Window/cursor_description.h"
#include "../Animation/animation_group.h"

namespace clan
{
	class ViewImpl
	{
	public:
		unsigned int find_next_tab_index(unsigned int tab_index) const;
		unsigned int find_prev_tab_index(unsigned int tab_index) const;
		unsigned int find_highest_tab_index() const;
		View *find_next_with_tab_index(unsigned int tab_index, const ViewImpl *search_from = 0, bool also_search_ancestors = true) const;
		View *find_prev_with_tab_index(unsigned int tab_index, const ViewImpl *search_from = 0, bool also_search_ancestors = true) const;

		void inverse_bubble(EventUI *e);

		View *_superview = 0;
		std::vector<std::shared_ptr<View>> _subviews;

		unsigned int tab_index = 0;
		FocusPolicy focus_policy = FocusPolicy::reject;

		BoxStyle _style;
		BoxGeometry _geometry;
		bool hidden = false;

		bool _needs_layout = true;

		Signal<void(ActivationChangeEvent &)> _sig_activated[4];
		Signal<void(ActivationChangeEvent &)> _sig_deactivated[4];
		Signal<void(CloseEvent &)> _sig_close[4];
		Signal<void(ResizeEvent &)> _sig_resize[4];
		Signal<void(FocusChangeEvent &)> _sig_focus_gained[4];
		Signal<void(FocusChangeEvent &)> _sig_focus_lost[4];
		Signal<void(PointerEvent &)> _sig_pointer_enter[4];
		Signal<void(PointerEvent &)> _sig_pointer_leave[4];
		Signal<void(PointerEvent &)> _sig_pointer_move[4];
		Signal<void(PointerEvent &)> _sig_pointer_press[4];
		Signal<void(PointerEvent &)> _sig_pointer_release[4];
		Signal<void(PointerEvent &)> _sig_pointer_double_click[4];
		Signal<void(PointerEvent &)> _sig_pointer_proximity_change[4];
		Signal<void(KeyEvent &)> _sig_key_press[4];
		Signal<void(KeyEvent &)> _sig_key_release[4];

		// Root view variables:
		View *_owner_view = 0;
		View *_focus_view = 0;
		View *_proximity_view = 0;

		AnimationGroup animation_group;

		StandardCursor cursor_type = StandardCursor::arrow;
		CursorDescription cursor_desc;
		Cursor cursor;
		bool is_custom_cursor = false;
		bool is_cursor_inherited = true;

	private:
		unsigned int find_prev_tab_index_helper(unsigned int tab_index) const;
	};
}
