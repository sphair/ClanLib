/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/Display/color.h"
#include "API/Display/pixel_format.h"
#include <map>

/////////////////////////////////////////////////////////////////////////////
// CL_Color operations:

CL_Color::CL_Color(const CL_Colorf& color)
	: color((static_cast<unsigned int>(color.alpha   * 255) << 24)
			  | (static_cast<unsigned int>(color.red   * 255) << 16) 
			  | (static_cast<unsigned int>(color.green * 255) << 8) 
			  | (static_cast<unsigned int>(color.blue  * 255)))
{
}

CL_Color CL_Color::from_pixelformat(unsigned int raw_color, const CL_PixelFormat &pf)
{
	const int in_r_shift = CL_PixelFormat::get_mask_shift(pf.get_red_mask());
	const int in_g_shift = CL_PixelFormat::get_mask_shift(pf.get_green_mask());
	const int in_b_shift = CL_PixelFormat::get_mask_shift(pf.get_blue_mask());
	const int in_a_shift = CL_PixelFormat::get_mask_shift(pf.get_alpha_mask());

	const int in_r_bits = CL_PixelFormat::get_mask_bits(pf.get_red_mask());
	const int in_g_bits = CL_PixelFormat::get_mask_bits(pf.get_green_mask());
	const int in_b_bits = CL_PixelFormat::get_mask_bits(pf.get_blue_mask());
	const int in_a_bits = CL_PixelFormat::get_mask_bits(pf.get_alpha_mask());

	const int in_r_bitmask = CL_PixelFormat::get_bitmask(in_r_bits, 0);
	const int in_g_bitmask = CL_PixelFormat::get_bitmask(in_g_bits, 0);
	const int in_b_bitmask = CL_PixelFormat::get_bitmask(in_b_bits, 0);
	const int in_a_bitmask = CL_PixelFormat::get_bitmask(in_a_bits, 0);

	unsigned int in_r = 0, in_g = 0, in_b = 0, in_a = 0xff;
	if (in_r_bitmask)
		in_r = ((raw_color >> in_r_shift) & in_r_bitmask) << (8 - in_r_bits);
	if (in_g_bitmask)
		in_g = ((raw_color >> in_g_shift) & in_g_bitmask) << (8 - in_g_bits);
	if (in_b_bitmask)
		in_b = ((raw_color >> in_b_shift) & in_b_bitmask) << (8 - in_b_bits);
	if (in_a_bitmask)
		in_a = ((raw_color >> in_a_shift) & in_a_bitmask) << (8 - in_a_bits);

	return CL_Color(in_r, in_g, in_b, in_a);
}

unsigned int CL_Color::to_pixelformat(const CL_PixelFormat &pf) const
{
	const int out_r_shift = CL_PixelFormat::get_mask_shift(pf.get_red_mask());
	const int out_g_shift = CL_PixelFormat::get_mask_shift(pf.get_green_mask());
	const int out_b_shift = CL_PixelFormat::get_mask_shift(pf.get_blue_mask());
	const int out_a_shift = CL_PixelFormat::get_mask_shift(pf.get_alpha_mask());

	const int out_r_bits = CL_PixelFormat::get_mask_bits(pf.get_red_mask());
	const int out_g_bits = CL_PixelFormat::get_mask_bits(pf.get_green_mask());
	const int out_b_bits = CL_PixelFormat::get_mask_bits(pf.get_blue_mask());
	const int out_a_bits = CL_PixelFormat::get_mask_bits(pf.get_alpha_mask());

	const unsigned int in_r = get_red();
	const unsigned int in_g = get_green();
	const unsigned int in_b = get_blue();
	const unsigned int in_a = get_alpha();

	unsigned int out_pixel = 0;
	out_pixel = 0;
	if (out_r_bits)
		out_pixel |= (in_r >> (8 - out_r_bits)) << out_r_shift;
	if (out_g_bits)
		out_pixel |= (in_g >> (8 - out_g_bits)) << out_g_shift;
	if (out_b_bits)
		out_pixel |= (in_b >> (8 - out_b_bits)) << out_b_shift;
	if (out_a_bits)
		out_pixel |= (in_a >> (8 - out_a_bits)) << out_a_shift;

	return out_pixel;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Color attributes:

CL_Color CL_Color::aliceblue(240, 248, 255);
CL_Color CL_Color::antiquewhite(250, 235, 215);
CL_Color CL_Color::aqua( 0, 255, 255);
CL_Color CL_Color::aquamarine(127, 255, 212);
CL_Color CL_Color::azure(240, 255, 255);
CL_Color CL_Color::beige(245, 245, 220);
CL_Color CL_Color::bisque(255, 228, 196);
CL_Color CL_Color::black( 0, 0, 0);
CL_Color CL_Color::blanchedalmond(255, 235, 205);
CL_Color CL_Color::blue( 0, 0, 255);
CL_Color CL_Color::blueviolet(138, 43, 226);
CL_Color CL_Color::brown(165, 42, 42);
CL_Color CL_Color::burlywood(222, 184, 135);
CL_Color CL_Color::cadetblue( 95, 158, 160);
CL_Color CL_Color::chartreuse(127, 255, 0);
CL_Color CL_Color::chocolate(210, 105, 30);
CL_Color CL_Color::coral(255, 127, 80);
CL_Color CL_Color::cornflowerblue(100, 149, 237);
CL_Color CL_Color::cornsilk(255, 248, 220);
CL_Color CL_Color::crimson(220, 20, 60);
CL_Color CL_Color::cyan( 0, 255, 255);
CL_Color CL_Color::darkblue( 0, 0, 139);
CL_Color CL_Color::darkcyan( 0, 139, 139);
CL_Color CL_Color::darkgoldenrod(184, 134, 11);
CL_Color CL_Color::darkgray(169, 169, 169);
CL_Color CL_Color::darkgreen( 0, 100, 0);
CL_Color CL_Color::darkgrey(169, 169, 169);
CL_Color CL_Color::darkkhaki(189, 183, 107);
CL_Color CL_Color::darkmagenta(139, 0, 139);
CL_Color CL_Color::darkolivegreen( 85, 107, 47);
CL_Color CL_Color::darkorange(255, 140, 0);
CL_Color CL_Color::darkorchid(153, 50, 204);
CL_Color CL_Color::darkred(139, 0, 0);
CL_Color CL_Color::darksalmon(233, 150, 122);
CL_Color CL_Color::darkseagreen(143, 188, 143);
CL_Color CL_Color::darkslateblue( 72, 61, 139);
CL_Color CL_Color::darkslategray( 47, 79, 79);
CL_Color CL_Color::darkslategrey( 47, 79, 79);
CL_Color CL_Color::darkturquoise( 0, 206, 209);
CL_Color CL_Color::darkviolet(148, 0, 211);
CL_Color CL_Color::deeppink(255, 20, 147);
CL_Color CL_Color::deepskyblue( 0, 191, 255);
CL_Color CL_Color::dimgray(105, 105, 105);
CL_Color CL_Color::dimgrey(105, 105, 105);
CL_Color CL_Color::dodgerblue( 30, 144, 255);
CL_Color CL_Color::firebrick(178, 34, 34);
CL_Color CL_Color::floralwhite(255, 250, 240);
CL_Color CL_Color::forestgreen( 34, 139, 34);
CL_Color CL_Color::fuchsia(255, 0, 255);
CL_Color CL_Color::gainsboro(220, 220, 220);
CL_Color CL_Color::ghostwhite(248, 248, 255);
CL_Color CL_Color::gold(255, 215, 0);
CL_Color CL_Color::goldenrod(218, 165, 32);
CL_Color CL_Color::gray(128, 128, 128);
CL_Color CL_Color::grey(128, 128, 128);
CL_Color CL_Color::green( 0, 128, 0);
CL_Color CL_Color::greenyellow(173, 255, 47);
CL_Color CL_Color::honeydew(240, 255, 240);
CL_Color CL_Color::hotpink(255, 105, 180);
CL_Color CL_Color::indianred(205, 92, 92);
CL_Color CL_Color::indigo( 75, 0, 130);
CL_Color CL_Color::ivory(255, 255, 240);
CL_Color CL_Color::khaki(240, 230, 140);
CL_Color CL_Color::lavender(230, 230, 250);
CL_Color CL_Color::lavenderblush(255, 240, 245);
CL_Color CL_Color::lawngreen(124, 252, 0);
CL_Color CL_Color::lemonchiffon(255, 250, 205);
CL_Color CL_Color::lightblue(173, 216, 230);
CL_Color CL_Color::lightcoral(240, 128, 128);
CL_Color CL_Color::lightcyan(224, 255, 255);
CL_Color CL_Color::lightgoldenrodyellow(250, 250, 210);
CL_Color CL_Color::lightgray(211, 211, 211);
CL_Color CL_Color::lightgreen(144, 238, 144);
CL_Color CL_Color::lightgrey(211, 211, 211);
CL_Color CL_Color::lightpink(255, 182, 193);
CL_Color CL_Color::lightsalmon(255, 160, 122);
CL_Color CL_Color::lightseagreen( 32, 178, 170);
CL_Color CL_Color::lightskyblue(135, 206, 250);
CL_Color CL_Color::lightslategray(119, 136, 153);
CL_Color CL_Color::lightslategrey(119, 136, 153);
CL_Color CL_Color::lightsteelblue(176, 196, 222);
CL_Color CL_Color::lightyellow(255, 255, 224);
CL_Color CL_Color::lime( 0, 255, 0);
CL_Color CL_Color::limegreen( 50, 205, 50);
CL_Color CL_Color::linen(250, 240, 230);
CL_Color CL_Color::magenta(255, 0, 255);
CL_Color CL_Color::maroon(128, 0, 0);
CL_Color CL_Color::mediumaquamarine(102, 205, 170);
CL_Color CL_Color::mediumblue( 0, 0, 205);
CL_Color CL_Color::mediumorchid(186, 85, 211);
CL_Color CL_Color::mediumpurple(147, 112, 219);
CL_Color CL_Color::mediumseagreen( 60, 179, 113);
CL_Color CL_Color::mediumslateblue(123, 104, 238);
CL_Color CL_Color::mediumspringgreen( 0, 250, 154);
CL_Color CL_Color::mediumturquoise( 72, 209, 204);
CL_Color CL_Color::mediumvioletred(199, 21, 133);
CL_Color CL_Color::midnightblue( 25, 25, 112);
CL_Color CL_Color::mintcream(245, 255, 250);
CL_Color CL_Color::mistyrose(255, 228, 225);
CL_Color CL_Color::moccasin(255, 228, 181);
CL_Color CL_Color::navajowhite(255, 222, 173);
CL_Color CL_Color::navy( 0, 0, 128);
CL_Color CL_Color::oldlace(253, 245, 230);
CL_Color CL_Color::olive(128, 128, 0);
CL_Color CL_Color::olivedrab(107, 142, 35);
CL_Color CL_Color::orange(255, 165, 0);
CL_Color CL_Color::orangered(255, 69, 0);
CL_Color CL_Color::orchid(218, 112, 214);
CL_Color CL_Color::palegoldenrod(238, 232, 170);
CL_Color CL_Color::palegreen(152, 251, 152);
CL_Color CL_Color::paleturquoise(175, 238, 238);
CL_Color CL_Color::palevioletred(219, 112, 147);
CL_Color CL_Color::papayawhip(255, 239, 213);
CL_Color CL_Color::peachpuff(255, 218, 185);
CL_Color CL_Color::peru(205, 133, 63);
CL_Color CL_Color::pink(255, 192, 203);
CL_Color CL_Color::plum(221, 160, 221);
CL_Color CL_Color::powderblue(176, 224, 230);
CL_Color CL_Color::purple(128, 0, 128);
CL_Color CL_Color::red(255, 0, 0);
CL_Color CL_Color::rosybrown(188, 143, 143);
CL_Color CL_Color::royalblue( 65, 105, 225);
CL_Color CL_Color::saddlebrown(139, 69, 19);
CL_Color CL_Color::salmon(250, 128, 114);
CL_Color CL_Color::sandybrown(244, 164, 96);
CL_Color CL_Color::seagreen( 46, 139, 87);
CL_Color CL_Color::seashell(255, 245, 238);
CL_Color CL_Color::sienna(160, 82, 45);
CL_Color CL_Color::silver(192, 192, 192);
CL_Color CL_Color::skyblue(135, 206, 235);
CL_Color CL_Color::slateblue(106, 90, 205);
CL_Color CL_Color::slategray(112, 128, 144);
CL_Color CL_Color::slategrey(112, 128, 144);
CL_Color CL_Color::snow(255, 250, 250);
CL_Color CL_Color::springgreen( 0, 255, 127);
CL_Color CL_Color::steelblue( 70, 130, 180);
CL_Color CL_Color::tan(210, 180, 140);
CL_Color CL_Color::teal( 0, 128, 128);
CL_Color CL_Color::thistle(216, 191, 216);
CL_Color CL_Color::tomato(255, 99, 71);
CL_Color CL_Color::turquoise( 64, 224, 208);
CL_Color CL_Color::violet(238, 130, 238);
CL_Color CL_Color::wheat(245, 222, 179);
CL_Color CL_Color::white(255, 255, 255);
CL_Color CL_Color::whitesmoke(245, 245, 245);
CL_Color CL_Color::yellow(255, 255, 0);
CL_Color CL_Color::yellowgreen(154, 205, 50);

CL_Color &CL_Color::find_color(const std::string &name)
{
	static std::map<std::string, CL_Color *> color_map;
	if (color_map.empty())
	{
		color_map["aliceblue"] = &CL_Color::aliceblue;
		color_map["antiquewhite"] = &CL_Color::antiquewhite;
		color_map["aqua"] = &CL_Color::aqua;
		color_map["aquamarine"] = &CL_Color::aquamarine;
		color_map["azure"] = &CL_Color::azure;
		color_map["beige"] = &CL_Color::beige;
		color_map["bisque"] = &CL_Color::bisque;
		color_map["black"] = &CL_Color::black;
		color_map["blanchedalmond"] = &CL_Color::blanchedalmond;
		color_map["blue"] = &CL_Color::blue;
		color_map["blueviolet"] = &CL_Color::blueviolet;
		color_map["brown"] = &CL_Color::brown;
		color_map["burlywood"] = &CL_Color::burlywood;
		color_map["cadetblue"] = &CL_Color::cadetblue;
		color_map["chartreuse"] = &CL_Color::chartreuse;
		color_map["chocolate"] = &CL_Color::chocolate;
		color_map["coral"] = &CL_Color::coral;
		color_map["cornflowerblue"] = &CL_Color::cornflowerblue;
		color_map["cornsilk"] = &CL_Color::cornsilk;
		color_map["crimson"] = &CL_Color::crimson;
		color_map["cyan"] = &CL_Color::cyan;
		color_map["darkblue"] = &CL_Color::darkblue;
		color_map["darkcyan"] = &CL_Color::darkcyan;
		color_map["darkgoldenrod"] = &CL_Color::darkgoldenrod;
		color_map["darkgray"] = &CL_Color::darkgray;
		color_map["darkgreen"] = &CL_Color::darkgreen;
		color_map["darkgrey"] = &CL_Color::darkgrey;
		color_map["darkkhaki"] = &CL_Color::darkkhaki;
		color_map["darkmagenta"] = &CL_Color::darkmagenta;
		color_map["darkolivegreen"] = &CL_Color::darkolivegreen;
		color_map["darkorange"] = &CL_Color::darkorange;
		color_map["darkorchid"] = &CL_Color::darkorchid;
		color_map["darkred"] = &CL_Color::darkred;
		color_map["darksalmon"] = &CL_Color::darksalmon;
		color_map["darkseagreen"] = &CL_Color::darkseagreen;
		color_map["darkslateblue"] = &CL_Color::darkslateblue;
		color_map["darkslategray"] = &CL_Color::darkslategray;
		color_map["darkslategrey"] = &CL_Color::darkslategrey;
		color_map["darkturquoise"] = &CL_Color::darkturquoise;
		color_map["darkviolet"] = &CL_Color::darkviolet;
		color_map["deeppink"] = &CL_Color::deeppink;
		color_map["deepskyblue"] = &CL_Color::deepskyblue;
		color_map["dimgray"] = &CL_Color::dimgray;
		color_map["dimgrey"] = &CL_Color::dimgrey;
		color_map["dodgerblue"] = &CL_Color::dodgerblue;
		color_map["firebrick"] = &CL_Color::firebrick;
		color_map["floralwhite"] = &CL_Color::floralwhite;
		color_map["forestgreen"] = &CL_Color::forestgreen;
		color_map["fuchsia"] = &CL_Color::fuchsia;
		color_map["gainsboro"] = &CL_Color::gainsboro;
		color_map["ghostwhite"] = &CL_Color::ghostwhite;
		color_map["gold"] = &CL_Color::gold;
		color_map["goldenrod"] = &CL_Color::goldenrod;
		color_map["gray"] = &CL_Color::gray;
		color_map["grey"] = &CL_Color::grey;
		color_map["green"] = &CL_Color::green;
		color_map["greenyellow"] = &CL_Color::greenyellow;
		color_map["honeydew"] = &CL_Color::honeydew;
		color_map["hotpink"] = &CL_Color::hotpink;
		color_map["indianred"] = &CL_Color::indianred;
		color_map["indigo"] = &CL_Color::indigo;
		color_map["ivory"] = &CL_Color::ivory;
		color_map["khaki"] = &CL_Color::khaki;
		color_map["lavender"] = &CL_Color::lavender;
		color_map["lavenderblush"] = &CL_Color::lavenderblush;
		color_map["lawngreen"] = &CL_Color::lawngreen;
		color_map["lemonchiffon"] = &CL_Color::lemonchiffon;
		color_map["lightblue"] = &CL_Color::lightblue;
		color_map["lightcoral"] = &CL_Color::lightcoral;
		color_map["lightcyan"] = &CL_Color::lightcyan;
		color_map["lightgoldenrodyellow"] = &CL_Color::lightgoldenrodyellow;
		color_map["lightgray"] = &CL_Color::lightgray;
		color_map["lightgreen"] = &CL_Color::lightgreen;
		color_map["lightgrey"] = &CL_Color::lightgrey;
		color_map["lightpink"] = &CL_Color::lightpink;
		color_map["lightsalmon"] = &CL_Color::lightsalmon;
		color_map["lightseagreen"] = &CL_Color::lightseagreen;
		color_map["lightskyblue"] = &CL_Color::lightskyblue;
		color_map["lightslategray"] = &CL_Color::lightslategray;
		color_map["lightslategrey"] = &CL_Color::lightslategrey;
		color_map["lightsteelblue"] = &CL_Color::lightsteelblue;
		color_map["lightyellow"] = &CL_Color::lightyellow;
		color_map["lime"] = &CL_Color::lime;
		color_map["limegreen"] = &CL_Color::limegreen;
		color_map["linen"] = &CL_Color::linen;
		color_map["magenta"] = &CL_Color::magenta;
		color_map["maroon"] = &CL_Color::maroon;
		color_map["mediumaquamarine"] = &CL_Color::mediumaquamarine;
		color_map["mediumblue"] = &CL_Color::mediumblue;
		color_map["mediumorchid"] = &CL_Color::mediumorchid;
		color_map["mediumpurple"] = &CL_Color::mediumpurple;
		color_map["mediumseagreen"] = &CL_Color::mediumseagreen;
		color_map["mediumslateblue"] = &CL_Color::mediumslateblue;
		color_map["mediumspringgreen"] = &CL_Color::mediumspringgreen;
		color_map["mediumturquoise"] = &CL_Color::mediumturquoise;
		color_map["mediumvioletred"] = &CL_Color::mediumvioletred;
		color_map["midnightblue"] = &CL_Color::midnightblue;
		color_map["mintcream"] = &CL_Color::mintcream;
		color_map["mistyrose"] = &CL_Color::mistyrose;
		color_map["moccasin"] = &CL_Color::moccasin;
		color_map["navajowhite"] = &CL_Color::navajowhite;
		color_map["navy"] = &CL_Color::navy;
		color_map["oldlace"] = &CL_Color::oldlace;
		color_map["olive"] = &CL_Color::olive;
		color_map["olivedrab"] = &CL_Color::olivedrab;
		color_map["orange"] = &CL_Color::orange;
		color_map["orangered"] = &CL_Color::orangered;
		color_map["orchid"] = &CL_Color::orchid;
		color_map["palegoldenrod"] = &CL_Color::palegoldenrod;
		color_map["palegreen"] = &CL_Color::palegreen;
		color_map["paleturquoise"] = &CL_Color::paleturquoise;
		color_map["palevioletred"] = &CL_Color::palevioletred;
		color_map["papayawhip"] = &CL_Color::papayawhip;
		color_map["peachpuff"] = &CL_Color::peachpuff;
		color_map["peru"] = &CL_Color::peru;
		color_map["pink"] = &CL_Color::pink;
		color_map["plum"] = &CL_Color::plum;
		color_map["powderblue"] = &CL_Color::powderblue;
		color_map["purple"] = &CL_Color::purple;
		color_map["red"] = &CL_Color::red;
		color_map["rosybrown"] = &CL_Color::rosybrown;
		color_map["royalblue"] = &CL_Color::royalblue;
		color_map["saddlebrown"] = &CL_Color::saddlebrown;
		color_map["salmon"] = &CL_Color::salmon;
		color_map["sandybrown"] = &CL_Color::sandybrown;
		color_map["seagreen"] = &CL_Color::seagreen;
		color_map["seashell"] = &CL_Color::seashell;
		color_map["sienna"] = &CL_Color::sienna;
		color_map["silver"] = &CL_Color::silver;
		color_map["skyblue"] = &CL_Color::skyblue;
		color_map["slateblue"] = &CL_Color::slateblue;
		color_map["slategray"] = &CL_Color::slategray;
		color_map["slategrey"] = &CL_Color::slategrey;
		color_map["snow"] = &CL_Color::snow;
		color_map["springgreen"] = &CL_Color::springgreen;
		color_map["steelblue"] = &CL_Color::steelblue;
		color_map["tan"] = &CL_Color::tan;
		color_map["teal"] = &CL_Color::teal;
		color_map["thistle"] = &CL_Color::thistle;
		color_map["tomato"] = &CL_Color::tomato;
		color_map["turquoise"] = &CL_Color::turquoise;
		color_map["violet"] = &CL_Color::violet;
		color_map["wheat"] = &CL_Color::wheat;
		color_map["white"] = &CL_Color::white;
		color_map["whitesmoke"] = &CL_Color::whitesmoke;
		color_map["yellow"] = &CL_Color::yellow;
		color_map["yellowgreen"] = &CL_Color::yellowgreen;
	}
	
	std::map<std::string, CL_Color *>::iterator it = color_map.find(name);
	if (it != color_map.end()) return *it->second;

	static CL_Color null(0,0,0,0);
	return null;
}
	
std::vector<std::string> &CL_Color::get_names()
{
	static std::vector<std::string> names;
	
	if (names.empty())
	{
		names.push_back("aliceblue");
		names.push_back("antiquewhite");
		names.push_back("aqua");
		names.push_back("aquamarine");
		names.push_back("azure");
		names.push_back("beige");
		names.push_back("bisque");
		names.push_back("black");
		names.push_back("blanchedalmond");
		names.push_back("blue");
		names.push_back("blueviolet");
		names.push_back("brown");
		names.push_back("burlywood");
		names.push_back("cadetblue");
		names.push_back("chartreuse");
		names.push_back("chocolate");
		names.push_back("coral");
		names.push_back("cornflowerblue");
		names.push_back("cornsilk");
		names.push_back("crimson");
		names.push_back("cyan");
		names.push_back("darkblue");
		names.push_back("darkcyan");
		names.push_back("darkgoldenrod");
		names.push_back("darkgray");
		names.push_back("darkgreen");
		names.push_back("darkgrey");
		names.push_back("darkkhaki");
		names.push_back("darkmagenta");
		names.push_back("darkolivegreen");
		names.push_back("darkorange");
		names.push_back("darkorchid");
		names.push_back("darkred");
		names.push_back("darksalmon");
		names.push_back("darkseagreen");
		names.push_back("darkslateblue");
		names.push_back("darkslategray");
		names.push_back("darkslategrey");
		names.push_back("darkturquoise");
		names.push_back("darkviolet");
		names.push_back("deeppink");
		names.push_back("deepskyblue");
		names.push_back("dimgray");
		names.push_back("dimgrey");
		names.push_back("dodgerblue");
		names.push_back("firebrick");
		names.push_back("floralwhite");
		names.push_back("forestgreen");
		names.push_back("fuchsia");
		names.push_back("gainsboro");
		names.push_back("ghostwhite");
		names.push_back("gold");
		names.push_back("goldenrod");
		names.push_back("gray");
		names.push_back("grey");
		names.push_back("green");
		names.push_back("greenyellow");
		names.push_back("honeydew");
		names.push_back("hotpink");
		names.push_back("indianred");
		names.push_back("indigo");
		names.push_back("ivory");
		names.push_back("khaki");
		names.push_back("lavender");
		names.push_back("lavenderblush");
		names.push_back("lawngreen");
		names.push_back("lemonchiffon");
		names.push_back("lightblue");
		names.push_back("lightcoral");
		names.push_back("lightcyan");
		names.push_back("lightgoldenrodyellow");
		names.push_back("lightgray");
		names.push_back("lightgreen");
		names.push_back("lightgrey");
		names.push_back("lightpink");
		names.push_back("lightsalmon");
		names.push_back("lightseagreen");
		names.push_back("lightskyblue");
		names.push_back("lightslategray");
		names.push_back("lightslategrey");
		names.push_back("lightsteelblue");
		names.push_back("lightyellow");
		names.push_back("lime");
		names.push_back("limegreen");
		names.push_back("linen");
		names.push_back("magenta");
		names.push_back("maroon");
		names.push_back("mediumaquamarine");
		names.push_back("mediumblue");
		names.push_back("mediumorchid");
		names.push_back("mediumpurple");
		names.push_back("mediumseagreen");
		names.push_back("mediumslateblue");
		names.push_back("mediumspringgreen");
		names.push_back("mediumturquoise");
		names.push_back("mediumvioletred");
		names.push_back("midnightblue");
		names.push_back("mintcream");
		names.push_back("mistyrose");
		names.push_back("moccasin");
		names.push_back("navajowhite");
		names.push_back("navy");
		names.push_back("oldlace");
		names.push_back("olive");
		names.push_back("olivedrab");
		names.push_back("orange");
		names.push_back("orangered");
		names.push_back("orchid");
		names.push_back("palegoldenrod");
		names.push_back("palegreen");
		names.push_back("paleturquoise");
		names.push_back("palevioletred");
		names.push_back("papayawhip");
		names.push_back("peachpuff");
		names.push_back("peru");
		names.push_back("pink");
		names.push_back("plum");
		names.push_back("powderblue");
		names.push_back("purple");
		names.push_back("red");
		names.push_back("rosybrown");
		names.push_back("royalblue");
		names.push_back("saddlebrown");
		names.push_back("salmon");
		names.push_back("sandybrown");
		names.push_back("seagreen");
		names.push_back("seashell");
		names.push_back("sienna");
		names.push_back("silver");
		names.push_back("skyblue");
		names.push_back("slateblue");
		names.push_back("slategray");
		names.push_back("slategrey");
		names.push_back("snow");
		names.push_back("springgreen");
		names.push_back("steelblue");
		names.push_back("tan");
		names.push_back("teal");
		names.push_back("thistle");
		names.push_back("tomato");
		names.push_back("turquoise");
		names.push_back("violet");
		names.push_back("wheat");
		names.push_back("white");
		names.push_back("whitesmoke");
		names.push_back("yellow");
		names.push_back("yellowgreen");
	}
	
	return names;
}
