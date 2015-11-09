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
**    Harry Storbacka
*/

#include "XML/precomp.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/Render/texture_2d_array.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Image/pixel_buffer_set.h"
#include "API/XML/Resources/xml_resource_manager.h"
#include "API/XML/Resources/xml_resource_node.h"
#include "API/XML/dom_element.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_format.h"

namespace clan
{
	Texture Texture::load(GraphicContext &gc, const std::string &id, const XMLResourceDocument &doc, const ImageImportDescription &import_desc)
	{
		XMLResourceNode resource = doc.get_resource(id);

		std::string type = resource.get_type();

		if (type != "texture")
			throw Exception(string_format("Resource '%1' is not of type 'texture'", id));

		std::string filename = resource.get_element().get_attribute("file");
		FileSystem fs = resource.get_file_system();

		Texture2D texture(gc, PathHelp::combine(resource.get_base_path(), filename), fs, import_desc);

		return Resource<Texture>(texture);
	}
}
