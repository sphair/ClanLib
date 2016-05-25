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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/2D/color.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include <map>

#ifndef WIN32
#include <cstdlib>
#endif

namespace clan
{
	Color::Color(const std::string &hexstr)
	{
		if (hexstr.empty() || hexstr.length() > 9)
			throw Exception(string_format("Bad hex color string: %1", hexstr));

		int pos = 0;
		if (hexstr[0] == '#')
			pos++;

		unsigned int color = strtoul(hexstr.substr(pos).c_str(), nullptr, 16);

		bool have_alpha = (hexstr.length() - pos) > 6;

		if (!have_alpha)
		{
			set_rgb8(color);
		}
		else
		{
			set_rgba8(color);
		}
	}

	void Color::set_rgba8(unsigned int color)
	{
		r = (unsigned char)(color >> 24);
		g = (unsigned char)(color >> 16);
		b = (unsigned char)(color >> 8);
		a = (unsigned char)color;
	}

	void Color::set_bgra8(unsigned int color)
	{
		b = (unsigned char)(color >> 24);
		g = (unsigned char)(color >> 16);
		r = (unsigned char)(color >> 8);
		a = (unsigned char)color;
	}

	void Color::set_rgb8(unsigned int color)
	{
		r = (unsigned char)(color >> 16);
		g = (unsigned char)(color >> 8);
		b = (unsigned char)color;
		a = 0xFF;
	}

	void Color::set_argb8(unsigned int color)
	{
		r = (unsigned char)(color >> 16);
		g = (unsigned char)(color >> 8);
		b = (unsigned char)color;
		a = (unsigned char)(color >> 24);
	}

	Color::Color(const Colorf& color)
		: Vec4ub(static_cast<unsigned char>(color.r * 255.0f),
		static_cast<unsigned char>(color.g * 255.0f),
		static_cast<unsigned char>(color.b * 255.0f),
		static_cast<unsigned char>(color.a * 255.0f))

	{
	}

	Color Color::aliceblue(StandardColor::aliceblue());
	Color Color::antiquewhite(StandardColor::antiquewhite());
	Color Color::aqua(StandardColor::aqua());
	Color Color::aquamarine(StandardColor::aquamarine());
	Color Color::azure(StandardColor::azure());
	Color Color::beige(StandardColor::beige());
	Color Color::bisque(StandardColor::bisque());
	Color Color::black(StandardColor::black());
	Color Color::blanchedalmond(StandardColor::blanchedalmond());
	Color Color::blue(StandardColor::blue());
	Color Color::blueviolet(StandardColor::blueviolet());
	Color Color::brown(StandardColor::brown());
	Color Color::burlywood(StandardColor::burlywood());
	Color Color::cadetblue(StandardColor::cadetblue());
	Color Color::chartreuse(StandardColor::chartreuse());
	Color Color::chocolate(StandardColor::chocolate());
	Color Color::coral(StandardColor::coral());
	Color Color::cornflowerblue(StandardColor::cornflowerblue());
	Color Color::cornsilk(StandardColor::cornsilk());
	Color Color::crimson(StandardColor::crimson());
	Color Color::cyan(StandardColor::cyan());
	Color Color::darkblue(StandardColor::darkblue());
	Color Color::darkcyan(StandardColor::darkcyan());
	Color Color::darkgoldenrod(StandardColor::darkgoldenrod());
	Color Color::darkgray(StandardColor::darkgray());
	Color Color::darkgreen(StandardColor::darkgreen());
	Color Color::darkgrey(StandardColor::darkgrey());
	Color Color::darkkhaki(StandardColor::darkkhaki());
	Color Color::darkmagenta(StandardColor::darkmagenta());
	Color Color::darkolivegreen(StandardColor::darkolivegreen());
	Color Color::darkorange(StandardColor::darkorange());
	Color Color::darkorchid(StandardColor::darkorchid());
	Color Color::darkred(StandardColor::darkred());
	Color Color::darksalmon(StandardColor::darksalmon());
	Color Color::darkseagreen(StandardColor::darkseagreen());
	Color Color::darkslateblue(StandardColor::darkslateblue());
	Color Color::darkslategray(StandardColor::darkslategray());
	Color Color::darkslategrey(StandardColor::darkslategrey());
	Color Color::darkturquoise(StandardColor::darkturquoise());
	Color Color::darkviolet(StandardColor::darkviolet());
	Color Color::deeppink(StandardColor::deeppink());
	Color Color::deepskyblue(StandardColor::deepskyblue());
	Color Color::dimgray(StandardColor::dimgray());
	Color Color::dimgrey(StandardColor::dimgrey());
	Color Color::dodgerblue(StandardColor::dodgerblue());
	Color Color::firebrick(StandardColor::firebrick());
	Color Color::floralwhite(StandardColor::floralwhite());
	Color Color::forestgreen(StandardColor::forestgreen());
	Color Color::fuchsia(StandardColor::fuchsia());
	Color Color::gainsboro(StandardColor::gainsboro());
	Color Color::ghostwhite(StandardColor::ghostwhite());
	Color Color::gold(StandardColor::gold());
	Color Color::goldenrod(StandardColor::goldenrod());
	Color Color::gray(StandardColor::gray());
	Color Color::grey(StandardColor::grey());
	Color Color::green(StandardColor::green());
	Color Color::greenyellow(StandardColor::greenyellow());
	Color Color::honeydew(StandardColor::honeydew());
	Color Color::hotpink(StandardColor::hotpink());
	Color Color::indianred(StandardColor::indianred());
	Color Color::indigo(StandardColor::indigo());
	Color Color::ivory(StandardColor::ivory());
	Color Color::khaki(StandardColor::khaki());
	Color Color::lavender(StandardColor::lavender());
	Color Color::lavenderblush(StandardColor::lavenderblush());
	Color Color::lawngreen(StandardColor::lawngreen());
	Color Color::lemonchiffon(StandardColor::lemonchiffon());
	Color Color::lightblue(StandardColor::lightblue());
	Color Color::lightcoral(StandardColor::lightcoral());
	Color Color::lightcyan(StandardColor::lightcyan());
	Color Color::lightgoldenrodyellow(StandardColor::lightgoldenrodyellow());
	Color Color::lightgray(StandardColor::lightgray());
	Color Color::lightgreen(StandardColor::lightgreen());
	Color Color::lightgrey(StandardColor::lightgrey());
	Color Color::lightpink(StandardColor::lightpink());
	Color Color::lightsalmon(StandardColor::lightsalmon());
	Color Color::lightseagreen(StandardColor::lightseagreen());
	Color Color::lightskyblue(StandardColor::lightskyblue());
	Color Color::lightslategray(StandardColor::lightslategray());
	Color Color::lightslategrey(StandardColor::lightslategrey());
	Color Color::lightsteelblue(StandardColor::lightsteelblue());
	Color Color::lightyellow(StandardColor::lightyellow());
	Color Color::lime(StandardColor::lime());
	Color Color::limegreen(StandardColor::limegreen());
	Color Color::linen(StandardColor::linen());
	Color Color::magenta(StandardColor::magenta());
	Color Color::maroon(StandardColor::maroon());
	Color Color::mediumaquamarine(StandardColor::mediumaquamarine());
	Color Color::mediumblue(StandardColor::mediumblue());
	Color Color::mediumorchid(StandardColor::mediumorchid());
	Color Color::mediumpurple(StandardColor::mediumpurple());
	Color Color::mediumseagreen(StandardColor::mediumseagreen());
	Color Color::mediumslateblue(StandardColor::mediumslateblue());
	Color Color::mediumspringgreen(StandardColor::mediumspringgreen());
	Color Color::mediumturquoise(StandardColor::mediumturquoise());
	Color Color::mediumvioletred(StandardColor::mediumvioletred());
	Color Color::midnightblue(StandardColor::midnightblue());
	Color Color::mintcream(StandardColor::mintcream());
	Color Color::mistyrose(StandardColor::mistyrose());
	Color Color::moccasin(StandardColor::moccasin());
	Color Color::navajowhite(StandardColor::navajowhite());
	Color Color::navy(StandardColor::navy());
	Color Color::oldlace(StandardColor::oldlace());
	Color Color::olive(StandardColor::olive());
	Color Color::olivedrab(StandardColor::olivedrab());
	Color Color::orange(StandardColor::orange());
	Color Color::orangered(StandardColor::orangered());
	Color Color::orchid(StandardColor::orchid());
	Color Color::palegoldenrod(StandardColor::palegoldenrod());
	Color Color::palegreen(StandardColor::palegreen());
	Color Color::paleturquoise(StandardColor::paleturquoise());
	Color Color::palevioletred(StandardColor::palevioletred());
	Color Color::papayawhip(StandardColor::papayawhip());
	Color Color::peachpuff(StandardColor::peachpuff());
	Color Color::peru(StandardColor::peru());
	Color Color::pink(StandardColor::pink());
	Color Color::plum(StandardColor::plum());
	Color Color::powderblue(StandardColor::powderblue());
	Color Color::purple(StandardColor::purple());
	Color Color::red(StandardColor::red());
	Color Color::rosybrown(StandardColor::rosybrown());
	Color Color::royalblue(StandardColor::royalblue());
	Color Color::saddlebrown(StandardColor::saddlebrown());
	Color Color::salmon(StandardColor::salmon());
	Color Color::sandybrown(StandardColor::sandybrown());
	Color Color::seagreen(StandardColor::seagreen());
	Color Color::seashell(StandardColor::seashell());
	Color Color::sienna(StandardColor::sienna());
	Color Color::silver(StandardColor::silver());
	Color Color::skyblue(StandardColor::skyblue());
	Color Color::slateblue(StandardColor::slateblue());
	Color Color::slategray(StandardColor::slategray());
	Color Color::slategrey(StandardColor::slategrey());
	Color Color::snow(StandardColor::snow());
	Color Color::springgreen(StandardColor::springgreen());
	Color Color::steelblue(StandardColor::steelblue());
	Color Color::tan(StandardColor::tan());
	Color Color::teal(StandardColor::teal());
	Color Color::thistle(StandardColor::thistle());
	Color Color::tomato(StandardColor::tomato());
	Color Color::transparent(StandardColor::transparent());
	Color Color::turquoise(StandardColor::turquoise());
	Color Color::violet(StandardColor::violet());
	Color Color::wheat(StandardColor::wheat());
	Color Color::white(StandardColor::white());
	Color Color::whitesmoke(StandardColor::whitesmoke());
	Color Color::yellow(StandardColor::yellow());
	Color Color::yellowgreen(StandardColor::yellowgreen());

	Color Color::gray10(StandardColor::gray10());
	Color Color::gray20(StandardColor::gray20());
	Color Color::gray30(StandardColor::gray30());
	Color Color::gray40(StandardColor::gray40());
	Color Color::gray50(StandardColor::gray50());
	Color Color::gray60(StandardColor::gray60());
	Color Color::gray70(StandardColor::gray70());
	Color Color::gray80(StandardColor::gray80());
	Color Color::gray90(StandardColor::gray90());

	bool Colorf::find_color(const std::string &name, Colorf &out_color)
	{
		return StandardColorf::parse(name, out_color);
	}

	Colorf Colorf::aliceblue(StandardColorf::aliceblue());
	Colorf Colorf::antiquewhite(StandardColorf::antiquewhite());
	Colorf Colorf::aqua(StandardColorf::aqua());
	Colorf Colorf::aquamarine(StandardColorf::aquamarine());
	Colorf Colorf::azure(StandardColorf::azure());
	Colorf Colorf::beige(StandardColorf::beige());
	Colorf Colorf::bisque(StandardColorf::bisque());
	Colorf Colorf::black(StandardColorf::black());
	Colorf Colorf::blanchedalmond(StandardColorf::blanchedalmond());
	Colorf Colorf::blue(StandardColorf::blue());
	Colorf Colorf::blueviolet(StandardColorf::blueviolet());
	Colorf Colorf::brown(StandardColorf::brown());
	Colorf Colorf::burlywood(StandardColorf::burlywood());
	Colorf Colorf::cadetblue(StandardColorf::cadetblue());
	Colorf Colorf::chartreuse(StandardColorf::chartreuse());
	Colorf Colorf::chocolate(StandardColorf::chocolate());
	Colorf Colorf::coral(StandardColorf::coral());
	Colorf Colorf::cornflowerblue(StandardColorf::cornflowerblue());
	Colorf Colorf::cornsilk(StandardColorf::cornsilk());
	Colorf Colorf::crimson(StandardColorf::crimson());
	Colorf Colorf::cyan(StandardColorf::cyan());
	Colorf Colorf::darkblue(StandardColorf::darkblue());
	Colorf Colorf::darkcyan(StandardColorf::darkcyan());
	Colorf Colorf::darkgoldenrod(StandardColorf::darkgoldenrod());
	Colorf Colorf::darkgray(StandardColorf::darkgray());
	Colorf Colorf::darkgreen(StandardColorf::darkgreen());
	Colorf Colorf::darkgrey(StandardColorf::darkgrey());
	Colorf Colorf::darkkhaki(StandardColorf::darkkhaki());
	Colorf Colorf::darkmagenta(StandardColorf::darkmagenta());
	Colorf Colorf::darkolivegreen(StandardColorf::darkolivegreen());
	Colorf Colorf::darkorange(StandardColorf::darkorange());
	Colorf Colorf::darkorchid(StandardColorf::darkorchid());
	Colorf Colorf::darkred(StandardColorf::darkred());
	Colorf Colorf::darksalmon(StandardColorf::darksalmon());
	Colorf Colorf::darkseagreen(StandardColorf::darkseagreen());
	Colorf Colorf::darkslateblue(StandardColorf::darkslateblue());
	Colorf Colorf::darkslategray(StandardColorf::darkslategray());
	Colorf Colorf::darkslategrey(StandardColorf::darkslategrey());
	Colorf Colorf::darkturquoise(StandardColorf::darkturquoise());
	Colorf Colorf::darkviolet(StandardColorf::darkviolet());
	Colorf Colorf::deeppink(StandardColorf::deeppink());
	Colorf Colorf::deepskyblue(StandardColorf::deepskyblue());
	Colorf Colorf::dimgray(StandardColorf::dimgray());
	Colorf Colorf::dimgrey(StandardColorf::dimgrey());
	Colorf Colorf::dodgerblue(StandardColorf::dodgerblue());
	Colorf Colorf::firebrick(StandardColorf::firebrick());
	Colorf Colorf::floralwhite(StandardColorf::floralwhite());
	Colorf Colorf::forestgreen(StandardColorf::forestgreen());
	Colorf Colorf::fuchsia(StandardColorf::fuchsia());
	Colorf Colorf::gainsboro(StandardColorf::gainsboro());
	Colorf Colorf::ghostwhite(StandardColorf::ghostwhite());
	Colorf Colorf::gold(StandardColorf::gold());
	Colorf Colorf::goldenrod(StandardColorf::goldenrod());
	Colorf Colorf::gray(StandardColorf::gray());
	Colorf Colorf::grey(StandardColorf::grey());
	Colorf Colorf::green(StandardColorf::green());
	Colorf Colorf::greenyellow(StandardColorf::greenyellow());
	Colorf Colorf::honeydew(StandardColorf::honeydew());
	Colorf Colorf::hotpink(StandardColorf::hotpink());
	Colorf Colorf::indianred(StandardColorf::indianred());
	Colorf Colorf::indigo(StandardColorf::indigo());
	Colorf Colorf::ivory(StandardColorf::ivory());
	Colorf Colorf::khaki(StandardColorf::khaki());
	Colorf Colorf::lavender(StandardColorf::lavender());
	Colorf Colorf::lavenderblush(StandardColorf::lavenderblush());
	Colorf Colorf::lawngreen(StandardColorf::lawngreen());
	Colorf Colorf::lemonchiffon(StandardColorf::lemonchiffon());
	Colorf Colorf::lightblue(StandardColorf::lightblue());
	Colorf Colorf::lightcoral(StandardColorf::lightcoral());
	Colorf Colorf::lightcyan(StandardColorf::lightcyan());
	Colorf Colorf::lightgoldenrodyellow(StandardColorf::lightgoldenrodyellow());
	Colorf Colorf::lightgray(StandardColorf::lightgray());
	Colorf Colorf::lightgreen(StandardColorf::lightgreen());
	Colorf Colorf::lightgrey(StandardColorf::lightgrey());
	Colorf Colorf::lightpink(StandardColorf::lightpink());
	Colorf Colorf::lightsalmon(StandardColorf::lightsalmon());
	Colorf Colorf::lightseagreen(StandardColorf::lightseagreen());
	Colorf Colorf::lightskyblue(StandardColorf::lightskyblue());
	Colorf Colorf::lightslategray(StandardColorf::lightslategray());
	Colorf Colorf::lightslategrey(StandardColorf::lightslategrey());
	Colorf Colorf::lightsteelblue(StandardColorf::lightsteelblue());
	Colorf Colorf::lightyellow(StandardColorf::lightyellow());
	Colorf Colorf::lime(StandardColorf::lime());
	Colorf Colorf::limegreen(StandardColorf::limegreen());
	Colorf Colorf::linen(StandardColorf::linen());
	Colorf Colorf::magenta(StandardColorf::magenta());
	Colorf Colorf::maroon(StandardColorf::maroon());
	Colorf Colorf::mediumaquamarine(StandardColorf::mediumaquamarine());
	Colorf Colorf::mediumblue(StandardColorf::mediumblue());
	Colorf Colorf::mediumorchid(StandardColorf::mediumorchid());
	Colorf Colorf::mediumpurple(StandardColorf::mediumpurple());
	Colorf Colorf::mediumseagreen(StandardColorf::mediumseagreen());
	Colorf Colorf::mediumslateblue(StandardColorf::mediumslateblue());
	Colorf Colorf::mediumspringgreen(StandardColorf::mediumspringgreen());
	Colorf Colorf::mediumturquoise(StandardColorf::mediumturquoise());
	Colorf Colorf::mediumvioletred(StandardColorf::mediumvioletred());
	Colorf Colorf::midnightblue(StandardColorf::midnightblue());
	Colorf Colorf::mintcream(StandardColorf::mintcream());
	Colorf Colorf::mistyrose(StandardColorf::mistyrose());
	Colorf Colorf::moccasin(StandardColorf::moccasin());
	Colorf Colorf::navajowhite(StandardColorf::navajowhite());
	Colorf Colorf::navy(StandardColorf::navy());
	Colorf Colorf::oldlace(StandardColorf::oldlace());
	Colorf Colorf::olive(StandardColorf::olive());
	Colorf Colorf::olivedrab(StandardColorf::olivedrab());
	Colorf Colorf::orange(StandardColorf::orange());
	Colorf Colorf::orangered(StandardColorf::orangered());
	Colorf Colorf::orchid(StandardColorf::orchid());
	Colorf Colorf::palegoldenrod(StandardColorf::palegoldenrod());
	Colorf Colorf::palegreen(StandardColorf::palegreen());
	Colorf Colorf::paleturquoise(StandardColorf::paleturquoise());
	Colorf Colorf::palevioletred(StandardColorf::palevioletred());
	Colorf Colorf::papayawhip(StandardColorf::papayawhip());
	Colorf Colorf::peachpuff(StandardColorf::peachpuff());
	Colorf Colorf::peru(StandardColorf::peru());
	Colorf Colorf::pink(StandardColorf::pink());
	Colorf Colorf::plum(StandardColorf::plum());
	Colorf Colorf::powderblue(StandardColorf::powderblue());
	Colorf Colorf::purple(StandardColorf::purple());
	Colorf Colorf::red(StandardColorf::red());
	Colorf Colorf::rosybrown(StandardColorf::rosybrown());
	Colorf Colorf::royalblue(StandardColorf::royalblue());
	Colorf Colorf::saddlebrown(StandardColorf::saddlebrown());
	Colorf Colorf::salmon(StandardColorf::salmon());
	Colorf Colorf::sandybrown(StandardColorf::sandybrown());
	Colorf Colorf::seagreen(StandardColorf::seagreen());
	Colorf Colorf::seashell(StandardColorf::seashell());
	Colorf Colorf::sienna(StandardColorf::sienna());
	Colorf Colorf::silver(StandardColorf::silver());
	Colorf Colorf::skyblue(StandardColorf::skyblue());
	Colorf Colorf::slateblue(StandardColorf::slateblue());
	Colorf Colorf::slategray(StandardColorf::slategray());
	Colorf Colorf::slategrey(StandardColorf::slategrey());
	Colorf Colorf::snow(StandardColorf::snow());
	Colorf Colorf::springgreen(StandardColorf::springgreen());
	Colorf Colorf::steelblue(StandardColorf::steelblue());
	Colorf Colorf::tan(StandardColorf::tan());
	Colorf Colorf::teal(StandardColorf::teal());
	Colorf Colorf::thistle(StandardColorf::thistle());
	Colorf Colorf::tomato(StandardColorf::tomato());
	Colorf Colorf::transparent(StandardColorf::transparent());
	Colorf Colorf::turquoise(StandardColorf::turquoise());
	Colorf Colorf::violet(StandardColorf::violet());
	Colorf Colorf::wheat(StandardColorf::wheat());
	Colorf Colorf::white(StandardColorf::white());
	Colorf Colorf::whitesmoke(StandardColorf::whitesmoke());
	Colorf Colorf::yellow(StandardColorf::yellow());
	Colorf Colorf::yellowgreen(StandardColorf::yellowgreen());

	Colorf Colorf::gray10(StandardColorf::gray10());
	Colorf Colorf::gray20(StandardColorf::gray20());
	Colorf Colorf::gray30(StandardColorf::gray30());
	Colorf Colorf::gray40(StandardColorf::gray40());
	Colorf Colorf::gray50(StandardColorf::gray50());
	Colorf Colorf::gray60(StandardColorf::gray60());
	Colorf Colorf::gray70(StandardColorf::gray70());
	Colorf Colorf::gray80(StandardColorf::gray80());
	Colorf Colorf::gray90(StandardColorf::gray90());

	bool StandardColorf::parse(const std::string &name, Colorf &out_color)
	{
		if (name.empty())
		{
			out_color = transparent();
			return false;
		}

		if (name[0] == '#')
		{
			out_color = Colorf(Color(name));
			return true;
		}

		static std::map<std::string, Colorf> color_map =
		{
			{ "aliceblue", aliceblue() },
			{ "antiquewhite", antiquewhite() },
			{ "aqua", aqua() },
			{ "aquamarine", aquamarine() },
			{ "azure", azure() },
			{ "beige", beige() },
			{ "bisque", bisque() },
			{ "black", black() },
			{ "blanchedalmond", blanchedalmond() },
			{ "blue", blue() },
			{ "blueviolet", blueviolet() },
			{ "brown", brown() },
			{ "burlywood", burlywood() },
			{ "cadetblue", cadetblue() },
			{ "chartreuse", chartreuse() },
			{ "chocolate", chocolate() },
			{ "coral", coral() },
			{ "cornflowerblue", cornflowerblue() },
			{ "cornsilk", cornsilk() },
			{ "crimson", crimson() },
			{ "cyan", cyan() },
			{ "darkblue", darkblue() },
			{ "darkcyan", darkcyan() },
			{ "darkgoldenrod", darkgoldenrod() },
			{ "darkgray", darkgray() },
			{ "darkgreen", darkgreen() },
			{ "darkgrey", darkgrey() },
			{ "darkkhaki", darkkhaki() },
			{ "darkmagenta", darkmagenta() },
			{ "darkolivegreen", darkolivegreen() },
			{ "darkorange", darkorange() },
			{ "darkorchid", darkorchid() },
			{ "darkred", darkred() },
			{ "darksalmon", darksalmon() },
			{ "darkseagreen", darkseagreen() },
			{ "darkslateblue", darkslateblue() },
			{ "darkslategray", darkslategray() },
			{ "darkslategrey", darkslategrey() },
			{ "darkturquoise", darkturquoise() },
			{ "darkviolet", darkviolet() },
			{ "deeppink", deeppink() },
			{ "deepskyblue", deepskyblue() },
			{ "dimgray", dimgray() },
			{ "dimgrey", dimgrey() },
			{ "dodgerblue", dodgerblue() },
			{ "firebrick", firebrick() },
			{ "floralwhite", floralwhite() },
			{ "forestgreen", forestgreen() },
			{ "fuchsia", fuchsia() },
			{ "gainsboro", gainsboro() },
			{ "ghostwhite", ghostwhite() },
			{ "gold", gold() },
			{ "goldenrod", goldenrod() },
			{ "gray", gray() },
			{ "grey", grey() },
			{ "green", green() },
			{ "greenyellow", greenyellow() },
			{ "honeydew", honeydew() },
			{ "hotpink", hotpink() },
			{ "indianred", indianred() },
			{ "indigo", indigo() },
			{ "ivory", ivory() },
			{ "khaki", khaki() },
			{ "lavender", lavender() },
			{ "lavenderblush", lavenderblush() },
			{ "lawngreen", lawngreen() },
			{ "lemonchiffon", lemonchiffon() },
			{ "lightblue", lightblue() },
			{ "lightcoral", lightcoral() },
			{ "lightcyan", lightcyan() },
			{ "lightgoldenrodyellow", lightgoldenrodyellow() },
			{ "lightgray", lightgray() },
			{ "lightgreen", lightgreen() },
			{ "lightgrey", lightgrey() },
			{ "lightpink", lightpink() },
			{ "lightsalmon", lightsalmon() },
			{ "lightseagreen", lightseagreen() },
			{ "lightskyblue", lightskyblue() },
			{ "lightslategray", lightslategray() },
			{ "lightslategrey", lightslategrey() },
			{ "lightsteelblue", lightsteelblue() },
			{ "lightyellow", lightyellow() },
			{ "lime", lime() },
			{ "limegreen", limegreen() },
			{ "linen", linen() },
			{ "magenta", magenta() },
			{ "maroon", maroon() },
			{ "mediumaquamarine", mediumaquamarine() },
			{ "mediumblue", mediumblue() },
			{ "mediumorchid", mediumorchid() },
			{ "mediumpurple", mediumpurple() },
			{ "mediumseagreen", mediumseagreen() },
			{ "mediumslateblue", mediumslateblue() },
			{ "mediumspringgreen", mediumspringgreen() },
			{ "mediumturquoise", mediumturquoise() },
			{ "mediumvioletred", mediumvioletred() },
			{ "midnightblue", midnightblue() },
			{ "mintcream", mintcream() },
			{ "mistyrose", mistyrose() },
			{ "moccasin", moccasin() },
			{ "navajowhite", navajowhite() },
			{ "navy", navy() },
			{ "oldlace", oldlace() },
			{ "olive", olive() },
			{ "olivedrab", olivedrab() },
			{ "orange", orange() },
			{ "orangered", orangered() },
			{ "orchid", orchid() },
			{ "palegoldenrod", palegoldenrod() },
			{ "palegreen", palegreen() },
			{ "paleturquoise", paleturquoise() },
			{ "palevioletred", palevioletred() },
			{ "papayawhip", papayawhip() },
			{ "peachpuff", peachpuff() },
			{ "peru", peru() },
			{ "pink", pink() },
			{ "plum", plum() },
			{ "powderblue", powderblue() },
			{ "purple", purple() },
			{ "red", red() },
			{ "rosybrown", rosybrown() },
			{ "royalblue", royalblue() },
			{ "saddlebrown", saddlebrown() },
			{ "salmon", salmon() },
			{ "sandybrown", sandybrown() },
			{ "seagreen", seagreen() },
			{ "seashell", seashell() },
			{ "sienna", sienna() },
			{ "silver", silver() },
			{ "skyblue", skyblue() },
			{ "slateblue", slateblue() },
			{ "slategray", slategray() },
			{ "slategrey", slategrey() },
			{ "snow", snow() },
			{ "springgreen", springgreen() },
			{ "steelblue", steelblue() },
			{ "tan", tan() },
			{ "teal", teal() },
			{ "thistle", thistle() },
			{ "transparent", transparent() },
			{ "tomato", tomato() },
			{ "turquoise", turquoise() },
			{ "violet", violet() },
			{ "wheat", wheat() },
			{ "white", white() },
			{ "whitesmoke", whitesmoke() },
			{ "yellow", yellow() },
			{ "yellowgreen", yellowgreen() },
			{ "gray10", gray10() },
			{ "gray20", gray20() },
			{ "gray30", gray30() },
			{ "gray40", gray40() },
			{ "gray50", gray50() },
			{ "gray60", gray60() },
			{ "gray70", gray70() },
			{ "gray80", gray80() },
			{ "gray90", gray90() }
		};

		auto it = color_map.find(name);
		if (it != color_map.end())
		{
			out_color = it->second;
			return true;
		}

		out_color = transparent();
		return false;
	}
}

