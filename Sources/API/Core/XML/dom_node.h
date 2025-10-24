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

//! clanCore="XML"
//! header=core.h

#ifndef header_dom_node
#define header_dom_node

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../System/sharedptr.h"
#include <string>

class CL_DomElement;
class CL_DomAttr;
class CL_DomText;
class CL_DomCDATASection;
class CL_DomEntityReference;
class CL_DomEntity;
class CL_DomProcessingInstruction;
class CL_DomComment;
class CL_DomDocument;
class CL_DomDocumentType;
class CL_DomDocumentFragment;
class CL_DomNotation;
class CL_DomNodeList;
class CL_DomNamedNodeMap;
class CL_DomNode_Generic;

//: DOM Node class.
//- !group=Core/XML!
//- !header=core.h!
//- <p>The Node interface is the primary datatype for the entire Document Object Model.
//- It represents a single node in the document tree. While all objects implementing
//- the Node interface expose methods for dealing with children, not all objects
//- implementing the Node interface may have children. For example, Text nodes may
//- not have children, and adding children to such nodes results in a CL_DOMException
//- being thrown.</p>
//- <p>The attributes 'node_name', 'node_value' and 'attributes' are included as a
//- mechanism to get at node information without casting down to the specific derived
//- interface. In cases where there is no obvious mapping of these attributes for a
//- specific node_type (e.g., node_value for an Element or attributes for a Comment),
//- this returns null. Note that the specialized interfaces may contain additional
//- and more convenient mechanisms to get and set the relevant information.</p>
class CL_API_CORE CL_DomNode
{
//! Construction:
public:
	CL_DomNode();

	CL_DomNode(const CL_DomNode &copy);
	
	~CL_DomNode();

//! Attributes:
public:
	//: An integer indicating which type of node this is.
	enum NodeType
	{
		NULL_NODE                     = 0,
		ELEMENT_NODE                  = 1,
		ATTRIBUTE_NODE                = 2,
		TEXT_NODE                     = 3,
		CDATA_SECTION_NODE            = 4,
		ENTITY_REFERENCE_NODE         = 5,
		ENTITY_NODE                   = 6,
		PROCESSING_INSTRUCTION_NODE   = 7,
		COMMENT_NODE                  = 8,
		DOCUMENT_NODE                 = 9,
		DOCUMENT_TYPE_NODE            = 10,
		DOCUMENT_FRAGMENT_NODE        = 11,
		NOTATION_NODE                 = 12
	};
	
	//: Returns the node name.
	//- <p>The return value vary according to the node type as follows:</p>
	//- <ul>
	//- <li>Element: name of tag</li>
	//- <li>Attr: name of attribute</li>
	//- <li>Text: "#text"</li>
	//- <li>CDataSection: "#cdata-section"</li>
	//- <li>EntityReference: name of entity referenced</li>
	//- <li>Entity: entity name</li>
	//- <li>ProcessingInstruction: target</li>
	//- <li>Comment: "#comment"</li>
	//- <li>Document: "#document"</li>
	//- <li>DocumentType: document type name</li>
	//- <li>DocumentFragment: "#document-fragment"</li>
	//- <li>Notation: notation name</li>
	//- </ul>
	std::string get_node_name() const;
	
	//: Returns the node value.
	//- <p>The return value vary according to the node type as follows:</p>
	//- <ul>
	//- <li>Element: null</li>
	//- <li>Attr: value of attribute</li>
	//- <li>Text: content of text node</li>
	//- <li>CDataSection: content of CDATA Section</li>
	//- <li>EntityReference: null</li>
	//- <li>Entity: null</li>
	//- <li>ProcessingInstruction: entire content excluding the target</li>
	//- <li>Comment: content of the comment</li>
	//- <li>Document: null</li>
	//- <li>DocumentType: null</li>
	//- <li>DocumentFragment: null</li>
	//- <li>Notation: null</li>
	//- </ul>
	std::string get_node_value() const;

	//: Sets the node value.
	void set_node_value(const std::string &value);

	//: Returns the node type (one of those in the NodeType enum).
	unsigned short get_node_type() const;
	
	//: Returns the parent of this node.
	//- <p>All nodes, except Document, DocumentFragment, and Attr may have a parent.
	//- However, if a node has just been created and not yet added to the tree, or if
	//- it has been removed from the tree, this is null.</p>
	CL_DomNode get_parent_node() const;

	//: Returns a NodeList that contains all children of this node.
	//- <p>If there are no children, this is a NodeList containing no nodes. The content
	//- of the returned NodeList is "live" in the sense that, for instance, changes to
	//- the children of the node object that it was created from are immediately reflected
	//- in the nodes returned by the NodeList accessors; it is not a static snapshot of
	//- the content of the node. This is true for every NodeList, including the ones
	//- returned by the getElementsByTagName method.</p>
	CL_DomNodeList get_child_nodes() const;

	//: The first child of this node.
	//- <p>If there is no such node, this returns a null node.</p>
	CL_DomNode get_first_child() const;
	
	//: The last child of this node.
	//- <p>If there is no such node, this returns a null node.</p>
	CL_DomNode get_last_child() const;
	
	//: The node immediately preceding this node.
	//- <p>If there is no such node, this returns a null node.</p>
	CL_DomNode get_previous_sibling() const;
	
	//: The node immediately following this node.
	//- <p>If there is no such node, this returns a null node.</p>
	CL_DomNode get_next_sibling() const;

	//: A NamedNodeMap containing the attributes of this node (if it is an Element) or null otherwise.
	CL_DomNamedNodeMap get_attributes();

	//: The Document object associated with this node.
	//- <p>This is also the Document object used to create new nodes. When this node is a Document this is null.</p>
	CL_DomDocument get_owner_document();

	//: Returns true if this is a null node.
	bool is_null() const;

	//: Returns true if this is an element node.
	bool is_element() const;

	//: Returns true if this is an attribute node.
	bool is_attr() const;

	//: Returns true if this is a text node.
	bool is_text() const;

	//: Returns true if this is a CDATA section node.
	bool is_cdata_section() const;

	//: Returns true if this is an entity reference node.
	bool is_entity_reference() const;

	//: Returns true if this is an entity node.
	bool is_entity() const;

	//: Returns true if this is a processing instruction node.
	bool is_processing_instruction() const;

	//: Returns true if this is a comment node.
	bool is_comment() const;

	//: Returns true if this is a document node.
	bool is_document() const;

	//: Returns true if this is a document type node.
	bool is_document_type() const;

	//: Returns true if this is a document fragment node.
	bool is_document_fragment() const;

	//: Returns true if this is a notation node.
	bool is_notation() const;

	//: Returns true if this node has any children.
	bool has_child_nodes() const;
	
//! Operations:
public:
	//: Copy assignment operator.
	//- <p>All objects in the DOM are handles to the underlying implementation. Therefore this doesn't
	//- actually copy contents between two CL_DomNode's, but instead change the two CL_DomNode's to point
	//- at the same node in the DOM.</p>
	CL_DomNode &operator =(const CL_DomNode &copy);

	//: Compare operator.
	bool operator ==(const CL_DomNode &other) const;

	//: Inserts the node new_child before the existing child node ref_child.
	//- <p>If refChild is a null node, inserts new_child at the end of the list of children.</p>
	//- <p>If newChild is a DocumentFragment object, all of its children are inserted, in the same order,
	//- before ref_child. If the new_child is already in the tree, it is first removed.</p>
	//param new_child: The node to insert.
	//param ref_child: The reference node, i.e., the node before which the new node must be inserted.
	//retval: The node being inserted.
	CL_DomNode insert_before(CL_DomNode &new_child, CL_DomNode &ref_child);

	//: Replaces the child node old_child with new_child in the list of children.
	//- <p>If the new_child is already in the tree, it is first removed.</p>
	//param new_child: The new node to put in the child list.
	//param old_child: The node being replaced in the list.
	//retval: The node replaced.
	CL_DomNode replace_child(CL_DomNode &new_child, CL_DomNode &old_child);

	//: Removes the child node indicated by old_child from the list of children, and returns it.
	CL_DomNode remove_child(CL_DomNode &old_child);

	//: Adds the node new_child to the end of the list of children of this node.
	//- <p>If the new_child is already in the tree, it is first removed.</p>
	CL_DomNode append_child(CL_DomNode new_child);

	//: Returns a duplicate of this node, i.e., serves as a generic copy constructor for nodes.
	//- <p>The duplicate node has no parent.</p>
	//- <p>Cloning an Element copies all attributes and their values, including those generated
	//- by the XML processor to represent defaulted attributes, but this method does not copy
	//- any text it contains unless it is a deep clone, since the text is contained in a child
	//- Text node. Cloning any other type of node simply returns a copy of this node.</p>
	//param deep: If true, recursively clone the subtree under the specified node; if false, clone only the node itself (and its attributes, if it is an Element).
	//retval: The duplicate node.
	CL_DomNode clone_node(bool deep) const;
	
	//: Returns whether this node is the same node as the given one.
    //- <p>This method provides a way to determine whether two Node references returned by the implementation reference the same object.
	//- When two Node references are references to the same object, even if through a proxy, the references may be used completely interchangeably,
	//- such that all attributes have the same values and calling the same DOM method on either reference always has exactly the same effect.</p>
	//param other: The node to test against.
	//retval: Returns true if the nodes are the same, false otherwise.
	bool is_same_node(const CL_DomNode &other) const;

	//: Returns the Element interface to this node.
	//- <p>If the node is not an Element node, then it returns a null node.</p>
	CL_DomElement to_element() const;

	//: Returns the Attribute interface to this node.
	//- <p>If the node is not an Attribute node, then it returns a null node.</p>
	CL_DomAttr to_attr() const;

	//: Returns the Text interface to this node.
	//- <p>If the node is not a Text node, then it returns a null node.</p>
	CL_DomText to_text() const;

	//: Returns the CDATA Section interface to this node.
	//- <p>If the node is not a CDATA Section node, then it returns a null node.</p>
	CL_DomCDATASection to_cdata_section() const;

	//: Returns the Entity Reference interface to this node.
	//- <p>If the node is not an Entity Reference node, then it returns a null node.</p>
	CL_DomEntityReference to_entity_reference() const;

	//: Returns the Entity interface to this node.
	//- <p>If the node is not an Entity node, then it returns a null node.</p>
	CL_DomEntity to_entity() const;

	//: Returns the Processing Instruction interface to this node.
	//- <p>If the node is not a Processing Instrucion node, then it returns a null node.</p>
	CL_DomProcessingInstruction to_processing_instruction() const;

	//: Returns the Comment interface to this node.
	//- <p>If the node is not a Comment node, then it returns a null node.</p>
	CL_DomComment to_comment() const;

	//: Returns the Document interface to this node.
	//- <p>If the node is not a Document node, then it returns a null node.</p>
	CL_DomDocument to_document() const;

	//: Returns the Document Type interface to this node.
	//- <p>If the node is not a Document Type node, then it returns a null node.</p>
	CL_DomDocumentType to_document_type() const;

	//: Returns the Document Fragment interface to this node.
	//- <p>If the node is not a DocumentFragment node, then it returns a null node.</p>
	CL_DomDocumentFragment to_document_fragment() const;

	//: Returns the Notation interface to this node.
	//- <p>If the node is not a Notation node, then it returns a null node.</p>
	CL_DomNotation to_notation() const;

	//: Returns the first child node with the specified node name.
	//- <p>Returns a null node if no child is found.</p>
	CL_DomNode named_item(const std::string &name) const;

//! Implementation:
protected:
	CL_DomNode(CL_DomDocument &doc, unsigned short node_type);

	CL_DomNode(const CL_SharedPtr<CL_DomNode_Generic> &impl);

	CL_SharedPtr<CL_DomNode_Generic> impl;

	friend class CL_DomDocument;
};

#endif
