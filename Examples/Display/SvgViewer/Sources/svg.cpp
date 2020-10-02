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
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "svg.h"
#include "svg_tree.h"

Svg::Svg(const std::string &filename)
{
	auto device = clan::File(filename);
	xml.load(device, false);
}

void Svg::render(clan::Canvas &canvas)
{
	if (!root_node)
	{
		SvgTreeBuilder builder(canvas);
		auto element = xml.get_document_element();
		builder.build(element);
		root_node = builder.node;
	}

	root_node->render(canvas);
}

const std::string Svg::svg_ns = "http://www.w3.org/2000/svg";
const std::string Svg::xlink_ns = "http://www.w3.org/1999/xlink";
