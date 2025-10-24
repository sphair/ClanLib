
#ifndef header_application_editor
#define header_application_editor

class Application
{
public:
	Application();
	~Application();

	void run(const CL_String &filename = CL_String());

	CL_ResourceManager get_resources() { return resources; }
	CL_GUIManager *get_gui() { return &gui; }

private:
	CL_ResourceManager resources;
	CL_GUIThemeDefault theme;
	CL_GUIWindowManagerSystem window_manager;
	CL_GUIManager gui;
};

#endif
