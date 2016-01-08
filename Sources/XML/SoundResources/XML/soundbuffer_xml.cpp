/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/Sound/soundbuffer.h"
#include "API/XML/Resources/xml_resource_document.h"
#include "API/XML/Resources/xml_resource_node.h"
#include "API/XML/dom_element.h"

namespace clan
{
	SoundBuffer SoundBuffer::load(const std::string &id, const XMLResourceDocument &doc)
	{
		XMLResourceNode resource = doc.get_resource(id);
		DomElement &element = resource.get_element();

		std::string name = resource.get_element().get_attribute("file");
		std::string sound_format = resource.get_element().get_attribute("format");
		bool streamed = (element.get_attribute("stream", "no") == "yes");

		return SoundBuffer(name, streamed, sound_format);
	}
}
