/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

class SvgElementVisitor
{
public:
	void visit(clan::DomElement &e);

protected:
	void a(clan::DomElement &e) { }
	void altGlyph(clan::DomElement &e) { }
	void altGlyphDef(clan::DomElement &e) { }
	void altGlyphItem(clan::DomElement &e) { }
	void animate(clan::DomElement &e) { }
	void animateColor(clan::DomElement &e) { }
	void animateMotion(clan::DomElement &e) { }
	void animateTransform(clan::DomElement &e) { }
	void circle(clan::DomElement &e) { }
	void clipPath(clan::DomElement &e) { }
	void color_profile(clan::DomElement &e) { }
	void cursor(clan::DomElement &e) { }
	void defs(clan::DomElement &e) { }
	void desc(clan::DomElement &e) { }
	void ellipse(clan::DomElement &e) { }
	void feBlend(clan::DomElement &e) { }
	void feColorMatrix(clan::DomElement &e) { }
	void feComponentTransfer(clan::DomElement &e) { }
	void feComposite(clan::DomElement &e) { }
	void feConvolveMatrix(clan::DomElement &e) { }
	void feDiffuseLighting(clan::DomElement &e) { }
	void feDisplacementMap(clan::DomElement &e) { }
	void feDistantLight(clan::DomElement &e) { }
	void feFlood(clan::DomElement &e) { }
	void feFuncA(clan::DomElement &e) { }
	void feFuncB(clan::DomElement &e) { }
	void feFuncG(clan::DomElement &e) { }
	void feFuncR(clan::DomElement &e) { }
	void feGaussianBlur(clan::DomElement &e) { }
	void feImage(clan::DomElement &e) { }
	void feMerge(clan::DomElement &e) { }
	void feMergeNode(clan::DomElement &e) { }
	void feMorphology(clan::DomElement &e) { }
	void feOffset(clan::DomElement &e) { }
	void fePointLight(clan::DomElement &e) { }
	void feSpecularLighting(clan::DomElement &e) { }
	void feSpotLight(clan::DomElement &e) { }
	void feTile(clan::DomElement &e) { }
	void feTurbulence(clan::DomElement &e) { }
	void filter(clan::DomElement &e) { }
	void font(clan::DomElement &e) { }
	void font_face(clan::DomElement &e) { }
	void font_face_format(clan::DomElement &e) { }
	void font_face_name(clan::DomElement &e) { }
	void font_face_src(clan::DomElement &e) { }
	void font_face_uri(clan::DomElement &e) { }
	void foreignObject(clan::DomElement &e) { }
	void g(clan::DomElement &e) { }
	void glyph(clan::DomElement &e) { }
	void glyphRef(clan::DomElement &e) { }
	void hkern(clan::DomElement &e) { }
	void image(clan::DomElement &e) { }
	void line(clan::DomElement &e) { }
	void linearGradient(clan::DomElement &e) { }
	void marker(clan::DomElement &e) { }
	void mask(clan::DomElement &e) { }
	void metadata(clan::DomElement &e) { }
	void missing_glyph(clan::DomElement &e) { }
	void mpath(clan::DomElement &e) { }
	void path(clan::DomElement &e) { }
	void pattern(clan::DomElement &e) { }
	void polygon(clan::DomElement &e) { }
	void polyline(clan::DomElement &e) { }
	void radialGradient(clan::DomElement &e) { }
	void rect(clan::DomElement &e) { }
	void script(clan::DomElement &e) { }
	void set(clan::DomElement &e) { }
	void stop(clan::DomElement &e) { }
	void style(clan::DomElement &e) { }
	void svg(clan::DomElement &e) { }
	void switch_(clan::DomElement &e) { }
	void symbol(clan::DomElement &e) { }
	void text(clan::DomElement &e) { }
	void textPath(clan::DomElement &e) { }
	void title(clan::DomElement &e) { }
	void tref(clan::DomElement &e) { }
	void tspan(clan::DomElement &e) { }
	void use(clan::DomElement &e) { }
	void view(clan::DomElement &e) { }
	void vkern(clan::DomElement &e) { }
};
