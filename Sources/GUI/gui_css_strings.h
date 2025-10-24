/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
		const CL_StringRef type_name = "checkbox";
		const CL_StringRef part_checker = "checker";
		const CL_StringRef part_focus = "focus";
	}

	namespace ComboBox
	{
		const CL_StringRef type_name = "combobox";
	}

	namespace GroupBox
	{
		const CL_StringRef type_name = "groupbox";
	}

	namespace Frame
	{
		const CL_StringRef type_name = "frame";
		const CL_StringRef part_header = "header";
		const CL_StringRef part_header_left = "headerleft";
		const CL_StringRef part_header_caption = "headercaption";
		const CL_StringRef part_header_right = "headerright";
		const CL_StringRef part_body = "body";
	}

	namespace ImageView
	{
		const CL_StringRef type_name = "imageview";
	}

	namespace LineEdit
	{
		const CL_StringRef type_name = "lineedit";
		const CL_StringRef part_selection = "selection";
		const CL_StringRef part_cursor = "cursor";
	}

	namespace Label
	{
		const CL_StringRef type_name = "label";
	}

	namespace MenuBar
	{
		const CL_StringRef type_name = "menubar";
		const CL_StringRef part_item = "item";
	}

	namespace ListView
	{
		const CL_StringRef type_name = "listview";
		const CL_StringRef part_row = "row";
		const CL_StringRef part_cell = "cell";
		const CL_StringRef part_opener = "row_opener";
		const CL_StringRef part_column = "column";
		const CL_StringRef part_columns_bg = "columns_bg";
		const CL_StringRef part_selection = "selection";
		const CL_StringRef part_icon_selection = "icon_selection";
		const CL_StringRef part_mouse_selection = "mouse_selection";

		const CL_StringRef part_selection_margin_left = "selection-margin-left";
		const CL_StringRef part_selection_margin_right = "selection-margin-right";
		const CL_StringRef part_selection_margin_bottom = "selection-margin-bottom";
		const CL_StringRef part_selection_margin_top = "selection-margin-top";

		namespace Header
		{
			const CL_StringRef type_name = "listview_header";
		}
	}

	namespace ProgressBar
	{
		const CL_StringRef type_name = "progressbar";
	}

	namespace PushButton
	{
		const CL_StringRef type_name = "button";
		const CL_StringRef part_focus = "focus";
	}

	namespace PopupMenuWindow
	{
		const CL_StringRef type_name = "popupmenu";
		const CL_StringRef part_item_row = "row";
		const CL_StringRef part_item_icon = "icon";
		const CL_StringRef part_item_label = "label";
		const CL_StringRef part_item_check = "check";
		const CL_StringRef part_separator = "separator";
		const CL_StringRef part_submenu_arrow = "submenu_arrow";
		const CL_StringRef part_item_accel_label = "accel_label";
		const CL_StringRef part_menubar_joiner = "joiner";
	}

	namespace RadioButton
	{
		const CL_StringRef type_name = "radiobutton";
		const CL_StringRef part_checker = "checker";
		const CL_StringRef part_focus = "focus";
	}

	namespace ScrollBar
	{
		const CL_StringRef type_name = "scrollbar";
	}

	namespace Spin
	{
		const CL_StringRef type_name = "spin";
		const CL_StringRef part_button_down = "button_down";
		const CL_StringRef part_button_up = "button_up";
		const CL_StringRef part_arrow_down = "arrow_down";
		const CL_StringRef part_arrow_up = "arrow_up";
	}

	namespace Slider
	{
		const CL_StringRef type_name = "slider";
		const CL_StringRef part_track_horizontal = "slidertrack.horizontal";
		const CL_StringRef part_track_vertical = "slidertrack.vertical";
/*		const CL_StringRef part_track_start_vertical = "slidertrack_start.vertical";
		const CL_StringRef part_track_start_horizontal = "slidertrack_start.horizontal";
		const CL_StringRef part_track_end_vertical = "slidertrack_end.vertical";
		const CL_StringRef part_track_end_horizontal = "slidertrack_end.horizontal";
*/		const CL_StringRef part_thumb_vertical = "sliderthumb.vertical";
		const CL_StringRef part_thumb_horizontal = "sliderthumb.horizontal";
		const CL_StringRef part_focus = "focus";
	}

	namespace Tab
	{
		const CL_StringRef type_name = "tab";

		namespace Header
		{
			const CL_StringRef type_name = "tabheader";
			const CL_StringRef part_page = "page";
			const CL_StringRef part_focus = "focus";
		}
	}

	namespace TabPage
	{
		const CL_StringRef type_name = "tabpage";
	}

	namespace ToolBar
	{
		const CL_StringRef type_name = "toolbar";
		const CL_StringRef part_item = "item";
	}

	namespace Window
	{
		const CL_StringRef type_name = "window";
		const CL_StringRef part_caption = "caption";
		const CL_StringRef part_frameleft = "frameleft";
		const CL_StringRef part_frameright = "frameright";
		const CL_StringRef part_framebottom = "framebottom";
		const CL_StringRef part_buttonclose = "buttonclose";
	}


	// Theme part states:
	const CL_StringRef normal = "normal";                      // all components, text-weight
	const CL_StringRef hot = "hot";
	const CL_StringRef defaulted = "defaulted";
	const CL_StringRef disabled = "disabled";
	const CL_StringRef focused = "focused";
	const CL_StringRef unfocused = "unfocused";					// lineedit selection, listview selection
	const CL_StringRef pressed = "pressed";
	const CL_StringRef flat = "flat";							// pushbutton
	const CL_StringRef checked = "checked";						// checkbox, radiobutton
	const CL_StringRef unchecked = "unchecked";					// checkbox, radiobutton
	const CL_StringRef indeterminated = "indeterminated";		// checkbox, radiobutton
	const CL_StringRef closed = "closed";						// listview
	const CL_StringRef open = "open";							// listview
	const CL_StringRef alternate = "alternate";					// listview
	const CL_StringRef details = "details";						// listview
	const CL_StringRef icons = "icons";							// listview
	const CL_StringRef selected = "selected";					// Menubar, PopupMenuWindow, TabHeader
	const CL_StringRef on = "on";								// toolbar - todo: use selected instead?
	const CL_StringRef horizontal = "horizontal";				// toolbar

	// Theme part properties:
	const CL_StringRef text_color = "text-color";
	const CL_StringRef text_color_disabled = "text-color-disabled";						// todo: remove!
	const CL_StringRef text_gap	= "text-gap";											// check, radio, toolbar
	const CL_StringRef first_tab_x_offset = "first-tab-x-offset";						// TabHeader
	const CL_StringRef layout = "layout";												// toolbar
	const CL_StringRef gap_width = "gap-width";											// slider   todo: replace with "text-gap"
	const CL_StringRef icon_width = "icon-width";										// listview, toolbar
	const CL_StringRef icon_height = "icon-height";										// listview, toolbar

	const CL_StringRef align = "align";													// generic 
	const CL_StringRef vertical_align = "vertical-align";								// generic 

	// Theme part property values:
	const CL_StringRef left = "left";													// align
	const CL_StringRef center = "center";												// align, vertical-align
	const CL_StringRef right = "right";													// align
	const CL_StringRef justify = "justify";												// align - todo
	const CL_StringRef top = "top";														// vertical-align
	const CL_StringRef bottom = "bottom";												// vertical-align

	const CL_StringRef icon_color = "icon-color";										// listview
	const CL_StringRef icon_color_selected = "icon-color-selected";						// listview
	const CL_StringRef icon_color_overlay = "icon-color-overlay";						// listview
	const CL_StringRef icon_color_selected_overlay = "icon-color-selected-overlay";		// listview
	const CL_StringRef opener_gap = "opener-icon-gap";									// listview
	const CL_StringRef opener_offset_x = "offset-x";									// listview
	const CL_StringRef opener_offset_y = "offset-y";									// listview
	const CL_StringRef icon_text_gap = "icon-text-gap";									// listview
	const CL_StringRef icon_offset_y = "icon-offset-y";									// listview
	const CL_StringRef indent_width = "indent-width";									// listview

	const CL_StringRef lineedit_offset_left = "line-edit-offset-left";					// listview (lineedit for renaming items)
	const CL_StringRef lineedit_offset_top = "line-edit-offset-top";					// listview
	const CL_StringRef lineedit_offset_bottom = "line-edit-offset-bottom";				// listview
	const CL_StringRef lineedit_offset_right = "line-edit-offset-right";				// listview
};


