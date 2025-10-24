
#pragma once

#include "property_component.h"
#include "main_window.h"
#include "selection.h"

class HolderComponent;

class PropertyItem
{
public:
	PropertyItem() : property_component(0) { }
	virtual ~PropertyItem() { }

	PropertyComponent *property_component;
	CL_String name;

	std::vector<HolderComponent*> get_selection()
	{
		return property_component->get_main_window()->get_selection()->get_selection();
	}

	virtual void selection_changed(const std::vector<HolderComponent *> &selection) { }

	virtual CL_GUIComponent *activate() = 0;
	virtual void deactivate(CL_GUIComponent *component) = 0;

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
	PropertyItemLineEdit(const CL_String &name, const CL_String &value) { this->name = name; this->value = value; }

	CL_GUIComponent *activate() { CL_LineEdit *c = new CL_LineEdit(property_component); c->set_text(value); return c; }
	void deactivate(CL_GUIComponent *component) { value = static_cast<CL_LineEdit *>(component)->get_text(); delete component; }

	int get_inactive_height(CL_GraphicContext &gc, CL_GUIThemePart &part, int width) { return part.get_font().get_text_size(gc, value).height; }
	void render_inactive(CL_GraphicContext &gc, CL_GUIThemePart &part, const CL_Rect &rect, const CL_Rect &clip_rect) { part.render_text(gc, value, rect, clip_rect); }

	CL_String value;
};
