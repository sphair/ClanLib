#pragma once

class SpriteComponent : public CL_GUIComponent
{
public:
	SpriteComponent(CL_GUIComponent *parent);
	~SpriteComponent() {};

// Attributes:
public:

// Operations:
public:
	void set_sprite(CL_Sprite *sprite);
	void set_sprite_description(CL_SpriteDescription *sprite_description);

// Implementation:
private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_style_changed();
	void on_resized();

	CL_Sprite *sprite;
	CL_SpriteDescription *sprite_description;
};
