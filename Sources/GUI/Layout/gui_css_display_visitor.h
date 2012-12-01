
#pragma once

#include "gui_css_box_visitor.h"

namespace clan
{

class GUICSSDisplayVisitor : public GUICSSBoxVisitor
{
public:
	void node(GUIComponent_Impl *node);

	void flex_node(GUIComponent_Impl *node);

	virtual void flex_horizontal_node(GUIComponent_Impl *node) = 0;
	virtual void flex_vertical_node(GUIComponent_Impl *node) = 0;

	void clan_grid_node(GUIComponent_Impl *node);
	void clan_stacked_node(GUIComponent_Impl *node);

protected:
	CSSUsedValue get_used_noncontent_width(const GUICSSUsedValues &values);
	CSSUsedValue get_used_noncontent_height(const GUICSSUsedValues &values);

	float get_css_relative_x(GUIComponent_Impl *node, float containing_width);
	float get_css_relative_y(GUIComponent_Impl *node, float containing_height);
};

}
