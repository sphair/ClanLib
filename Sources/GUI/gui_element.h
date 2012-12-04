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

namespace clan
{

class GUIElement_Impl;
class CSSBoxProperties;
class CSSDocument;
class CSSResourceCache;

/// \brief A GUI element
class GUIElement
{
/// \name Construction
/// \{
public:
	/// \brief Creates a GUI element
	explicit GUIElement(GUIElement *parent);
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns the standard W3C CSS properties active for this component
	const CSSBoxProperties &get_css_properties() const;

	/// \brief Returns the standard W3C CSS properties active for this component
	CSSBoxProperties &get_css_properties();

	/// \brief Returns the first child component.
	const GUIElement *get_first_child() const;

	/// \brief Get Last child
	///
	/// \return last_child
	GUIElement *get_last_child();

	/// \brief Get First child
	///
	/// \return first_child
	GUIElement *get_first_child();

	/// \brief Returns the last child component.
	const GUIElement *get_last_child() const;

	/// \brief Returns the previous sibling component.
	const GUIElement *get_previous_sibling() const;

	/// \brief Get Previous sibling
	///
	/// \return previous_sibling
	GUIElement *get_previous_sibling();

	/// \brief Returns the next sibling component.
	const GUIElement *get_next_sibling() const;

	/// \brief Get Next sibling
	///
	/// \return next_sibling
	GUIElement *get_next_sibling();

	/// \brief Returns the parent component.
	/** <p>Only child components has a parent.</p>*/
	const GUIElement *get_parent_component() const;

	/// \brief Get Parent component
	///
	/// \return parent_component
	GUIElement *get_parent_component();

	/// \brief Returns the DOM tag name
	std::string get_tag_name() const;

	/// \brief Returns the DOM id attribute
	std::string get_id() const;

	/// \brief Returns the DOM class attribute
	std::string get_class() const;

	/// \brief Returns whether a DOM pseudo class is present or not.
	bool get_pseudo_class(const std::string &name) const;

	/// \brief Returns all pseudo classes currently present
	std::vector<std::string> get_pseudo_classes() const;

/// \}

/// \name Operations
/// \{
public:
	/// \brief Sets the DOM tag name
	void set_tag_name(const std::string &name); 

	/// \brief Sets the DOM class
	void set_class(const std::string &name);

	/// \brief Sets the DOM id
	void set_id(const std::string &name);

	/// \brief Controls the presence of a DOM pseudo class
	///
	/// \return true, if state changed
	bool set_pseudo_class(const std::string &name, bool enable);

	/// \brief Makes this element a child of the given element, removing it from the previous parent.
	void set_parent(GUIElement *new_parent);

	/// \brief Re-evaluates which CSS selectors match this component
	void update_style(CSSResourceCache *resource_cache, CSSDocument &document);

/// \}

/// \name Implementation
/// \{
public:
	/// \brief Constructs a GUIElement
	///
	/// \param other = GUIElement
	GUIElement(GUIElement &other);

	GUIElement &operator =(const GUIElement &other);

	std::shared_ptr<GUIElement_Impl> impl;
/// \}
};

}
