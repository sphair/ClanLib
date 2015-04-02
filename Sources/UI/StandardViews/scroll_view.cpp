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
#include "API/UI/StandardViews/scroll_view.h"
#include "API/UI/StandardViews/scrollbar_view.h"
#include "API/UI/Events/pointer_event.h"
#include <algorithm>

namespace clan
{
	class ScrollViewContentContainer : public View
	{
	public:
		void layout_subviews(Canvas &canvas) override
		{
			for (auto &view : subviews())
			{
				// To do: maybe we need a mode to specify if the X axis is locked or infinite
				float width = geometry().content.get_width(); //view->get_preferred_width(canvas);
				float height = view->get_preferred_height(canvas, width);
				BoxGeometry geometry = BoxGeometry::from_content_box(style_cascade(), Rectf(0.0f, 0.0f, width, height));
				geometry.content.translate(-geometry.content.get_top_left());
				view->set_geometry(geometry);

				view->layout_subviews(canvas); // Maybe this should be handled in View?
			}
		}
	};

	class ScrollViewImpl
	{
	public:
		ScrollView *view = nullptr;
		std::shared_ptr<ScrollBarView> scroll_x = std::make_shared<ScrollBarView>();
		std::shared_ptr<ScrollBarView> scroll_y = std::make_shared<ScrollBarView>();
		std::shared_ptr<ScrollViewContentContainer> content_container = std::make_shared<ScrollViewContentContainer>();
		std::shared_ptr<View> content = std::make_shared<View>();
		ContentOverflow overflow_x = ContentOverflow::hidden;
		ContentOverflow overflow_y = ContentOverflow::automatic;
		Pointf content_offset;
	};

	ScrollView::ScrollView() : impl(new ScrollViewImpl())
	{
		impl->view = this;
		
		impl->scroll_x->set_hidden();
		impl->scroll_y->set_hidden();

		impl->content_container->style()->set("flex: 1 1 main-size");

		impl->content_container->set_content_clipped(true);
		impl->content_container->add_subview(impl->content);

		style()->set("flex-direction: column");

		add_subview(impl->content_container);
		add_subview(impl->scroll_x);
		add_subview(impl->scroll_y);
	}

	ScrollView::~ScrollView()
	{
	}

	std::shared_ptr<View> ScrollView::content_view() const
	{
		return impl->content;
	}

	std::shared_ptr<ScrollBarView> ScrollView::scrollbar_x_view() const
	{
		return impl->scroll_x;
	}

	std::shared_ptr<ScrollBarView> ScrollView::scrollbar_y_view() const
	{
		return impl->scroll_y;
	}
	
	ContentOverflow ScrollView::overflow_x() const
	{
		return impl->overflow_x;
	}
	
	ContentOverflow ScrollView::overflow_y() const
	{
		return impl->overflow_y;
	}
	
	void ScrollView::set_overflow_x(ContentOverflow value)
	{
		if (impl->overflow_x == value)
			return;
		
		impl->overflow_x = value;
		set_needs_layout();
	}
	
	void ScrollView::set_overflow_y(ContentOverflow value)
	{
		if (impl->overflow_y == value)
			return;
		
		impl->overflow_y = value;
		set_needs_layout();
	}
	
	void ScrollView::set_overflow(ContentOverflow value_x, ContentOverflow value_y)
	{
		set_overflow_x(value_x);
		set_overflow_y(value_y);
	}
	
	Pointf ScrollView::content_offset() const
	{
		return impl->content_offset;
	}
	
	void ScrollView::set_content_offset(const Pointf &offset, bool animated)
	{
		if (impl->content_offset == offset)
			return;
		
		impl->content_offset = offset;
		impl->content->set_view_transform(Mat4f::translate(-offset.x, -offset.y, 0.0f));
	}
	
	void ScrollView::layout_subviews(Canvas &canvas)
	{
		View::layout_subviews(canvas);
	}
	
	float ScrollView::get_preferred_width(Canvas &canvas)
	{
		return View::get_preferred_width(canvas);
	}
	
	float ScrollView::get_preferred_height(Canvas &canvas, float width)
	{
		return View::get_preferred_height(canvas, width);
	}
	
	float ScrollView::get_first_baseline_offset(Canvas &canvas, float width)
	{
		return View::get_first_baseline_offset(canvas, width);
	}
	
	float ScrollView::get_last_baseline_offset(Canvas &canvas, float width)
	{
		return View::get_last_baseline_offset(canvas, width);
	}
}
