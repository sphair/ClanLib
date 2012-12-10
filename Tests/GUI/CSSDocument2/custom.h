
#pragma once

class Custom : public clan::GUIComponent
{
public:
	Custom(clan::GUIComponent *parent);

	float get_preferred_content_width();
	float get_preferred_content_height(float width);
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
};

