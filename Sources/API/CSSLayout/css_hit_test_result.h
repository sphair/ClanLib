
#pragma once

class CL_CSSHitTestResult
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

	CL_CSSHitTestResult() : type(no_objects_available), text_offset(0) { }

	Type type;
	CL_CSSLayoutNode node;
	int text_offset;
};
