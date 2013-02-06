
#include "precomp.h"
#include "drag_drop_operation.h"
#include "solution_explorer.h"
#include "UIController/document_editor.h"

using namespace clan;

DragDropOperation::DragDropOperation(SolutionExplorer *solution_explorer)
: GUIComponent(solution_explorer, "drag-drop")
{
	func_input_pointer_moved().set(this, &DragDropOperation::on_input_pointer_moved);
	func_input_released().set(this, &DragDropOperation::on_input_released);
	set_visible(false, false);
}

void DragDropOperation::begin_drag()
{
	capture_mouse(true);
}

SolutionExplorer *DragDropOperation::get_solution_explorer()
{
	return static_cast<SolutionExplorer*>(get_parent_component());
}

bool DragDropOperation::on_input_pointer_moved(const InputEvent &e)
{
	GUIComponent *c = find_known_component(get_top_level_component_at(e.mouse_pos));
	if (c == get_solution_explorer()->listview)
	{
		set_cursor(drag_allowed_cursor());
	}
	else if (dynamic_cast<DocumentEditor*>(c))
	{
		set_cursor(drag_allowed_cursor());
	}
	else
	{
		set_cursor(cursor_no);
	}
	return true;
}

bool DragDropOperation::on_input_released(const InputEvent &e)
{
	if (e.id == mouse_left)
	{
		capture_mouse(false);
		set_cursor(cursor_arrow);

		GUIComponent *c = find_known_component(get_top_level_component_at(e.mouse_pos));
		if (dynamic_cast<DocumentEditor*>(c))
		{
			InputEvent event_local = e;
			event_local.mouse_pos = c->window_to_component_coords(component_to_window_coords(e.mouse_pos));
			static_cast<DocumentEditor*>(c)->files_dropped(get_solution_explorer()->get_selected_filenames(), event_local);
		}
	}
	return true;
}

StandardCursor DragDropOperation::drag_allowed_cursor()
{
	return cursor_hand; // To do: we need our own special icon indicating a drag'n'drop operation
}

GUIComponent *DragDropOperation::get_top_level_component_at(Point p)
{
	return get_top_level_component()->get_component_at(get_top_level_component()->window_to_component_coords(component_to_window_coords(p)));
}

GUIComponent *DragDropOperation::find_known_component(GUIComponent *c)
{
	while (c)
	{
		DocumentEditor *editor = dynamic_cast<DocumentEditor*>(c);
		if (c == get_solution_explorer()->listview || c == editor)
			break;
		c = c->get_parent_component();
	}
	return c;
}
