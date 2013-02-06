
#pragma once

class UIController;
class DocumentEditor;

class UIControllerListener
{
public:
	UIControllerListener(UIController *controller) : controller(controller) { }
	virtual ~UIControllerListener() { }
	virtual void set_active(DocumentEditor *editor) { }
	virtual void editor_destroyed(DocumentEditor *editor) { }

protected:
	UIController *get_controller() { return controller; }

private:
	UIController *controller;
};
