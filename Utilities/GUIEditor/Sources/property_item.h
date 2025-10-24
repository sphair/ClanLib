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
**    Magnus Norddahl
*/


#pragma once

#include "property_component.h"
#include "main_window.h"
#include "selection.h"
#include "anchor_popup.h"
#include "holder_component.h"

class HolderComponent;

class PropertyItem
{
public:
	PropertyItem() : property_component(0) { }
	virtual ~PropertyItem() { }

	PropertyComponent *property_component;
	CL_String name;
/*
	std::vector<HolderComponent*> get_selection()
	{
		return property_component->get_main_window()->get_selection()->get_selection();
	}
*/
	virtual void selection_changed(const std::vector<HolderComponent *> &selection) { }

	virtual CL_GUIComponent *activate() = 0;
	virtual void deactivate(CL_GUIComponent *component) = 0;
	virtual void apply_changes(HolderComponent *component) = 0;

	virtual int get_inactive_height(CL_GraphicContext &gc, CL_GUIThemePart &part, int width) = 0;
	virtual void render_inactive(CL_GraphicContext &gc, CL_GUIThemePart &part, const CL_Rect &rect, const CL_Rect &clip_rect) = 0;
};

class PropertyItemHeader : public PropertyItem
{
public:
	PropertyItemHeader(const CL_String &name) { this->name = name; }

	CL_GUIComponent *activate() { return 0; }
	void deactivate(CL_GUIComponent *component) { }

	int get_inactive_height(CL_GraphicContext &gc, CL_GUIThemePart &part, int width) { return 0; }
	void render_inactive(CL_GraphicContext &gc, CL_GUIThemePart &part, const CL_Rect &rect, const CL_Rect &clip_rect) { }
};

class PropertyItemLineEdit : public PropertyItem
{
public:
	PropertyItemLineEdit(const CL_String &name) { this->name = name; }
	PropertyItemLineEdit(const CL_String &name, const CL_String &value) { this->name = name; this->value = value; }

	CL_GUIComponent *activate()
	{
		CL_LineEdit *c = new CL_LineEdit(property_component);
		c->func_enter_pressed().set(this, &PropertyItemLineEdit::on_enter_pressed);
		c->set_text(value);
		return c;
	}

	void deactivate(CL_GUIComponent *component)
	{
		value = static_cast<CL_LineEdit *>(component)->get_text();
		delete component;
	}

	int get_inactive_height(CL_GraphicContext &gc, CL_GUIThemePart &part, int width)
	{
		return part.get_font().get_text_size(gc, value).height;
	}

	void render_inactive(CL_GraphicContext &gc, CL_GUIThemePart &part, const CL_Rect &rect, const CL_Rect &clip_rect)
	{
		part.render_text(gc, value, rect, clip_rect);
	}

	void on_enter_pressed()
	{
		property_component->deactivate();
	}

	CL_String value;
};

class PropertyItemAnchor : public PropertyItem
{
public:
	PropertyItemAnchor() : cap_tl(cl_anchor_top_left), cap_br(cl_anchor_top_left)
	{
		name = "Anchor";
	}

	CL_GUIComponent *activate()
	{
		return new AnchorPopup(cap_tl, cap_br, property_component);
	}

	void deactivate(CL_GUIComponent *component)
	{
		cap_tl = static_cast<AnchorPopup*>(component)->get_anchor_tl();
		cap_br = static_cast<AnchorPopup*>(component)->get_anchor_br();
		delete component;
	}

	CL_String get_value()
	{
		return cl_format("TL: %1 BR: %2", to_string(cap_tl), to_string(cap_br));
	}

	CL_String to_string(CL_ComponentAnchorPoint cap)
	{
		switch (cap)
		{
		case cl_anchor_top_left:
			return "top left";
		case cl_anchor_top_right:
			return "top right";
		case cl_anchor_bottom_left:
			return "bottom left";
		case cl_anchor_bottom_right:
			return "bottom right";
		case cl_anchor_relative:
			return "relative";
		default:
			return "unknown";
		}
	}

	int get_inactive_height(CL_GraphicContext &gc, CL_GUIThemePart &part, int width)
	{
		return part.get_font().get_text_size(gc, get_value()).height;
	}

	void render_inactive(CL_GraphicContext &gc, CL_GUIThemePart &part, const CL_Rect &rect, const CL_Rect &clip_rect)
	{
		part.render_text(gc, get_value(), rect, clip_rect);
	}

	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
		if (!selection.empty())
		{
			HolderComponent *component = selection[0];
			cap_tl = component->get_anchor_tl();
			cap_br = component->get_anchor_br();
		}
	}

	void apply_changes(HolderComponent *component)
	{
		component->set_anchor_tl(cap_tl);
		component->set_anchor_br(cap_br);
	}

private:
	CL_ComponentAnchorPoint cap_tl;
	CL_ComponentAnchorPoint cap_br;
};
