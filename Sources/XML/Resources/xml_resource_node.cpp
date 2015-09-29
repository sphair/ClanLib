/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "XML/precomp.h"
#include "API/XML/Resources/xml_resource_node.h"
#include "API/XML/Resources/xml_resource_document.h"
#include "API/XML/dom_element.h"
#include "API/Core/IOData/path_help.h"
#include "xml_resource_document_impl.h"
#include <memory>
#include <vector>

namespace clan
{
	class XMLResourceDocument_Impl;

	class XMLResourceNode_Impl
	{
	public:
		std::weak_ptr<XMLResourceDocument_Impl> resource_document;
		DomElement element;
	};

	XMLResourceNode::XMLResourceNode()
	{
	}

	XMLResourceNode::XMLResourceNode(DomElement element, XMLResourceDocument &resource_document)
		: impl(std::make_shared<XMLResourceNode_Impl>())
	{
		impl->element = element;
		impl->resource_document = std::weak_ptr<XMLResourceDocument_Impl>(resource_document.impl);
	}

	XMLResourceNode::~XMLResourceNode()
	{
	}

	bool XMLResourceNode::is_null() const
	{
		return !impl;
	}

	std::string XMLResourceNode::get_type() const
	{
		return impl->element.get_local_name();
	}

	std::string XMLResourceNode::get_name() const
	{
		return impl->element.get_attribute("name");
	}

	DomElement &XMLResourceNode::get_element()
	{
		return impl->element;
	}

	XMLResourceDocument XMLResourceNode::get_document()
	{
		return XMLResourceDocument(impl->resource_document);
	}

	FileSystem XMLResourceNode::get_file_system() const
	{
		return impl->resource_document.lock()->fs;
	}

	std::string XMLResourceNode::get_base_path() const
	{
		return impl->resource_document.lock()->base_path;
	}

	IODevice XMLResourceNode::open_file(const std::string &filename, File::OpenMode mode, unsigned int access, unsigned int share, unsigned int flags) const
	{
		return get_file_system().open_file(PathHelp::combine(get_base_path(), filename), mode, access, share, flags);
	}

	bool XMLResourceNode::operator ==(const XMLResourceNode &other) const
	{
		return impl == other.impl;
	}
}
