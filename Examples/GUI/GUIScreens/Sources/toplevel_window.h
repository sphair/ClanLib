
#pragma once

class WindowManager;

class TopLevelWindow
{
public:
	TopLevelWindow(WindowManager *wm, CL_GUITopLevelWindow *owner, CL_GUIComponent *component, CL_GUITopLevelDescription description);

	bool is_visible() const { return visible; }
	bool is_enabled() const { return enabled; }
	bool is_minimized() const { return minimized; }
	bool is_maximized() const { return maximized; }
	const CL_Rect &get_geometry() const { return geometry; }
	CL_GUIComponent *get_component() const { return component; }
	CL_Texture &get_texture() { return texture; }

	void set_visible(bool show, bool activate);
	void set_geometry(const CL_Rect &box);
	void set_enabled(bool enable) { enabled = enable; }

	CL_FrameBuffer &get_frame_buffer() { return frame_buffer; }

private:
	void create_texture();
	void destroy_texture();

	WindowManager *wm;
	CL_GUITopLevelWindow *owner;
	CL_GUIComponent *component;
	bool visible;
	bool enabled;
	bool minimized;
	bool maximized;
	CL_Rect geometry;
	CL_Texture texture;
	CL_FrameBuffer frame_buffer;
};
