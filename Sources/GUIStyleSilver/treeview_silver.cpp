/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    Kenneth Gangstø
**    (if your name is missing here, please add it)
*/

#include "API/Display/display.h"
#include "API/Display/font.h"
#include "API/Display/keys.h"
#include "API/GUI/scrollbar.h"
#include "API/GUIStyleSilver/treeview_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeView_Silver::CL_TreeView_Silver(
	CL_TreeView *treeview,
	CL_StyleManager_Silver *style)
:
	CL_ComponentStyle(treeview),
	treeview(treeview),
	style(style)
{
	CL_ResourceManager *resources = style->get_resources();
	font = new CL_Font("TreeView/font", resources);
	font->set_color( CL_Color::black );

	CL_Component *client_area = treeview->get_client_area();
	client_area->set_clipping(true);

	// Create a vertical scrollbar
	scrollbar = new CL_ScrollBar(false, treeview);
	visible_scrollbar = true;

	// Create the header component
	header_area = new CL_Component(treeview);

	// Connect the dots
	slots.connect(treeview->sig_paint(), this, &CL_TreeView_Silver::on_paint);
	slots.connect(header_area->sig_paint(), this, &CL_TreeView_Silver::on_paint_header);
	slots.connect(client_area->sig_paint(), this, &CL_TreeView_Silver::on_paint_client);
	slots.connect(treeview->sig_paint_node(), this, &CL_TreeView_Silver::on_paint_node);

	slots.connect(client_area->sig_mouse_down(), this, &CL_TreeView_Silver::on_mouse_down);

	slots.connect(treeview->sig_resize(), this, &CL_TreeView_Silver::on_resize);
	slots.connect(treeview->sig_item_added(), this, &CL_TreeView_Silver::on_item_changed);
	slots.connect(treeview->sig_item_removed(), this, &CL_TreeView_Silver::on_item_changed);
	slots.connect(treeview->sig_item_expanded(), this, &CL_TreeView_Silver::on_item_changed);
	slots.connect(treeview->sig_item_collapsed(), this, &CL_TreeView_Silver::on_item_changed);
	slots.connect(treeview->sig_column_added(), this, &CL_TreeView_Silver::on_column_added);
	slots.connect(treeview->sig_column_removed(), this, &CL_TreeView_Silver::on_column_removed);
	slots.connect(treeview->sig_clear(), this, &CL_TreeView_Silver::on_clear);

	// Make sure we're arranged properly
	on_resize(0,0);
}

CL_TreeView_Silver::~CL_TreeView_Silver()
{
	delete header_area;
	delete scrollbar;
	delete font;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_TreeView_Silver::on_item_changed(const CL_TreeNode &node)
{
	update_scrollbar(true);
}

void CL_TreeView_Silver::on_column_added(int index)
{
}

void CL_TreeView_Silver::on_column_removed(int index)
{
}
                                                               
void CL_TreeView_Silver::on_clear()
{
	update_scrollbar(true);
}

void CL_TreeView_Silver::on_mouse_down(const CL_InputEvent &key)
{
	if(key.id == CL_MOUSE_WHEEL_UP)
	{
		scrollbar->decrease(16);
		return;
	}
	if(key.id == CL_MOUSE_WHEEL_DOWN)
	{
		scrollbar->increase(16);
		return;
	}
}

void CL_TreeView_Silver::on_resize(int old_width, int old_height)
{
	int header_height = 20;

	int width = treeview->get_width();
	int height = treeview->get_height();

	// Position the header area
	header_area->set_position(CL_Rect(0, 0, width, header_height));

	// Position the client area
	CL_Component *client_area = treeview->get_client_area();
	if(treeview->is_header_visible())
	{
		client_area->set_position(CL_Rect(1, header_height, width - 1, height - 1));
		header_area->show(true);
	}
	else
	{
		client_area->set_position(CL_Rect(1, 1, width - 1, height - 1));
		header_area->show(false);
	}

	update_scrollbar(false);

	// Scrollbar was added, shrink other areas
	if(visible_scrollbar)
	{
		client_area->set_width(client_area->get_width() - 19);
		header_area->set_width(header_area->get_width() - 19);

		// Position the scrollbar
		scrollbar->set_position(CL_Rect(width - 19, 0, width, height));
	}
}

void CL_TreeView_Silver::on_paint()
{
	// Outline
	CL_Rect rect = treeview->get_screen_rect();
	CL_Display::draw_rect(rect, CL_Color(128, 142, 159));
}

void CL_TreeView_Silver::on_paint_header()
{
	// Header
	if(treeview->is_header_visible())
	{
		CL_Rect rect = header_area->get_screen_rect();

		// Header background
		CL_Display::fill_rect(
			CL_Rect(
				rect.left + 1,
				rect.top + 1,
				rect.left + rect.get_width() - 1,
				rect.top + rect.get_height() - 4),
			CL_Gradient(
				CL_Color(245, 246, 247), CL_Color(245, 246, 247), CL_Color(235, 238, 241), CL_Color(235, 238, 241)));

		CL_Display::fill_rect(
			CL_Rect(
				rect.left + 1,
				rect.top + rect.get_height() - 4,
				rect.left + rect.get_width() - 1,
				rect.top + rect.get_height()),
			CL_Gradient(
				CL_Color(235, 238, 241), CL_Color(235, 238, 241), CL_Color(186, 187, 188), CL_Color(186, 187, 188)));

		// Draw columns
		int x = 0;
		int columns = treeview->get_column_count();
		for(int i = 0; i < columns; ++i)
		{
			font->draw(
				rect.left + x + 6,
				rect.top + 4, treeview->get_column_name(i));
				
			x += treeview->get_column_width(i);
			
			CL_Display::draw_line(
				rect.left + x,
				rect.top + 1,
				rect.left + x,
				rect.top + rect.get_height(),
				CL_Color(128, 142, 159));
			CL_Display::draw_line(
				rect.left + x + 1,
				rect.top + 1,
				rect.left + x + 1,
				rect.top + rect.get_height(),
				CL_Color::white);
		}
	}
}

void CL_TreeView_Silver::on_paint_client()
{
	// Draw white background
	CL_Rect rect = treeview->get_client_area()->get_screen_rect();
	CL_Display::fill_rect(rect, CL_Color::white);

	// Draw nodes
	CL_Point point(0, 0);
	treeview->draw_nodes(point);
}

void CL_TreeView_Silver::on_paint_node(CL_TreeNode *node, CL_Point &point)
{
	CL_Component *component = node->get_component();
	CL_Component *client_area = treeview->get_client_area();

	int height = component->get_height();
	int mid = (height) / 2;

	// Should we draw decoration ?
	bool draw_decoration = true;
	if(node->is_root())
		draw_decoration = node->get_treeview()->is_root_decoration_visible();

	// Find what y offset we're drawing at
	point.y -= scrollbar->get_value();

	// Find what x offset we're drawing at
	int x_offset = 0;
	if(draw_decoration)
		x_offset = 12;

	int screen_x = client_area->get_screen_x();
	int screen_y = client_area->get_screen_y();
	
	// Is the node visible?
	if (point.y + height >= 0 && point.y <= client_area->get_height())
	{
		if(draw_decoration)
		{
			// Draw collapse box
			if(node->has_children())
			{
				CL_Display::draw_rect(
					CL_Rect(
						screen_x + point.x,
						screen_y + point.y + mid - 5,
						screen_x + point.x + 9,
						screen_y + point.y + mid + 4),
					CL_Color(128, 142, 159));

				CL_Display::fill_rect(
					CL_Rect(
						screen_x + point.x + 1,
						screen_y + point.y + mid - 4,
						screen_x + point.x + 8,
						screen_y + point.y + mid + 3),
					CL_Gradient(
						CL_Color::white,
						CL_Color::white,
						CL_Color(230, 235, 240),
						CL_Color(230, 235, 240)));

				CL_Display::fill_rect(
					CL_Rect(
						screen_x + point.x + 2,
						screen_y + point.y + mid - 3,
						screen_x + point.x + 7,
						screen_y + point.y + mid + 2),
					CL_Gradient(
						CL_Color(217, 222, 227),
						CL_Color(217, 222, 227),
						CL_Color::white,
						CL_Color::white));

				// Horizontal line
				CL_Display::draw_line(
					screen_x + point.x + 2.0f,
					screen_y + point.y + mid - 1.0f,
					screen_x + point.x + 7.0f,
					screen_y + point.y + mid - 1,
					CL_Color::black);

				// Vertical line
				if(node->is_collapsed())
					CL_Display::draw_line(
						screen_x + point.x + 4.0f,
						screen_y + point.y + mid - 3.0f,
						screen_x + point.x + 4.0f,
						screen_y + point.y + mid + 2.0f,
						CL_Color::black);
			}
		}

		// Size and place child component
		component->set_position(point.x + x_offset, point.y);
		node->set_placement_offset(point.x + x_offset);

		// Draw component
		component->show(true);
		component->sig_begin_paint();
		component->sig_paint()();
		component->sig_end_paint();
	}
	else
	{
		component->show(false);
	}

	// Move down for next component
	point.y += scrollbar->get_value();
	point.y += height;

	// If it has any children, adjust the x-position for the children
	if(node->is_collapsed() == false)
		point.x += 3 + x_offset;
}

void CL_TreeView_Silver::update_scrollbar(bool resize)
{
	int max_height = treeview->get_items_height();
	int component_height = treeview->get_client_area()->get_height();

	if(max_height > component_height)
	{
		if(visible_scrollbar == false)
		{
			visible_scrollbar = true;
			treeview->add_child(scrollbar, false);

			if(resize)
				on_resize(0, 0);
		}

		scrollbar->set_max_value(max_height - component_height + 1);
		scrollbar->set_value(0);
	}
	else
	{
		if(visible_scrollbar)
		{
			visible_scrollbar = false;
			scrollbar->set_range(0, 0);
			scrollbar->set_value(0);
			treeview->remove_child(scrollbar);

			if(resize)
				on_resize(0, 0);
		}
	}
}
