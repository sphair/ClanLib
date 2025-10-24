/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
*/

#ifndef header_property_component
#define header_property_component

#include "callback_info.h"

class PropertyComponent_Impl;
class MainWindow;
class HolderComponent;


class CL_API_GUI PropertyComponent : public CL_GUIComponent
{
//! Construction:
public:
	PropertyComponent(MainWindow *main_window);

//! Attributes:
public:

//! Operations:
public:
	void clear();

//! Events:
public:
	void on_selection_changed();

//! Implementation:
private:
	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_resized();
	void on_property_changed(CL_InputEvent event);
	void on_anchoring_changed(CL_RadioButton *rb);
	void on_add_tab(CL_Tab*);
	void on_delete_tab(CL_Tab*);

	void update_property_list(CL_GUIComponent *comp);
	void add_field(const CL_StringRef &property_name, const CL_StringRef &value);
	void add_header(const CL_StringRef &header);
	void add_anchor_radios();

	CL_Rect get_child_rect(CL_GUIComponent *parent);
	MainWindow *main_window;
	HolderComponent *selected_holder;
	CL_GUIComponent *selected_component;
	CL_GUIComponent *container;

	std::map<CL_String, CL_LineEdit*> properties;
	CL_SlotContainer slots;
	CallbackInfo callback_info;
};

#endif
