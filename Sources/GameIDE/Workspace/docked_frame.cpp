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

#include "GameIDE/precomp.h"
#include "docked_frame.h"
#include "workspace.h"
#include "API/GameIDE/Workspace/dockable_component.h"

namespace clan
{

DockedFrame::DockedFrame(GUIComponent *parent, DockableComponent *dockable)
: GUIComponent(parent, "docked-frame"), dockable(dockable)
{
	func_resized() = bind_member(this, &DockedFrame::on_resized);
	func_render() = bind_member(this, &DockedFrame::on_render);
	dockable->set_parent_component(this);
	dockable->set_visible(true, false);
	set_visible(false, false);
	on_resized();
	request_repaint();
}

void DockedFrame::on_render(Canvas &canvas, const Rect &update_box)
{
}

void DockedFrame::on_resized()
{
	dockable->set_geometry(get_size());
}

}
