/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/Display/display.h"
#include "gui_manager_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Construction:

CL_GUIManager::CL_GUIManager(CL_StyleManager *style)
:
	CL_Component(CL_Rect(0, 0, CL_Display::get_width(), CL_Display::get_height()), NULL, style)
{
	impl = new CL_GUIManager_Generic(this);
	set_focusable(false);
}

CL_GUIManager::~CL_GUIManager()
{
	delete impl;
	impl = NULL; //SETH> Don't remove this, it's used to check "shuttong down" to avoid
	//a crash in the recursive delete process
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Attributes:

CL_Component *CL_GUIManager::get_focus() const
{
	return impl->get_focus();
}

CL_Component *CL_GUIManager::get_capture() const
{
	return impl->get_capture();
}

bool CL_GUIManager::is_input_enabled() const
{
	return impl->is_input_enabled();
}

CL_EventTrigger *CL_GUIManager::get_update_trigger()
{
	return &impl->update_trigger;
}

bool CL_GUIManager::is_shutting_down() const
{
	return impl == NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Operations:

void CL_GUIManager::set_focus(CL_Component *component)
{
	impl->set_focus(component);
}

void CL_GUIManager::run(CL_Component *modal_component)
{
	impl->run(modal_component);
}

void CL_GUIManager::show()
{
	impl->show();
}

void CL_GUIManager::quit()
{
	impl->quit();
}

void CL_GUIManager::enable_input()
{
	impl->enable_input();
}

void CL_GUIManager::disable_input()
{
	impl->disable_input();
}

void CL_GUIManager::gui_capture_mouse(CL_Component *component)
{
	impl->gui_capture_mouse(component);
}

void CL_GUIManager::gui_release_mouse()
{
	impl->gui_release_mouse();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Signals:

CL_Signal_v1<CL_Component *> &CL_GUIManager::sig_focus_changed()
{
	return impl->sig_focus_changed;
}

CL_Component * CL_GUIManager::get_modal_component() const
{
	return impl->get_modal_component();
}
