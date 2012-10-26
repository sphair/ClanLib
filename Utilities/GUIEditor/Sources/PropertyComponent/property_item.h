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
**    Magnus Norddahl
*/

#pragma once

class GridObject;

class PropertyItem
{
public:
	PropertyItem() : property_component(0) { }
	virtual ~PropertyItem() { }

	PropertyComponent *property_component;
	std::string name;

	virtual void selection_changed(const std::vector<GridObject *> &selection) { }

	virtual GUIComponent *activate() = 0;
	virtual void deactivate(GUIComponent *component) = 0;
	virtual void apply_changes(GridObject *component) = 0;

	virtual int get_inactive_height(GraphicContext &gc, GUIThemePart &part, int width) = 0;
	virtual void render_inactive(GraphicContext &gc, GUIThemePart &part, const Rect &rect, const Rect &clip_rect) = 0;
};
