/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Mark Page
*/

#pragma once

#include <memory>
#include "API/CSSLayout/ComputedValues/css_computed_values.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"
#include "API/Core/Signals/callback_v1.h"
#include "API/Core/Signals/signal_v0.h"

namespace clan
{

class GUIComponent;
class GUIElement_Impl;
class CSSDocument;
class CSSResourceCache;

/// \brief A GUI element
class GUIElement
{
/// \name Construction
/// \{
public:
	/// \brief Creates a GUI element
	GUIElement();

	~GUIElement();

/// \}

/// \name Attributes
/// \{
public:
	const GUIElement *get_first_child() const { return first_child; }
	GUIElement *get_first_child() { return first_child; }
	const GUIElement *get_last_child() const { return last_child; }
	GUIElement *get_last_child() { return last_child; }
	const GUIElement *get_previous_sibling() const { return prev_sibling; }
	GUIElement *get_previous_sibling() { return prev_sibling; }
	const GUIElement *get_next_sibling() const { return next_sibling; }
	GUIElement *get_next_sibling() { return next_sibling; }
	const CSSComputedValues &get_css_values() const;
	const GUIElement *get_parent_component() const { return parent; }
	const std::string &get_tag_name() const { return tag_name; }
	const std::string &get_id() const { return id;}
	const std::string &get_class() const { return class_string; }
	GUIElement *get_parent_component() { return parent; }

	/// \brief Returns whether a DOM pseudo class is present or not.
	bool get_pseudo_class(const std::string &name) const;

	/// \brief Returns all pseudo classes currently present
	std::vector<std::string> get_pseudo_classes() const { return pseudo_classes; }


/// \}

/// \name Operations
/// \{
public:
	/// \brief Sets the component owning this element
	void set_component(GUIComponent *component);

	void set_tag_name(const std::string &name);
	void set_class(const std::string &name);
	void set_id(const std::string &name);

	/// \brief Controls the presence of a DOM pseudo class
	///
	/// \return true, if state changed
	bool set_pseudo_class(const std::string &name, bool enable);

	/// \brief Makes this element a child of the given element, removing it from the previous parent.
	void set_parent(GUIElement *new_parent);

/// \}
/// \name Signals and callbacks
/// \{
public:
	Signal_v0 sig_style_changed;
	Callback_v1<CSSComputedBox &> func_apply_properties;

/// \}

/// \name Implementation
/// \{
private:
	/// \brief Updates style_needs_update recursively
	void set_style_needs_update();

	/// \brief Re-evaluates which CSS selectors match this component
	void update_style();

	GUIElement(const GUIElement &other); // Do not implement; copy construction not allowed
	GUIElement &operator =(const GUIElement &other); // Do not implement; copy construction not allowed

	GUIComponent *component;

	GUIElement *parent;
	GUIElement *prev_sibling;
	GUIElement *next_sibling;
	GUIElement *first_child;
	GUIElement *last_child;

	CSSComputedValues computed_values;

	std::string tag_name;
	std::string id;
	std::string class_string;
	std::vector<std::string> pseudo_classes;

	bool style_needs_update;
/// \}
};

}
