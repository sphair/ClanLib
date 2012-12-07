
#pragma once

class Custom : public GUIComponent
{
public:

	Custom(GUIComponent *parent);

	virtual ~Custom();
	float get_preferred_content_width();
	float get_preferred_content_height(float width);
	void on_render(Canvas &canvas, const Rect &update_rect);

};

