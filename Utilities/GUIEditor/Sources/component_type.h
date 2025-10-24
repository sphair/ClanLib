/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Kenneth Gangstoe
*/

#pragma once

class ComponentType
{
public:
	ComponentType(const CL_StringRef &name, int id, const CL_StringRef &icon)
	: name(name), id(id), icon(icon)
	{
	}

	virtual CL_GUIComponent *create_component(CL_GUIComponent *parent) const = 0;

	CL_String name;
	int id;
	CL_String icon;
};

template<typename Type>
class ComponentTypeTemplate : public ComponentType
{
public:
	ComponentTypeTemplate(const CL_StringRef &name, int id, const CL_StringRef &icon)
	: ComponentType(name, id, icon)
	{
	}

	CL_GUIComponent *create_component(CL_GUIComponent *parent) const
	{
		CL_GUIComponent *component = new Type(parent);
		component->set_geometry(CL_Rect(0, 0, 75, 23));
		return component;
	}
};

class ButtonComponentType : public ComponentType
{
public:
	ButtonComponentType(const CL_StringRef &name, int id, const CL_StringRef &icon)	
	: ComponentType(name, id, icon) {}

	virtual CL_GUIComponent *create_component(CL_GUIComponent *parent) const
	{
		CL_PushButton *component = new CL_PushButton(parent);
		component->set_text("Button");
		component->set_geometry(CL_Rect(0, 0, 75, 23));
		return component;
	}
};

class SliderComponentType : public ComponentType
{
public:
	SliderComponentType(const CL_StringRef &name, int id, const CL_StringRef &icon)	
	: ComponentType(name, id, icon) {}

	virtual CL_GUIComponent *create_component(CL_GUIComponent *parent) const
	{
		CL_Slider *component = new CL_Slider(parent);
		component->set_ranges(0, 100, 10, 10);
		component->set_geometry(CL_Rect(0, 0, 75, 24));
		return component;
	}
};

class CheckBoxComponentType : public ComponentType
{
public:
	CheckBoxComponentType(const CL_StringRef &name, int id, const CL_StringRef &icon)	
	: ComponentType(name, id, icon) {}

	virtual CL_GUIComponent *create_component(CL_GUIComponent *parent) const
	{
		CL_CheckBox *component = new CL_CheckBox(parent);
		component->set_text("CheckBox");
		component->set_geometry(CL_Rect(0, 0, 75, 17));
		return component;
	}
};

class RadioButtonComponentType : public ComponentType
{
public:
	RadioButtonComponentType(const CL_StringRef &name, int id, const CL_StringRef &icon)	
	: ComponentType(name, id, icon) {}

	virtual CL_GUIComponent *create_component(CL_GUIComponent *parent) const
	{
		CL_RadioButton *component = new CL_RadioButton(parent);
		component->set_geometry(CL_Rect(0, 0, 75, 17));
		component->set_text("Radiobutton");
		return component;
	}
};

class LabelComponentType : public ComponentType
{
public:
	LabelComponentType(const CL_StringRef &name, int id, const CL_StringRef &icon)	
	: ComponentType(name, id, icon) {}

	virtual CL_GUIComponent *create_component(CL_GUIComponent *parent) const
	{
		CL_Label *component = new CL_Label(parent);
		component->set_geometry(CL_Rect(0, 0, 75, 13));
		component->set_text("Label");
		return component;
	}
};

class FrameComponentType : public ComponentType
{
public:
	FrameComponentType(const CL_StringRef &name, int id, const CL_StringRef &icon)	
	: ComponentType(name, id, icon) {}

	virtual CL_GUIComponent *create_component(CL_GUIComponent *parent) const
	{
		CL_Frame *component = new CL_Frame(parent);
		component->set_header_text("Frame");
		component->set_geometry(CL_Rect(0, 0, 75, 23));
		return component;
	}
};

class ScrollBarHorizontalComponentType : public ComponentType
{
public:
	ScrollBarHorizontalComponentType(const CL_StringRef &name, int id, const CL_StringRef &icon)	
	: ComponentType(name, id, icon) {}

	virtual CL_GUIComponent *create_component(CL_GUIComponent *parent) const
	{
		CL_ScrollBar *component = new CL_ScrollBar(parent);
		component->set_horizontal();
		component->set_ranges(0, 100, 2, 10);
		component->set_geometry(CL_Rect(0, 0, 80, 17));
		return component;
	}
};

class ScrollBarVerticalComponentType : public ComponentType
{
public:
	ScrollBarVerticalComponentType(const CL_StringRef &name, int id, const CL_StringRef &icon)	
	: ComponentType(name, id, icon) {}

	virtual CL_GUIComponent *create_component(CL_GUIComponent *parent) const
	{
		CL_ScrollBar *component = new CL_ScrollBar(parent);
		component->set_vertical();
		component->set_ranges(0, 100, 2, 10);
		component->set_geometry(CL_Rect(0, 0, 17, 80));
		return component;
	}
};

class TabComponentType : public ComponentType
{
public:
	TabComponentType(const CL_StringRef &name, int id, const CL_StringRef &icon)	
	: ComponentType(name, id, icon) {}

	virtual CL_GUIComponent *create_component(CL_GUIComponent *parent) const
	{
		static int tab_page_count = 0;

		CL_Tab *component = new CL_Tab(parent);
		component->set_geometry(CL_Rect(0, 0, 75, 23));

		CL_TabPage *page1 = component->add_page("Page 1");
		page1->set_id_name(cl_format("%1%2", page1->get_type_name(), tab_page_count++));

		return component;
	}
};
