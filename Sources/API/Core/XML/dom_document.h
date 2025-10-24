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

/// \addtogroup clanCore_XML clanCore XML
/// \{

#pragma once

#include "../api_core.h"
#include "dom_node.h"
#include <vector>

class CL_IODevice;
class CL_DomDocumentType;
class CL_DomImplementation;
class CL_DomCDATASection;
class CL_DomProcessingInstruction;
class CL_DomElement;
class CL_DomDocumentFragment;
class CL_DomComment;
class CL_DomText;
class CL_DomAttr;
class CL_DomEntityReference;
class CL_DomNodeList;
class CL_DomDocument_Generic;

/// \brief DOM Document class.
///
/// <p>The Document interface represents the entire HTML or XML document.
///    Conceptually, it is the root of the document tree, and provides the
///    primary access to the document's data.</p>
///    <p>Since elements, text nodes, comments, processing instructions, etc.
///    cannot exist outside the context of a Document, the Document interface
///    also contains the factory methods needed to create these objects. The
///    Node objects created have a ownerDocument attribute which associates
///    them with the Document within whose context they were created.</p>
/// \xmlonly !group=Core/XML! !header=core.h! \endxmlonly
class CL_API_CORE CL_DomDocument : public CL_DomNode
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a DOM Document.
	CL_DomDocument();

	/// \brief Constructs a DomDocument
	///
	/// \param input = IODevice
	/// \param eat_whitespace = bool
	CL_DomDocument(CL_IODevice &input, bool eat_whitespace = true);

	CL_DomDocument(
		const CL_DomString &namespace_uri,
		const CL_DomString &qualified_name,
		const CL_DomDocumentType &doctype);

	/// \brief Constructs a DomDocument
	///
	/// \param impl = Shared Ptr
	CL_DomDocument(const CL_SharedPtr<CL_DomNode_Generic> &impl);

	~CL_DomDocument();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the Document Type Declaration (see CL_DomDocumentType)associated with this document.
	/** <p>For HTML documents as well as XML documents without a document type declaration this
	    returns null. The DOM Level 1 does not support editing the Document Type Declaration,
	    therefore docType cannot be altered in any way.</p>*/
	CL_DomDocumentType get_doctype();

	/// \brief Returns the CL_DomImplementation object that handles this document.
	CL_DomImplementation get_implementation();

	/// \brief Returns the root document element node.
	/** <p>This is a convenience attribute that allows direct access to the child node that is
	    the root element of the document. For HTML documents, this is the element with the tag name "HTML".</p>*/
	CL_DomElement get_document_element();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Creates an element of the type specified.
	/** <p>Note that the instance returned implements the Element interface, so attributes can be
	    specified directly on the returned object.</p>
	    \param tag_name The name of the element type to instantiate. For XML, this is case-sensitive.*/
	CL_DomElement create_element(const CL_DomString &tag_name);

	/// \brief Creates an element of the given qualified name and namespace URI.
	CL_DomElement create_element_ns(
		const CL_DomString &namespace_uri,
		const CL_DomString &qualified_name);

	/// \brief Creates an empty DocumentFragment object.
	CL_DomDocumentFragment create_document_fragment();

	/// \brief Creates a Text node given the specified string.
	///
	/// \param data The data for the node.
	CL_DomText create_text_node(const CL_DomString &data);

	/// \brief Creates a Comment node given the specified string.
	///
	/// \param data The data for the node.
	CL_DomComment create_comment(const CL_DomString &data);

	/// \brief Creates a CDATASection node whose value is the specified string.
	///
	/// \param data The data for the CDATASection contents.
	CL_DomCDATASection create_cdata_section(const CL_DomString &data);

	/// \brief Creates a ProcessingInstruction node given the specified name and data strings.
	///
	/// \param target The target part of the processing instruction.
	/// \param data The data for the node.
	CL_DomProcessingInstruction create_processing_instruction(
		const CL_DomString &target,
		const CL_DomString &data);

	/// \brief Creates an Attr of the given name.
	/** <p>Note that the Attr instance can then be set on an Element using the setAttribute method.</p>
	    \param name The name of the attribute.*/
	CL_DomAttr create_attribute(const CL_DomString &name);

	/// \brief Creates an attribute of the given qualified name and namespace URI.
	/** <p>Note that the Attr instance can then be set on an Element using the setAttribute method.</p>*/
	CL_DomAttr create_attribute_ns(
		const CL_DomString &namespace_uri,
		const CL_DomString &qualified_name);

	/// \brief Creates an EntityReference object.
	///
	/// \param name The name of the entity to reference.
	CL_DomEntityReference create_entity_reference(const CL_DomString &name);

	/// \brief Returns a NodeList of all the Elements with a given tag name.
	/** <p>The list is in the order in which they would be encountered
	    in a preorder traversal of the Document tree.</p>
	    \param tag_name The name of the tag to match on. The special value "*" matches all tags.*/
	CL_DomNodeList get_elements_by_tag_name(const CL_DomString &tag_name);

	/// \brief Returns a NodeList of all the Elements with a given local name and namespace URI.
	/** <p>The list is in the order in which they would be encountered
	    in a preorder traversal of the Document tree.</p>*/
	CL_DomNodeList get_elements_by_tag_name_ns(
		const CL_DomString &namespace_uri,
		const CL_DomString &qualified_name);

	/// \brief Returns the Element whose ID is given by element_id.
	CL_DomElement get_element_by_id(const CL_DomString &element_id);

	/// \brief Imports a node from another document to this document.
	/** <p>The returned node has no parent. The source node is not
	    altered or removed from the original document; this method
	    creates a new copy of the source node.</p>
	    <p>For all nodes, importing a node creates a node object
	    owned by the importing document, with attribute values
	    identical to the source node's node_name and node_type, plus
	    the attributes related to namespaces (prefix, local_name,
	    and namespace_uri). As in the clone_node operation on a Node,
	    the source node is not altered.</p>*/
	CL_DomNode import_node(
		const CL_DomNode &node,
		bool deep);

	/// \brief Loads the DOM document as XML from an input source.
	///
	/// \param input Input source to read from.
	/// \param eat_whitespace Passed on to CL_XMLTokenizer::set_eat_whitespace.
	/// \param insert_point Place in the DOM to insert the loaded XML code.
	/// \return List of all top level nodes created.
	std::vector<CL_DomNode> load(
		CL_IODevice &input,
		bool eat_whitespace = true,
		CL_DomNode insert_point = CL_DomNode());

	/// \brief Saves the DOM document as XML to an input source.
	///
	/// \param output Output source to write to.
	/// \param insert_whitespace Passed on to CL_XMLWriter::set_insert_whitespace.
	void save(CL_IODevice &output, bool insert_whitespace = true);

	/// \brief Removes all nodes from the DOM document.
	void clear_all();

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
