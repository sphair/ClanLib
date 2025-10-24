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
**    Magnus Norddahl
*/

/// \addtogroup clanCore_CSS clanCore CSS
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"

class CL_CSSBox;
class CL_CSSInlineTextBox;
class CL_CSSInlineObjectBox;
class CL_DomElement;
class CL_CSSBoxLayout_Impl;

/// (Internal ClanLib Class)
/// \xmlonly !hide! \endxmlonly
class CL_API_CORE CL_CSSBoxLayout
{
/// \name Construction
/// \{

public:
	/// \brief CSS Box Layout constructor
	CL_CSSBoxLayout();

	CL_CSSBoxLayout(CL_SharedPtr<CL_CSSBoxLayout_Impl> impl);

	/// \brief CSS Box Layout destructor
	~CL_CSSBoxLayout();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Body box
	///
	/// \return body_box
	CL_CSSBox get_body_box();

/// \}
/// \name Operations
/// \{

public:

	/// \brief Create inline box
	///
	/// \return CSSBox
	CL_CSSBox create_inline_box();

	/// \brief Create inline text box
	///
	/// \param text = String Ref
	///
	/// \return CSSInline Text Box
	CL_CSSInlineTextBox create_inline_text_box(const CL_StringRef &text);

	/// \brief Create inline object box
	///
	/// \param element = Dom Element
	///
	/// \return CSSInline Object Box
	CL_CSSInlineObjectBox create_inline_object_box(const CL_DomElement &element);

	/// \brief Create block box
	///
	/// \return CSSBox
	CL_CSSBox create_block_box();

	/// \brief Create list item box
	///
	/// \return CSSBox
	CL_CSSBox create_list_item_box();

	/// \brief Create run in box
	///
	/// \return CSSBox
	CL_CSSBox create_run_in_box();

	/// \brief Create compact box
	///
	/// \return CSSBox
	CL_CSSBox create_compact_box();

	/// \brief Create marker box
	///
	/// \return CSSBox
	CL_CSSBox create_marker_box();

	/// \brief Create table box
	///
	/// \return CSSBox
	CL_CSSBox create_table_box();

	/// \brief Create inline table box
	///
	/// \return CSSBox
	CL_CSSBox create_inline_table_box();

	/// \brief Create table row group box
	///
	/// \return CSSBox
	CL_CSSBox create_table_row_group_box();

	/// \brief Create table header group box
	///
	/// \return CSSBox
	CL_CSSBox create_table_header_group_box();

	/// \brief Create table footer group box
	///
	/// \return CSSBox
	CL_CSSBox create_table_footer_group_box();

	/// \brief Create table row box
	///
	/// \return CSSBox
	CL_CSSBox create_table_row_box();

	/// \brief Create table column group box
	///
	/// \return CSSBox
	CL_CSSBox create_table_column_group_box();

	/// \brief Create table column box
	///
	/// \return CSSBox
	CL_CSSBox create_table_column_box();

	/// \brief Create table cell box
	///
	/// \return CSSBox
	CL_CSSBox create_table_cell_box();

	/// \brief Create table caption box
	///
	/// \return CSSBox
	CL_CSSBox create_table_caption_box();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_CSSBoxLayout_Impl> impl;
/// \}
};

class CL_CSSBox_Impl;

/// (Internal ClanLib Class)
/// \xmlonly !hide! \endxmlonly
class CL_API_CORE CL_CSSBox
{
/// \name Construction
/// \{

public:
	CL_CSSBox();

	CL_CSSBox(CL_SharedPtr<CL_CSSBox_Impl> impl);

	~CL_CSSBox();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Box layout
	///
	/// \return box_layout
	CL_CSSBoxLayout get_box_layout();

	enum Type
	{
		NULL_BOX,
		INLINE_BOX,
		INLINE_TEXT_BOX,
		INLINE_OBJECT_BOX,
		BLOCK_BOX,
		LIST_ITEM_BOX,
		RUN_IN_BOX,
		COMPACT_BOX,
		MARKER_BOX,
		TABLE_BOX,
		INLINE_TABLE_BOX,
		TABLE_ROW_GROUP_BOX,
		TABLE_HEADER_GROUP_BOX,
		TABLE_FOOTER_GROUP_BOX,
		TABLE_ROW_BOX,
		TABLE_COLUMN_GROUP_BOX,
		TABLE_COLUMN_BOX,
		TABLE_CELL_BOX,
		TABLE_CAPTION_BOX
	};

	/// \brief Get Type
	///
	/// \return type
	Type get_type() const;

	/// \brief Returns the parent of this box.
	/** <p>If there is no such box, this returns a null box.</p>*/
	CL_CSSBox get_parent_box() const;

	/// \brief The first child of this box.
	/** <p>If there is no such box, this returns a null box.</p>*/
	CL_CSSBox get_first_child() const;

	/// \brief The last child of this box.
	/** <p>If there is no such box, this returns a null box.</p>*/
	CL_CSSBox get_last_child() const;

	/// \brief The box immediately preceding this box.
	/** <p>If there is no such box, this returns a null box.</p>*/
	CL_CSSBox get_previous_sibling() const;

	/// \brief The box immediately following this box.
	/** <p>If there is no such box, this returns a null box.</p>*/
	CL_CSSBox get_next_sibling() const;

	/// \brief Returns true if this is a null box.
	bool is_null() const;

	/// \brief Returns true if this is an anonymous (generated) box.
	bool is_anonymous() const;

	/// \brief Returns true if this is an inline box.
	bool is_inline_box() const;

	/// \brief Returns true if this is an inline text box.
	bool is_inline_text_box() const;

	/// \brief Returns true if this is an inline object box.
	bool is_inline_object_box() const;

	/// \brief Returns true if this is a block box.
	bool is_block_box() const;

	/// \brief Returns true if this is a list item box.
	bool is_list_item_box() const;

	/// \brief Returns true if this is a run-in box.
	bool is_run_in_box() const;

	/// \brief Returns true if this is a compact box.
	bool is_compact_box() const;

	/// \brief Returns true if this is a marker box.
	bool is_marker_box() const;

	/// \brief Returns true if this is a table box.
	bool is_table_box() const;

	/// \brief Returns true if this is an inline-table box.
	bool is_inline_table_box() const;

	/// \brief Returns true if this is a table row group box.
	bool is_table_row_group_box() const;

	/// \brief Returns true if this is a table header group box.
	bool is_table_header_group_box() const;

	/// \brief Returns true if this is a table footer group box.
	bool is_table_footer_group_box() const;

	/// \brief Returns true if this is a table row box.
	bool is_table_row_box() const;

	/// \brief Returns true if this is a table column group box.
	bool is_table_column_group_box() const;

	/// \brief Returns true if this is a table column box.
	bool is_table_column_box() const;

	/// \brief Returns true if this is a table cell box.
	bool is_table_cell_box() const;

	/// \brief Returns true if this is a table caption box.
	bool is_table_caption_box() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief To inline text box
	///
	/// \return CSSInline Text Box
	CL_CSSInlineTextBox to_inline_text_box() const;

	/// \brief To inline object box
	///
	/// \return CSSInline Object Box
	CL_CSSInlineObjectBox to_inline_object_box() const;

	/// \brief Adds the box new_box to the end of the list of children of this box.
	/** <p>If any anonymous boxes are required to make the layout valid, they
	    are automatically generated when the child is appended.  For example,
	    if a table cell box is appended to a table box, a table row box is
	    automatically generated and inserted in between the table and the
	    cell.</p>
	    <p>If new_box is already in the tree, it is first removed.</p>*/
	CL_CSSBox append_child(CL_CSSBox new_child);

	/// \brief Removes the box from the list of children of this box.
	CL_CSSBox remove_child(CL_CSSBox child);

/// \}
/// \name Implementation
/// \{

protected:
	CL_SharedPtr<CL_CSSBox_Impl> impl;
/// \}
};

/// (Internal ClanLib Class)
/// \xmlonly !hide! \endxmlonly
class CL_API_CORE CL_CSSInlineTextBox : CL_CSSBox
{
/// \name Construction
/// \{

public:
	CL_CSSInlineTextBox();

	CL_CSSInlineTextBox(CL_SharedPtr<CL_CSSBox_Impl> impl);

	~CL_CSSInlineTextBox();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Text
	///
	/// \return text
	CL_StringRef get_text() const;
/// \}
};

/// (Internal ClanLib Class)
/// \xmlonly !hide! \endxmlonly
class CL_API_CORE CL_CSSInlineObjectBox : CL_CSSBox
{
/// \name Construction
/// \{

public:
	CL_CSSInlineObjectBox();

	CL_CSSInlineObjectBox(CL_SharedPtr<CL_CSSBox_Impl> impl);

	~CL_CSSInlineObjectBox();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Object element
	///
	/// \return object_element
	CL_DomElement get_object_element() const;
/// \}
};

/// \}
