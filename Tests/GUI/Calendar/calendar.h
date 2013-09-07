#pragma once

#include <ClanLib/core.h>

class CalendarComponent_Impl;

class CalendarComponent : public clan::GUIComponent
{
// Construction:
public:
	CalendarComponent(clan::GUIComponent *parent);

// Attributes:
public:
	virtual clan::Size get_css_size() const;

	clan::DateTime get_selected_date() const;

// Signals:
public:
	void clear_selection();

	clan::Callback_v0 &func_selection_changed();

// Operations:
public:
	void set_current_date(const clan::DateTime &date);

	void set_day_names(std::string comma_separated_monday_to_sunday);

	void set_day_short_names(std::string comma_separated_monday_to_sunday);

	void set_month_names(std::string comma_separated_monday_to_sunday);

	void set_month_short_names(std::string comma_separated_monday_to_sunday);

private:
	std::shared_ptr<CalendarComponent_Impl> impl;
};