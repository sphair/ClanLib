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
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/XML/Resources/xml_resource_node.h"
#include "API/XML/dom_element.h"
#include "API/Display/Render/shader_object.h"

namespace clan
{
	ShaderObject ShaderObject::load(GraphicContext &gc, const std::string &resource_id, const XMLResourceDocument &resources)
	{
		XMLResourceNode resource = resources.get_resource(resource_id);
		std::string filename = resource.get_element().get_attribute("file");
		std::string type = resource.get_element().get_tag_name();

		ShaderType shader_type;
		if (type == "fragment-shader")
			shader_type = ShaderType::fragment;
		else if (type == "vertex-shader")
			shader_type = ShaderType::vertex;
		else
			throw Exception("ShaderObject: Unknown shader type: " + type);

		FileSystem fs = resource.get_file_system();

		IODevice file = fs.open_file(PathHelp::combine(resource.get_base_path(), filename), File::open_existing, File::access_read, File::share_read);
		int size = file.get_size();
		std::string source(size, 0);
		file.read(&source[0], size);

		ShaderObject shader_object(gc, shader_type, source);

		if (resource.get_element().get_attribute("compile", "true") == "true")
			if (!shader_object.compile())
				throw Exception(string_format("Unable to compile shader program %1: %2", resource_id, shader_object.get_info_log()));

		return shader_object;
	}
}
