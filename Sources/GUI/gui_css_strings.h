/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
*/

#pragma once


namespace CssStr
{
	// Component & part names:
	namespace CheckBox
	{
		const CL_StringRef type_name = cl_text("checkbox");
		const CL_StringRef part_checker = cl_text("checker");
		const CL_StringRef part_focus = cl_text("focus");
	}

	namespace ComboBox
	{
		const CL_StringRef type_name = cl_text("combobox");
	}

	namespace GroupBox
	{
		const CL_StringRef type_name = cl_text("groupbox");
	}

	namespace Frame
	{
		const CL_StringRef type_name = cl_text("frame");
		const CL_StringRef part_header = cl_text("header");
		const CL_StringRef part_header_left = cl_text("headerleft");
		const CL_StringRef part_header_caption = cl_text("headercaption");
		const CL_StringRef part_header_right = cl_text("headerright");
		const CL_StringRef part_body = cl_text("body");
	}

	namespace ImageView
	{
		const CL_StringRef type_name = cl_text("imageview");
	}

	namespace LineEdit
	{
		const CL_StringRef type_name = cl_text("lineedit");
		const CL_StringRef part_selection = cl_text("selection");
		const CL_StringRef part_cursor = cl_text("cursor");
	}

	namespace Label
	{
		const CL_StringRef type_name = cl_text("label");
	}

	namespace MenuBar
	{
		const CL_StringRef type_name = cl_text("menubar");
		const CL_StringRef part_item = cl_text("item");
	}

	namespace ListView
	{
		const CL_StringRef type_name = cl_text("listview");
		const CL_StringRef part_row = cl_text("row");
		const CL_StringRef part_cell = cl_text("cell");
		const CL_StringRef part_opener = cl_text("row_opener");
		const CL_StringRef part_column = cl_text("column");
		const CL_StringRef part_columns_bg = cl_text("columns_bg");
		const CL_StringRef part_selection = cl_text("selection");
		const CL_StringRef part_icon_selection = cl_text("icon_selection");
		const CL_StringRef part_mouse_selection = cl_text("mouse_selection");

		namespace Header
		{
			const CL_StringRef type_name = cl_text("listview_header");
		}
	}

	namespace ProgressBar
	{
		const CL_StringRef type_name = cl_text("progressbar");
	}

	namespace PushButton
	{
		const CL_StringRef type_name = cl_text("button");
		const CL_StringRef part_focus = cl_text("focus");
	}

	namespace PopupMenuWindow
	{
		const CL_StringRef type_name = cl_text("popupmenu");
		const CL_StringRef part_item_row = cl_text("row");
		const CL_StringRef part_item_icon = cl_text("icon");
		const CL_StringRef part_item_label = cl_text("label");
		const CL_StringRef part_separator = cl_text("separator");
		const CL_StringRef part_submenu_arrow = cl_text("submenu_arrow");
		const CL_StringRef part_item_accel_label = cl_text("accel_label");
		const CL_StringRef part_menubar_joiner = cl_text("joiner");
	}

	namespace RadioButton
	{
		const CL_StringRef type_name = cl_text("radiobutton");
		const CL_StringRef part_checker = cl_text("checker");
	}

	namespace ScrollBar
	{
		const CL_StringRef type_name = cl_text("scrollbar");
	}

	namespace Spin
	{
		const CL_StringRef type_name = cl_text("spin");
		const CL_StringRef part_button_down = cl_text("button_down");
		const CL_StringRef part_button_up = cl_text("button_up");
		const CL_StringRef part_arrow_down = cl_text("arrow_down");
		const CL_StringRef part_arrow_up = cl_text("arrow_up");
	}

	namespace Slider
	{
		const CL_StringRef type_name = cl_text("slider");
		const CL_StringRef part_track_horizontal = cl_text("slidertrack.horizontal");
		const CL_StringRef part_track_vertical = cl_text("slidertrack.vertical");
/*		const CL_StringRef part_track_start_vertical = cl_text("slidertrack_start.vertical");
		const CL_StringRef part_track_start_horizontal = cl_text("slidertrack_start.horizontal");
		const CL_StringRef part_track_end_vertical = cl_text("slidertrack_end.vertical");
		const CL_StringRef part_track_end_horizontal = cl_text("slidertrack_end.horizontal");
*/		const CL_StringRef part_thumb_vertical = cl_text("sliderthumb.vertical");
		const CL_StringRef part_thumb_horizontal = cl_text("sliderthumb.horizontal");
	}

	namespace Tab
	{
		const CL_StringRef type_name = cl_text("tab");

		namespace Header
		{
			const CL_StringRef type_name = cl_text("tabheader");
			const CL_StringRef part_page = cl_text("page");
		}
	}

	namespace TabPage
	{
		const CL_StringRef type_name = cl_text("tabpage");
	}

	namespace ToolBar
	{
		const CL_StringRef type_name = cl_text("toolbar");
		const CL_StringRef part_item = cl_text("item");
	}

	namespace Window
	{
		const CL_StringRef type_name = cl_text("window");
		const CL_StringRef part_caption = cl_text("caption");
		const CL_StringRef part_frameleft = cl_text("frameleft");
		const CL_StringRef part_frameright = cl_text("frameright");
		const CL_StringRef part_framebottom = cl_text("framebottom");
		const CL_StringRef part_buttonclose = cl_text("buttonclose");
	}


	// Theme part states:
	const CL_StringRef normal = cl_text("normal");                      // all components, text-weight
	const CL_StringRef hot = cl_text("hot");
	const CL_StringRef defaulted = cl_text("defaulted");
	const CL_StringRef disabled = cl_text("disabled");
	const CL_StringRef focused = cl_text("focused");
	const CL_StringRef unfocused = cl_text("unfocused");				// lineedit selection, listview selection
	const CL_StringRef pressed = cl_text("pressed");
	const CL_StringRef flat = cl_text("flat");							// pushbutton
	const CL_StringRef checked = cl_text("checked");					// checkbox, radiobutton
	const CL_StringRef unchecked = cl_text("unchecked");				// checkbox, radiobutton
	const CL_StringRef indeterminated = cl_text("indeterminated");		// checkbox, radiobutton
	const CL_StringRef closed = cl_text("closed");						// listview
	const CL_StringRef open = cl_text("open");							// listview
	const CL_StringRef alternate = cl_text("alternate");				// listview
	const CL_StringRef details = cl_text("details");					// listview
	const CL_StringRef icons = cl_text("icons");						// listview
	const CL_StringRef selected = cl_text("selected");					// Menubar, PopupMenuWindow, TabHeader
	const CL_StringRef on = cl_text("on");								// toolbar - todo: use selected instead?
	const CL_StringRef horizontal = cl_text("horizontal");				// toolbar

	// Theme part properties:
	const CL_StringRef text_color = cl_text("text-color");
	const CL_StringRef text_color_disabled = cl_text("text-color-disabled");					// todo: remove!
	const CL_StringRef text_gap	= cl_text("text-gap");											// check, radio, toolbar
	const CL_StringRef first_tab_x_offset = cl_text("first-tab-x-offset");						// TabHeader
	const CL_StringRef layout = cl_text("layout");												// toolbar
	const CL_StringRef gap_width = cl_text("gap-width");										// slider   todo: replace with "text-gap"
	const CL_StringRef icon_width = cl_text("icon-width");										// listview, toolbar
	const CL_StringRef icon_height = cl_text("icon-height");									// listview, toolbar

	const CL_StringRef align = cl_text("align");												// generic 
	const CL_StringRef vertical_align = cl_text("vertical-align");								// generic 

	// Theme part property values:
	const CL_StringRef left = cl_text("left");													// align
	const CL_StringRef center = cl_text("center");												// align, vertical-align
	const CL_StringRef right = cl_text("right");												// align
	const CL_StringRef justify = cl_text("justify");											// align - todo
	const CL_StringRef top = cl_text("top");													// vertical-align
	const CL_StringRef bottom = cl_text("bottom");												// vertical-align

	const CL_StringRef icon_color = cl_text("icon-color");										// listview
	const CL_StringRef icon_color_selected = cl_text("icon-color-selected");					// listview
	const CL_StringRef icon_color_overlay = cl_text("icon-color-overlay");						// listview
	const CL_StringRef icon_color_selected_overlay = cl_text("icon-color-selected-overlay");	// listview
	const CL_StringRef opener_gap = cl_text("opener-icon-gap");									// listview
	const CL_StringRef opener_offset_x = cl_text("offset-x");									// listview
	const CL_StringRef opener_offset_y = cl_text("offset-y");									// listview
	const CL_StringRef icon_text_gap = cl_text("icon-text-gap");								// listview
	const CL_StringRef icon_offset_y = cl_text("icon-offset-y");								// listview
	const CL_StringRef indent_width = cl_text("indent-width");									// listview

	const CL_StringRef lineedit_offset_left = cl_text("line-edit-offset-left");					// listview (lineedit for renaming items)
	const CL_StringRef lineedit_offset_top = cl_text("line-edit-offset-top");					// listview
	const CL_StringRef lineedit_offset_bottom = cl_text("line-edit-offset-bottom");				// listview
	const CL_StringRef lineedit_offset_right = cl_text("line-edit-offset-right");				// listview
};


