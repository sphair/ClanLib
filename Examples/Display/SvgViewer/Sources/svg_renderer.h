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
**    Mark Page
*/

#pragma once

#include "svg_element_visitor.h"

class SvgRenderer : private SvgElementVisitor
{
public:
	SvgRenderer(clan::Canvas &canvas);
	void render(clan::DomElement &svg_element);

protected:
	void g(clan::DomElement &e) override;
	void line(clan::DomElement &e) override;
	void polyline(clan::DomElement &e) override;
	void rect(clan::DomElement &e) override;
	void circle(clan::DomElement &e) override;
	void ellipse(clan::DomElement &e) override;
	void polygon(clan::DomElement &e) override;
	void path(clan::DomElement &e) override;
	void text(clan::DomElement &e) override;
	void image(clan::DomElement &e) override;

private:
	void render_path(clan::Path &path, clan::DomElement &e);

	clan::Mat4f get_transform(clan::DomElement &e);

	clan::Canvas &canvas;
};
