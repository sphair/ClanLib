#pragma once

class Application;
class SpriteComponent;
class ResourceItem;
class TexturePacker;
class TextureGroupComponent;

class MainWindow : public CL_Window
{
public:
	MainWindow(Application *application);
	~MainWindow();

// Attributes:
public:

// Operations:
public:

// Implementation:
private:
	CL_GUITopLevelDescription get_startup_description();

	void create_components();

	void on_close();
	void on_resized();

	void load_resources();
	void show_resource(ResourceItem *resource_item);

	Application *application;

	TexturePacker *packer;

	CL_Tab *tab;
	CL_TabPage *tab_page_1;
	CL_TabPage *tab_page_2;

	// Tab 1
	void on_selection_changed(CL_ListViewSelection selection);

	CL_ListView *resource_list;
	SpriteComponent *sprite_component;

	// Tab 2
	void on_button_generate_textures();
	void on_button_save_resources();

	CL_PushButton *button_pack;
	CL_PushButton *button_save;
	CL_Spin *spin_border;
	CL_Label *label_border;

	CL_RadioButton *radio_texture32;
	CL_RadioButton *radio_texture64;
	CL_RadioButton *radio_texture128;
	CL_RadioButton *radio_texture256;
	CL_RadioButton *radio_texture512;
	CL_RadioButton *radio_texture512x256;
	CL_RadioButton *radio_texture1024;
	CL_RadioButton *radio_texture2048;

	CL_Label *generation_result;

	TextureGroupComponent *texturegroup_component;
};
