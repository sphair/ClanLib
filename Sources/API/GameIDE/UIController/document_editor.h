
#pragma once

#include "../Workspace/dockable_component.h"

class UIController;

class DocumentEditor : public DockableComponent
{
public:
	DocumentEditor(UIController *controller, std::string filename);

	UIController *get_controller() { return controller; }
	std::string get_filename() { return filename; }

	void set_dirty(bool dirty);

	virtual void cut() { }
	virtual void paste() { }
	virtual void copy() { }

	virtual void undo() { }
	virtual void redo() { }

	virtual void save() { }

	virtual void files_dropped(const std::vector<std::string> &filenames, const clan::InputEvent &e) { }

private:
	void on_visibility_change(bool new_state);

	UIController *controller;
	std::string filename;

	bool dirty;
};
