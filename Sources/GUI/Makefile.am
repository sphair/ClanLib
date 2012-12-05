lib_LTLIBRARIES = libclan30GUI.la

libclan30GUI_la_SOURCES = \
	gui_component_impl.cpp \
	gui_component_description.cpp \
	WindowManager/gui_window_manager_direct.cpp \
	WindowManager/gui_window_manager.cpp \
	WindowManager/gui_window_manager_texture_window.cpp \
	WindowManager/gui_window_manager_provider_direct.cpp \
	WindowManager/gui_window_manager_texture.cpp \
	WindowManager/gui_window_manager_provider_texture.cpp \
	WindowManager/gui_window_manager_system.cpp \
	WindowManager/gui_window_manager_provider_system.cpp \
	setup_gui.cpp \
	gui_xml_loader_version_1_0.cpp \
	Components/statusbar.cpp \
	Components/tab_header.cpp \
	Components/scrollbar_impl.cpp \
	Components/spin.cpp \
	Components/tab_page.cpp \
	Components/toolbar_item.cpp \
	Components/window.cpp \
	Components/imageview.cpp \
	Components/folderbrowsedialog.cpp \
	Components/frame.cpp \
	Components/main_window.cpp \
	Components/ListView/listview_layout_thumbnails.cpp \
	Components/ListView/listview_layout.cpp \
	Components/ListView/listview_header.cpp \
	Components/ListView/listview_item.cpp \
	Components/ListView/listview_layout_icons.cpp \
	Components/ListView/listview_icon_list.cpp \
	Components/ListView/listview_selected_item.cpp \
	Components/ListView/listview_renderer.cpp \
	Components/ListView/listview.cpp \
	Components/ListView/listview_icon.cpp \
	Components/ListView/listview_column_header.cpp \
	Components/ListView/listview_column_data.cpp \
	Components/ListView/listview_layout_details.cpp \
	Components/slider.cpp \
	Components/message_box.cpp \
	Components/progressbar.cpp \
	Components/push_button.cpp \
	Components/scrollbar.cpp \
	Components/label.cpp \
	Components/combobox.cpp \
	Components/label_span.cpp \
	Components/tab.cpp \
	Components/Ribbon/ribbon_menu.cpp \
	Components/Ribbon/ribbon_page.cpp \
	Components/Ribbon/ribbon.cpp \
	Components/Ribbon/ribbon_section.cpp \
	Components/openfiledialog.cpp \
	Components/Menus/popupmenu.cpp \
	Components/Menus/popupmenu_item.cpp \
	Components/Menus/menu_modal_loop.cpp \
	Components/Menus/menubar.cpp \
	Components/Menus/popupmenu_window.cpp \
	Components/tooltip.cpp \
	Components/groupbox.cpp \
	Components/textedit.cpp \
	Components/message_box_component.cpp \
	Components/savefiledialog.cpp \
	Components/radiobutton.cpp \
	Components/checkbox.cpp \
	Components/toolbar.cpp \
	Components/lineedit.cpp \
	Layout/gui_layout.cpp \
	Layout/gui_layout_provider_corners.cpp \
	Layout/gui_layout_corners.cpp \
	gui_component_select_node.cpp \
	Theme/gui_theme_layout_manager.cpp \
	gui_manager_impl.cpp \
	gui_manager.cpp \
	accelerator_key.cpp \
	precomp.cpp \
	accelerator_table.cpp \
	gui_component.cpp

libclan30GUI_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanGUI)

libclan30GUI_la_CXXFLAGS=$(clanGUI_CXXFLAGS) $(extra_CFLAGS_clanGUI)

# EOF #
