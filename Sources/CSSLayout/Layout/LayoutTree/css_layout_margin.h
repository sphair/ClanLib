
#pragma once

namespace clan
{

// 8.3.1 Collapsing margins

class CSSLayoutMargin
{
public:
	CSSLayoutMargin();

	static CSSLayoutMargin create_top_margin(CSSLayoutNode *node);
	static CSSLayoutMargin create_bottom_margin(CSSLayoutNode *node);
	static void try_collapse_margins(std::vector<CSSLayoutMargin> &margins);

	enum MarginType
	{
		margin_top,
		margin_bottom
	};

	CSSUsedValue size;
	bool clearance;
	MarginType first_type;
	MarginType last_type;
	CSSLayoutNode *first_node;
	CSSLayoutNode *last_node;

private:
	static void collapse_margin(std::vector<CSSLayoutMargin> &margins, size_t index);
	static bool are_margins_adjoining(const std::vector<CSSLayoutMargin> &margins, size_t index);
};

CSSLayoutMargin::CSSLayoutMargin()
: size(0.0f), clearance(false), first_node(0), last_node(0)
{
}

CSSLayoutMargin CSSLayoutMargin::create_top_margin(CSSLayoutNode *node)
{
	CSSLayoutMargin margin;
	margin.clearance = (node->get_element_node()->computed_properties.clear.type != CSSValueClear::type_none);
	margin.first_type = margin_top;
	margin.first_node = node;
	margin.last_type = margin_top;
	margin.last_node = node;
	return margin;
}

CSSLayoutMargin CSSLayoutMargin::create_bottom_margin(CSSLayoutNode *node)
{
	CSSLayoutMargin margin;
	margin.clearance = false;
	margin.first_type = margin_bottom;
	margin.first_node = node;
	margin.last_type = margin_bottom;
	margin.last_node = node;
	return margin;
}

void CSSLayoutMargin::try_collapse_margins(std::vector<CSSLayoutMargin> &margins)
{
	size_t i = 0;
	while (i + 1 < margins.size())
	{
		if (are_margins_adjoining(margins, i))
		{
			collapse_margin(margins, i);
		}
		else
		{
			i++;
		}
	}
}

void CSSLayoutMargin::collapse_margin(std::vector<CSSLayoutMargin> &margins, size_t index)
{
	margins[index].last_type = margins[index + 1].last_type;
	margins[index].last_node = margins[index + 1].last_node;
	margins.erase(margins.begin() + index + 1);
}

bool CSSLayoutMargin::are_margins_adjoining(const std::vector<CSSLayoutMargin> &margins, size_t index)
{
	if (margins[index].last_node->get_block_formatting_context() != margins[index+1].first_node->get_block_formatting_context())
		return false;

	if (margins[index+1].clearance)
		return false;

	// To do: no line boxes, no clearance, no padding and no border separate them (Note that certain zero-height line boxes (see 9.4.2) are ignored for this purpose.)

	bool allowed_pair = false;
	if (margins[index].last_type == margin_top && margins[index+1].first_type == margin_top)
	{
		allowed_pair = (get_in_flow_child(margins[index].last_node) == margins[index+1].first_node));
	}
	else if (margins[index].last_type == margin_bottom && margins[index+1].first_type == margin_top)
	{
		allowed_pair = (get_in_flow_sibling(margins[index].last_node) == margins[index+1].first_node);
	}
	else if (margins[index].last_type == margin_bottom && margins[index+1].first_type == margin_bottom)
	{
		allowed_pair = (is_last_in_flow_child(margins[index].last_node) && parent_has_auto_height(margins[index].last_node));
	}
	else if (margins[index].last_type == margin_top && margins[index+1].first_type == margin_bottom && margins[index].last_node == margins[index+1].first_node)
	{
		allowed_pair = (margins[index].last_node->min_height == 0.0f && zero_or_auto_height(margins[index].last_node) && has_no_in_flow_children(margins[index].last_node));
	}
	return allowed_pair;
}

}
