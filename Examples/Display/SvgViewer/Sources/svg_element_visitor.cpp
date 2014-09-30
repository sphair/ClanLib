
#include "precomp.h"
#include "svg_element_visitor.h"

void SvgElementVisitor::visit(clan::DomElement &e)
{
	// To do: do a more efficient search for the correct handler

	auto local_name = e.get_local_name();
	if (local_name == "a") a(e);
	else if (local_name == "altGlyph") altGlyph(e);
	else if (local_name == "altGlyphDef") altGlyphDef(e);
	else if (local_name == "altGlyphItem") altGlyphItem(e);
	else if (local_name == "animate") animate(e);
	else if (local_name == "animateColor") animateColor(e);
	else if (local_name == "animateMotion") animateMotion(e);
	else if (local_name == "animateTransform") animateTransform(e);
	else if (local_name == "circle") circle(e);
	else if (local_name == "clipPath") clipPath(e);
	else if (local_name == "color-profile") color_profile(e);
	else if (local_name == "cursor") cursor(e);
	else if (local_name == "defs") defs(e);
	else if (local_name == "desc") desc(e);
	else if (local_name == "ellipse") ellipse(e);
	else if (local_name == "feBlend") feBlend(e);
	else if (local_name == "feColorMatrix") feColorMatrix(e);
	else if (local_name == "feComponentTransfer") feComponentTransfer(e);
	else if (local_name == "feComposite") feComposite(e);
	else if (local_name == "feConvolveMatrix") feConvolveMatrix(e);
	else if (local_name == "feDiffuseLighting") feDiffuseLighting(e);
	else if (local_name == "feDisplacementMap") feDisplacementMap(e);
	else if (local_name == "feDistantLight") feDistantLight(e);
	else if (local_name == "feFlood") feFlood(e);
	else if (local_name == "feFuncA") feFuncA(e);
	else if (local_name == "feFuncB") feFuncB(e);
	else if (local_name == "feFuncG") feFuncG(e);
	else if (local_name == "feFuncR") feFuncR(e);
	else if (local_name == "feGaussianBlur") feGaussianBlur(e);
	else if (local_name == "feImage") feImage(e);
	else if (local_name == "feMerge") feMerge(e);
	else if (local_name == "feMergeNode") feMergeNode(e);
	else if (local_name == "feMorphology") feMorphology(e);
	else if (local_name == "feOffset") feOffset(e);
	else if (local_name == "fePointLight") fePointLight(e);
	else if (local_name == "feSpecularLighting") feSpecularLighting(e);
	else if (local_name == "feSpotLight") feSpotLight(e);
	else if (local_name == "feTile") feTile(e);
	else if (local_name == "feTurbulence") feTurbulence(e);
	else if (local_name == "filter") filter(e);
	else if (local_name == "font") font(e);
	else if (local_name == "font-face") font_face(e);
	else if (local_name == "font-face-format") font_face_format(e);
	else if (local_name == "font-face-name") font_face_name(e);
	else if (local_name == "font-face-src") font_face_src(e);
	else if (local_name == "font-face-uri") font_face_uri(e);
	else if (local_name == "foreignObject") foreignObject(e);
	else if (local_name == "g") g(e);
	else if (local_name == "glyph") glyph(e);
	else if (local_name == "glyphRef") glyphRef(e);
	else if (local_name == "hkern") hkern(e);
	else if (local_name == "image") image(e);
	else if (local_name == "line") line(e);
	else if (local_name == "linearGradient") linearGradient(e);
	else if (local_name == "marker") marker(e);
	else if (local_name == "mask") mask(e);
	else if (local_name == "metadata") metadata(e);
	else if (local_name == "missing-glyph") missing_glyph(e);
	else if (local_name == "mpath") mpath(e);
	else if (local_name == "path") path(e);
	else if (local_name == "pattern") pattern(e);
	else if (local_name == "polygon") polygon(e);
	else if (local_name == "polyline") polyline(e);
	else if (local_name == "radialGradient") radialGradient(e);
	else if (local_name == "rect") rect(e);
	else if (local_name == "script") script(e);
	else if (local_name == "set") set(e);
	else if (local_name == "stop") stop(e);
	else if (local_name == "style") style(e);
	else if (local_name == "svg") svg(e);
	else if (local_name == "switch") switch_(e);
	else if (local_name == "symbol") symbol(e);
	else if (local_name == "text") text(e);
	else if (local_name == "textPath") textPath(e);
	else if (local_name == "title") title(e);
	else if (local_name == "tref") tref(e);
	else if (local_name == "tspan") tspan(e);
	else if (local_name == "use") use(e);
	else if (local_name == "view") view(e);
	else if (local_name == "vkern") vkern(e);
}
