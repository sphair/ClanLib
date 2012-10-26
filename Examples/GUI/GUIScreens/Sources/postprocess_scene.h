
#pragma once

class PostProcessScene
{
public:
	PostProcessScene(CL_DisplayWindow display_window);
	
	void begin_scene();
	void render_component(CL_GUIComponent *component, CL_Texture &texture, CL_Rect geometry);
	void end_scene();

	CL_DisplayWindow &get_display_window() { return display_window; }

private:
	CL_DisplayWindow display_window;
	CL_GraphicContext gc;
};
