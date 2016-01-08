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

class SvgElementVisitor
{
public:
	void visit(clan::DomElement &e);

protected:
	virtual void a(clan::DomElement &e) { }
	virtual void altGlyph(clan::DomElement &e) { }
	virtual void altGlyphDef(clan::DomElement &e) { }
	virtual void altGlyphItem(clan::DomElement &e) { }
	virtual void animate(clan::DomElement &e) { }
	virtual void animateColor(clan::DomElement &e) { }
	virtual void animateMotion(clan::DomElement &e) { }
	virtual void animateTransform(clan::DomElement &e) { }
	virtual void circle(clan::DomElement &e) { }
	virtual void clipPath(clan::DomElement &e) { }
	virtual void color_profile(clan::DomElement &e) { }
	virtual void cursor(clan::DomElement &e) { }
	virtual void defs(clan::DomElement &e) { }
	virtual void desc(clan::DomElement &e) { }
	virtual void ellipse(clan::DomElement &e) { }
	virtual void feBlend(clan::DomElement &e) { }
	virtual void feColorMatrix(clan::DomElement &e) { }
	virtual void feComponentTransfer(clan::DomElement &e) { }
	virtual void feComposite(clan::DomElement &e) { }
	virtual void feConvolveMatrix(clan::DomElement &e) { }
	virtual void feDiffuseLighting(clan::DomElement &e) { }
	virtual void feDisplacementMap(clan::DomElement &e) { }
	virtual void feDistantLight(clan::DomElement &e) { }
	virtual void feFlood(clan::DomElement &e) { }
	virtual void feFuncA(clan::DomElement &e) { }
	virtual void feFuncB(clan::DomElement &e) { }
	virtual void feFuncG(clan::DomElement &e) { }
	virtual void feFuncR(clan::DomElement &e) { }
	virtual void feGaussianBlur(clan::DomElement &e) { }
	virtual void feImage(clan::DomElement &e) { }
	virtual void feMerge(clan::DomElement &e) { }
	virtual void feMergeNode(clan::DomElement &e) { }
	virtual void feMorphology(clan::DomElement &e) { }
	virtual void feOffset(clan::DomElement &e) { }
	virtual void fePointLight(clan::DomElement &e) { }
	virtual void feSpecularLighting(clan::DomElement &e) { }
	virtual void feSpotLight(clan::DomElement &e) { }
	virtual void feTile(clan::DomElement &e) { }
	virtual void feTurbulence(clan::DomElement &e) { }
	virtual void filter(clan::DomElement &e) { }
	virtual void font(clan::DomElement &e) { }
	virtual void font_face(clan::DomElement &e) { }
	virtual void font_face_format(clan::DomElement &e) { }
	virtual void font_face_name(clan::DomElement &e) { }
	virtual void font_face_src(clan::DomElement &e) { }
	virtual void font_face_uri(clan::DomElement &e) { }
	virtual void foreignObject(clan::DomElement &e) { }
	virtual void g(clan::DomElement &e) { }
	virtual void glyph(clan::DomElement &e) { }
	virtual void glyphRef(clan::DomElement &e) { }
	virtual void hkern(clan::DomElement &e) { }
	virtual void image(clan::DomElement &e) { }
	virtual void line(clan::DomElement &e) { }
	virtual void linearGradient(clan::DomElement &e) { }
	virtual void marker(clan::DomElement &e) { }
	virtual void mask(clan::DomElement &e) { }
	virtual void metadata(clan::DomElement &e) { }
	virtual void missing_glyph(clan::DomElement &e) { }
	virtual void mpath(clan::DomElement &e) { }
	virtual void path(clan::DomElement &e) { }
	virtual void pattern(clan::DomElement &e) { }
	virtual void polygon(clan::DomElement &e) { }
	virtual void polyline(clan::DomElement &e) { }
	virtual void radialGradient(clan::DomElement &e) { }
	virtual void rect(clan::DomElement &e) { }
	virtual void script(clan::DomElement &e) { }
	virtual void set(clan::DomElement &e) { }
	virtual void stop(clan::DomElement &e) { }
	virtual void style(clan::DomElement &e) { }
	virtual void svg(clan::DomElement &e) { }
	virtual void switch_(clan::DomElement &e) { }
	virtual void symbol(clan::DomElement &e) { }
	virtual void text(clan::DomElement &e) { }
	virtual void textPath(clan::DomElement &e) { }
	virtual void title(clan::DomElement &e) { }
	virtual void tref(clan::DomElement &e) { }
	virtual void tspan(clan::DomElement &e) { }
	virtual void use(clan::DomElement &e) { }
	virtual void view(clan::DomElement &e) { }
	virtual void vkern(clan::DomElement &e) { }
};
