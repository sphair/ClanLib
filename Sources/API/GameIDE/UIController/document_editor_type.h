
#pragma once

class UIController;
class DocumentEditor;

class DocumentEditorType
{
public:
	virtual ~DocumentEditorType() { }
	virtual std::string get_name() = 0;
	virtual void register_ui(UIController *controller) = 0;
	virtual DocumentEditor *open(UIController *controller, std::string filename) = 0;
};
