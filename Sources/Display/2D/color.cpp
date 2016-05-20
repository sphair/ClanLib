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

	Color Color::aliceblue(Color::_aliceblue());
	Color Color::antiquewhite(Color::_antiquewhite());
	Color Color::aqua(Color::_aqua());
	Color Color::aquamarine(Color::_aquamarine());
	Color Color::azure(Color::_azure());
	Color Color::beige(Color::_beige());
	Color Color::bisque(Color::_bisque());
	Color Color::black(Color::_black());
	Color Color::blanchedalmond(Color::_blanchedalmond());
	Color Color::blue(Color::_blue());
	Color Color::blueviolet(Color::_blueviolet());
	Color Color::brown(Color::_brown());
	Color Color::burlywood(Color::_burlywood());
	Color Color::cadetblue(Color::_cadetblue());
	Color Color::chartreuse(Color::_chartreuse());
	Color Color::chocolate(Color::_chocolate());
	Color Color::coral(Color::_coral());
	Color Color::cornflowerblue(Color::_cornflowerblue());
	Color Color::cornsilk(Color::_cornsilk());
	Color Color::crimson(Color::_crimson());
	Color Color::cyan(Color::_cyan());
	Color Color::darkblue(Color::_darkblue());
	Color Color::darkcyan(Color::_darkcyan());
	Color Color::darkgoldenrod(Color::_darkgoldenrod());
	Color Color::darkgray(Color::_darkgray());
	Color Color::darkgreen(Color::_darkgreen());
	Color Color::darkgrey(Color::_darkgrey());
	Color Color::darkkhaki(Color::_darkkhaki());
	Color Color::darkmagenta(Color::_darkmagenta());
	Color Color::darkolivegreen(Color::_darkolivegreen());
	Color Color::darkorange(Color::_darkorange());
	Color Color::darkorchid(Color::_darkorchid());
	Color Color::darkred(Color::_darkred());
	Color Color::darksalmon(Color::_darksalmon());
	Color Color::darkseagreen(Color::_darkseagreen());
	Color Color::darkslateblue(Color::_darkslateblue());
	Color Color::darkslategray(Color::_darkslategray());
	Color Color::darkslategrey(Color::_darkslategrey());
	Color Color::darkturquoise(Color::_darkturquoise());
	Color Color::darkviolet(Color::_darkviolet());
	Color Color::deeppink(Color::_deeppink());
	Color Color::deepskyblue(Color::_deepskyblue());
	Color Color::dimgray(Color::_dimgray());
	Color Color::dimgrey(Color::_dimgrey());
	Color Color::dodgerblue(Color::_dodgerblue());
	Color Color::firebrick(Color::_firebrick());
	Color Color::floralwhite(Color::_floralwhite());
	Color Color::forestgreen(Color::_forestgreen());
	Color Color::fuchsia(Color::_fuchsia());
	Color Color::gainsboro(Color::_gainsboro());
	Color Color::ghostwhite(Color::_ghostwhite());
	Color Color::gold(Color::_gold());
	Color Color::goldenrod(Color::_goldenrod());
	Color Color::gray(Color::_gray());
	Color Color::grey(Color::_grey());
	Color Color::green(Color::_green());
	Color Color::greenyellow(Color::_greenyellow());
	Color Color::honeydew(Color::_honeydew());
	Color Color::hotpink(Color::_hotpink());
	Color Color::indianred(Color::_indianred());
	Color Color::indigo(Color::_indigo());
	Color Color::ivory(Color::_ivory());
	Color Color::khaki(Color::_khaki());
	Color Color::lavender(Color::_lavender());
	Color Color::lavenderblush(Color::_lavenderblush());
	Color Color::lawngreen(Color::_lawngreen());
	Color Color::lemonchiffon(Color::_lemonchiffon());
	Color Color::lightblue(Color::_lightblue());
	Color Color::lightcoral(Color::_lightcoral());
	Color Color::lightcyan(Color::_lightcyan());
	Color Color::lightgoldenrodyellow(Color::_lightgoldenrodyellow());
	Color Color::lightgray(Color::_lightgray());
	Color Color::lightgreen(Color::_lightgreen());
	Color Color::lightgrey(Color::_lightgrey());
	Color Color::lightpink(Color::_lightpink());
	Color Color::lightsalmon(Color::_lightsalmon());
	Color Color::lightseagreen(Color::_lightseagreen());
	Color Color::lightskyblue(Color::_lightskyblue());
	Color Color::lightslategray(Color::_lightslategray());
	Color Color::lightslategrey(Color::_lightslategrey());
	Color Color::lightsteelblue(Color::_lightsteelblue());
	Color Color::lightyellow(Color::_lightyellow());
	Color Color::lime(Color::_lime());
	Color Color::limegreen(Color::_limegreen());
	Color Color::linen(Color::_linen());
	Color Color::magenta(Color::_magenta());
	Color Color::maroon(Color::_maroon());
	Color Color::mediumaquamarine(Color::_mediumaquamarine());
	Color Color::mediumblue(Color::_mediumblue());
	Color Color::mediumorchid(Color::_mediumorchid());
	Color Color::mediumpurple(Color::_mediumpurple());
	Color Color::mediumseagreen(Color::_mediumseagreen());
	Color Color::mediumslateblue(Color::_mediumslateblue());
	Color Color::mediumspringgreen(Color::_mediumspringgreen());
	Color Color::mediumturquoise(Color::_mediumturquoise());
	Color Color::mediumvioletred(Color::_mediumvioletred());
	Color Color::midnightblue(Color::_midnightblue());
	Color Color::mintcream(Color::_mintcream());
	Color Color::mistyrose(Color::_mistyrose());
	Color Color::moccasin(Color::_moccasin());
	Color Color::navajowhite(Color::_navajowhite());
	Color Color::navy(Color::_navy());
	Color Color::oldlace(Color::_oldlace());
	Color Color::olive(Color::_olive());
	Color Color::olivedrab(Color::_olivedrab());
	Color Color::orange(Color::_orange());
	Color Color::orangered(Color::_orangered());
	Color Color::orchid(Color::_orchid());
	Color Color::palegoldenrod(Color::_palegoldenrod());
	Color Color::palegreen(Color::_palegreen());
	Color Color::paleturquoise(Color::_paleturquoise());
	Color Color::palevioletred(Color::_palevioletred());
	Color Color::papayawhip(Color::_papayawhip());
	Color Color::peachpuff(Color::_peachpuff());
	Color Color::peru(Color::_peru());
	Color Color::pink(Color::_pink());
	Color Color::plum(Color::_plum());
	Color Color::powderblue(Color::_powderblue());
	Color Color::purple(Color::_purple());
	Color Color::red(Color::_red());
	Color Color::rosybrown(Color::_rosybrown());
	Color Color::royalblue(Color::_royalblue());
	Color Color::saddlebrown(Color::_saddlebrown());
	Color Color::salmon(Color::_salmon());
	Color Color::sandybrown(Color::_sandybrown());
	Color Color::seagreen(Color::_seagreen());
	Color Color::seashell(Color::_seashell());
	Color Color::sienna(Color::_sienna());
	Color Color::silver(Color::_silver());
	Color Color::skyblue(Color::_skyblue());
	Color Color::slateblue(Color::_slateblue());
	Color Color::slategray(Color::_slategray());
	Color Color::slategrey(Color::_slategrey());
	Color Color::snow(Color::_snow());
	Color Color::springgreen(Color::_springgreen());
	Color Color::steelblue(Color::_steelblue());
	Color Color::tan(Color::_tan());
	Color Color::teal(Color::_teal());
	Color Color::thistle(Color::_thistle());
	Color Color::tomato(Color::_tomato());
	Color Color::transparent(Color::_transparent());
	Color Color::turquoise(Color::_turquoise());
	Color Color::violet(Color::_violet());
	Color Color::wheat(Color::_wheat());
	Color Color::white(Color::_white());
	Color Color::whitesmoke(Color::_whitesmoke());
	Color Color::yellow(Color::_yellow());
	Color Color::yellowgreen(Color::_yellowgreen());

	Color Color::gray10(Color::_gray10());
	Color Color::gray20(Color::_gray20());
	Color Color::gray30(Color::_gray30());
	Color Color::gray40(Color::_gray40());
	Color Color::gray50(Color::_gray50());
	Color Color::gray60(Color::_gray60());
	Color Color::gray70(Color::_gray70());
	Color Color::gray80(Color::_gray80());
	Color Color::gray90(Color::_gray90());

	bool Colorf::find_color(const std::string &name, Colorf &out_color)
	{
		if (name.empty())
		{
			out_color = Colorf::transparent;
			return false;
		}

		if (name[0] == '#')
		{
			out_color = Colorf(Color(name));
			return true;
		}

		static std::map<std::string, Colorf *> color_map;
		if (color_map.empty())
		{
			color_map["aliceblue"] = &Colorf::aliceblue;
			color_map["antiquewhite"] = &Colorf::antiquewhite;
			color_map["aqua"] = &Colorf::aqua;
			color_map["aquamarine"] = &Colorf::aquamarine;
			color_map["azure"] = &Colorf::azure;
			color_map["beige"] = &Colorf::beige;
			color_map["bisque"] = &Colorf::bisque;
			color_map["black"] = &Colorf::black;
			color_map["blanchedalmond"] = &Colorf::blanchedalmond;
			color_map["blue"] = &Colorf::blue;
			color_map["blueviolet"] = &Colorf::blueviolet;
			color_map["brown"] = &Colorf::brown;
			color_map["burlywood"] = &Colorf::burlywood;
			color_map["cadetblue"] = &Colorf::cadetblue;
			color_map["chartreuse"] = &Colorf::chartreuse;
			color_map["chocolate"] = &Colorf::chocolate;
			color_map["coral"] = &Colorf::coral;
			color_map["cornflowerblue"] = &Colorf::cornflowerblue;
			color_map["cornsilk"] = &Colorf::cornsilk;
			color_map["crimson"] = &Colorf::crimson;
			color_map["cyan"] = &Colorf::cyan;
			color_map["darkblue"] = &Colorf::darkblue;
			color_map["darkcyan"] = &Colorf::darkcyan;
			color_map["darkgoldenrod"] = &Colorf::darkgoldenrod;
			color_map["darkgray"] = &Colorf::darkgray;
			color_map["darkgreen"] = &Colorf::darkgreen;
			color_map["darkgrey"] = &Colorf::darkgrey;
			color_map["darkkhaki"] = &Colorf::darkkhaki;
			color_map["darkmagenta"] = &Colorf::darkmagenta;
			color_map["darkolivegreen"] = &Colorf::darkolivegreen;
			color_map["darkorange"] = &Colorf::darkorange;
			color_map["darkorchid"] = &Colorf::darkorchid;
			color_map["darkred"] = &Colorf::darkred;
			color_map["darksalmon"] = &Colorf::darksalmon;
			color_map["darkseagreen"] = &Colorf::darkseagreen;
			color_map["darkslateblue"] = &Colorf::darkslateblue;
			color_map["darkslategray"] = &Colorf::darkslategray;
			color_map["darkslategrey"] = &Colorf::darkslategrey;
			color_map["darkturquoise"] = &Colorf::darkturquoise;
			color_map["darkviolet"] = &Colorf::darkviolet;
			color_map["deeppink"] = &Colorf::deeppink;
			color_map["deepskyblue"] = &Colorf::deepskyblue;
			color_map["dimgray"] = &Colorf::dimgray;
			color_map["dimgrey"] = &Colorf::dimgrey;
			color_map["dodgerblue"] = &Colorf::dodgerblue;
			color_map["firebrick"] = &Colorf::firebrick;
			color_map["floralwhite"] = &Colorf::floralwhite;
			color_map["forestgreen"] = &Colorf::forestgreen;
			color_map["fuchsia"] = &Colorf::fuchsia;
			color_map["gainsboro"] = &Colorf::gainsboro;
			color_map["ghostwhite"] = &Colorf::ghostwhite;
			color_map["gold"] = &Colorf::gold;
			color_map["goldenrod"] = &Colorf::goldenrod;
			color_map["gray"] = &Colorf::gray;
			color_map["grey"] = &Colorf::grey;
			color_map["green"] = &Colorf::green;
			color_map["greenyellow"] = &Colorf::greenyellow;
			color_map["honeydew"] = &Colorf::honeydew;
			color_map["hotpink"] = &Colorf::hotpink;
			color_map["indianred"] = &Colorf::indianred;
			color_map["indigo"] = &Colorf::indigo;
			color_map["ivory"] = &Colorf::ivory;
			color_map["khaki"] = &Colorf::khaki;
			color_map["lavender"] = &Colorf::lavender;
			color_map["lavenderblush"] = &Colorf::lavenderblush;
			color_map["lawngreen"] = &Colorf::lawngreen;
			color_map["lemonchiffon"] = &Colorf::lemonchiffon;
			color_map["lightblue"] = &Colorf::lightblue;
			color_map["lightcoral"] = &Colorf::lightcoral;
			color_map["lightcyan"] = &Colorf::lightcyan;
			color_map["lightgoldenrodyellow"] = &Colorf::lightgoldenrodyellow;
			color_map["lightgray"] = &Colorf::lightgray;
			color_map["lightgreen"] = &Colorf::lightgreen;
			color_map["lightgrey"] = &Colorf::lightgrey;
			color_map["lightpink"] = &Colorf::lightpink;
			color_map["lightsalmon"] = &Colorf::lightsalmon;
			color_map["lightseagreen"] = &Colorf::lightseagreen;
			color_map["lightskyblue"] = &Colorf::lightskyblue;
			color_map["lightslategray"] = &Colorf::lightslategray;
			color_map["lightslategrey"] = &Colorf::lightslategrey;
			color_map["lightsteelblue"] = &Colorf::lightsteelblue;
			color_map["lightyellow"] = &Colorf::lightyellow;
			color_map["lime"] = &Colorf::lime;
			color_map["limegreen"] = &Colorf::limegreen;
			color_map["linen"] = &Colorf::linen;
			color_map["magenta"] = &Colorf::magenta;
			color_map["maroon"] = &Colorf::maroon;
			color_map["mediumaquamarine"] = &Colorf::mediumaquamarine;
			color_map["mediumblue"] = &Colorf::mediumblue;
			color_map["mediumorchid"] = &Colorf::mediumorchid;
			color_map["mediumpurple"] = &Colorf::mediumpurple;
			color_map["mediumseagreen"] = &Colorf::mediumseagreen;
			color_map["mediumslateblue"] = &Colorf::mediumslateblue;
			color_map["mediumspringgreen"] = &Colorf::mediumspringgreen;
			color_map["mediumturquoise"] = &Colorf::mediumturquoise;
			color_map["mediumvioletred"] = &Colorf::mediumvioletred;
			color_map["midnightblue"] = &Colorf::midnightblue;
			color_map["mintcream"] = &Colorf::mintcream;
			color_map["mistyrose"] = &Colorf::mistyrose;
			color_map["moccasin"] = &Colorf::moccasin;
			color_map["navajowhite"] = &Colorf::navajowhite;
			color_map["navy"] = &Colorf::navy;
			color_map["oldlace"] = &Colorf::oldlace;
			color_map["olive"] = &Colorf::olive;
			color_map["olivedrab"] = &Colorf::olivedrab;
			color_map["orange"] = &Colorf::orange;
			color_map["orangered"] = &Colorf::orangered;
			color_map["orchid"] = &Colorf::orchid;
			color_map["palegoldenrod"] = &Colorf::palegoldenrod;
			color_map["palegreen"] = &Colorf::palegreen;
			color_map["paleturquoise"] = &Colorf::paleturquoise;
			color_map["palevioletred"] = &Colorf::palevioletred;
			color_map["papayawhip"] = &Colorf::papayawhip;
			color_map["peachpuff"] = &Colorf::peachpuff;
			color_map["peru"] = &Colorf::peru;
			color_map["pink"] = &Colorf::pink;
			color_map["plum"] = &Colorf::plum;
			color_map["powderblue"] = &Colorf::powderblue;
			color_map["purple"] = &Colorf::purple;
			color_map["red"] = &Colorf::red;
			color_map["rosybrown"] = &Colorf::rosybrown;
			color_map["royalblue"] = &Colorf::royalblue;
			color_map["saddlebrown"] = &Colorf::saddlebrown;
			color_map["salmon"] = &Colorf::salmon;
			color_map["sandybrown"] = &Colorf::sandybrown;
			color_map["seagreen"] = &Colorf::seagreen;
			color_map["seashell"] = &Colorf::seashell;
			color_map["sienna"] = &Colorf::sienna;
			color_map["silver"] = &Colorf::silver;
			color_map["skyblue"] = &Colorf::skyblue;
			color_map["slateblue"] = &Colorf::slateblue;
			color_map["slategray"] = &Colorf::slategray;
			color_map["slategrey"] = &Colorf::slategrey;
			color_map["snow"] = &Colorf::snow;
			color_map["springgreen"] = &Colorf::springgreen;
			color_map["steelblue"] = &Colorf::steelblue;
			color_map["tan"] = &Colorf::tan;
			color_map["teal"] = &Colorf::teal;
			color_map["thistle"] = &Colorf::thistle;
			color_map["transparent"] = &Colorf::transparent;
			color_map["tomato"] = &Colorf::tomato;
			color_map["turquoise"] = &Colorf::turquoise;
			color_map["violet"] = &Colorf::violet;
			color_map["wheat"] = &Colorf::wheat;
			color_map["white"] = &Colorf::white;
			color_map["whitesmoke"] = &Colorf::whitesmoke;
			color_map["yellow"] = &Colorf::yellow;
			color_map["yellowgreen"] = &Colorf::yellowgreen;
			color_map["gray10"] = &Colorf::gray10;
			color_map["gray20"] = &Colorf::gray20;
			color_map["gray30"] = &Colorf::gray30;
			color_map["gray40"] = &Colorf::gray40;
			color_map["gray50"] = &Colorf::gray50;
			color_map["gray60"] = &Colorf::gray60;
			color_map["gray70"] = &Colorf::gray70;
			color_map["gray80"] = &Colorf::gray80;
			color_map["gray90"] = &Colorf::gray90;
		}

		auto it = color_map.find(name);
		if (it != color_map.end())
		{
			out_color = *it->second;
			return true;
		}

		out_color = Colorf::transparent;
		return false;
	}

	Colorf Colorf::aliceblue(Colorf::_aliceblue());
	Colorf Colorf::antiquewhite(Colorf::_antiquewhite());
	Colorf Colorf::aqua(Colorf::_aqua());
	Colorf Colorf::aquamarine(Colorf::_aquamarine());
	Colorf Colorf::azure(Colorf::_azure());
	Colorf Colorf::beige(Colorf::_beige());
	Colorf Colorf::bisque(Colorf::_bisque());
	Colorf Colorf::black(Colorf::_black());
	Colorf Colorf::blanchedalmond(Colorf::_blanchedalmond());
	Colorf Colorf::blue(Colorf::_blue());
	Colorf Colorf::blueviolet(Colorf::_blueviolet());
	Colorf Colorf::brown(Colorf::_brown());
	Colorf Colorf::burlywood(Colorf::_burlywood());
	Colorf Colorf::cadetblue(Colorf::_cadetblue());
	Colorf Colorf::chartreuse(Colorf::_chartreuse());
	Colorf Colorf::chocolate(Colorf::_chocolate());
	Colorf Colorf::coral(Colorf::_coral());
	Colorf Colorf::cornflowerblue(Colorf::_cornflowerblue());
	Colorf Colorf::cornsilk(Colorf::_cornsilk());
	Colorf Colorf::crimson(Colorf::_crimson());
	Colorf Colorf::cyan(Colorf::_cyan());
	Colorf Colorf::darkblue(Colorf::_darkblue());
	Colorf Colorf::darkcyan(Colorf::_darkcyan());
	Colorf Colorf::darkgoldenrod(Colorf::_darkgoldenrod());
	Colorf Colorf::darkgray(Colorf::_darkgray());
	Colorf Colorf::darkgreen(Colorf::_darkgreen());
	Colorf Colorf::darkgrey(Colorf::_darkgrey());
	Colorf Colorf::darkkhaki(Colorf::_darkkhaki());
	Colorf Colorf::darkmagenta(Colorf::_darkmagenta());
	Colorf Colorf::darkolivegreen(Colorf::_darkolivegreen());
	Colorf Colorf::darkorange(Colorf::_darkorange());
	Colorf Colorf::darkorchid(Colorf::_darkorchid());
	Colorf Colorf::darkred(Colorf::_darkred());
	Colorf Colorf::darksalmon(Colorf::_darksalmon());
	Colorf Colorf::darkseagreen(Colorf::_darkseagreen());
	Colorf Colorf::darkslateblue(Colorf::_darkslateblue());
	Colorf Colorf::darkslategray(Colorf::_darkslategray());
	Colorf Colorf::darkslategrey(Colorf::_darkslategrey());
	Colorf Colorf::darkturquoise(Colorf::_darkturquoise());
	Colorf Colorf::darkviolet(Colorf::_darkviolet());
	Colorf Colorf::deeppink(Colorf::_deeppink());
	Colorf Colorf::deepskyblue(Colorf::_deepskyblue());
	Colorf Colorf::dimgray(Colorf::_dimgray());
	Colorf Colorf::dimgrey(Colorf::_dimgrey());
	Colorf Colorf::dodgerblue(Colorf::_dodgerblue());
	Colorf Colorf::firebrick(Colorf::_firebrick());
	Colorf Colorf::floralwhite(Colorf::_floralwhite());
	Colorf Colorf::forestgreen(Colorf::_forestgreen());
	Colorf Colorf::fuchsia(Colorf::_fuchsia());
	Colorf Colorf::gainsboro(Colorf::_gainsboro());
	Colorf Colorf::ghostwhite(Colorf::_ghostwhite());
	Colorf Colorf::gold(Colorf::_gold());
	Colorf Colorf::goldenrod(Colorf::_goldenrod());
	Colorf Colorf::gray(Colorf::_gray());
	Colorf Colorf::grey(Colorf::_grey());
	Colorf Colorf::green(Colorf::_green());
	Colorf Colorf::greenyellow(Colorf::_greenyellow());
	Colorf Colorf::honeydew(Colorf::_honeydew());
	Colorf Colorf::hotpink(Colorf::_hotpink());
	Colorf Colorf::indianred(Colorf::_indianred());
	Colorf Colorf::indigo(Colorf::_indigo());
	Colorf Colorf::ivory(Colorf::_ivory());
	Colorf Colorf::khaki(Colorf::_khaki());
	Colorf Colorf::lavender(Colorf::_lavender());
	Colorf Colorf::lavenderblush(Colorf::_lavenderblush());
	Colorf Colorf::lawngreen(Colorf::_lawngreen());
	Colorf Colorf::lemonchiffon(Colorf::_lemonchiffon());
	Colorf Colorf::lightblue(Colorf::_lightblue());
	Colorf Colorf::lightcoral(Colorf::_lightcoral());
	Colorf Colorf::lightcyan(Colorf::_lightcyan());
	Colorf Colorf::lightgoldenrodyellow(Colorf::_lightgoldenrodyellow());
	Colorf Colorf::lightgray(Colorf::_lightgray());
	Colorf Colorf::lightgreen(Colorf::_lightgreen());
	Colorf Colorf::lightgrey(Colorf::_lightgrey());
	Colorf Colorf::lightpink(Colorf::_lightpink());
	Colorf Colorf::lightsalmon(Colorf::_lightsalmon());
	Colorf Colorf::lightseagreen(Colorf::_lightseagreen());
	Colorf Colorf::lightskyblue(Colorf::_lightskyblue());
	Colorf Colorf::lightslategray(Colorf::_lightslategray());
	Colorf Colorf::lightslategrey(Colorf::_lightslategrey());
	Colorf Colorf::lightsteelblue(Colorf::_lightsteelblue());
	Colorf Colorf::lightyellow(Colorf::_lightyellow());
	Colorf Colorf::lime(Colorf::_lime());
	Colorf Colorf::limegreen(Colorf::_limegreen());
	Colorf Colorf::linen(Colorf::_linen());
	Colorf Colorf::magenta(Colorf::_magenta());
	Colorf Colorf::maroon(Colorf::_maroon());
	Colorf Colorf::mediumaquamarine(Colorf::_mediumaquamarine());
	Colorf Colorf::mediumblue(Colorf::_mediumblue());
	Colorf Colorf::mediumorchid(Colorf::_mediumorchid());
	Colorf Colorf::mediumpurple(Colorf::_mediumpurple());
	Colorf Colorf::mediumseagreen(Colorf::_mediumseagreen());
	Colorf Colorf::mediumslateblue(Colorf::_mediumslateblue());
	Colorf Colorf::mediumspringgreen(Colorf::_mediumspringgreen());
	Colorf Colorf::mediumturquoise(Colorf::_mediumturquoise());
	Colorf Colorf::mediumvioletred(Colorf::_mediumvioletred());
	Colorf Colorf::midnightblue(Colorf::_midnightblue());
	Colorf Colorf::mintcream(Colorf::_mintcream());
	Colorf Colorf::mistyrose(Colorf::_mistyrose());
	Colorf Colorf::moccasin(Colorf::_moccasin());
	Colorf Colorf::navajowhite(Colorf::_navajowhite());
	Colorf Colorf::navy(Colorf::_navy());
	Colorf Colorf::oldlace(Colorf::_oldlace());
	Colorf Colorf::olive(Colorf::_olive());
	Colorf Colorf::olivedrab(Colorf::_olivedrab());
	Colorf Colorf::orange(Colorf::_orange());
	Colorf Colorf::orangered(Colorf::_orangered());
	Colorf Colorf::orchid(Colorf::_orchid());
	Colorf Colorf::palegoldenrod(Colorf::_palegoldenrod());
	Colorf Colorf::palegreen(Colorf::_palegreen());
	Colorf Colorf::paleturquoise(Colorf::_paleturquoise());
	Colorf Colorf::palevioletred(Colorf::_palevioletred());
	Colorf Colorf::papayawhip(Colorf::_papayawhip());
	Colorf Colorf::peachpuff(Colorf::_peachpuff());
	Colorf Colorf::peru(Colorf::_peru());
	Colorf Colorf::pink(Colorf::_pink());
	Colorf Colorf::plum(Colorf::_plum());
	Colorf Colorf::powderblue(Colorf::_powderblue());
	Colorf Colorf::purple(Colorf::_purple());
	Colorf Colorf::red(Colorf::_red());
	Colorf Colorf::rosybrown(Colorf::_rosybrown());
	Colorf Colorf::royalblue(Colorf::_royalblue());
	Colorf Colorf::saddlebrown(Colorf::_saddlebrown());
	Colorf Colorf::salmon(Colorf::_salmon());
	Colorf Colorf::sandybrown(Colorf::_sandybrown());
	Colorf Colorf::seagreen(Colorf::_seagreen());
	Colorf Colorf::seashell(Colorf::_seashell());
	Colorf Colorf::sienna(Colorf::_sienna());
	Colorf Colorf::silver(Colorf::_silver());
	Colorf Colorf::skyblue(Colorf::_skyblue());
	Colorf Colorf::slateblue(Colorf::_slateblue());
	Colorf Colorf::slategray(Colorf::_slategray());
	Colorf Colorf::slategrey(Colorf::_slategrey());
	Colorf Colorf::snow(Colorf::_snow());
	Colorf Colorf::springgreen(Colorf::_springgreen());
	Colorf Colorf::steelblue(Colorf::_steelblue());
	Colorf Colorf::tan(Colorf::_tan());
	Colorf Colorf::teal(Colorf::_teal());
	Colorf Colorf::thistle(Colorf::_thistle());
	Colorf Colorf::tomato(Colorf::_tomato());
	Colorf Colorf::transparent(Colorf::_transparent());
	Colorf Colorf::turquoise(Colorf::_turquoise());
	Colorf Colorf::violet(Colorf::_violet());
	Colorf Colorf::wheat(Colorf::_wheat());
	Colorf Colorf::white(Colorf::_white());
	Colorf Colorf::whitesmoke(Colorf::_whitesmoke());
	Colorf Colorf::yellow(Colorf::_yellow());
	Colorf Colorf::yellowgreen(Colorf::_yellowgreen());

	Colorf Colorf::gray10(Colorf::_gray10());
	Colorf Colorf::gray20(Colorf::_gray20());
	Colorf Colorf::gray30(Colorf::_gray30());
	Colorf Colorf::gray40(Colorf::_gray40());
	Colorf Colorf::gray50(Colorf::_gray50());
	Colorf Colorf::gray60(Colorf::_gray60());
	Colorf Colorf::gray70(Colorf::_gray70());
	Colorf Colorf::gray80(Colorf::_gray80());
	Colorf Colorf::gray90(Colorf::_gray90());
}

