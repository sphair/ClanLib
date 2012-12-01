
#pragma once

#include "gui_css_display_visitor.h"

namespace clan
{

class GUILayoutBoxContent : public GUICSSDisplayVisitor
{
public:
	void flex_horizontal_node(GUIComponent_Impl *node);
	void flex_vertical_node(GUIComponent_Impl *node);

private:
	void find_preferred_width(GUIComponent_Impl *node);
	void find_preferred_height(GUIComponent_Impl *node);
	CSSUsedValue align_vertical(GUIComponent_Impl *node, GUIComponent *child);
	CSSUsedValue align_horizontal(GUIComponent_Impl *node, GUIComponent *child);
	void set_horizontal_geometry(GUIComponent_Impl *node);
	void set_vertical_geometry(GUIComponent_Impl *node);
	void set_child_geometry(GUIComponent_Impl *node, GUIComponent *child, CSSUsedValue x, CSSUsedValue y);
};

}
