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

//! clanGUI="Framework"
//! header=gui.h

#ifndef header_gui_manager
#define header_gui_manager

#ifdef CL_API_DLL
#ifdef CL_GUI_EXPORT
#define CL_API_GUI __declspec(dllexport)
#else
#define CL_API_GUI __declspec(dllimport)
#endif
#else
#define CL_API_GUI
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include <stddef.h>

#include "component.h"

class CL_ComponentManager;
class CL_StyleManager;
class CL_EventTrigger;
class CL_GUIManager_Generic;
class CL_Component_Generic;

//: Root component.
//- !group=GUI/Framework!
//- !header=gui.h!
//- <p>The GUI manager component is the 'root component' of any GUI system. It
//- is responsible channeling input and output from the general clanlib display
//- API and into the GUI system.</p>
//- <p>The GUI manager must to be the top-level component in any component tree.</p>
//- <p>Modal dialog interfaces in clanGUI is archived by constructing a GUI manager
//- with a parent component. This will disable the input of the previous GUI, but
//- will keep calling its painting signals.</p>
class CL_API_GUI CL_GUIManager : public CL_Component
{
//! Construction:
public:
	//: GUI Manager Constructor
	CL_GUIManager(CL_StyleManager *style);

	//: GUI Manager Destructor
	virtual ~CL_GUIManager();

//! Attributes:
public:
	//: Returns the currently focused component.
	CL_Component *get_focus() const;

	//: Returns the currently captured component.
	CL_Component *get_capture() const;

	//: Returns true if input is enabled.
	bool is_input_enabled() const;
	
	//: Return component on top of modal stack or NULL if none
	CL_Component * get_modal_component() const;

	//: Returns the event trigger for the gui manager.
	CL_EventTrigger *get_update_trigger();

	//: Returns true if gui manager is currently shutting down
	bool is_shutting_down() const;

//! Operations:
public:
	//: Sets the focus component of the gui.
	void set_focus(CL_Component *component);

	//: Draws the GUI once.
	void show();

	//: Runs the GUI message pump continuosly.
	//- <p>This function runs the GUI mesage pump continously until CL_GUIManager::quit() is called.
	//- It will redraw the screen and user events to components as they occour.</p>
	//param modal_component: This parameter indicates if input events should only occour from a certain
	//param modal_component: component or beneath in the component tree. This is used to start modal
	//param modal_component: message pumps in the system, where you can only interface with a popup or
	//param modal_component: similar.
	void run(CL_Component *modal_component = 0);

	//: Break run loop.
	void quit();

	//: Tells the GUI to start accepting input.
	void enable_input();
	
	//: Tells the GUI to stop accepting input.
	void disable_input();

	//: Sets the GUI in capture mode. [should we move this out of API space? -- mbn]
	void gui_capture_mouse(CL_Component *component);

	//: Takes GUI out of capture mode. [should we move this out of API space? -- mbn]
	void gui_release_mouse();

//! Signals:
public:
	//: Called when a new component get the focus
	CL_Signal_v1<CL_Component *> &sig_focus_changed();
	
//! Implementation:
private:
	CL_GUIManager_Generic *impl;

	friend class CL_Component_Generic;
};

#endif
