/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

namespace clan
{

class Workspace;
class DockableComponent;

class ViewFrame : public GUIComponent
{
public:
	ViewFrame(GUIComponent *parent, DockableComponent *dockable);
	DockableComponent *get_dockable() { return dockable; }

private:
	void on_render(Canvas &canvas, const Rect &update_box);
	void on_resized();
	bool on_input_doubleclick(const InputEvent &input_event);

	DockableComponent *dockable;
	GUIThemePart part_header;
	Rect header_box;
	Rect scene_box;
};

}
