#pragma once

#include "../view.h"
#include "../texture_packer.h"

class TextureGroupComponent;

class TexturePackerView : public View
{
public:
	TexturePackerView(CL_GUIComponent *parent, MainWindow *mainwindow);
	~TexturePackerView();

private:
	void on_resized();

	void on_button_generate_textures();
	void on_button_save_resources();
	void on_button_browse_resource_file();

	void load_resource_file(const CL_String &file);

	TexturePacker packer;

	CL_Label *label_selected_resource_file;
	CL_LineEdit *lineedit_selected_resource_file;
	CL_PushButton *button_browse_resource_file;

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
