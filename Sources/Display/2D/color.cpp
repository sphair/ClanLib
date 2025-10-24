/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/Display/Image/pixel_format.h"
#include "API/Core/Text/string_types.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include <map>

#ifndef WIN32
#include <cstdlib>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_Color construction:

CL_Color::CL_Color(const CL_StringRef &hexstr)
{
	if( hexstr.empty() || hexstr.length() > 9 )
		throw CL_Exception(cl_format("Bad hex color string: %1", hexstr));

	int pos = 0;
	if( hexstr[0] == '#' )
		pos++;

	color = strtoul(CL_StringHelp::text_to_local8(hexstr.substr(pos)).c_str(), 0, 16);
 
	bool have_alpha = (hexstr.length()-pos) > 6;

	if( !have_alpha )
	{
		color |= 0xFF000000;
	}
	else  // rgba -> argb
	{
		int tmp_color = 0;
		tmp_color |= (color >> 24) << 16; // r
		tmp_color |= (0x0000FFFF & (color >> 8)); // g, b
		tmp_color |= (color & 0xFF) << 24; // a
		color = tmp_color;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Color operations:

CL_Color::CL_Color(const CL_Colord& color)
	: color((static_cast<unsigned int>(color.a * 255) << 24)
			  | (static_cast<unsigned int>(color.r * 255) << 16) 
			  | (static_cast<unsigned int>(color.g * 255) << 8) 
			  | (static_cast<unsigned int>(color.b * 255)))
{
}

CL_Color::CL_Color(const CL_Colorf& color)
	: color((static_cast<unsigned int>(color.a * 255) << 24)
			  | (static_cast<unsigned int>(color.r * 255) << 16) 
			  | (static_cast<unsigned int>(color.g * 255) << 8) 
			  | (static_cast<unsigned int>(color.b * 255)))
{
}

/*
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
*/

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
CL_Color CL_Color::transparent(0,0,0,0);
CL_Color CL_Color::turquoise( 64, 224, 208);
CL_Color CL_Color::violet(238, 130, 238);
CL_Color CL_Color::wheat(245, 222, 179);
CL_Color CL_Color::white(255, 255, 255);
CL_Color CL_Color::whitesmoke(245, 245, 245);
CL_Color CL_Color::yellow(255, 255, 0);
CL_Color CL_Color::yellowgreen(154, 205, 50);

CL_Color CL_Color::gray10(26,26,26);
CL_Color CL_Color::gray20(51,51,51);
CL_Color CL_Color::gray30(77,77,77);
CL_Color CL_Color::gray40(104,104,104);
CL_Color CL_Color::gray50(128,128,128);
CL_Color CL_Color::gray60(153,153,153);
CL_Color CL_Color::gray70(178,178,178);
CL_Color CL_Color::gray80(204,204,204);
CL_Color CL_Color::gray90(230,230,230);

CL_Color CL_Color::find_color(const CL_StringRef &name)
{
	if( name.empty() )
		return CL_Color::transparent;

	if( name[0] == '#')
	{
		return CL_Color(name);
	}

	static std::map<CL_String8, CL_Color *> color_map;
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
		color_map["transparent"] = &CL_Color::transparent;
		color_map["tomato"] = &CL_Color::tomato;
		color_map["turquoise"] = &CL_Color::turquoise;
		color_map["violet"] = &CL_Color::violet;
		color_map["wheat"] = &CL_Color::wheat;
		color_map["white"] = &CL_Color::white;
		color_map["whitesmoke"] = &CL_Color::whitesmoke;
		color_map["yellow"] = &CL_Color::yellow;
		color_map["yellowgreen"] = &CL_Color::yellowgreen;
		color_map["gray10"] = &CL_Color::gray10;
		color_map["gray20"] = &CL_Color::gray20;
		color_map["gray30"] = &CL_Color::gray30;
		color_map["gray40"] = &CL_Color::gray40;
		color_map["gray50"] = &CL_Color::gray50;
		color_map["gray60"] = &CL_Color::gray60;
		color_map["gray70"] = &CL_Color::gray70;
		color_map["gray80"] = &CL_Color::gray80;
		color_map["gray90"] = &CL_Color::gray90;
	}
	
	std::map<CL_String8, CL_Color *>::iterator it = color_map.find(CL_StringHelp::text_to_local8(name));
	if (it != color_map.end()) return *it->second;

	static CL_Color null(0,0,0,0);
	return null;
}
	
std::vector<CL_String8> &CL_Color::get_names()
{
	static std::vector<CL_String8> names;
	
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
		names.push_back("transparent");
		names.push_back("turquoise");
		names.push_back("violet");
		names.push_back("wheat");
		names.push_back("white");
		names.push_back("whitesmoke");
		names.push_back("yellow");
		names.push_back("yellowgreen");
		names.push_back("gray10");
		names.push_back("gray20");
		names.push_back("gray30");
		names.push_back("gray40");
		names.push_back("gray50");
		names.push_back("gray60");
		names.push_back("gray70");
		names.push_back("gray80");
		names.push_back("gray90");
	}
	
	return names;
}


/////////////////////////////////////////////////////////////////////////////
// CL_Colord attributes:

CL_Colord CL_Colord::aliceblue(40/255.0, 248/255.0, 255/255.0);
CL_Colord CL_Colord::antiquewhite(250/255.0, 235/255.0, 215/255.0);
CL_Colord CL_Colord::aqua( 0/255.0, 255/255.0, 255/255.0);
CL_Colord CL_Colord::aquamarine(127/255.0, 255/255.0, 212/255.0);
CL_Colord CL_Colord::azure(240/255.0, 255/255.0, 255/255.0);
CL_Colord CL_Colord::beige(245/255.0, 245/255.0, 220/255.0);
CL_Colord CL_Colord::bisque(255/255.0, 228/255.0, 196/255.0);
CL_Colord CL_Colord::black( 0/255.0, 0/255.0, 0/255.0 );
CL_Colord CL_Colord::blanchedalmond(255/255.0, 235/255.0, 205/255.0);
CL_Colord CL_Colord::blue( 0/255.0, 0/255.0, 255/255.0);
CL_Colord CL_Colord::blueviolet(138/255.0, 43/255.0, 226/255.0);
CL_Colord CL_Colord::brown(165/255.0, 42/255.0, 42/255.0);
CL_Colord CL_Colord::burlywood(222/255.0, 184/255.0, 135/255.0);
CL_Colord CL_Colord::cadetblue( 95/255.0, 158/255.0, 160/255.0);
CL_Colord CL_Colord::chartreuse(127/255.0, 255/255.0, 0/255.0);
CL_Colord CL_Colord::chocolate(210/255.0, 105/255.0, 30/255.0);
CL_Colord CL_Colord::coral(255/255.0, 127/255.0, 80/255.0);
CL_Colord CL_Colord::cornflowerblue(100/255.0, 149/255.0, 237/255.0);
CL_Colord CL_Colord::cornsilk(255/255.0, 248/255.0, 220/255.0);
CL_Colord CL_Colord::crimson(220/255.0, 20/255.0, 60/255.0);
CL_Colord CL_Colord::cyan( 0/255.0, 255/255.0, 255/255.0);
CL_Colord CL_Colord::darkblue( 0/255.0, 0/255.0, 139/255.0);
CL_Colord CL_Colord::darkcyan( 0/255.0, 139/255.0, 139/255.0);
CL_Colord CL_Colord::darkgoldenrod(184/255.0, 134/255.0, 11/255.0);
CL_Colord CL_Colord::darkgray(169/255.0, 169/255.0, 169/255.0);
CL_Colord CL_Colord::darkgreen( 0/255.0, 100/255.0, 0/255.0);
CL_Colord CL_Colord::darkgrey(169/255.0, 169/255.0, 169/255.0);
CL_Colord CL_Colord::darkkhaki(189/255.0, 183/255.0, 107/255.0);
CL_Colord CL_Colord::darkmagenta(139/255.0, 0/255.0, 139/255.0);
CL_Colord CL_Colord::darkolivegreen( 85/255.0, 107/255.0, 47/255.0);
CL_Colord CL_Colord::darkorange(255/255.0, 140/255.0, 0/255.0);
CL_Colord CL_Colord::darkorchid(153/255.0, 50/255.0, 204/255.0);
CL_Colord CL_Colord::darkred(139/255.0, 0/255.0, 0/255.0);
CL_Colord CL_Colord::darksalmon(233/255.0, 150/255.0, 122/255.0);
CL_Colord CL_Colord::darkseagreen(143/255.0, 188/255.0, 143/255.0);
CL_Colord CL_Colord::darkslateblue( 72/255.0, 61/255.0, 139/255.0);
CL_Colord CL_Colord::darkslategray( 47/255.0, 79/255.0, 79/255.0);
CL_Colord CL_Colord::darkslategrey( 47/255.0, 79/255.0, 79/255.0);
CL_Colord CL_Colord::darkturquoise( 0/255.0, 206/255.0, 209/255.0);
CL_Colord CL_Colord::darkviolet(148/255.0, 0/255.0, 211/255.0);
CL_Colord CL_Colord::deeppink(255/255.0, 20/255.0, 147/255.0);
CL_Colord CL_Colord::deepskyblue( 0/255.0, 191/255.0, 255/255.0);
CL_Colord CL_Colord::dimgray(105/255.0, 105/255.0, 105/255.0);
CL_Colord CL_Colord::dimgrey(105/255.0, 105/255.0, 105/255.0);
CL_Colord CL_Colord::dodgerblue( 30/255.0, 144/255.0, 255/255.0);
CL_Colord CL_Colord::firebrick(178/255.0, 34/255.0, 34/255.0);
CL_Colord CL_Colord::floralwhite(255/255.0, 250/255.0, 240/255.0);
CL_Colord CL_Colord::forestgreen( 34/255.0, 139/255.0, 34/255.0);
CL_Colord CL_Colord::fuchsia(255/255.0, 0/255.0, 255/255.0);
CL_Colord CL_Colord::gainsboro(220/255.0, 220/255.0, 220/255.0);
CL_Colord CL_Colord::ghostwhite(248/255.0, 248/255.0, 255/255.0);
CL_Colord CL_Colord::gold(255/255.0, 215/255.0, 0/255.0);
CL_Colord CL_Colord::goldenrod(218/255.0, 165/255.0, 32/255.0);
CL_Colord CL_Colord::gray(128/255.0, 128/255.0, 128/255.0);
CL_Colord CL_Colord::grey(128/255.0, 128/255.0, 128/255.0);
CL_Colord CL_Colord::green( 0/255.0, 128/255.0, 0/255.0);
CL_Colord CL_Colord::greenyellow(173/255.0, 255/255.0, 47/255.0);
CL_Colord CL_Colord::honeydew(240/255.0, 255/255.0, 240/255.0);
CL_Colord CL_Colord::hotpink(255/255.0, 105/255.0, 180/255.0);
CL_Colord CL_Colord::indianred(205/255.0, 92/255.0, 92/255.0);
CL_Colord CL_Colord::indigo( 75/255.0, 0/255.0, 130/255.0);
CL_Colord CL_Colord::ivory(255/255.0, 255/255.0, 240/255.0);
CL_Colord CL_Colord::khaki(240/255.0, 230/255.0, 140/255.0);
CL_Colord CL_Colord::lavender(230/255.0, 230/255.0, 250/255.0);
CL_Colord CL_Colord::lavenderblush(255/255.0, 240/255.0, 245/255.0);
CL_Colord CL_Colord::lawngreen(124/255.0, 252/255.0, 0/255.0);
CL_Colord CL_Colord::lemonchiffon(255/255.0, 250/255.0, 205/255.0);
CL_Colord CL_Colord::lightblue(173/255.0, 216/255.0, 230/255.0);
CL_Colord CL_Colord::lightcoral(240/255.0, 128/255.0, 128/255.0);
CL_Colord CL_Colord::lightcyan(224/255.0, 255/255.0, 255/255.0);
CL_Colord CL_Colord::lightgoldenrodyellow(250/255.0, 250/255.0, 210/255.0);
CL_Colord CL_Colord::lightgray(211/255.0, 211/255.0, 211/255.0);
CL_Colord CL_Colord::lightgreen(144/255.0, 238/255.0, 144/255.0);
CL_Colord CL_Colord::lightgrey(211/255.0, 211/255.0, 211/255.0);
CL_Colord CL_Colord::lightpink(255/255.0, 182/255.0, 193/255.0);
CL_Colord CL_Colord::lightsalmon(255/255.0, 160/255.0, 122/255.0);
CL_Colord CL_Colord::lightseagreen( 32/255.0, 178/255.0, 170/255.0);
CL_Colord CL_Colord::lightskyblue(135/255.0, 206/255.0, 250/255.0);
CL_Colord CL_Colord::lightslategray(119/255.0, 136/255.0, 153/255.0);
CL_Colord CL_Colord::lightslategrey(119/255.0, 136/255.0, 153/255.0);
CL_Colord CL_Colord::lightsteelblue(176/255.0, 196/255.0, 222/255.0);
CL_Colord CL_Colord::lightyellow(255/255.0, 255/255.0, 224/255.0);
CL_Colord CL_Colord::lime( 0/255.0, 255/255.0, 0/255.0);
CL_Colord CL_Colord::limegreen( 50/255.0, 205/255.0, 50/255.0);
CL_Colord CL_Colord::linen(250/255.0, 240/255.0, 230/255.0);
CL_Colord CL_Colord::magenta(255/255.0, 0/255.0, 255/255.0);
CL_Colord CL_Colord::maroon(128/255.0, 0/255.0, 0/255.0);
CL_Colord CL_Colord::mediumaquamarine(102/255.0, 205/255.0, 170/255.0);
CL_Colord CL_Colord::mediumblue( 0/255.0, 0/255.0, 205/255.0);
CL_Colord CL_Colord::mediumorchid(186/255.0, 85/255.0, 211/255.0);
CL_Colord CL_Colord::mediumpurple(147/255.0, 112/255.0, 219/255.0);
CL_Colord CL_Colord::mediumseagreen( 60/255.0, 179/255.0, 113/255.0);
CL_Colord CL_Colord::mediumslateblue(123/255.0, 104/255.0, 238/255.0);
CL_Colord CL_Colord::mediumspringgreen( 0/255.0, 250/255.0, 154/255.0);
CL_Colord CL_Colord::mediumturquoise( 72/255.0, 209/255.0, 204/255.0);
CL_Colord CL_Colord::mediumvioletred(199/255.0, 21/255.0, 133/255.0);
CL_Colord CL_Colord::midnightblue( 25/255.0, 25/255.0, 112/255.0);
CL_Colord CL_Colord::mintcream(245/255.0, 255/255.0, 250/255.0);
CL_Colord CL_Colord::mistyrose(255/255.0, 228/255.0, 225/255.0);
CL_Colord CL_Colord::moccasin(255/255.0, 228/255.0, 181/255.0);
CL_Colord CL_Colord::navajowhite(255/255.0, 222/255.0, 173/255.0);
CL_Colord CL_Colord::navy( 0/255.0, 0/255.0, 128/255.0);
CL_Colord CL_Colord::oldlace(253/255.0, 245/255.0, 230/255.0);
CL_Colord CL_Colord::olive(128/255.0, 128/255.0, 0/255.0);
CL_Colord CL_Colord::olivedrab(107/255.0, 142/255.0, 35/255.0);
CL_Colord CL_Colord::orange(255/255.0, 165/255.0, 0/255.0);
CL_Colord CL_Colord::orangered(255/255.0, 69/255.0, 0/255.0);
CL_Colord CL_Colord::orchid(218/255.0, 112/255.0, 214/255.0);
CL_Colord CL_Colord::palegoldenrod(238/255.0, 232/255.0, 170/255.0);
CL_Colord CL_Colord::palegreen(152/255.0, 251/255.0, 152/255.0);
CL_Colord CL_Colord::paleturquoise(175/255.0, 238/255.0, 238/255.0);
CL_Colord CL_Colord::palevioletred(219/255.0, 112/255.0, 147/255.0);
CL_Colord CL_Colord::papayawhip(255/255.0, 239/255.0, 213/255.0);
CL_Colord CL_Colord::peachpuff(255/255.0, 218/255.0, 185/255.0);
CL_Colord CL_Colord::peru(205/255.0, 133/255.0, 63/255.0);
CL_Colord CL_Colord::pink(255/255.0, 192/255.0, 203/255.0);
CL_Colord CL_Colord::plum(221/255.0, 160/255.0, 221/255.0);
CL_Colord CL_Colord::powderblue(176/255.0, 224/255.0, 230/255.0);
CL_Colord CL_Colord::purple(128/255.0, 0/255.0, 128/255.0);
CL_Colord CL_Colord::red(255/255.0, 0/255.0, 0/255.0);
CL_Colord CL_Colord::rosybrown(188/255.0, 143/255.0, 143/255.0);
CL_Colord CL_Colord::royalblue( 65/255.0, 105/255.0, 225/255.0);
CL_Colord CL_Colord::saddlebrown(139/255.0, 69/255.0, 19/255.0);
CL_Colord CL_Colord::salmon(250/255.0, 128/255.0, 114/255.0);
CL_Colord CL_Colord::sandybrown(244/255.0, 164/255.0, 96/255.0);
CL_Colord CL_Colord::seagreen( 46/255.0, 139/255.0, 87/255.0);
CL_Colord CL_Colord::seashell(255/255.0, 245/255.0, 238/255.0);
CL_Colord CL_Colord::sienna(160/255.0, 82/255.0, 45/255.0);
CL_Colord CL_Colord::silver(192/255.0, 192/255.0, 192/255.0);
CL_Colord CL_Colord::skyblue(135/255.0, 206/255.0, 235/255.0);
CL_Colord CL_Colord::slateblue(106/255.0, 90/255.0, 205/255.0);
CL_Colord CL_Colord::slategray(112/255.0, 128/255.0, 144/255.0);
CL_Colord CL_Colord::slategrey(112/255.0, 128/255.0, 144/255.0);
CL_Colord CL_Colord::snow(255/255.0, 250/255.0, 250/255.0);
CL_Colord CL_Colord::springgreen( 0/255.0, 255/255.0, 127/255.0);
CL_Colord CL_Colord::steelblue( 70/255.0, 130/255.0, 180/255.0);
CL_Colord CL_Colord::tan(210/255.0, 180/255.0, 140/255.0);
CL_Colord CL_Colord::teal( 0/255.0, 128/255.0, 128/255.0);
CL_Colord CL_Colord::thistle(216/255.0, 191/255.0, 216/255.0);
CL_Colord CL_Colord::tomato(255/255.0, 99/255.0, 71/255.0);
CL_Colord CL_Colord::transparent(0.0,0.0,0.0,0.0);
CL_Colord CL_Colord::turquoise( 64/255.0, 224/255.0, 208/255.0);
CL_Colord CL_Colord::violet(238/255.0, 130/255.0, 238/255.0);
CL_Colord CL_Colord::wheat(245/255.0, 222/255.0, 179/255.0);
CL_Colord CL_Colord::white(255/255.0, 255/255.0, 255/255.0);
CL_Colord CL_Colord::whitesmoke(245/255.0, 245/255.0, 245/255.0);
CL_Colord CL_Colord::yellow(255/255.0, 255/255.0, 0/255.0);
CL_Colord CL_Colord::yellowgreen(154/255.0, 205/255.0, 50/255.0);

CL_Colord CL_Colord::gray10(0.1, 0.1, 0.1);
CL_Colord CL_Colord::gray20(0.2, 0.2, 0.2);
CL_Colord CL_Colord::gray30(0.3, 0.3, 0.3);
CL_Colord CL_Colord::gray40(0.4, 0.4, 0.4);
CL_Colord CL_Colord::gray50(0.5, 0.5, 0.5);
CL_Colord CL_Colord::gray60(0.6, 0.6, 0.6);
CL_Colord CL_Colord::gray70(0.7, 0.7, 0.7);
CL_Colord CL_Colord::gray80(0.8, 0.8, 0.8);
CL_Colord CL_Colord::gray90(0.9, 0.9, 0.9);

/////////////////////////////////////////////////////////////////////////////
// CL_Colorf attributes:

CL_Colorf CL_Colorf::aliceblue(40.0f/255.0f, 248.0f/255.0f, 255.0f/255.0f);
CL_Colorf CL_Colorf::antiquewhite(250.0f/255.0f, 235.0f/255.0f, 215.0f/255.0f);
CL_Colorf CL_Colorf::aqua( 0.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f);
CL_Colorf CL_Colorf::aquamarine(127.0f/255.0f, 255.0f/255.0f, 212.0f/255.0f);
CL_Colorf CL_Colorf::azure(240.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f);
CL_Colorf CL_Colorf::beige(245.0f/255.0f, 245.0f/255.0f, 220.0f/255.0f);
CL_Colorf CL_Colorf::bisque(255.0f/255.0f, 228.0f/255.0f, 196.0f/255.0f);
CL_Colorf CL_Colorf::black( 0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f );
CL_Colorf CL_Colorf::blanchedalmond(255.0f/255.0f, 235.0f/255.0f, 205.0f/255.0f);
CL_Colorf CL_Colorf::blue( 0.0f/255.0f, 0.0f/255.0f, 255.0f/255.0f);
CL_Colorf CL_Colorf::blueviolet(138.0f/255.0f, 43.0f/255.0f, 226.0f/255.0f);
CL_Colorf CL_Colorf::brown(165.0f/255.0f, 42.0f/255.0f, 42.0f/255.0f);
CL_Colorf CL_Colorf::burlywood(222.0f/255.0f, 184.0f/255.0f, 135.0f/255.0f);
CL_Colorf CL_Colorf::cadetblue( 95.0f/255.0f, 158.0f/255.0f, 160.0f/255.0f);
CL_Colorf CL_Colorf::chartreuse(127.0f/255.0f, 255.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::chocolate(210.0f/255.0f, 105.0f/255.0f, 30.0f/255.0f);
CL_Colorf CL_Colorf::coral(255.0f/255.0f, 127.0f/255.0f, 80.0f/255.0f);
CL_Colorf CL_Colorf::cornflowerblue(100.0f/255.0f, 149.0f/255.0f, 237.0f/255.0f);
CL_Colorf CL_Colorf::cornsilk(255.0f/255.0f, 248.0f/255.0f, 220.0f/255.0f);
CL_Colorf CL_Colorf::crimson(220.0f/255.0f, 20.0f/255.0f, 60.0f/255.0f);
CL_Colorf CL_Colorf::cyan( 0.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f);
CL_Colorf CL_Colorf::darkblue( 0.0f/255.0f, 0.0f/255.0f, 139.0f/255.0f);
CL_Colorf CL_Colorf::darkcyan( 0.0f/255.0f, 139.0f/255.0f, 139.0f/255.0f);
CL_Colorf CL_Colorf::darkgoldenrod(184.0f/255.0f, 134.0f/255.0f, 11.0f/255.0f);
CL_Colorf CL_Colorf::darkgray(169.0f/255.0f, 169.0f/255.0f, 169.0f/255.0f);
CL_Colorf CL_Colorf::darkgreen( 0.0f/255.0f, 100.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::darkgrey(169.0f/255.0f, 169.0f/255.0f, 169.0f/255.0f);
CL_Colorf CL_Colorf::darkkhaki(189.0f/255.0f, 183.0f/255.0f, 107.0f/255.0f);
CL_Colorf CL_Colorf::darkmagenta(139.0f/255.0f, 0.0f/255.0f, 139.0f/255.0f);
CL_Colorf CL_Colorf::darkolivegreen( 85.0f/255.0f, 107.0f/255.0f, 47.0f/255.0f);
CL_Colorf CL_Colorf::darkorange(255.0f/255.0f, 140.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::darkorchid(153.0f/255.0f, 50.0f/255.0f, 204.0f/255.0f);
CL_Colorf CL_Colorf::darkred(139.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::darksalmon(233.0f/255.0f, 150.0f/255.0f, 122.0f/255.0f);
CL_Colorf CL_Colorf::darkseagreen(143.0f/255.0f, 188.0f/255.0f, 143.0f/255.0f);
CL_Colorf CL_Colorf::darkslateblue( 72.0f/255.0f, 61.0f/255.0f, 139.0f/255.0f);
CL_Colorf CL_Colorf::darkslategray( 47.0f/255.0f, 79.0f/255.0f, 79.0f/255.0f);
CL_Colorf CL_Colorf::darkslategrey( 47.0f/255.0f, 79.0f/255.0f, 79.0f/255.0f);
CL_Colorf CL_Colorf::darkturquoise( 0.0f/255.0f, 206.0f/255.0f, 209.0f/255.0f);
CL_Colorf CL_Colorf::darkviolet(148.0f/255.0f, 0.0f/255.0f, 211.0f/255.0f);
CL_Colorf CL_Colorf::deeppink(255.0f/255.0f, 20.0f/255.0f, 147.0f/255.0f);
CL_Colorf CL_Colorf::deepskyblue( 0.0f/255.0f, 191.0f/255.0f, 255.0f/255.0f);
CL_Colorf CL_Colorf::dimgray(105.0f/255.0f, 105.0f/255.0f, 105.0f/255.0f);
CL_Colorf CL_Colorf::dimgrey(105.0f/255.0f, 105.0f/255.0f, 105.0f/255.0f);
CL_Colorf CL_Colorf::dodgerblue( 30.0f/255.0f, 144.0f/255.0f, 255.0f/255.0f);
CL_Colorf CL_Colorf::firebrick(178.0f/255.0f, 34.0f/255.0f, 34.0f/255.0f);
CL_Colorf CL_Colorf::floralwhite(255.0f/255.0f, 250.0f/255.0f, 240.0f/255.0f);
CL_Colorf CL_Colorf::forestgreen( 34.0f/255.0f, 139.0f/255.0f, 34.0f/255.0f);
CL_Colorf CL_Colorf::fuchsia(255.0f/255.0f, 0.0f/255.0f, 255.0f/255.0f);
CL_Colorf CL_Colorf::gainsboro(220.0f/255.0f, 220.0f/255.0f, 220.0f/255.0f);
CL_Colorf CL_Colorf::ghostwhite(248.0f/255.0f, 248.0f/255.0f, 255.0f/255.0f);
CL_Colorf CL_Colorf::gold(255.0f/255.0f, 215.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::goldenrod(218.0f/255.0f, 165.0f/255.0f, 32.0f/255.0f);
CL_Colorf CL_Colorf::gray(128.0f/255.0f, 128.0f/255.0f, 128.0f/255.0f);
CL_Colorf CL_Colorf::grey(128.0f/255.0f, 128.0f/255.0f, 128.0f/255.0f);
CL_Colorf CL_Colorf::green( 0.0f/255.0f, 128.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::greenyellow(173.0f/255.0f, 255.0f/255.0f, 47.0f/255.0f);
CL_Colorf CL_Colorf::honeydew(240.0f/255.0f, 255.0f/255.0f, 240.0f/255.0f);
CL_Colorf CL_Colorf::hotpink(255.0f/255.0f, 105.0f/255.0f, 180.0f/255.0f);
CL_Colorf CL_Colorf::indianred(205.0f/255.0f, 92.0f/255.0f, 92.0f/255.0f);
CL_Colorf CL_Colorf::indigo( 75.0f/255.0f, 0.0f/255.0f, 130.0f/255.0f);
CL_Colorf CL_Colorf::ivory(255.0f/255.0f, 255.0f/255.0f, 240.0f/255.0f);
CL_Colorf CL_Colorf::khaki(240.0f/255.0f, 230.0f/255.0f, 140.0f/255.0f);
CL_Colorf CL_Colorf::lavender(230.0f/255.0f, 230.0f/255.0f, 250.0f/255.0f);
CL_Colorf CL_Colorf::lavenderblush(255.0f/255.0f, 240.0f/255.0f, 245.0f/255.0f);
CL_Colorf CL_Colorf::lawngreen(124.0f/255.0f, 252.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::lemonchiffon(255.0f/255.0f, 250.0f/255.0f, 205.0f/255.0f);
CL_Colorf CL_Colorf::lightblue(173.0f/255.0f, 216.0f/255.0f, 230.0f/255.0f);
CL_Colorf CL_Colorf::lightcoral(240.0f/255.0f, 128.0f/255.0f, 128.0f/255.0f);
CL_Colorf CL_Colorf::lightcyan(224.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f);
CL_Colorf CL_Colorf::lightgoldenrodyellow(250.0f/255.0f, 250.0f/255.0f, 210.0f/255.0f);
CL_Colorf CL_Colorf::lightgray(211.0f/255.0f, 211.0f/255.0f, 211.0f/255.0f);
CL_Colorf CL_Colorf::lightgreen(144.0f/255.0f, 238.0f/255.0f, 144.0f/255.0f);
CL_Colorf CL_Colorf::lightgrey(211.0f/255.0f, 211.0f/255.0f, 211.0f/255.0f);
CL_Colorf CL_Colorf::lightpink(255.0f/255.0f, 182.0f/255.0f, 193.0f/255.0f);
CL_Colorf CL_Colorf::lightsalmon(255.0f/255.0f, 160.0f/255.0f, 122.0f/255.0f);
CL_Colorf CL_Colorf::lightseagreen( 32.0f/255.0f, 178.0f/255.0f, 170.0f/255.0f);
CL_Colorf CL_Colorf::lightskyblue(135.0f/255.0f, 206.0f/255.0f, 250.0f/255.0f);
CL_Colorf CL_Colorf::lightslategray(119.0f/255.0f, 136.0f/255.0f, 153.0f/255.0f);
CL_Colorf CL_Colorf::lightslategrey(119.0f/255.0f, 136.0f/255.0f, 153.0f/255.0f);
CL_Colorf CL_Colorf::lightsteelblue(176.0f/255.0f, 196.0f/255.0f, 222.0f/255.0f);
CL_Colorf CL_Colorf::lightyellow(255.0f/255.0f, 255.0f/255.0f, 224.0f/255.0f);
CL_Colorf CL_Colorf::lime( 0.0f/255.0f, 255.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::limegreen( 50.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f);
CL_Colorf CL_Colorf::linen(250.0f/255.0f, 240.0f/255.0f, 230.0f/255.0f);
CL_Colorf CL_Colorf::magenta(255.0f/255.0f, 0.0f/255.0f, 255.0f/255.0f);
CL_Colorf CL_Colorf::maroon(128.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::mediumaquamarine(102.0f/255.0f, 205.0f/255.0f, 170.0f/255.0f);
CL_Colorf CL_Colorf::mediumblue( 0.0f/255.0f, 0.0f/255.0f, 205.0f/255.0f);
CL_Colorf CL_Colorf::mediumorchid(186.0f/255.0f, 85.0f/255.0f, 211.0f/255.0f);
CL_Colorf CL_Colorf::mediumpurple(147.0f/255.0f, 112.0f/255.0f, 219.0f/255.0f);
CL_Colorf CL_Colorf::mediumseagreen( 60.0f/255.0f, 179.0f/255.0f, 113.0f/255.0f);
CL_Colorf CL_Colorf::mediumslateblue(123.0f/255.0f, 104.0f/255.0f, 238.0f/255.0f);
CL_Colorf CL_Colorf::mediumspringgreen( 0.0f/255.0f, 250.0f/255.0f, 154.0f/255.0f);
CL_Colorf CL_Colorf::mediumturquoise( 72.0f/255.0f, 209.0f/255.0f, 204.0f/255.0f);
CL_Colorf CL_Colorf::mediumvioletred(199.0f/255.0f, 21.0f/255.0f, 133.0f/255.0f);
CL_Colorf CL_Colorf::midnightblue( 25.0f/255.0f, 25.0f/255.0f, 112.0f/255.0f);
CL_Colorf CL_Colorf::mintcream(245.0f/255.0f, 255.0f/255.0f, 250.0f/255.0f);
CL_Colorf CL_Colorf::mistyrose(255.0f/255.0f, 228.0f/255.0f, 225.0f/255.0f);
CL_Colorf CL_Colorf::moccasin(255.0f/255.0f, 228.0f/255.0f, 181.0f/255.0f);
CL_Colorf CL_Colorf::navajowhite(255.0f/255.0f, 222.0f/255.0f, 173.0f/255.0f);
CL_Colorf CL_Colorf::navy( 0.0f/255.0f, 0.0f/255.0f, 128.0f/255.0f);
CL_Colorf CL_Colorf::oldlace(253.0f/255.0f, 245.0f/255.0f, 230.0f/255.0f);
CL_Colorf CL_Colorf::olive(128.0f/255.0f, 128.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::olivedrab(107.0f/255.0f, 142.0f/255.0f, 35.0f/255.0f);
CL_Colorf CL_Colorf::orange(255.0f/255.0f, 165.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::orangered(255.0f/255.0f, 69.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::orchid(218.0f/255.0f, 112.0f/255.0f, 214.0f/255.0f);
CL_Colorf CL_Colorf::palegoldenrod(238.0f/255.0f, 232.0f/255.0f, 170.0f/255.0f);
CL_Colorf CL_Colorf::palegreen(152.0f/255.0f, 251.0f/255.0f, 152.0f/255.0f);
CL_Colorf CL_Colorf::paleturquoise(175.0f/255.0f, 238.0f/255.0f, 238.0f/255.0f);
CL_Colorf CL_Colorf::palevioletred(219.0f/255.0f, 112.0f/255.0f, 147.0f/255.0f);
CL_Colorf CL_Colorf::papayawhip(255.0f/255.0f, 239.0f/255.0f, 213.0f/255.0f);
CL_Colorf CL_Colorf::peachpuff(255.0f/255.0f, 218.0f/255.0f, 185.0f/255.0f);
CL_Colorf CL_Colorf::peru(205.0f/255.0f, 133.0f/255.0f, 63.0f/255.0f);
CL_Colorf CL_Colorf::pink(255.0f/255.0f, 192.0f/255.0f, 203.0f/255.0f);
CL_Colorf CL_Colorf::plum(221.0f/255.0f, 160.0f/255.0f, 221.0f/255.0f);
CL_Colorf CL_Colorf::powderblue(176.0f/255.0f, 224.0f/255.0f, 230.0f/255.0f);
CL_Colorf CL_Colorf::purple(128.0f/255.0f, 0.0f/255.0f, 128.0f/255.0f);
CL_Colorf CL_Colorf::red(255.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::rosybrown(188.0f/255.0f, 143.0f/255.0f, 143.0f/255.0f);
CL_Colorf CL_Colorf::royalblue( 65.0f/255.0f, 105.0f/255.0f, 225.0f/255.0f);
CL_Colorf CL_Colorf::saddlebrown(139.0f/255.0f, 69.0f/255.0f, 19.0f/255.0f);
CL_Colorf CL_Colorf::salmon(250.0f/255.0f, 128.0f/255.0f, 114.0f/255.0f);
CL_Colorf CL_Colorf::sandybrown(244.0f/255.0f, 164.0f/255.0f, 96.0f/255.0f);
CL_Colorf CL_Colorf::seagreen( 46.0f/255.0f, 139.0f/255.0f, 87.0f/255.0f);
CL_Colorf CL_Colorf::seashell(255.0f/255.0f, 245.0f/255.0f, 238.0f/255.0f);
CL_Colorf CL_Colorf::sienna(160.0f/255.0f, 82.0f/255.0f, 45.0f/255.0f);
CL_Colorf CL_Colorf::silver(192.0f/255.0f, 192.0f/255.0f, 192.0f/255.0f);
CL_Colorf CL_Colorf::skyblue(135.0f/255.0f, 206.0f/255.0f, 235.0f/255.0f);
CL_Colorf CL_Colorf::slateblue(106.0f/255.0f, 90.0f/255.0f, 205.0f/255.0f);
CL_Colorf CL_Colorf::slategray(112.0f/255.0f, 128.0f/255.0f, 144.0f/255.0f);
CL_Colorf CL_Colorf::slategrey(112.0f/255.0f, 128.0f/255.0f, 144.0f/255.0f);
CL_Colorf CL_Colorf::snow(255.0f/255.0f, 250.0f/255.0f, 250.0f/255.0f);
CL_Colorf CL_Colorf::springgreen( 0.0f/255.0f, 255.0f/255.0f, 127.0f/255.0f);
CL_Colorf CL_Colorf::steelblue( 70.0f/255.0f, 130.0f/255.0f, 180.0f/255.0f);
CL_Colorf CL_Colorf::tan(210.0f/255.0f, 180.0f/255.0f, 140.0f/255.0f);
CL_Colorf CL_Colorf::teal( 0.0f/255.0f, 128.0f/255.0f, 128.0f/255.0f);
CL_Colorf CL_Colorf::thistle(216.0f/255.0f, 191.0f/255.0f, 216.0f/255.0f);
CL_Colorf CL_Colorf::tomato(255.0f/255.0f, 99.0f/255.0f, 71.0f/255.0f);
CL_Colorf CL_Colorf::transparent(0.0f,0.0f,0.0f,0.0f);
CL_Colorf CL_Colorf::turquoise( 64.0f/255.0f, 224.0f/255.0f, 208.0f/255.0f);
CL_Colorf CL_Colorf::violet(238.0f/255.0f, 130.0f/255.0f, 238.0f/255.0f);
CL_Colorf CL_Colorf::wheat(245.0f/255.0f, 222.0f/255.0f, 179.0f/255.0f);
CL_Colorf CL_Colorf::white(255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f);
CL_Colorf CL_Colorf::whitesmoke(245.0f/255.0f, 245.0f/255.0f, 245.0f/255.0f);
CL_Colorf CL_Colorf::yellow(255.0f/255.0f, 255.0f/255.0f, 0.0f/255.0f);
CL_Colorf CL_Colorf::yellowgreen(154.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f);

CL_Colorf CL_Colorf::gray10(0.1f, 0.1f, 0.1f);
CL_Colorf CL_Colorf::gray20(0.2f, 0.2f, 0.2f);
CL_Colorf CL_Colorf::gray30(0.3f, 0.3f, 0.3f);
CL_Colorf CL_Colorf::gray40(0.4f, 0.4f, 0.4f);
CL_Colorf CL_Colorf::gray50(0.5f, 0.5f, 0.5f);
CL_Colorf CL_Colorf::gray60(0.6f, 0.6f, 0.6f);
CL_Colorf CL_Colorf::gray70(0.7f, 0.7f, 0.7f);
CL_Colorf CL_Colorf::gray80(0.8f, 0.8f, 0.8f);
CL_Colorf CL_Colorf::gray90(0.9f, 0.9f, 0.9f);

