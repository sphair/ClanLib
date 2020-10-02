/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "XML/precomp.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/Render/shader_object.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/XML/Resources/xml_resource_node.h"
#include "API/XML/dom_element.h"

namespace clan
{
	ProgramObject ProgramObject::load(
		GraphicContext &gc,
		const std::string &resource_id,
		const XMLResourceDocument &resources)
	{
		ProgramObject program_object(gc);

		XMLResourceNode resource = resources.get_resource(resource_id);

		DomNode node = resource.get_element().get_first_child();

		while (!node.is_null())
		{
			if (node.is_element())
			{
				DomElement element = node.to_element();
				if (element.get_tag_name() == "shader")
				{
					ShaderObject shader = ShaderObject::load(gc, element.get_attribute("name"), resources);
					program_object.attach(shader);
				}
				else if (element.get_tag_name() == "bind-attribute")
				{
					program_object.bind_attribute_location(
						StringHelp::text_to_int(element.get_attribute("index")),
						element.get_attribute("name"));
				}
			}
			node = node.get_next_sibling();
		}

		if (!resource.get_element().get_attribute("shader").empty())
		{
			ShaderObject shader = ShaderObject::load(gc, resource.get_element().get_attribute("shader"), resources);
			program_object.attach(shader);
		}

		if (resource.get_element().get_attribute("link", "true") == "true")
			if (!program_object.link())
				throw Exception(string_format("Unable to link program object: %1", program_object.get_info_log()));

		return program_object;
	}
}
