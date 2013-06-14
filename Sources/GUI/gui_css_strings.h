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
**    Harry Storbacka
*/

#pragma once

namespace clan
{

namespace CssStr
{
	// Component & part names:
	namespace CheckBox
	{
		const std::string type_name = "checkbox";
		const std::string part_checker = "checker";
		const std::string part_focus = "focus";
	}

	namespace ComboBox
	{
		const std::string type_name = "combobox";
		const std::string part_opener = "opener";
		const std::string part_opener_glyph = "opener_glyph";
	}

	namespace GroupBox
	{
		const std::string type_name = "groupbox";
	}

	namespace DragBox
	{
		const std::string type_name = "dragbox";
	}
	namespace MessageBox
	{
		const std::string type_name = "messagebox";
		const std::string button_area_height = "button-area-height";
		const std::string part_icon = "icon";
	}

	namespace Frame
	{
		const std::string type_name = "frame";
		const std::string part_header = "header";
		const std::string part_header_left = "headerleft";
		const std::string part_header_icon = "headericon";
		const std::string part_header_caption = "headercaption";
		const std::string part_header_right = "headerright";
		const std::string part_body = "body";
	}

	namespace ImageView
	{
		const std::string type_name = "imageview";
	}

	namespace LineEdit
	{
		const std::string type_name = "lineedit";
		const std::string part_selection = "selection";
		const std::string part_cursor = "cursor";
	}

	namespace TextEdit
	{
		const std::string type_name = "textedit";
		const std::string part_selection = "selection";
		const std::string part_cursor = "cursor";
	}

	namespace Label
	{
		const std::string type_name = "label";
	}

	namespace LabelSpan
	{
		const std::string type_name = "label_span";
	}

	namespace MenuBar
	{
		const std::string type_name = "menubar";
		const std::string part_item = "item";
	}

	namespace ListView
	{
		const std::string type_name = "listview";
		const std::string part_row = "row";
		const std::string part_cell = "cell";
		const std::string part_opener = "row_opener";
		const std::string part_column = "column";
		const std::string part_columns_bg = "columns_bg";
		const std::string part_selection = "selection";
		const std::string part_icon_selection = "icon_selection";
		const std::string part_mouse_selection = "mouse_selection";

		const std::string part_selection_margin_left = "selection-margin-left";
		const std::string part_selection_margin_right = "selection-margin-right";
		const std::string part_selection_margin_bottom = "selection-margin-bottom";
		const std::string part_selection_margin_top = "selection-margin-top";

		namespace Header
		{
			const std::string type_name = "listview_header";
		}
	}

	namespace ProgressBar
	{
		const std::string type_name = "progressbar";
	}

	namespace PushButton
	{
		const std::string type_name = "button";
		const std::string part_focus = "focus";
	}

	namespace PopupMenuWindow
	{
		const std::string type_name = "popupmenu";
		const std::string part_item_row = "row";
		const std::string part_item_icon = "icon";
		const std::string part_item_label = "label";
		const std::string part_item_check = "check";
		const std::string part_separator = "separator";
		const std::string part_submenu_arrow = "submenu_arrow";
		const std::string part_item_accel_label = "accel_label";
		const std::string part_menubar_joiner = "joiner";
	}

	namespace RadioButton
	{
		const std::string type_name = "radiobutton";
		const std::string part_checker = "checker";
		const std::string part_focus = "focus";
	}

	namespace ScrollBar
	{
		const std::string type_name = "scrollbar";
	}

	namespace StatusBar
	{
		const std::string type_name = "statusbar";
	}

	namespace Spin
	{
		const std::string type_name = "spin";
		const std::string part_button_down = "button_down";
		const std::string part_button_up = "button_up";
		const std::string part_arrow_down = "arrow_down";
		const std::string part_arrow_up = "arrow_up";
	}

	namespace Slider
	{
		const std::string type_name = "slider";
		const std::string part_focus = "focus";
	}

	namespace Tab
	{
		const std::string type_name = "tab";

		namespace Header
		{
			const std::string type_name = "tabheader";
			const std::string part_page = "page";
			const std::string part_focus = "focus";
		}
	}

	namespace TabPage
	{
		const std::string type_name = "tabpage";
	}

	namespace ToolBar
	{
		const std::string type_name = "toolbar";
		const std::string part_item = "item";
	}

	namespace ToolTip
	{
		const std::string type_name = "tooltip";
	}

	// Theme part states:
	const std::string normal = "normal";                      // all components, text-weight
	const std::string hover = "hover";
	const std::string defaulted = "defaulted";
	const std::string disabled = "disabled";
	const std::string focused = "focused";
	const std::string unfocused = "unfocused";					// lineedit selection, listview selection
	const std::string pressed = "pressed";
	const std::string toggled = "toggled";						// pushbutton
	const std::string checked = "checked";						// checkbox, radiobutton
	const std::string unchecked = "unchecked";					// checkbox, radiobutton
	const std::string indeterminated = "indeterminated";		// checkbox, radiobutton
	const std::string closed = "closed";						// listview
	const std::string open = "open";							// listview
	const std::string alternate = "alternate";					// listview
	const std::string details = "details";						// listview
	const std::string icons = "icons";							// listview
	const std::string selected = "selected";					// Menubar, PopupMenuWindow, TabHeader
	const std::string on = "on";								// toolbar - todo: use selected instead?
	const std::string horizontal = "horizontal";				// toolbar

	// Theme part properties:
	const std::string text_gap	= "text-gap";											// check, radio, toolbar
	const std::string first_tab_x_offset = "first-tab-x-offset";						// TabHeader
	const std::string layout = "layout";												// toolbar
	const std::string icon_width = "icon-width";										// listview, toolbar
	const std::string icon_height = "icon-height";										// listview, toolbar

	// Theme part property values:

	const std::string icon_color = "icon-color";										// listview
	const std::string icon_color_selected = "icon-color-selected";						// listview
	const std::string icon_color_overlay = "icon-color-overlay";						// listview
	const std::string icon_color_selected_overlay = "icon-color-selected-overlay";		// listview
	const std::string opener_gap = "opener-icon-gap";									// listview
	const std::string opener_offset_x = "offset-x";									// listview
	const std::string opener_offset_y = "offset-y";									// listview
	const std::string icon_text_gap = "icon-text-gap";									// listview
	const std::string icon_offset_y = "icon-offset-y";									// listview
	const std::string indent_width = "indent-width";									// listview

	const std::string lineedit_offset_left = "line-edit-offset-left";					// listview (lineedit for renaming items)
	const std::string lineedit_offset_top = "line-edit-offset-top";					// listview
	const std::string lineedit_offset_bottom = "line-edit-offset-bottom";				// listview
	const std::string lineedit_offset_right = "line-edit-offset-right";				// listview
}

}
