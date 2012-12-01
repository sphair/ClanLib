
#pragma once

#include "gui_css_display_visitor.h"

namespace clan
{

class GUIFindPreferredWidth : public GUICSSDisplayVisitor
{
public:
	void flex_horizontal_node(GUIComponent_Impl *node);
	void flex_vertical_node(GUIComponent_Impl *node);
};

}
