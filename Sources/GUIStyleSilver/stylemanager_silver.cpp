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

#include "API/Display/display.h"
#include "API/Display/surface.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUI/component_manager.h"
#include "API/guistylesilver.h"

// Construction:

CL_StyleManager_Silver::CL_StyleManager_Silver(CL_ResourceManager *resources)
: CL_StyleManager(resources)
{
}

CL_StyleManager_Silver::~CL_StyleManager_Silver()
{
}

// Overridables:
void CL_StyleManager_Silver::connect_styles(
	const std::string &type,
	CL_Component *component)
{
	if (type == "label")
		component->set_style(new CL_Label_Silver((CL_Label *) component, this));

	else if (type == "button")
		component->set_style(new CL_Button_Silver((CL_Button *) component, this));

	else if (type == "frame")
		component->set_style(new CL_Frame_Silver((CL_Frame *) component, this));

	else if (type == "image")
		component->set_style(new CL_Image_Silver((CL_Image *) component, this));

	else if (type == "inputbox")
		component->set_style(new CL_InputBox_Silver((CL_InputBox *) component, this));

	else if (type == "richedit")
		component->set_style(new CL_RichEdit_Silver((CL_RichEdit *) component, this));

	else if (type == "menu")
		component->set_style(new CL_Menu_Silver((CL_Menu *) component, this));

	else if (type == "menu_node")
		component->set_style(new CL_MenuNode_Silver((CL_MenuNode *) component, this));

	else if (type == "menu_item")
		component->set_style(new CL_MenuItem_Silver((CL_MenuItem *) component, this));

	else if (type == "scrollbar")
		component->set_style(new CL_ScrollBar_Silver((CL_ScrollBar *) component, this));

	else if (type == "progressbar")
		component->set_style(new CL_ProgressBar_Silver((CL_ProgressBar *) component, this));

	else if (type == "radiobutton")
		component->set_style(new CL_RadioButton_Silver((CL_RadioButton *) component, this));

	else if (type == "checkbox")
		component->set_style(new CL_CheckBox_Silver((CL_CheckBox *) component, this));

	else if (type == "listbox")
		component->set_style(new CL_ListBox_Silver((CL_ListBox *) component, this));

	else if (type == "treeview")
		component->set_style(new CL_TreeView_Silver((CL_TreeView *) component, this));

	else if (type == "treeitem")
		component->set_style(new CL_TreeItem_Silver((CL_TreeItem *) component, this));

	else if (type == "window")
		component->set_style(new CL_Window_Silver((CL_Window *) component, this));
}
