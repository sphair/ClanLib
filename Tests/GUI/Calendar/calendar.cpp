
#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/Display/Window/keys.h>

#include <map>

#include <ClanLib/csslayout.h>

#include "calendar.h"


class CalendarComponent_Impl
{
public:
	CalendarComponent_Impl() : component(0), has_mouse_over(false), selection_valid(false), hot_xy_index(-1,-1)
	{
		day_names = clan::StringHelp::split_text("Sunday,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday", ",");
		day_short_names = clan::StringHelp::split_text("Su,Mo,Tu,We,Th,Fr,Sa", ",");
		month_names = clan::StringHelp::split_text("January,February,March,April,May,June,July,August,September,October,November,December", ",");
		month_short_names = clan::StringHelp::split_text("Jan,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec", ",");
		
		date_view = clan::DateTime::get_current_local_time();
	}

	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	void on_process_message(std::shared_ptr<clan::GUIMessage> &msg);
	void on_enablemode_changed();
	void on_resize();

	void on_mouse_move(const clan::InputEvent &input_event);
	void on_mouse_left_down(const clan::InputEvent &input_event);
	void on_mouse_left_up(const clan::InputEvent &input_event);
	void on_mouse_leave();

	void create_parts();
	int get_days_in_month(int month);
	clan::Rect get_day_name_box(int day);
	int get_day_at_pos(const clan::Point &pos);
	clan::Vec2i get_xy_index_at_pos(const clan::Point &pos);
	clan::Callback_v0 func_selection_changed;

	CalendarComponent *component;
	clan::Callback_v0 func_clicked;
	clan::CSSLayout layout;
	clan::DateTime current_date;

	clan::GUIThemePart part_day;
	clan::GUIThemePart part_week;
	clan::GUIThemePart part_next_month;
	clan::GUIThemePart part_prev_month;
	clan::GUIThemePart part_day_name;
	clan::GUIThemePart part_month_year;

	std::vector<std::string> day_names;
	std::vector<std::string> day_short_names;
	std::vector<std::string> month_names;
	std::vector<std::string> month_short_names;

	bool has_mouse_over;
	clan::Point last_mouse_pos;
	clan::DateTime date_view;
	clan::DateTime date_selected;

	clan::Rect content_box;
	clan::Rect next_month_box;
	clan::Rect prev_month_box;
	clan::Rect month_year_box;

	clan::Vec2i hot_xy_index;
	int day_table[6][7];
	bool selection_valid; // initially false, set when a date is picked.
};

//////////////////////////////////////////////////////////////////////////
// Construction

CalendarComponent::CalendarComponent(clan::GUIComponent *parent)
: clan::GUIComponent(parent, "calendar"), impl(new CalendarComponent_Impl)
{
	impl->component = this;

	set_blocks_default_action(false);
	set_focus_policy(focus_local);

	func_process_message().set(impl.get(), &CalendarComponent_Impl::on_process_message);
	func_render().set(impl.get(), &CalendarComponent_Impl::on_render);
	func_enablemode_changed().set(impl.get(), &CalendarComponent_Impl::on_enablemode_changed);
	func_resized().set(impl.get(), &CalendarComponent_Impl::on_resize);

	impl->create_parts();
}

//////////////////////////////////////////////////////////////////////////
// Attributes

clan::Size CalendarComponent::get_css_size() const
{
	clan::Size size;
	size.height = 
		impl->part_next_month.get_css_height() +
		impl->part_day_name.get_css_height() + 
		6 * impl->part_day.get_css_height();

	size.width = clan::max(
		impl->part_prev_month.get_css_width() + 
		impl->part_next_month.get_css_width() + 
		impl->part_month_year.get_css_width(), 
		// OR
		7 * impl->part_day.get_css_width());

	clan::Rect shrink = get_content_shrink_box();	
	size.width += abs(shrink.left) + abs(shrink.right);
	size.height += abs(shrink.top) + abs(shrink.bottom);

	return size;
}

clan::DateTime CalendarComponent::get_selected_date() const
{
	return impl->date_selected;
}

//////////////////////////////////////////////////////////////////////////
// Operations

void CalendarComponent::set_current_date(const clan::DateTime &date)
{
	impl->current_date = date;

	request_repaint();
}

//////////////////////////////////////////////////////////////////////////
// Operations

void CalendarComponent::set_day_names( std::string comma_separated_monday_to_sunday )
{
	impl->day_names = clan::StringHelp::split_text(comma_separated_monday_to_sunday, ",");
}

void CalendarComponent::set_day_short_names( std::string comma_separated_monday_to_sunday )
{
	impl->day_short_names = clan::StringHelp::split_text(comma_separated_monday_to_sunday, ",");
}

void CalendarComponent::set_month_names( std::string comma_separated_monday_to_sunday )
{
	impl->day_names = clan::StringHelp::split_text(comma_separated_monday_to_sunday, ",");
}

void CalendarComponent::set_month_short_names( std::string comma_separated_monday_to_sunday )
{

}

void CalendarComponent::clear_selection()
{
	impl->selection_valid = false;
}

//////////////////////////////////////////////////////////////////////////
// Signals

clan::Callback_v0 &CalendarComponent::func_selection_changed()
{
	return impl->func_selection_changed;
}

//////////////////////////////////////////////////////////////////////////
// Implementation

void CalendarComponent_Impl::on_render(clan::Canvas &canvas, const clan::Rect &update_rect)
{
	//////////////////////////////////////////////////////////////////////////
	// Header: month navigation, current month & year text

	// Draw prev & next month graphics
	part_prev_month.render_box(canvas, prev_month_box);
	part_next_month.render_box(canvas, next_month_box);

	// Month name & year
	part_month_year.render_box(canvas, month_year_box);
	part_month_year.render_text(canvas,
		month_names[date_view.get_month()-1] + " " + clan::StringHelp::int_to_text(date_view.get_year()),
		part_month_year.get_content_box(month_year_box));

	//////////////////////////////////////////////////////////////////////////
	// Day names row (mo, tu, we...)
	for (int i=0; i<7; i++)
	{
		clan::Rect day_name_box = get_day_name_box(i);
		clan::Rect day_name_content_box = part_day_name.get_content_box(day_name_box);
		part_day_name.render_box(canvas, day_name_box);
		part_day_name.render_text(canvas, day_short_names[(i+1)%7], day_name_content_box);
		day_name_box.translate(day_name_box.get_width(), 0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Day numbers

	clan::DateTime today = clan::DateTime::get_current_local_time();

	clan::Rect tmp_day_name_box = get_day_name_box(0);
	clan::Rect day_rect = part_day.get_css_size();
	day_rect.translate(content_box.left, tmp_day_name_box.bottom);

	int prev_month = date_view.get_month()-1;
	if (prev_month < 1)
		prev_month = 12;

	int days_in_month = get_days_in_month(date_view.get_month());
	int days_in_prev_month = get_days_in_month(prev_month);

	clan::DateTime first_of_month(date_view.get_year(), date_view.get_month(), 1);
	int num_days_of_prev_month = first_of_month.get_day_of_week()-1;
	if (num_days_of_prev_month < 1)
		num_days_of_prev_month += 7;

	int day_number = days_in_prev_month - num_days_of_prev_month + 1;

	part_day.set_pseudo_class("filler", true);
	bool drawing_prev_month_days = true;
	bool drawing_next_month_days = false;

	for (int w=0; w<6; w++)
	{
		clan::Rect day_rect_iterator = day_rect;

		for (int d=0;d<7;d++)
		{
			if (clan::Vec2i(d,w) == hot_xy_index)
			{
				part_day.set_pseudo_class("hot", true);
			}

			if (!drawing_prev_month_days && !drawing_next_month_days)
			{
				if (today.get_year() == date_view.get_year() &&
					today.get_month() == date_view.get_month() && 
					today.get_day() == day_number)
				{
					part_day.set_pseudo_class("today", true);
				}
				if (selection_valid && 
					date_selected.get_day() == day_number && 
					date_selected.get_month() == date_view.get_month() && 
					date_selected.get_year() == date_view.get_year())
				{
					part_day.set_pseudo_class("selected", true);
				}
			}

			part_day.render_box(canvas, day_rect_iterator);
			clan::Rect day_content_box = part_day.get_content_box(day_rect_iterator);
			part_day.render_text(canvas, clan::StringHelp::int_to_text(day_number), day_content_box);
			day_rect_iterator.translate(day_rect.get_width(), 0);

			day_table[w][d] = day_number; // update table for clicking.

			if (clan::Vec2i(d,w) == hot_xy_index)
			{
				part_day.set_pseudo_class("hot", false);
			}
			if (!drawing_prev_month_days && !drawing_next_month_days)
			{
				if (today.get_year() == date_view.get_year() &&
					today.get_month() == date_view.get_month() && 
					today.get_day() == day_number)
				{
					part_day.set_pseudo_class("today", false);
				}
				if (selection_valid && 
					date_selected.get_day() == day_number && 
					date_selected.get_month() == date_view.get_month() && 
					date_selected.get_year() == date_view.get_year())
				{
					part_day.set_pseudo_class("selected", false);
				}
			}

			day_number++;

			if (drawing_prev_month_days)
			{
				if (day_number > days_in_prev_month )
				{
					drawing_prev_month_days = false;
					day_number = 1;
					part_day.set_pseudo_class("filler", false);
				}
			}
			else
			{
				if (day_number > days_in_month)
				{
					day_number = 1;
					part_day.set_pseudo_class("filler", true);
					drawing_next_month_days = true;
				}
			}
		}

		day_rect.translate(0, day_rect.get_height());
	}
}

void CalendarComponent_Impl::on_process_message(std::shared_ptr<clan::GUIMessage> &msg)
{
	if (!component->is_enabled())
		return;

	std::shared_ptr<clan::GUIMessage_Input> input_msg = std::dynamic_pointer_cast<clan::GUIMessage_Input>(msg);
	if (input_msg)
	{
		const clan::InputEvent &input_event = input_msg->input_event;
		if (input_event.type == clan::InputEvent::pointer_moved)
			on_mouse_move(input_event);
		else if (input_event.type == clan::InputEvent::pressed && input_event.id == clan::mouse_left)
			on_mouse_left_down(input_event);
		else if (input_event.type == clan::InputEvent::released && input_event.id == clan::mouse_left)
			on_mouse_left_up(input_event);
	}

	std::shared_ptr<clan::GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<clan::GUIMessage_Pointer>(msg);
	if (pointer)
	{
		if (pointer->pointer_type == clan::GUIMessage_Pointer::pointer_leave)
			on_mouse_leave();
	}
}

void CalendarComponent_Impl::on_enablemode_changed()
{

}

void CalendarComponent_Impl::create_parts()
{
	part_next_month = clan::GUIThemePart(component, "next_month");
	part_prev_month = clan::GUIThemePart(component, "prev_month");
	part_day_name = clan::GUIThemePart(component, "day_name");
	part_day = clan::GUIThemePart(component, "day");
	part_week = clan::GUIThemePart(component, "week");
	part_month_year = clan::GUIThemePart(component, "month_year");
}

int CalendarComponent_Impl::get_days_in_month( int month )
{
	if (month < 1 || month > 12)
		throw clan::Exception("CalendarComponent: Month parameter not in 1-12 interval.");

	int days_per_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

	int num_days = days_per_month[month-1];

	if (month == 2)
	{
		int year = date_view.get_year();
		bool leap_year = (year%4) == 0; // Leap years are every 4th year
		if ((year%100) == 0 && (year%400) != 0) // Except for those divisible by 100 unless they are also divisible by 400
			leap_year = false;
		if (leap_year)
		{
			num_days = 29;
		}
	}

	return num_days;
}

void CalendarComponent_Impl::on_mouse_move(const clan::InputEvent &input_event)
{
	bool changed = false;

	clan::Point mouse_pos = input_event.mouse_pos;

	clan::Vec2i xy_index = get_xy_index_at_pos(mouse_pos);
	if (xy_index != hot_xy_index)
	{
		hot_xy_index = xy_index;
		changed = true;
	}

	changed |= part_prev_month.set_pseudo_class("hot", prev_month_box.contains(mouse_pos));
	changed |= part_next_month.set_pseudo_class("hot", next_month_box.contains(mouse_pos));
	changed |= part_month_year.set_pseudo_class("hot", month_year_box.contains(mouse_pos));

	if (changed)
		component->request_repaint();
}

void CalendarComponent_Impl::on_mouse_left_down(const clan::InputEvent &input_event)
{
	if (next_month_box.contains(input_event.mouse_pos))
	{
		int month = date_view.get_month();
		int year = date_view.get_year();

		month++;
		if (month > 12)
		{
			month = 1;
			year++;
			date_view.set_year(year);
		}
		date_view.set_month(month);
		component->request_repaint();
	}
	else if (prev_month_box.contains(input_event.mouse_pos))
	{
		int month = date_view.get_month();
		int year = date_view.get_year();

		month--;
		if (month < 1)
		{
			month = 12;
			year--;
			date_view.set_year(year);
		}
		date_view.set_month(month);
		component->request_repaint();
	}
	else
	{
		int tmp_day = get_day_at_pos(input_event.mouse_pos);
		if (tmp_day != -1)
		{
			if (selection_valid)
			{
				if (tmp_day != date_selected.get_day())
				{
					date_selected = date_view;
					date_selected.set_day(tmp_day);
					if (!func_selection_changed.is_null())
						func_selection_changed.invoke();
					component->request_repaint();
				}
			}
			else
			{
				date_selected = date_view;
				date_selected.set_day(tmp_day);
				selection_valid = true;
				if (!func_selection_changed.is_null())
					func_selection_changed.invoke();
				component->request_repaint();
			}
		}
	}
}

void CalendarComponent_Impl::on_mouse_left_up(const clan::InputEvent &input_event)
{

}

void CalendarComponent_Impl::on_resize()
{
	content_box = component->get_content_box();

	prev_month_box = part_next_month.get_css_size();
	prev_month_box.translate(content_box.left, content_box.top);

	next_month_box = part_next_month.get_css_size();
	next_month_box.translate(content_box.right - next_month_box.get_width(), content_box.top);

	month_year_box = clan::Rect(prev_month_box.right, content_box.top, next_month_box.left, next_month_box.bottom);

	component->request_repaint();
}

clan::Rect CalendarComponent_Impl::get_day_name_box(int day)
{
	clan::Rect box = part_day_name.get_css_size();
	box.translate(content_box.left, prev_month_box.bottom);
	box.translate(day * box.get_width(), 0);
	return box;
}

void CalendarComponent_Impl::on_mouse_leave()
{
	hot_xy_index = clan::Vec2i(-1,-1);
	part_month_year.set_pseudo_class("hot", false);
	part_prev_month.set_pseudo_class("hot", false);
	part_next_month.set_pseudo_class("hot", false);
	component->request_repaint();
}

clan::Vec2i CalendarComponent_Impl::get_xy_index_at_pos(const clan::Point &pos)
{
	clan::Rect day_name_box = get_day_name_box(0);
	clan::Rect day_box = part_day.get_css_size();
	clan::Rect days_area(clan::Point(day_name_box.left, day_name_box.bottom), clan::Size(day_box.right*7, day_box.bottom*6));

	clan::Vec2i xy_index(-1,-1);
	if (days_area.contains(pos))
	{
		xy_index = (pos - days_area.get_top_left()) / day_box.get_bottom_right();
	}

	return xy_index;
}

int CalendarComponent_Impl::get_day_at_pos(const clan::Point &pos)
{
	clan::Vec2i xy_index = get_xy_index_at_pos(pos);

	int day = day_table[xy_index.y][xy_index.x];

	if (xy_index.y <= 1 && day > 15)
		day = -1;
	else if (xy_index.y > 3 && day < 15)
		day = -1;

	return day;
}

