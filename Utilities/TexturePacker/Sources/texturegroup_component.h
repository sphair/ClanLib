#pragma once

class TextureGroupComponent : public CL_GUIComponent
{
public:
	TextureGroupComponent(CL_GUIComponent *parent);
	~TextureGroupComponent() {};

// Attributes:
public:

// Operations:
public:
	void set_texturegroup(CL_TextureGroup *group);

// Implementation:
private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_render_texture(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_style_changed();
	void on_resized();
	void on_scroll();

	CL_TextureGroup *texture_group;

	CL_ScrollBar *scroll_horizontal;
	CL_ScrollBar *scroll_vertical;
	CL_GUIComponent *component_texture;
};
