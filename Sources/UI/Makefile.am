lib_LTLIBRARIES = libclan40UI.la

libclan40UI_la_SOURCES = \
./UIThread/ui_thread.cpp \
./ViewController/view_controller.cpp \
./precomp.cpp \
./StandardViews/popup_view.cpp \
./StandardViews/scrollbar_view.cpp \
./StandardViews/span_layout_view.cpp \
./StandardViews/span_layout_view_impl.cpp \
./StandardViews/image_view.cpp \
./StandardViews/text_field_view.cpp \
./StandardViews/label_view.cpp \
./StandardViews/button_view.cpp \
./StandardViews/texture_view.cpp \
./StandardViews/texture_view_impl.cpp \
./StandardViews/window_view.cpp \
./StandardViews/window_view_impl.cpp \
./SystemDialogs/folder_browse_dialog.cpp \
./SystemDialogs/open_file_dialog.cpp \
./SystemDialogs/save_file_dialog.cpp \
./View/view.cpp \
./View/inline_layout.cpp \
./View/block_layout.cpp \
./View/vbox_layout.cpp \
./View/hbox_layout.cpp \
./View/grid_layout.cpp \
./View/positioned_layout.cpp \
./Style/box_style.cpp \
./Style/text_style.cpp \
./Style/box_style_impl.cpp \
./Style/box_geometry.cpp

libclan40UI_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanUI)

libclan40UI_la_CXXFLAGS=$(clanUI_CXXFLAGS) $(extra_CFLAGS_clanUI)
# EOF #

