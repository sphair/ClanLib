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

		unsigned int color = strtoul(StringHelp::text_to_local8(hexstr.substr(pos)).c_str(), nullptr, 16);

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

	Color Color::aliceblue(240, 248, 255);
	Color Color::antiquewhite(250, 235, 215);
	Color Color::aqua(0, 255, 255);
	Color Color::aquamarine(127, 255, 212);
	Color Color::azure(240, 255, 255);
	Color Color::beige(245, 245, 220);
	Color Color::bisque(255, 228, 196);
	Color Color::black(0, 0, 0);
	Color Color::blanchedalmond(255, 235, 205);
	Color Color::blue(0, 0, 255);
	Color Color::blueviolet(138, 43, 226);
	Color Color::brown(165, 42, 42);
	Color Color::burlywood(222, 184, 135);
	Color Color::cadetblue(95, 158, 160);
	Color Color::chartreuse(127, 255, 0);
	Color Color::chocolate(210, 105, 30);
	Color Color::coral(255, 127, 80);
	Color Color::cornflowerblue(100, 149, 237);
	Color Color::cornsilk(255, 248, 220);
	Color Color::crimson(220, 20, 60);
	Color Color::cyan(0, 255, 255);
	Color Color::darkblue(0, 0, 139);
	Color Color::darkcyan(0, 139, 139);
	Color Color::darkgoldenrod(184, 134, 11);
	Color Color::darkgray(169, 169, 169);
	Color Color::darkgreen(0, 100, 0);
	Color Color::darkgrey(169, 169, 169);
	Color Color::darkkhaki(189, 183, 107);
	Color Color::darkmagenta(139, 0, 139);
	Color Color::darkolivegreen(85, 107, 47);
	Color Color::darkorange(255, 140, 0);
	Color Color::darkorchid(153, 50, 204);
	Color Color::darkred(139, 0, 0);
	Color Color::darksalmon(233, 150, 122);
	Color Color::darkseagreen(143, 188, 143);
	Color Color::darkslateblue(72, 61, 139);
	Color Color::darkslategray(47, 79, 79);
	Color Color::darkslategrey(47, 79, 79);
	Color Color::darkturquoise(0, 206, 209);
	Color Color::darkviolet(148, 0, 211);
	Color Color::deeppink(255, 20, 147);
	Color Color::deepskyblue(0, 191, 255);
	Color Color::dimgray(105, 105, 105);
	Color Color::dimgrey(105, 105, 105);
	Color Color::dodgerblue(30, 144, 255);
	Color Color::firebrick(178, 34, 34);
	Color Color::floralwhite(255, 250, 240);
	Color Color::forestgreen(34, 139, 34);
	Color Color::fuchsia(255, 0, 255);
	Color Color::gainsboro(220, 220, 220);
	Color Color::ghostwhite(248, 248, 255);
	Color Color::gold(255, 215, 0);
	Color Color::goldenrod(218, 165, 32);
	Color Color::gray(128, 128, 128);
	Color Color::grey(128, 128, 128);
	Color Color::green(0, 128, 0);
	Color Color::greenyellow(173, 255, 47);
	Color Color::honeydew(240, 255, 240);
	Color Color::hotpink(255, 105, 180);
	Color Color::indianred(205, 92, 92);
	Color Color::indigo(75, 0, 130);
	Color Color::ivory(255, 255, 240);
	Color Color::khaki(240, 230, 140);
	Color Color::lavender(230, 230, 250);
	Color Color::lavenderblush(255, 240, 245);
	Color Color::lawngreen(124, 252, 0);
	Color Color::lemonchiffon(255, 250, 205);
	Color Color::lightblue(173, 216, 230);
	Color Color::lightcoral(240, 128, 128);
	Color Color::lightcyan(224, 255, 255);
	Color Color::lightgoldenrodyellow(250, 250, 210);
	Color Color::lightgray(211, 211, 211);
	Color Color::lightgreen(144, 238, 144);
	Color Color::lightgrey(211, 211, 211);
	Color Color::lightpink(255, 182, 193);
	Color Color::lightsalmon(255, 160, 122);
	Color Color::lightseagreen(32, 178, 170);
	Color Color::lightskyblue(135, 206, 250);
	Color Color::lightslategray(119, 136, 153);
	Color Color::lightslategrey(119, 136, 153);
	Color Color::lightsteelblue(176, 196, 222);
	Color Color::lightyellow(255, 255, 224);
	Color Color::lime(0, 255, 0);
	Color Color::limegreen(50, 205, 50);
	Color Color::linen(250, 240, 230);
	Color Color::magenta(255, 0, 255);
	Color Color::maroon(128, 0, 0);
	Color Color::mediumaquamarine(102, 205, 170);
	Color Color::mediumblue(0, 0, 205);
	Color Color::mediumorchid(186, 85, 211);
	Color Color::mediumpurple(147, 112, 219);
	Color Color::mediumseagreen(60, 179, 113);
	Color Color::mediumslateblue(123, 104, 238);
	Color Color::mediumspringgreen(0, 250, 154);
	Color Color::mediumturquoise(72, 209, 204);
	Color Color::mediumvioletred(199, 21, 133);
	Color Color::midnightblue(25, 25, 112);
	Color Color::mintcream(245, 255, 250);
	Color Color::mistyrose(255, 228, 225);
	Color Color::moccasin(255, 228, 181);
	Color Color::navajowhite(255, 222, 173);
	Color Color::navy(0, 0, 128);
	Color Color::oldlace(253, 245, 230);
	Color Color::olive(128, 128, 0);
	Color Color::olivedrab(107, 142, 35);
	Color Color::orange(255, 165, 0);
	Color Color::orangered(255, 69, 0);
	Color Color::orchid(218, 112, 214);
	Color Color::palegoldenrod(238, 232, 170);
	Color Color::palegreen(152, 251, 152);
	Color Color::paleturquoise(175, 238, 238);
	Color Color::palevioletred(219, 112, 147);
	Color Color::papayawhip(255, 239, 213);
	Color Color::peachpuff(255, 218, 185);
	Color Color::peru(205, 133, 63);
	Color Color::pink(255, 192, 203);
	Color Color::plum(221, 160, 221);
	Color Color::powderblue(176, 224, 230);
	Color Color::purple(128, 0, 128);
	Color Color::red(255, 0, 0);
	Color Color::rosybrown(188, 143, 143);
	Color Color::royalblue(65, 105, 225);
	Color Color::saddlebrown(139, 69, 19);
	Color Color::salmon(250, 128, 114);
	Color Color::sandybrown(244, 164, 96);
	Color Color::seagreen(46, 139, 87);
	Color Color::seashell(255, 245, 238);
	Color Color::sienna(160, 82, 45);
	Color Color::silver(192, 192, 192);
	Color Color::skyblue(135, 206, 235);
	Color Color::slateblue(106, 90, 205);
	Color Color::slategray(112, 128, 144);
	Color Color::slategrey(112, 128, 144);
	Color Color::snow(255, 250, 250);
	Color Color::springgreen(0, 255, 127);
	Color Color::steelblue(70, 130, 180);
	Color Color::tan(210, 180, 140);
	Color Color::teal(0, 128, 128);
	Color Color::thistle(216, 191, 216);
	Color Color::tomato(255, 99, 71);
	Color Color::transparent(0, 0, 0, 0);
	Color Color::turquoise(64, 224, 208);
	Color Color::violet(238, 130, 238);
	Color Color::wheat(245, 222, 179);
	Color Color::white(255, 255, 255);
	Color Color::whitesmoke(245, 245, 245);
	Color Color::yellow(255, 255, 0);
	Color Color::yellowgreen(154, 205, 50);

	Color Color::gray10(26, 26, 26);
	Color Color::gray20(51, 51, 51);
	Color Color::gray30(77, 77, 77);
	Color Color::gray40(104, 104, 104);
	Color Color::gray50(128, 128, 128);
	Color Color::gray60(153, 153, 153);
	Color Color::gray70(178, 178, 178);
	Color Color::gray80(204, 204, 204);
	Color Color::gray90(230, 230, 230);

	Colorf Colorf::find_color(const std::string &name)
	{
		if (name.empty())
			return Colorf::transparent;

		if (name[0] == '#')
		{
			return Colorf(Color(name));
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

		auto it = color_map.find(StringHelp::text_to_local8(name));
		if (it != color_map.end()) return *it->second;

		return Colorf::transparent;
	}

	Colorf Colorf::aliceblue(40.0f / 255.0f, 248.0f / 255.0f, 255.0f / 255.0f);
	Colorf Colorf::antiquewhite(250.0f / 255.0f, 235.0f / 255.0f, 215.0f / 255.0f);
	Colorf Colorf::aqua(0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	Colorf Colorf::aquamarine(127.0f / 255.0f, 255.0f / 255.0f, 212.0f / 255.0f);
	Colorf Colorf::azure(240.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	Colorf Colorf::beige(245.0f / 255.0f, 245.0f / 255.0f, 220.0f / 255.0f);
	Colorf Colorf::bisque(255.0f / 255.0f, 228.0f / 255.0f, 196.0f / 255.0f);
	Colorf Colorf::black(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::blanchedalmond(255.0f / 255.0f, 235.0f / 255.0f, 205.0f / 255.0f);
	Colorf Colorf::blue(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
	Colorf Colorf::blueviolet(138.0f / 255.0f, 43.0f / 255.0f, 226.0f / 255.0f);
	Colorf Colorf::brown(165.0f / 255.0f, 42.0f / 255.0f, 42.0f / 255.0f);
	Colorf Colorf::burlywood(222.0f / 255.0f, 184.0f / 255.0f, 135.0f / 255.0f);
	Colorf Colorf::cadetblue(95.0f / 255.0f, 158.0f / 255.0f, 160.0f / 255.0f);
	Colorf Colorf::chartreuse(127.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::chocolate(210.0f / 255.0f, 105.0f / 255.0f, 30.0f / 255.0f);
	Colorf Colorf::coral(255.0f / 255.0f, 127.0f / 255.0f, 80.0f / 255.0f);
	Colorf Colorf::cornflowerblue(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f);
	Colorf Colorf::cornsilk(255.0f / 255.0f, 248.0f / 255.0f, 220.0f / 255.0f);
	Colorf Colorf::crimson(220.0f / 255.0f, 20.0f / 255.0f, 60.0f / 255.0f);
	Colorf Colorf::cyan(0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	Colorf Colorf::darkblue(0.0f / 255.0f, 0.0f / 255.0f, 139.0f / 255.0f);
	Colorf Colorf::darkcyan(0.0f / 255.0f, 139.0f / 255.0f, 139.0f / 255.0f);
	Colorf Colorf::darkgoldenrod(184.0f / 255.0f, 134.0f / 255.0f, 11.0f / 255.0f);
	Colorf Colorf::darkgray(169.0f / 255.0f, 169.0f / 255.0f, 169.0f / 255.0f);
	Colorf Colorf::darkgreen(0.0f / 255.0f, 100.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::darkgrey(169.0f / 255.0f, 169.0f / 255.0f, 169.0f / 255.0f);
	Colorf Colorf::darkkhaki(189.0f / 255.0f, 183.0f / 255.0f, 107.0f / 255.0f);
	Colorf Colorf::darkmagenta(139.0f / 255.0f, 0.0f / 255.0f, 139.0f / 255.0f);
	Colorf Colorf::darkolivegreen(85.0f / 255.0f, 107.0f / 255.0f, 47.0f / 255.0f);
	Colorf Colorf::darkorange(255.0f / 255.0f, 140.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::darkorchid(153.0f / 255.0f, 50.0f / 255.0f, 204.0f / 255.0f);
	Colorf Colorf::darkred(139.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::darksalmon(233.0f / 255.0f, 150.0f / 255.0f, 122.0f / 255.0f);
	Colorf Colorf::darkseagreen(143.0f / 255.0f, 188.0f / 255.0f, 143.0f / 255.0f);
	Colorf Colorf::darkslateblue(72.0f / 255.0f, 61.0f / 255.0f, 139.0f / 255.0f);
	Colorf Colorf::darkslategray(47.0f / 255.0f, 79.0f / 255.0f, 79.0f / 255.0f);
	Colorf Colorf::darkslategrey(47.0f / 255.0f, 79.0f / 255.0f, 79.0f / 255.0f);
	Colorf Colorf::darkturquoise(0.0f / 255.0f, 206.0f / 255.0f, 209.0f / 255.0f);
	Colorf Colorf::darkviolet(148.0f / 255.0f, 0.0f / 255.0f, 211.0f / 255.0f);
	Colorf Colorf::deeppink(255.0f / 255.0f, 20.0f / 255.0f, 147.0f / 255.0f);
	Colorf Colorf::deepskyblue(0.0f / 255.0f, 191.0f / 255.0f, 255.0f / 255.0f);
	Colorf Colorf::dimgray(105.0f / 255.0f, 105.0f / 255.0f, 105.0f / 255.0f);
	Colorf Colorf::dimgrey(105.0f / 255.0f, 105.0f / 255.0f, 105.0f / 255.0f);
	Colorf Colorf::dodgerblue(30.0f / 255.0f, 144.0f / 255.0f, 255.0f / 255.0f);
	Colorf Colorf::firebrick(178.0f / 255.0f, 34.0f / 255.0f, 34.0f / 255.0f);
	Colorf Colorf::floralwhite(255.0f / 255.0f, 250.0f / 255.0f, 240.0f / 255.0f);
	Colorf Colorf::forestgreen(34.0f / 255.0f, 139.0f / 255.0f, 34.0f / 255.0f);
	Colorf Colorf::fuchsia(255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
	Colorf Colorf::gainsboro(220.0f / 255.0f, 220.0f / 255.0f, 220.0f / 255.0f);
	Colorf Colorf::ghostwhite(248.0f / 255.0f, 248.0f / 255.0f, 255.0f / 255.0f);
	Colorf Colorf::gold(255.0f / 255.0f, 215.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::goldenrod(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f);
	Colorf Colorf::gray(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
	Colorf Colorf::grey(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
	Colorf Colorf::green(0.0f / 255.0f, 128.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::greenyellow(173.0f / 255.0f, 255.0f / 255.0f, 47.0f / 255.0f);
	Colorf Colorf::honeydew(240.0f / 255.0f, 255.0f / 255.0f, 240.0f / 255.0f);
	Colorf Colorf::hotpink(255.0f / 255.0f, 105.0f / 255.0f, 180.0f / 255.0f);
	Colorf Colorf::indianred(205.0f / 255.0f, 92.0f / 255.0f, 92.0f / 255.0f);
	Colorf Colorf::indigo(75.0f / 255.0f, 0.0f / 255.0f, 130.0f / 255.0f);
	Colorf Colorf::ivory(255.0f / 255.0f, 255.0f / 255.0f, 240.0f / 255.0f);
	Colorf Colorf::khaki(240.0f / 255.0f, 230.0f / 255.0f, 140.0f / 255.0f);
	Colorf Colorf::lavender(230.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f);
	Colorf Colorf::lavenderblush(255.0f / 255.0f, 240.0f / 255.0f, 245.0f / 255.0f);
	Colorf Colorf::lawngreen(124.0f / 255.0f, 252.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::lemonchiffon(255.0f / 255.0f, 250.0f / 255.0f, 205.0f / 255.0f);
	Colorf Colorf::lightblue(173.0f / 255.0f, 216.0f / 255.0f, 230.0f / 255.0f);
	Colorf Colorf::lightcoral(240.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
	Colorf Colorf::lightcyan(224.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	Colorf Colorf::lightgoldenrodyellow(250.0f / 255.0f, 250.0f / 255.0f, 210.0f / 255.0f);
	Colorf Colorf::lightgray(211.0f / 255.0f, 211.0f / 255.0f, 211.0f / 255.0f);
	Colorf Colorf::lightgreen(144.0f / 255.0f, 238.0f / 255.0f, 144.0f / 255.0f);
	Colorf Colorf::lightgrey(211.0f / 255.0f, 211.0f / 255.0f, 211.0f / 255.0f);
	Colorf Colorf::lightpink(255.0f / 255.0f, 182.0f / 255.0f, 193.0f / 255.0f);
	Colorf Colorf::lightsalmon(255.0f / 255.0f, 160.0f / 255.0f, 122.0f / 255.0f);
	Colorf Colorf::lightseagreen(32.0f / 255.0f, 178.0f / 255.0f, 170.0f / 255.0f);
	Colorf Colorf::lightskyblue(135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f);
	Colorf Colorf::lightslategray(119.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f);
	Colorf Colorf::lightslategrey(119.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f);
	Colorf Colorf::lightsteelblue(176.0f / 255.0f, 196.0f / 255.0f, 222.0f / 255.0f);
	Colorf Colorf::lightyellow(255.0f / 255.0f, 255.0f / 255.0f, 224.0f / 255.0f);
	Colorf Colorf::lime(0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::limegreen(50.0f / 255.0f, 205.0f / 255.0f, 50.0f / 255.0f);
	Colorf Colorf::linen(250.0f / 255.0f, 240.0f / 255.0f, 230.0f / 255.0f);
	Colorf Colorf::magenta(255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
	Colorf Colorf::maroon(128.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::mediumaquamarine(102.0f / 255.0f, 205.0f / 255.0f, 170.0f / 255.0f);
	Colorf Colorf::mediumblue(0.0f / 255.0f, 0.0f / 255.0f, 205.0f / 255.0f);
	Colorf Colorf::mediumorchid(186.0f / 255.0f, 85.0f / 255.0f, 211.0f / 255.0f);
	Colorf Colorf::mediumpurple(147.0f / 255.0f, 112.0f / 255.0f, 219.0f / 255.0f);
	Colorf Colorf::mediumseagreen(60.0f / 255.0f, 179.0f / 255.0f, 113.0f / 255.0f);
	Colorf Colorf::mediumslateblue(123.0f / 255.0f, 104.0f / 255.0f, 238.0f / 255.0f);
	Colorf Colorf::mediumspringgreen(0.0f / 255.0f, 250.0f / 255.0f, 154.0f / 255.0f);
	Colorf Colorf::mediumturquoise(72.0f / 255.0f, 209.0f / 255.0f, 204.0f / 255.0f);
	Colorf Colorf::mediumvioletred(199.0f / 255.0f, 21.0f / 255.0f, 133.0f / 255.0f);
	Colorf Colorf::midnightblue(25.0f / 255.0f, 25.0f / 255.0f, 112.0f / 255.0f);
	Colorf Colorf::mintcream(245.0f / 255.0f, 255.0f / 255.0f, 250.0f / 255.0f);
	Colorf Colorf::mistyrose(255.0f / 255.0f, 228.0f / 255.0f, 225.0f / 255.0f);
	Colorf Colorf::moccasin(255.0f / 255.0f, 228.0f / 255.0f, 181.0f / 255.0f);
	Colorf Colorf::navajowhite(255.0f / 255.0f, 222.0f / 255.0f, 173.0f / 255.0f);
	Colorf Colorf::navy(0.0f / 255.0f, 0.0f / 255.0f, 128.0f / 255.0f);
	Colorf Colorf::oldlace(253.0f / 255.0f, 245.0f / 255.0f, 230.0f / 255.0f);
	Colorf Colorf::olive(128.0f / 255.0f, 128.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::olivedrab(107.0f / 255.0f, 142.0f / 255.0f, 35.0f / 255.0f);
	Colorf Colorf::orange(255.0f / 255.0f, 165.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::orangered(255.0f / 255.0f, 69.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::orchid(218.0f / 255.0f, 112.0f / 255.0f, 214.0f / 255.0f);
	Colorf Colorf::palegoldenrod(238.0f / 255.0f, 232.0f / 255.0f, 170.0f / 255.0f);
	Colorf Colorf::palegreen(152.0f / 255.0f, 251.0f / 255.0f, 152.0f / 255.0f);
	Colorf Colorf::paleturquoise(175.0f / 255.0f, 238.0f / 255.0f, 238.0f / 255.0f);
	Colorf Colorf::palevioletred(219.0f / 255.0f, 112.0f / 255.0f, 147.0f / 255.0f);
	Colorf Colorf::papayawhip(255.0f / 255.0f, 239.0f / 255.0f, 213.0f / 255.0f);
	Colorf Colorf::peachpuff(255.0f / 255.0f, 218.0f / 255.0f, 185.0f / 255.0f);
	Colorf Colorf::peru(205.0f / 255.0f, 133.0f / 255.0f, 63.0f / 255.0f);
	Colorf Colorf::pink(255.0f / 255.0f, 192.0f / 255.0f, 203.0f / 255.0f);
	Colorf Colorf::plum(221.0f / 255.0f, 160.0f / 255.0f, 221.0f / 255.0f);
	Colorf Colorf::powderblue(176.0f / 255.0f, 224.0f / 255.0f, 230.0f / 255.0f);
	Colorf Colorf::purple(128.0f / 255.0f, 0.0f / 255.0f, 128.0f / 255.0f);
	Colorf Colorf::red(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::rosybrown(188.0f / 255.0f, 143.0f / 255.0f, 143.0f / 255.0f);
	Colorf Colorf::royalblue(65.0f / 255.0f, 105.0f / 255.0f, 225.0f / 255.0f);
	Colorf Colorf::saddlebrown(139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f);
	Colorf Colorf::salmon(250.0f / 255.0f, 128.0f / 255.0f, 114.0f / 255.0f);
	Colorf Colorf::sandybrown(244.0f / 255.0f, 164.0f / 255.0f, 96.0f / 255.0f);
	Colorf Colorf::seagreen(46.0f / 255.0f, 139.0f / 255.0f, 87.0f / 255.0f);
	Colorf Colorf::seashell(255.0f / 255.0f, 245.0f / 255.0f, 238.0f / 255.0f);
	Colorf Colorf::sienna(160.0f / 255.0f, 82.0f / 255.0f, 45.0f / 255.0f);
	Colorf Colorf::silver(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);
	Colorf Colorf::skyblue(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f);
	Colorf Colorf::slateblue(106.0f / 255.0f, 90.0f / 255.0f, 205.0f / 255.0f);
	Colorf Colorf::slategray(112.0f / 255.0f, 128.0f / 255.0f, 144.0f / 255.0f);
	Colorf Colorf::slategrey(112.0f / 255.0f, 128.0f / 255.0f, 144.0f / 255.0f);
	Colorf Colorf::snow(255.0f / 255.0f, 250.0f / 255.0f, 250.0f / 255.0f);
	Colorf Colorf::springgreen(0.0f / 255.0f, 255.0f / 255.0f, 127.0f / 255.0f);
	Colorf Colorf::steelblue(70.0f / 255.0f, 130.0f / 255.0f, 180.0f / 255.0f);
	Colorf Colorf::tan(210.0f / 255.0f, 180.0f / 255.0f, 140.0f / 255.0f);
	Colorf Colorf::teal(0.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
	Colorf Colorf::thistle(216.0f / 255.0f, 191.0f / 255.0f, 216.0f / 255.0f);
	Colorf Colorf::tomato(255.0f / 255.0f, 99.0f / 255.0f, 71.0f / 255.0f);
	Colorf Colorf::transparent(0.0f, 0.0f, 0.0f, 0.0f);
	Colorf Colorf::turquoise(64.0f / 255.0f, 224.0f / 255.0f, 208.0f / 255.0f);
	Colorf Colorf::violet(238.0f / 255.0f, 130.0f / 255.0f, 238.0f / 255.0f);
	Colorf Colorf::wheat(245.0f / 255.0f, 222.0f / 255.0f, 179.0f / 255.0f);
	Colorf Colorf::white(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	Colorf Colorf::whitesmoke(245.0f / 255.0f, 245.0f / 255.0f, 245.0f / 255.0f);
	Colorf Colorf::yellow(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f);
	Colorf Colorf::yellowgreen(154.0f / 255.0f, 205.0f / 255.0f, 50.0f / 255.0f);

	Colorf Colorf::gray10(0.1f, 0.1f, 0.1f);
	Colorf Colorf::gray20(0.2f, 0.2f, 0.2f);
	Colorf Colorf::gray30(0.3f, 0.3f, 0.3f);
	Colorf Colorf::gray40(0.4f, 0.4f, 0.4f);
	Colorf Colorf::gray50(0.5f, 0.5f, 0.5f);
	Colorf Colorf::gray60(0.6f, 0.6f, 0.6f);
	Colorf Colorf::gray70(0.7f, 0.7f, 0.7f);
	Colorf Colorf::gray80(0.8f, 0.8f, 0.8f);
	Colorf Colorf::gray90(0.9f, 0.9f, 0.9f);
}
