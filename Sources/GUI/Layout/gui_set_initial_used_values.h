
#pragma once

#include "gui_css_box_visitor.h"

namespace clan
{

class GUISetInitialUsedValues : public GUICSSBoxVisitor
{
public:
	void node(GUIComponent_Impl *node);
};

}
