#pragma once

#include <ClanLib/core.h>

class CL_CalendarComponent_Impl;

class CL_CalendarComponent : public CL_GUIComponent
{
// Construction:
public:
	CL_CalendarComponent(CL_GUIComponent *parent);

// Attributes:
public:
	virtual CL_Size get_preferred_size() const;

	CL_DateTime get_selected_date() const;

// Signals:
public:
	void clear_selection();

	CL_Callback_v0 &func_selection_changed();

// Operations:
public:
	void set_current_date(const CL_DateTime &date);

	void set_day_names(CL_String comma_separated_monday_to_sunday);

	void set_day_short_names(CL_String comma_separated_monday_to_sunday);

	void set_month_names(CL_String comma_separated_monday_to_sunday);

	void set_month_short_names(CL_String comma_separated_monday_to_sunday);

private:
	CL_SharedPtr<CL_CalendarComponent_Impl> impl;
};