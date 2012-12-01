
#pragma once

#include "gui_css_display_visitor.h"

namespace clan
{

class GUIFindPreferredHeight : public GUICSSDisplayVisitor
{
public:
	void flex_horizontal_node(GUIComponent_Impl *node);
	void flex_vertical_node(GUIComponent_Impl *node);
};

}
