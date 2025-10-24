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

#include "../API/GUI/component_type.h"

#include "API/GUI/button.h"
#include "API/GUI/checkbox.h"
#include "API/GUI/filedialog.h"
#include "API/GUI/frame.h"
#include "API/GUI/image.h"
#include "API/GUI/inputbox.h"
#include "API/GUI/label.h"
#include "API/GUI/listbox.h"
#include "API/GUI/progressbar.h"
#include "API/GUI/radiobutton.h"
#include "API/GUI/scrollbar.h"
#include "API/GUI/window.h"
#include "API/GUI/treeview.h"

class CL_ComponentType_Component : public CL_ComponentType
{
public:
	CL_ComponentType_Component() : CL_ComponentType()
	{
		set_class_name("CL_Component");
		set_option("x", NUMBER, "0");
		set_option("y", NUMBER, "0");
		set_option("width", NUMBER, "100");
		set_option("height", NUMBER, "20");
		set_option("visible", BOOL, "1");
		set_option("enabled", BOOL, "1");
		set_option("tab_id", NUMBER, "0");
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_Component(parent, style);
	}
};

class CL_ComponentType_Button : public CL_ComponentType_Component
{
public:
	CL_ComponentType_Button() : CL_ComponentType_Component()
	{
		set_class_name("CL_Button");
		set_option("text", STRING, "");
		set_option("togglemode", NUMBER, "0");
		set_option("toggled", BOOL, "0");
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_Button(parent, style);
	}
};

class CL_ComponentType_CheckBox : public CL_ComponentType_Button
{
public:
	CL_ComponentType_CheckBox() : CL_ComponentType_Button()
	{
		set_class_name("CL_CheckBox");
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_CheckBox(parent, style);
	}
};

class CL_ComponentType_FileDialog : public CL_ComponentType_Component
{
public:
	CL_ComponentType_FileDialog() : CL_ComponentType_Component()
	{
		set_class_name("CL_FileDialog");
		set_option("file", STRING, "");
		set_option("filter", STRING, "*.*");
		set_option("show_hidden", BOOL, "0");
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_FileDialog(parent, style);
	}
};

class CL_ComponentType_Frame : public CL_ComponentType_Component
{
public:
	CL_ComponentType_Frame() : CL_ComponentType_Component()
	{
		set_class_name("CL_Frame");
		set_option("filled", BOOL, "1");
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_Frame(parent, style);
	}
};

class CL_ComponentType_Image : public CL_ComponentType_Component
{
public:
	CL_ComponentType_Image() : CL_ComponentType_Component()
	{
		set_class_name("CL_Image");
		// TODO: add options
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_Image(parent, style);
	}
};

class CL_ComponentType_InputBox : public CL_ComponentType_Component
{
public:
	CL_ComponentType_InputBox() : CL_ComponentType_Component()
	{
		set_class_name("CL_InputBox");
		set_option("text", STRING, "");
		set_option("passwordmode", NUMBER, "0");
		set_option("read_only", BOOL, "0");
		set_option("max_length", NUMBER, "0");
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_InputBox(parent, style);
	}
};

class CL_ComponentType_Label : public CL_ComponentType_Component
{
public:
	CL_ComponentType_Label() : CL_ComponentType_Component()
	{
		set_class_name("CL_Label");
		set_option("text", STRING, "");
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_Label(parent, style);
	}
};

class CL_ComponentType_ListBox : public CL_ComponentType_Component
{
public:
	CL_ComponentType_ListBox() : CL_ComponentType_Component()
	{
		set_class_name("CL_ListBox");
		set_option("item", NUMBER, "0");
		set_option("max_visible_items", NUMBER, "-1");
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_ListBox(parent, style);
	}
};

class CL_ComponentType_ProgressBar : public CL_ComponentType_Component
{
public:
	CL_ComponentType_ProgressBar() : CL_ComponentType_Component()
	{
		set_class_name("CL_ProgressBar");
		set_option("steps", NUMBER, "100");
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_ProgressBar(parent, style);
	}
};

class CL_ComponentType_RadioButton : public CL_ComponentType_Component
{
public:
	CL_ComponentType_RadioButton() : CL_ComponentType_Component()
	{
		set_class_name("CL_RadioButton");
		// TODO: add options
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_RadioButton(parent, style);
	}
};

class CL_ComponentType_ScrollBar : public CL_ComponentType_Component
{
public:
	CL_ComponentType_ScrollBar() : CL_ComponentType_Component()
	{
		set_class_name("CL_ScrollBar");
		set_option("min", NUMBER, "0");
		set_option("max", NUMBER, "100");
		set_option("value", NUMBER, "0");
		set_option("orientation", NUMBER, "0");
		set_option("tracking", BOOL, "0");
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_ScrollBar(parent, style);
	}
};

class CL_ComponentType_Window : public CL_ComponentType_Component
{
public:
	CL_ComponentType_Window() : CL_ComponentType_Component()
	{
		set_class_name("CL_Window");
		set_option("title", STRING, "");
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_Window(parent, style);
	}
};

class CL_ComponentType_TreeView : public CL_ComponentType_Component
{
public:
	CL_ComponentType_TreeView() : CL_ComponentType_Component()
	{
		set_class_name("CL_TreeView");
		// TODO: add options
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_TreeView(parent, style);
	}
};

// TODO:

// Add missing components:
// - (Menu)
// - (PopupMenu)
