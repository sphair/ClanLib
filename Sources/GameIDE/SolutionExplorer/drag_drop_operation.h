
#pragma once

class SolutionExplorer;

class DragDropOperation : public clan::GUIComponent
{
public:
	DragDropOperation(SolutionExplorer *solution_explorer);
	void begin_drag();

private:
	SolutionExplorer *get_solution_explorer();
	bool on_input_pointer_moved(const clan::InputEvent &e);
	bool on_input_released(const clan::InputEvent &e);
	clan::GUIComponent *get_top_level_component_at(clan::Point p);
	clan::GUIComponent *find_known_component(clan::GUIComponent *c);
	static clan::StandardCursor drag_allowed_cursor();
};
