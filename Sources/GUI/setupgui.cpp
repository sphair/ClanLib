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
#include "API/GUI/setupgui.h"
#include "API/GUI/component_manager.h"
#include "default_component_types.h"

static int ref_count = 0;

CL_SetupGUI::CL_SetupGUI(bool register_resources_only)
{
	CL_SetupGUI::init(register_resources_only);
}

CL_SetupGUI::~CL_SetupGUI()
{
	CL_SetupGUI::deinit();
}

void CL_SetupGUI::init(bool register_resources_only)
{
	ref_count++;
	if (ref_count != 1) return;

	if (register_resources_only) return;

	CL_ComponentType::component_types["component"] = new CL_ComponentType_Component;
	CL_ComponentType::component_types["button"] = new CL_ComponentType_Button;
	CL_ComponentType::component_types["checkbox"] = new CL_ComponentType_CheckBox;
	CL_ComponentType::component_types["filedialog"] = new CL_ComponentType_FileDialog;
	CL_ComponentType::component_types["frame"] = new CL_ComponentType_Frame;
	CL_ComponentType::component_types["image"] = new CL_ComponentType_Image;
	CL_ComponentType::component_types["inputbox"] = new CL_ComponentType_InputBox;
	CL_ComponentType::component_types["label"] = new CL_ComponentType_Label;
	CL_ComponentType::component_types["listbox"] = new CL_ComponentType_ListBox;
	CL_ComponentType::component_types["progressbar"] = new CL_ComponentType_ProgressBar;
	CL_ComponentType::component_types["radiobutton"] = new CL_ComponentType_RadioButton;
	CL_ComponentType::component_types["scrollbar"] = new CL_ComponentType_ScrollBar;
	CL_ComponentType::component_types["window"] = new CL_ComponentType_Window;
	CL_ComponentType::component_types["treeview"] = new CL_ComponentType_TreeView;
}

void CL_SetupGUI::deinit()
{
	ref_count--;
	if (ref_count != 0) return;

	delete CL_ComponentType::component_types["component"];
	delete CL_ComponentType::component_types["button"];
	delete CL_ComponentType::component_types["checkbox"];
	delete CL_ComponentType::component_types["filedialog"];
	delete CL_ComponentType::component_types["frame"];
	delete CL_ComponentType::component_types["image"];
	delete CL_ComponentType::component_types["inputbox"];
	delete CL_ComponentType::component_types["label"];
	delete CL_ComponentType::component_types["listbox"];
	delete CL_ComponentType::component_types["progressbar"];
	delete CL_ComponentType::component_types["radiobutton"];
	delete CL_ComponentType::component_types["scrollbar"];
	delete CL_ComponentType::component_types["window"];
	delete CL_ComponentType::component_types["treeview"];
}
