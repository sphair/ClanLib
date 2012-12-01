
#pragma once

#include "GUI/gui_component_impl.h"

namespace clan
{

class GUIComponent_Impl;

class GUICSSBoxVisitor
{
public:
	virtual void node(GUIComponent_Impl *node) = 0;
};

}
