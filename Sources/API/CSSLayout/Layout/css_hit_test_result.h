
#pragma once

namespace clan
{
/// \addtogroup clanCSSLayout_Layout clanCSSLayout Layout
/// \{

class CSSHitTestResult
{
public:
	enum Type
	{
		no_objects_available,
		outside_top,
		outside_left,
		outside_right,
		outside_bottom,
		inside
	};

	CSSHitTestResult() : type(no_objects_available), text_offset(0) { }

	Type type;
	CSSLayoutNode node;
	int text_offset;
};

/// \}
}
