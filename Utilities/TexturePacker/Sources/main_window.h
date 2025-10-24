#pragma once

class Application;
class SpriteComponent;
class ResourceItem;
class TexturePacker;
class TextureGroupComponent;
class View;
class ViewWorkspace;

class MainWindow : public CL_Window
{
public:
	MainWindow(CL_GUIManager *gui, CL_ResourceManager *resources);
	~MainWindow();

// Attributes:
public:

// Operations:
public:
	void add_view(View *view);
	void remove_view(View *view);

// Implementation:
private:
	CL_GUITopLevelDescription get_window_description();

	void create_components();

	void create_welcome_view();
	void create_resource_viewer_view();
	void create_texture_packer_view();
	void create_css_packer_view();
	void create_ico_creator_view();

	int find_view_index(View *view);

	bool on_close();
	void on_close2();
	void on_resized();
	void on_view_close(View *view);

	Application *application;

	std::vector<View *> views;
	ViewWorkspace *workspace;

	CL_ResourceManager *resources;

	CL_MenuBar *menubar;
};
