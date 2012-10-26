
#pragma once

class PostProcessScene;
class TopLevelWindow;

class WindowManager : CL_GUIWindowManagerProvider
{
public:
	static CL_GUIWindowManager create(PostProcessScene *scene);
	static WindowManager *cast(const CL_GUIWindowManager &wm);

private:
	WindowManager(PostProcessScene *scene);
	~WindowManager();
	TopLevelWindow *get_toplevel_window(CL_GUITopLevelWindow *handle) const;

	// CL_GUIWindowManagerProvider interface:
	void destroy();
	void set_site(CL_GUIWindowManagerSite *site);
	void create_window(CL_GUITopLevelWindow *handle, CL_GUITopLevelWindow *owner, CL_GUIComponent *component, CL_GUITopLevelDescription description);
	void destroy_window(CL_GUITopLevelWindow *handle);
	void enable_window(CL_GUITopLevelWindow *handle, bool enable);
	bool has_focus(CL_GUITopLevelWindow *handle) const;
	void set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate_root_win);
	void set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area);
	CL_Rect get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const;
	CL_Point screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const;
	CL_Point window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const;
	CL_GraphicContext& get_gc(CL_GUITopLevelWindow *handle) const;
	CL_InputContext& get_ic(CL_GUITopLevelWindow *handle) const;
	CL_GraphicContext begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);
	void set_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect);
	void reset_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc);
	void push_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect);
	void pop_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc);
	void end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);
	void request_repaint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);
	void bring_to_front(CL_GUITopLevelWindow *handle);
	bool is_minimized(CL_GUITopLevelWindow *handle) const;
	bool is_maximized(CL_GUITopLevelWindow *handle) const;
	void capture_mouse(CL_GUITopLevelWindow *handle, bool state);
	CL_DisplayWindow get_display_window(CL_GUITopLevelWindow *handle) const;
	void set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor);
	void set_cursor(CL_GUITopLevelWindow *handle, enum CL_StandardCursor type);
	void update();
	void setup_painting();
	void complete_painting();
	CL_GUIWindowManager::CL_WindowManagerType get_window_manager_type() const;

	PostProcessScene *scene;
	CL_GUIWindowManagerSite *site;
	std::map<CL_GUITopLevelWindow *, TopLevelWindow *> window_map;
	std::vector<CL_GUITopLevelWindow *> zorder;

	friend class TopLevelWindow;

	CL_SlotContainer slots;
};
