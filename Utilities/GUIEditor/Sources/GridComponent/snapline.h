/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Kenneth Gangstoe
*/

#pragma once

struct SnapLineTarget;

class SnapLine
{
// Enums & Structs:
public:
	enum SnapLineType
	{
		type_top,		// A horizontal snapline aligned to the top edge of a control.
		type_bottom,		// A horizontal snapline aligned to the bottom edge of a control.
		type_left,		// A vertical snapline aligned to the left edge of a control.
		type_right,		// A vertical snapline aligned to the right edge of a control.
		type_horizontal,	// A horizontal snapline not associated with an edge of a control.
		type_vertical,	// A vertical snapline not associated with an edge of a control.
		type_baseline	// A horizontal snapline associated with a feature of a control; for example, the base of the text string in Label component.
	};

	enum SnapLinePriority
	{
		priority_low,
		priority_medium,
		priority_high,
		priority_always
	};

	struct SnapLineTarget
	{
		SnapLineTarget(Rect rect, std::vector<SnapLine> snaplines) : rect(rect), snaplines(snaplines) {};

		Rect rect;
		std::vector<SnapLine> snaplines;
	};

// Construction:
public:
	SnapLine(SnapLineType type, int offset, SnapLinePriority priority = priority_low);
	
// Attributes:
public:
	// Gets a value indicating whether the snapline has a horizontal orientation.
	bool is_horizontal() const;

	// Gets a value indicating whether the snapline has a vertical orientation.
	bool is_vertical() const;

	// Gets the number of pixels that the snapline is offset from the origin of the associated control.
	int get_offset() const;

	// Gets a value indicating the relative importance of the snapline.
	SnapLinePriority get_priority() const;

	// Gets the type of a snapline, which indicates the general location and orientation.
	SnapLineType get_type() const;

	// Returns a value indicating whether the specified SnapLine should snap to another SnapLine.
	static bool should_snap(SnapLine line1, SnapLine line2);

// Operations:
public:
	// Potentially snap a source position to a collection of target positions using snaplines.
	static Vec2i snap(
		const Rect &source_rect,
		const std::vector<SnapLine> &source_snaplines, 
		const std::vector<SnapLineTarget> &targets);

// Implementation:
private:
	static int find_closest_snapdistance(const SnapLine &source_snapline, int source_position, const std::vector<SnapLineTarget> &targets);

	SnapLineType type;
	SnapLinePriority priority;

	int offset;
};


