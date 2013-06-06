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
/// \addtogroup clanGameIDE_System clanGameIDE System
/// \{

class EditorMainWindow;
class Workspace;
class Dock;

class DockableComponent : public GUIComponent
{
public:
	DockableComponent();

	void set_title(const std::string &title);
	std::string get_title() const { return title; }
	void show();
	void show_as_docked(Dock *dock);
	void show_as_window();
	void hide();

	int get_last_docked_id() const { return last_docked_id; }

protected:
	virtual void on_render(Canvas &canvas, const Rect &update_box) { }
	virtual void on_resized() { }

	void set_dockable_window_geometry(Rect new_window_geometry) { window_geometry = new_window_geometry; }

private:
	enum VisibilityState
	{
		state_docked = 0,
		state_window = 1,
		state_hidden = 2
	};

	Dock *dock;
	VisibilityState visibility;
	std::string title;
	Rect window_geometry;

	VisibilityState last_visibility;
	int last_docked_id;

	friend class Workspace;
};

}

/// \}
