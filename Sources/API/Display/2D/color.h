/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Mark Page
*/

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/Text/string_types.h"
#include <vector>

class CL_PixelFormat;
class CL_Colord;
class CL_Colorf;

/// \brief Color description class.
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Color
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a color.
	///
	CL_Color() : color(0) { return; }

	explicit CL_Color(const CL_Colord&);
	explicit CL_Color(const CL_Colorf&);

	/// \brief Constructs a color.
	///
	/// Color components are specified in the range 0 to 255.\n
	/// An alpha value of 0 means complete transparency, while 255 means completely opaque (solid).
	///
	/// \param red Red color component.
	/// \param green Green color component.
	/// \param blue Blue color component.
	/// \param alpha Alpha (transparency) color component.
	CL_Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha = 255)
		: color((alpha<<24) | (red<<16) | (green<<8) | blue) { return; }

	/// \brief Constructs a color.
	///
	/// Color components are specified in the range 0 to 255.\n
	/// An alpha value of 0 means complete transparency, while 255 means completely opaque (solid).
	///
	/// \param hexstr The colors rgba components as a hexadecimal string of the format "#rrggbbaa", where the '#' and "aa" parts are optional.
	CL_Color(const CL_StringRef &hexstr);

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the alpha color component, in the range 0-255.
	unsigned int get_alpha() const { return (color >> 24) & 0xff; }

	/// \brief Returns the red color component, in the range 0-255.
	unsigned int get_red() const { return (color >> 16) & 0xff; }

	/// \brief Returns the green color component, in the range 0-255.
	unsigned int get_green() const { return (color >> 8) & 0xff; }

	/// \brief Returns the blue color component, in the range 0-255.
	unsigned int get_blue() const { return color & 0xff; }

	/// \brief Returns the alpha color component, in the range 0-1.
	double get_alpha_d() const { return float(get_alpha()) / 255.0; }

	/// \brief Returns the red color component, in the range 0-1.
	double get_red_d() const { return float(get_red()) / 255.0; }

	/// \brief Returns the green color component, in the range 0-1.
	double get_green_d() const { return float(get_green()) / 255.0; }

	/// \brief Returns the blue color component, in the range 0-1.
	double get_blue_d() const { return float(get_blue()) / 255.0; }

	/// \brief Returns the alpha color component, in the range 0-1.
	float get_alpha_f() const { return float(get_alpha()) / 255.0f; }

	/// \brief Returns the red color component, in the range 0-1.
	float get_red_f() const { return float(get_red()) / 255.0f; }

	/// \brief Returns the green color component, in the range 0-1.
	float get_green_f() const { return float(get_green()) / 255.0f; }

	/// \brief Returns the blue color component, in the range 0-1.
	float get_blue_f() const { return float(get_blue()) / 255.0f; }

	/// \brief Color in ARGB8888 format.
	unsigned int color;

// Operations:
public:
	/// \brief Color == Color operator (deep compare)
	bool operator==(const CL_Color &c) const
	{ return (color == c.color); }

	/// \brief Color != Color operator (deep compare)
	bool operator!=(const CL_Color &c) const
	{ return (color != c.color); }

/// \}
/// \name Statics
/// \{

public:
	/// \brief <img src="../images/aliceblue-chip.png" width=16 height=16 > rgb(240, 248, 255).
	static CL_Color aliceblue;

	/// \brief <img src="../images/antiquewhite-chip.png" width=16 height=16 > rgb(250, 235, 215).
	static CL_Color antiquewhite;

	/// \brief <img src="../images/aqua-chip.png" width=16 height=16 > rgb( 0, 255, 255).
	static CL_Color aqua;

	/// \brief <img src="../images/aquamarine-chip.png" width=16 height=16 > rgb(127, 255, 212).
	static CL_Color aquamarine;

	/// \brief <img src="../images/azure-chip.png" width=16 height=16 > rgb(240, 255, 255).
	static CL_Color azure;

	/// \brief <img src="../images/beige-chip.png" width=16 height=16 > rgb(245, 245, 220).
	static CL_Color beige;

	/// \brief <img src="../images/bisque-chip.png" width=16 height=16 > rgb(255, 228, 196).
	static CL_Color bisque;

	/// \brief <img src="../images/black-chip.png" width=16 height=16 > rgb( 0, 0, 0).
	static CL_Color black;

	/// \brief <img src="../images/blanchedalmond-chip.png" width=16 height=16 > rgb(255, 235, 205).
	static CL_Color blanchedalmond;

	/// \brief <img src="../images/blue-chip.png" width=16 height=16 > rgb( 0, 0, 255).
	static CL_Color blue;

	/// \brief <img src="../images/blueviolet-chip.png" width=16 height=16 > rgb(138, 43, 226).
	static CL_Color blueviolet;

	/// \brief <img src="../images/brown-chip.png" width=16 height=16 > rgb(165, 42, 42).
	static CL_Color brown;

	/// \brief <img src="../images/burlywood-chip.png" width=16 height=16 > rgb(222, 184, 135).
	static CL_Color burlywood;

	/// \brief <img src="../images/cadetblue-chip.png" width=16 height=16 > rgb( 95, 158, 160).
	static CL_Color cadetblue;

	/// \brief <img src="../images/chartreuse-chip.png" width=16 height=16 > rgb(127, 255, 0).
	static CL_Color chartreuse;

	/// \brief <img src="../images/chocolate-chip.png" width=16 height=16 > rgb(210, 105, 30).
	static CL_Color chocolate;

	/// \brief <img src="../images/coral-chip.png" width=16 height=16 > rgb(255, 127, 80).
	static CL_Color coral;

	/// \brief <img src="../images/cornflowerblue-chip.png" width=16 height=16 > rgb(100, 149, 237).
	static CL_Color cornflowerblue;

	/// \brief <img src="../images/cornsilk-chip.png" width=16 height=16 > rgb(255, 248, 220).
	static CL_Color cornsilk;

	/// \brief <img src="../images/crimson-chip.png" width=16 height=16 > rgb(220, 20, 60).
	static CL_Color crimson;

	/// \brief <img src="../images/cyan-chip.png" width=16 height=16 > rgb( 0, 255, 255).
	static CL_Color cyan;

	/// \brief <img src="../images/darkblue-chip.png" width=16 height=16 > rgb( 0, 0, 139).
	static CL_Color darkblue;

	/// \brief <img src="../images/darkcyan-chip.png" width=16 height=16 > rgb( 0, 139, 139).
	static CL_Color darkcyan;

	/// \brief <img src="../images/darkgoldenrod-chip.png" width=16 height=16 > rgb(184, 134, 11).
	static CL_Color darkgoldenrod;

	/// \brief <img src="../images/darkgray-chip.png" width=16 height=16 > rgb(169, 169, 169).
	static CL_Color darkgray;

	/// \brief <img src="../images/darkgreen-chip.png" width=16 height=16 > rgb( 0, 100, 0).
	static CL_Color darkgreen;

	/// \brief <img src="../images/darkgrey-chip.png" width=16 height=16 > rgb(169, 169, 169).
	static CL_Color darkgrey;

	/// \brief <img src="../images/darkkhaki-chip.png" width=16 height=16 > rgb(189, 183, 107).
	static CL_Color darkkhaki;

	/// \brief <img src="../images/darkmagenta-chip.png" width=16 height=16 > rgb(139, 0, 139).
	static CL_Color darkmagenta;

	/// \brief <img src="../images/darkolivegreen-chip.png" width=16 height=16 > rgb( 85, 107, 47).
	static CL_Color darkolivegreen;

	/// \brief <img src="../images/darkorange-chip.png" width=16 height=16 > rgb(255, 140, 0).
	static CL_Color darkorange;

	/// \brief <img src="../images/darkorchid-chip.png" width=16 height=16 > rgb(153, 50, 204).
	static CL_Color darkorchid;

	/// \brief <img src="../images/darkred-chip.png" width=16 height=16 > rgb(139, 0, 0).
	static CL_Color darkred;

	/// \brief <img src="../images/darksalmon-chip.png" width=16 height=16 > rgb(233, 150, 122).
	static CL_Color darksalmon;

	/// \brief <img src="../images/darkseagreen-chip.png" width=16 height=16 > rgb(143, 188, 143).
	static CL_Color darkseagreen;

	/// \brief <img src="../images/darkslateblue-chip.png" width=16 height=16 > rgb( 72, 61, 139).
	static CL_Color darkslateblue;

	/// \brief <img src="../images/darkslategray-chip.png" width=16 height=16 > rgb( 47, 79, 79).
	static CL_Color darkslategray;

	/// \brief <img src="../images/darkslategrey-chip.png" width=16 height=16 > rgb( 47, 79, 79).
	static CL_Color darkslategrey;

	/// \brief <img src="../images/darkturquoise-chip.png" width=16 height=16 > rgb( 0, 206, 209).
	static CL_Color darkturquoise;

	/// \brief <img src="../images/darkviolet-chip.png" width=16 height=16 > rgb(148, 0, 211).
	static CL_Color darkviolet;

	/// \brief <img src="../images/deeppink-chip.png" width=16 height=16 > rgb(255, 20, 147).
	static CL_Color deeppink;

	/// \brief <img src="../images/deepskyblue-chip.png" width=16 height=16 > rgb( 0, 191, 255).
	static CL_Color deepskyblue;

	/// \brief <img src="../images/dimgray-chip.png" width=16 height=16 > rgb(105, 105, 105).
	static CL_Color dimgray;

	/// \brief <img src="../images/dimgrey-chip.png" width=16 height=16 > rgb(105, 105, 105).
	static CL_Color dimgrey;

	/// \brief <img src="../images/dodgerblue-chip.png" width=16 height=16 > rgb( 30, 144, 255).
	static CL_Color dodgerblue;

	/// \brief <img src="../images/firebrick-chip.png" width=16 height=16 > rgb(178, 34, 34).
	static CL_Color firebrick;

	/// \brief <img src="../images/floralwhite-chip.png" width=16 height=16 > rgb(255, 250, 240).
	static CL_Color floralwhite;

	/// \brief <img src="../images/forestgreen-chip.png" width=16 height=16 > rgb( 34, 139, 34).
	static CL_Color forestgreen;

	/// \brief <img src="../images/fuchsia-chip.png" width=16 height=16 > rgb(255, 0, 255).
	static CL_Color fuchsia;

	/// \brief <img src="../images/gainsboro-chip.png" width=16 height=16 > rgb(220, 220, 220).
	static CL_Color gainsboro;

	/// \brief <img src="../images/ghostwhite-chip.png" width=16 height=16 > rgb(248, 248, 255).
	static CL_Color ghostwhite;

	/// \brief <img src="../images/gold-chip.png" width=16 height=16 > rgb(255, 215, 0).
	static CL_Color gold;

	/// \brief <img src="../images/goldenrod-chip.png" width=16 height=16 > rgb(218, 165, 32).
	static CL_Color goldenrod;

	/// \brief <img src="../images/gray-chip.png" width=16 height=16 > rgb(128, 128, 128).
	static CL_Color gray;

	/// \brief <img src="../images/grey-chip.png" width=16 height=16 > rgb(128, 128, 128).
	static CL_Color grey;

	/// \brief <img src="../images/green-chip.png" width=16 height=16 > rgb( 0, 128, 0).
	static CL_Color green;

	/// \brief <img src="../images/greenyellow-chip.png" width=16 height=16 > rgb(173, 255, 47).
	static CL_Color greenyellow;

	/// \brief <img src="../images/honeydew-chip.png" width=16 height=16 > rgb(240, 255, 240).
	static CL_Color honeydew;

	/// \brief <img src="../images/hotpink-chip.png" width=16 height=16 > rgb(255, 105, 180).
	static CL_Color hotpink;

	/// \brief <img src="../images/indianred-chip.png" width=16 height=16 > rgb(205, 92, 92).
	static CL_Color indianred;

	/// \brief <img src="../images/indigo-chip.png" width=16 height=16 > rgb( 75, 0, 130).
	static CL_Color indigo;

	/// \brief <img src="../images/ivory-chip.png" width=16 height=16 > rgb(255, 255, 240).
	static CL_Color ivory;

	/// \brief <img src="../images/khaki-chip.png" width=16 height=16 > rgb(240, 230, 140).
	static CL_Color khaki;

	/// \brief <img src="../images/lavender-chip.png" width=16 height=16 > rgb(230, 230, 250).
	static CL_Color lavender;

	/// \brief <img src="../images/lavenderblush-chip.png" width=16 height=16 > rgb(255, 240, 245).
	static CL_Color lavenderblush;

	/// \brief <img src="../images/lawngreen-chip.png" width=16 height=16 > rgb(124, 252, 0).
	static CL_Color lawngreen;

	/// \brief <img src="../images/lemonchiffon-chip.png" width=16 height=16 > rgb(255, 250, 205).
	static CL_Color lemonchiffon;

	/// \brief <img src="../images/lightblue-chip.png" width=16 height=16 > rgb(173, 216, 230).
	static CL_Color lightblue;

	/// \brief <img src="../images/lightcoral-chip.png" width=16 height=16 > rgb(240, 128, 128).
	static CL_Color lightcoral;

	/// \brief <img src="../images/lightcyan-chip.png" width=16 height=16 > rgb(224, 255, 255).
	static CL_Color lightcyan;

	/// \brief <img src="../images/lightgoldenrodyellow-chip.png" width=16 height=16 > rgb(250, 250, 210).
	static CL_Color lightgoldenrodyellow;

	/// \brief <img src="../images/lightgray-chip.png" width=16 height=16 > rgb(211, 211, 211).
	static CL_Color lightgray;

	/// \brief <img src="../images/lightgreen-chip.png" width=16 height=16 > rgb(144, 238, 144).
	static CL_Color lightgreen;

	/// \brief <img src="../images/lightgrey-chip.png" width=16 height=16 > rgb(211, 211, 211).
	static CL_Color lightgrey;

	/// \brief <img src="../images/lightpink-chip.png" width=16 height=16 > rgb(255, 182, 193).
	static CL_Color lightpink;

	/// \brief <img src="../images/lightsalmon-chip.png" width=16 height=16 > rgb(255, 160, 122).
	static CL_Color lightsalmon;

	/// \brief <img src="../images/lightseagreen-chip.png" width=16 height=16 > rgb( 32, 178, 170).
	static CL_Color lightseagreen;

	/// \brief <img src="../images/lightskyblue-chip.png" width=16 height=16 > rgb(135, 206, 250).
	static CL_Color lightskyblue;

	/// \brief <img src="../images/lightslategray-chip.png" width=16 height=16 > rgb(119, 136, 153).
	static CL_Color lightslategray;

	/// \brief <img src="../images/lightslategrey-chip.png" width=16 height=16 > rgb(119, 136, 153).
	static CL_Color lightslategrey;

	/// \brief <img src="../images/lightsteelblue-chip.png" width=16 height=16 > rgb(176, 196, 222).
	static CL_Color lightsteelblue;

	/// \brief <img src="../images/lightyellow-chip.png" width=16 height=16 > rgb(255, 255, 224).
	static CL_Color lightyellow;

	/// \brief <img src="../images/lime-chip.png" width=16 height=16 > rgb( 0, 255, 0).
	static CL_Color lime;

	/// \brief <img src="../images/limegreen-chip.png" width=16 height=16 > rgb( 50, 205, 50).
	static CL_Color limegreen;

	/// \brief <img src="../images/linen-chip.png" width=16 height=16 > rgb(250, 240, 230).
	static CL_Color linen;

	/// \brief <img src="../images/magenta-chip.png" width=16 height=16 > rgb(255, 0, 255).
	static CL_Color magenta;

	/// \brief <img src="../images/maroon-chip.png" width=16 height=16 > rgb(128, 0, 0).
	static CL_Color maroon;

	/// \brief <img src="../images/mediumaquamarine-chip.png" width=16 height=16 > rgb(102, 205, 170).
	static CL_Color mediumaquamarine;

	/// \brief <img src="../images/mediumblue-chip.png" width=16 height=16 > rgb( 0, 0, 205).
	static CL_Color mediumblue;

	/// \brief <img src="../images/mediumorchid-chip.png" width=16 height=16 > rgb(186, 85, 211).
	static CL_Color mediumorchid;

	/// \brief <img src="../images/mediumpurple-chip.png" width=16 height=16 > rgb(147, 112, 219).
	static CL_Color mediumpurple;

	/// \brief <img src="../images/mediumseagreen-chip.png" width=16 height=16 > rgb( 60, 179, 113).
	static CL_Color mediumseagreen;

	/// \brief <img src="../images/mediumslateblue-chip.png" width=16 height=16 > rgb(123, 104, 238).
	static CL_Color mediumslateblue;

	/// \brief <img src="../images/mediumspringgreen-chip.png" width=16 height=16 > rgb( 0, 250, 154).
	static CL_Color mediumspringgreen;

	/// \brief <img src="../images/mediumturquoise-chip.png" width=16 height=16 > rgb( 72, 209, 204).
	static CL_Color mediumturquoise;

	/// \brief <img src="../images/mediumvioletred-chip.png" width=16 height=16 > rgb(199, 21, 133).
	static CL_Color mediumvioletred;

	/// \brief <img src="../images/midnightblue-chip.png" width=16 height=16 > rgb( 25, 25, 112).
	static CL_Color midnightblue;

	/// \brief <img src="../images/mintcream-chip.png" width=16 height=16 > rgb(245, 255, 250).
	static CL_Color mintcream;

	/// \brief <img src="../images/mistyrose-chip.png" width=16 height=16 > rgb(255, 228, 225).
	static CL_Color mistyrose;

	/// \brief <img src="../images/moccasin-chip.png" width=16 height=16 > rgb(255, 228, 181).
	static CL_Color moccasin;

	/// \brief <img src="../images/navajowhite-chip.png" width=16 height=16 > rgb(255, 222, 173).
	static CL_Color navajowhite;

	/// \brief <img src="../images/navy-chip.png" width=16 height=16 > rgb( 0, 0, 128).
	static CL_Color navy;

	/// \brief <img src="../images/oldlace-chip.png" width=16 height=16 > rgb(253, 245, 230).
	static CL_Color oldlace;

	/// \brief <img src="../images/olive-chip.png" width=16 height=16 > rgb(128, 128, 0).
	static CL_Color olive;

	/// \brief <img src="../images/olivedrab-chip.png" width=16 height=16 > rgb(107, 142, 35).
	static CL_Color olivedrab;

	/// \brief <img src="../images/orange-chip.png" width=16 height=16 > rgb(255, 165, 0).
	static CL_Color orange;

	/// \brief <img src="../images/orangered-chip.png" width=16 height=16 > rgb(255, 69, 0).
	static CL_Color orangered;

	/// \brief <img src="../images/orchid-chip.png" width=16 height=16 > rgb(218, 112, 214).
	static CL_Color orchid;

	/// \brief <img src="../images/palegoldenrod-chip.png" width=16 height=16 > rgb(238, 232, 170).
	static CL_Color palegoldenrod;

	/// \brief <img src="../images/palegreen-chip.png" width=16 height=16 > rgb(152, 251, 152).
	static CL_Color palegreen;

	/// \brief <img src="../images/paleturquoise-chip.png" width=16 height=16 > rgb(175, 238, 238).
	static CL_Color paleturquoise;

	/// \brief <img src="../images/palevioletred-chip.png" width=16 height=16 > rgb(219, 112, 147).
	static CL_Color palevioletred;

	/// \brief <img src="../images/papayawhip-chip.png" width=16 height=16 > rgb(255, 239, 213).
	static CL_Color papayawhip;

	/// \brief <img src="../images/peachpuff-chip.png" width=16 height=16 > rgb(255, 218, 185).
	static CL_Color peachpuff;

	/// \brief <img src="../images/peru-chip.png" width=16 height=16 > rgb(205, 133, 63).
	static CL_Color peru;

	/// \brief <img src="../images/pink-chip.png" width=16 height=16 > rgb(255, 192, 203).
	static CL_Color pink;

	/// \brief <img src="../images/plum-chip.png" width=16 height=16 > rgb(221, 160, 221).
	static CL_Color plum;

	/// \brief <img src="../images/powderblue-chip.png" width=16 height=16 > rgb(176, 224, 230).
	static CL_Color powderblue;

	/// \brief <img src="../images/purple-chip.png" width=16 height=16 > rgb(128, 0, 128).
	static CL_Color purple;

	/// \brief <img src="../images/red-chip.png" width=16 height=16 > rgb(255, 0, 0).
	static CL_Color red;

	/// \brief <img src="../images/rosybrown-chip.png" width=16 height=16 > rgb(188, 143, 143).
	static CL_Color rosybrown;

	/// \brief <img src="../images/royalblue-chip.png" width=16 height=16 > rgb( 65, 105, 225).
	static CL_Color royalblue;

	/// \brief <img src="../images/saddlebrown-chip.png" width=16 height=16 > rgb(139, 69, 19).
	static CL_Color saddlebrown;

	/// \brief <img src="../images/salmon-chip.png" width=16 height=16 > rgb(250, 128, 114).
	static CL_Color salmon;

	/// \brief <img src="../images/sandybrown-chip.png" width=16 height=16 > rgb(244, 164, 96).
	static CL_Color sandybrown;

	/// \brief <img src="../images/seagreen-chip.png" width=16 height=16 > rgb( 46, 139, 87).
	static CL_Color seagreen;

	/// \brief <img src="../images/seashell-chip.png" width=16 height=16 > rgb(255, 245, 238).
	static CL_Color seashell;

	/// \brief <img src="../images/sienna-chip.png" width=16 height=16 > rgb(160, 82, 45).
	static CL_Color sienna;

	/// \brief <img src="../images/silver-chip.png" width=16 height=16 > rgb(192, 192, 192).
	static CL_Color silver;

	/// \brief <img src="../images/skyblue-chip.png" width=16 height=16 > rgb(135, 206, 235).
	static CL_Color skyblue;

	/// \brief <img src="../images/slateblue-chip.png" width=16 height=16 > rgb(106, 90, 205).
	static CL_Color slateblue;

	/// \brief <img src="../images/slategray-chip.png" width=16 height=16 > rgb(112, 128, 144).
	static CL_Color slategray;

	/// \brief <img src="../images/slategrey-chip.png" width=16 height=16 > rgb(112, 128, 144).
	static CL_Color slategrey;

	/// \brief <img src="../images/snow-chip.png" width=16 height=16 > rgb(255, 250, 250).
	static CL_Color snow;

	/// \brief <img src="../images/springgreen-chip.png" width=16 height=16 > rgb( 0, 255, 127).
	static CL_Color springgreen;

	/// \brief <img src="../images/steelblue-chip.png" width=16 height=16 > rgb( 70, 130, 180).
	static CL_Color steelblue;

	/// \brief <img src="../images/tan-chip.png" width=16 height=16 > rgb(210, 180, 140).
	static CL_Color tan;

	/// \brief <img src="../images/teal-chip.png" width=16 height=16 > rgb( 0, 128, 128).
	static CL_Color teal;

	/// \brief <img src="../images/thistle-chip.png" width=16 height=16 > rgb(216, 191, 216).
	static CL_Color thistle;

	/// \brief <img src="../images/tomato-chip.png" width=16 height=16 > rgb(255, 99, 71).
	static CL_Color tomato;

	/// \brief <img src="../images/turquoise-chip.png" width=16 height=16 > rgb( 64, 224, 208).
	static CL_Color turquoise;

	/// \brief <img src="../images/violet-chip.png" width=16 height=16 > rgb(238, 130, 238).
	static CL_Color violet;

	/// \brief <img src="../images/wheat-chip.png" width=16 height=16 > rgb(245, 222, 179).
	static CL_Color wheat;

	/// \brief <img src="../images/white-chip.png" width=16 height=16 > rgb(255, 255, 255).
	static CL_Color white;

	/// \brief <img src="../images/whitesmoke-chip.png" width=16 height=16 > rgb(245, 245, 245).
	static CL_Color whitesmoke;

	/// \brief <img src="../images/yellow-chip.png" width=16 height=16 > rgb(255, 255, 0).
	static CL_Color yellow;

	/// \brief <img src="../images/yellowgreen-chip.png" width=16 height=16 > rgb(154, 205, 50).
	static CL_Color yellowgreen;

	/// \brief rgba(0, 0, 0, 0).
	static CL_Color transparent;

	static CL_Color gray10;
	static CL_Color gray20;
	static CL_Color gray30;
	static CL_Color gray40;
	static CL_Color gray50;
	static CL_Color gray60;
	static CL_Color gray70;
	static CL_Color gray80;
	static CL_Color gray90;

	/// \brief Find and returns the static color matching a string.
	///
	/// \param name Name of color to match, eg. "blue".
	///
	/// \return Reference to matching static color, or transparent (rgba(0,0,0,0)) if there was no match.
	static CL_Color find_color(const CL_StringRef &name);

	/// \brief Returns the names of static colors defined.
	static std::vector<CL_String8> &get_names();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Set alpha color component, in the range 0-255.
	void set_alpha(unsigned int value) { color = (color & 0x00ffffff) | (value << 24); }

	/// \brief Set red color component, in the range 0-255.
	void set_red(unsigned int value) { color = (color & 0xff00ffff) | (value << 16); }

	/// \brief Set green color component, in the range 0-255.
	void set_green(unsigned int value) { color = (color & 0xffff00ff) | (value << 8); }

	/// \brief Set blue color component, in the range 0-255.
	void set_blue(unsigned int value) { color = (color & 0xffffff00) | value; }

	/// \brief Set alpha color component, in the range 0-1.
	void set_alpha_d(float value) { set_alpha((unsigned int) (value*255)); }

	/// \brief Set red color component, in the range 0-1.
	void set_red_d(float value) { set_red((unsigned int) (value*255)); }

	/// \brief Set green color component, in the range 0-1.
	void set_green_d(float value) { set_green((unsigned int) (value*255)); }

	/// \brief Set blue color component, in the range 0-1.
	void set_blue_d(float value) { set_blue((unsigned int) (value*255)); }

	/// \brief Set color based on rgba color components in the range 0-255.
	void set_color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha = 255)
	{ color = (alpha<<24) | (red<<16) | (green<<8) | blue; }

	/// \brief Set color based on rgba color components in the range 0-1.
	void set_color_d(float red, float green, float blue, float alpha = 1.0)
	{
		color =
			(((unsigned int) (alpha*255))<<24) |
			(((unsigned int) (red*255))<<16) |
			(((unsigned int) (green*255))<<8) |
			((unsigned int) (blue*255));
	}

	/// \brief Converts the color to a given pixel format.
	//unsigned int to_pixelformat(const CL_PixelFormat &pf) const;

	/// \brief Converts a raw pixel in a given pixel format to a color.
	//static CL_Color from_pixelformat(unsigned int raw_color, const CL_PixelFormat &pf);
/// \}
};

/// \brief Floating point color description class (for double).
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class CL_Colord
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a color.
	CL_Colord() : r(0.0), g(0.0), b(0.0), a(0.0)
	{
	}

	/// \brief Constructs a color.
	///
	/// Color components are specified in the range 0 to 1.\n
	/// An alpha value of 0 means complete transparency, while 1 means completely opaque (solid).
	///
	/// \param r = Red color component.
	/// \param g = Green color component.
	/// \param b =  Blue color component.
	/// \param a = Alpha (transparency) color component.
	CL_Colord(double r, double g, double b, double a = 1.0)
		: r(r), g(g), b(b), a(a)
	{
	}

	/// \brief Constructs a color.
	///
	/// Color components are specified in the range 0 to 1.\n
	/// An alpha value of 0 means complete transparency, while 1 means completely opaque (solid).
	///
	/// \param array_rgba = Red,Green,Blue,Alpha color component.
	CL_Colord(const double *array_rgba)
		: r(array_rgba[0]), g(array_rgba[1]), b(array_rgba[2]), a(array_rgba[3])
	{
	}

	/// \brief Constructs a color.
	///
	/// Color components are specified in the range 0 to 255.\n
	/// An alpha value of 0 means complete transparency, while 255 means completely opaque (solid).
	///
	/// \param r = Red color component.
	/// \param g = Green color component.
	/// \param b = Blue color component.
	/// \param a = Alpha (transparency) color component.
	CL_Colord(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255)
		: r(r/255.0),
		  g(g/255.0),
		  b(b/255.0),
		  a(a/255.0)
	{
	}

	explicit CL_Colord(const CL_Color& color)
		: r(color.get_red()/255.0),
		  g(color.get_green()/255.0),
		  b(color.get_blue()/255.0),
		  a(color.get_alpha()/255.0)
	{
	}

	/// \brief Constructs a color.
	///
	/// Color components are specified in the range 0 to 255.\n
	/// An alpha value of 0 means complete transparency, while 255 means completely opaque (solid).
	///
	/// \param hexstr The colors rgba components as a hexadecimal string of the format "#rrggbbaa", where the '#' and "aa" parts are optional.
	CL_Colord(const CL_StringRef &hexstr)
	{
		*this = (CL_Colord)CL_Color::find_color(hexstr);
	}

/// \}
/// \name Attributes
/// \{

public:
	double r, g, b, a;

	/// \brief Get Red
	///
	/// \return red
	double get_red() const { return r; }

	/// \brief Get Green
	///
	/// \return green
	double get_green() const { return g; }

	/// \brief Get Blue
	///
	/// \return blue
	double get_blue() const { return b; }

	/// \brief Get Alpha
	///
	/// \return alpha
	double get_alpha() const { return a; }

/// \}
/// \name Operations
/// \{

public:
	/// \brief Normalize the color by ensuring that all color values lie inbetween (0.0, 1.0)
	void normalize()
	{
		r = (r < 0.0) ? 0.0 : ((r > 1.0) ? 1.0 : r);
		g = (g < 0.0) ? 0.0 : ((g > 1.0) ? 1.0 : g);
		b = (b < 0.0) ? 0.0 : ((b > 1.0) ? 1.0 : b);
		a = (a < 0.0) ? 0.0 : ((a > 1.0) ? 1.0 : a);
	}

	/// \brief Set alpha color component, in the range 0-1.
	void set_alpha(double value) { a = value; }

	/// \brief Set red color component, in the range 0-1.
	void set_red(double value) {  r = value; }

	/// \brief Set green color component, in the range 0-1.
	void set_green(double value) { g= value; }

	/// \brief Set blue color component, in the range 0-1.
	void set_blue(double value) { b = value; }

	/// \brief Color == Color operator (deep compare)
	bool operator==(const CL_Colord &c) const
	{
		return (r == c.r) && (g == c.g) && (b == c.b) && (a == c.a);
	}

	/// \brief Color != Color operator (deep compare)
	bool operator!=(const CL_Colord &c) const
	{
		return (r != c.r) || (g != c.g) || (b != c.b) || (a != c.a);
	}

	/// \brief Type conversion operator.
	operator CL_Color() const
	{
		return CL_Color(*this);
	}

/// \}
/// \name Statics
/// \{

public:
	/// \brief <img src="../images/aliceblue-chip.png" width=16 height=16 > rgb(240, 248, 255).
	static CL_Colord aliceblue;

	/// \brief <img src="../images/antiquewhite-chip.png" width=16 height=16 > rgb(250, 235, 215).
	static CL_Colord antiquewhite;

	/// \brief <img src="../images/aqua-chip.png" width=16 height=16 > rgb( 0, 255, 255).
	static CL_Colord aqua;

	/// \brief <img src="../images/aquamarine-chip.png" width=16 height=16 > rgb(127, 255, 212).
	static CL_Colord aquamarine;

	/// \brief <img src="../images/azure-chip.png" width=16 height=16 > rgb(240, 255, 255).
	static CL_Colord azure;

	/// \brief <img src="../images/beige-chip.png" width=16 height=16 > rgb(245, 245, 220).
	static CL_Colord beige;

	/// \brief <img src="../images/bisque-chip.png" width=16 height=16 > rgb(255, 228, 196).
	static CL_Colord bisque;

	/// \brief <img src="../images/black-chip.png" width=16 height=16 > rgb( 0, 0, 0).
	static CL_Colord black;

	/// \brief <img src="../images/blanchedalmond-chip.png" width=16 height=16 > rgb(255, 235, 205).
	static CL_Colord blanchedalmond;

	/// \brief <img src="../images/blue-chip.png" width=16 height=16 > rgb( 0, 0, 255).
	static CL_Colord blue;

	/// \brief <img src="../images/blueviolet-chip.png" width=16 height=16 > rgb(138, 43, 226).
	static CL_Colord blueviolet;

	/// \brief <img src="../images/brown-chip.png" width=16 height=16 > rgb(165, 42, 42).
	static CL_Colord brown;

	/// \brief <img src="../images/burlywood-chip.png" width=16 height=16 > rgb(222, 184, 135).
	static CL_Colord burlywood;

	/// \brief <img src="../images/cadetblue-chip.png" width=16 height=16 > rgb( 95, 158, 160).
	static CL_Colord cadetblue;

	/// \brief <img src="../images/chartreuse-chip.png" width=16 height=16 > rgb(127, 255, 0).
	static CL_Colord chartreuse;

	/// \brief <img src="../images/chocolate-chip.png" width=16 height=16 > rgb(210, 105, 30).
	static CL_Colord chocolate;

	/// \brief <img src="../images/coral-chip.png" width=16 height=16 > rgb(255, 127, 80).
	static CL_Colord coral;

	/// \brief <img src="../images/cornflowerblue-chip.png" width=16 height=16 > rgb(100, 149, 237).
	static CL_Colord cornflowerblue;

	/// \brief <img src="../images/cornsilk-chip.png" width=16 height=16 > rgb(255, 248, 220).
	static CL_Colord cornsilk;

	/// \brief <img src="../images/crimson-chip.png" width=16 height=16 > rgb(220, 20, 60).
	static CL_Colord crimson;

	/// \brief <img src="../images/cyan-chip.png" width=16 height=16 > rgb( 0, 255, 255).
	static CL_Colord cyan;

	/// \brief <img src="../images/darkblue-chip.png" width=16 height=16 > rgb( 0, 0, 139).
	static CL_Colord darkblue;

	/// \brief <img src="../images/darkcyan-chip.png" width=16 height=16 > rgb( 0, 139, 139).
	static CL_Colord darkcyan;

	/// \brief <img src="../images/darkgoldenrod-chip.png" width=16 height=16 > rgb(184, 134, 11).
	static CL_Colord darkgoldenrod;

	/// \brief <img src="../images/darkgray-chip.png" width=16 height=16 > rgb(169, 169, 169).
	static CL_Colord darkgray;

	/// \brief <img src="../images/darkgreen-chip.png" width=16 height=16 > rgb( 0, 100, 0).
	static CL_Colord darkgreen;

	/// \brief <img src="../images/darkgrey-chip.png" width=16 height=16 > rgb(169, 169, 169).
	static CL_Colord darkgrey;

	/// \brief <img src="../images/darkkhaki-chip.png" width=16 height=16 > rgb(189, 183, 107).
	static CL_Colord darkkhaki;

	/// \brief <img src="../images/darkmagenta-chip.png" width=16 height=16 > rgb(139, 0, 139).
	static CL_Colord darkmagenta;

	/// \brief <img src="../images/darkolivegreen-chip.png" width=16 height=16 > rgb( 85, 107, 47).
	static CL_Colord darkolivegreen;

	/// \brief <img src="../images/darkorange-chip.png" width=16 height=16 > rgb(255, 140, 0).
	static CL_Colord darkorange;

	/// \brief <img src="../images/darkorchid-chip.png" width=16 height=16 > rgb(153, 50, 204).
	static CL_Colord darkorchid;

	/// \brief <img src="../images/darkred-chip.png" width=16 height=16 > rgb(139, 0, 0).
	static CL_Colord darkred;

	/// \brief <img src="../images/darksalmon-chip.png" width=16 height=16 > rgb(233, 150, 122).
	static CL_Colord darksalmon;

	/// \brief <img src="../images/darkseagreen-chip.png" width=16 height=16 > rgb(143, 188, 143).
	static CL_Colord darkseagreen;

	/// \brief <img src="../images/darkslateblue-chip.png" width=16 height=16 > rgb( 72, 61, 139).
	static CL_Colord darkslateblue;

	/// \brief <img src="../images/darkslategray-chip.png" width=16 height=16 > rgb( 47, 79, 79).
	static CL_Colord darkslategray;

	/// \brief <img src="../images/darkslategrey-chip.png" width=16 height=16 > rgb( 47, 79, 79).
	static CL_Colord darkslategrey;

	/// \brief <img src="../images/darkturquoise-chip.png" width=16 height=16 > rgb( 0, 206, 209).
	static CL_Colord darkturquoise;

	/// \brief <img src="../images/darkviolet-chip.png" width=16 height=16 > rgb(148, 0, 211).
	static CL_Colord darkviolet;

	/// \brief <img src="../images/deeppink-chip.png" width=16 height=16 > rgb(255, 20, 147).
	static CL_Colord deeppink;

	/// \brief <img src="../images/deepskyblue-chip.png" width=16 height=16 > rgb( 0, 191, 255).
	static CL_Colord deepskyblue;

	/// \brief <img src="../images/dimgray-chip.png" width=16 height=16 > rgb(105, 105, 105).
	static CL_Colord dimgray;

	/// \brief <img src="../images/dimgrey-chip.png" width=16 height=16 > rgb(105, 105, 105).
	static CL_Colord dimgrey;

	/// \brief <img src="../images/dodgerblue-chip.png" width=16 height=16 > rgb( 30, 144, 255).
	static CL_Colord dodgerblue;

	/// \brief <img src="../images/firebrick-chip.png" width=16 height=16 > rgb(178, 34, 34).
	static CL_Colord firebrick;

	/// \brief <img src="../images/floralwhite-chip.png" width=16 height=16 > rgb(255, 250, 240).
	static CL_Colord floralwhite;

	/// \brief <img src="../images/forestgreen-chip.png" width=16 height=16 > rgb( 34, 139, 34).
	static CL_Colord forestgreen;

	/// \brief <img src="../images/fuchsia-chip.png" width=16 height=16 > rgb(255, 0, 255).
	static CL_Colord fuchsia;

	/// \brief <img src="../images/gainsboro-chip.png" width=16 height=16 > rgb(220, 220, 220).
	static CL_Colord gainsboro;

	/// \brief <img src="../images/ghostwhite-chip.png" width=16 height=16 > rgb(248, 248, 255).
	static CL_Colord ghostwhite;

	/// \brief <img src="../images/gold-chip.png" width=16 height=16 > rgb(255, 215, 0).
	static CL_Colord gold;

	/// \brief <img src="../images/goldenrod-chip.png" width=16 height=16 > rgb(218, 165, 32).
	static CL_Colord goldenrod;

	/// \brief <img src="../images/gray-chip.png" width=16 height=16 > rgb(128, 128, 128).
	static CL_Colord gray;

	/// \brief <img src="../images/grey-chip.png" width=16 height=16 > rgb(128, 128, 128).
	static CL_Colord grey;

	/// \brief <img src="../images/green-chip.png" width=16 height=16 > rgb( 0, 128, 0).
	static CL_Colord green;

	/// \brief <img src="../images/greenyellow-chip.png" width=16 height=16 > rgb(173, 255, 47).
	static CL_Colord greenyellow;

	/// \brief <img src="../images/honeydew-chip.png" width=16 height=16 > rgb(240, 255, 240).
	static CL_Colord honeydew;

	/// \brief <img src="../images/hotpink-chip.png" width=16 height=16 > rgb(255, 105, 180).
	static CL_Colord hotpink;

	/// \brief <img src="../images/indianred-chip.png" width=16 height=16 > rgb(205, 92, 92).
	static CL_Colord indianred;

	/// \brief <img src="../images/indigo-chip.png" width=16 height=16 > rgb( 75, 0, 130).
	static CL_Colord indigo;

	/// \brief <img src="../images/ivory-chip.png" width=16 height=16 > rgb(255, 255, 240).
	static CL_Colord ivory;

	/// \brief <img src="../images/khaki-chip.png" width=16 height=16 > rgb(240, 230, 140).
	static CL_Colord khaki;

	/// \brief <img src="../images/lavender-chip.png" width=16 height=16 > rgb(230, 230, 250).
	static CL_Colord lavender;

	/// \brief <img src="../images/lavenderblush-chip.png" width=16 height=16 > rgb(255, 240, 245).
	static CL_Colord lavenderblush;

	/// \brief <img src="../images/lawngreen-chip.png" width=16 height=16 > rgb(124, 252, 0).
	static CL_Colord lawngreen;

	/// \brief <img src="../images/lemonchiffon-chip.png" width=16 height=16 > rgb(255, 250, 205).
	static CL_Colord lemonchiffon;

	/// \brief <img src="../images/lightblue-chip.png" width=16 height=16 > rgb(173, 216, 230).
	static CL_Colord lightblue;

	/// \brief <img src="../images/lightcoral-chip.png" width=16 height=16 > rgb(240, 128, 128).
	static CL_Colord lightcoral;

	/// \brief <img src="../images/lightcyan-chip.png" width=16 height=16 > rgb(224, 255, 255).
	static CL_Colord lightcyan;

	/// \brief <img src="../images/lightgoldenrodyellow-chip.png" width=16 height=16 > rgb(250, 250, 210).
	static CL_Colord lightgoldenrodyellow;

	/// \brief <img src="../images/lightgray-chip.png" width=16 height=16 > rgb(211, 211, 211).
	static CL_Colord lightgray;

	/// \brief <img src="../images/lightgreen-chip.png" width=16 height=16 > rgb(144, 238, 144).
	static CL_Colord lightgreen;

	/// \brief <img src="../images/lightgrey-chip.png" width=16 height=16 > rgb(211, 211, 211).
	static CL_Colord lightgrey;

	/// \brief <img src="../images/lightpink-chip.png" width=16 height=16 > rgb(255, 182, 193).
	static CL_Colord lightpink;

	/// \brief <img src="../images/lightsalmon-chip.png" width=16 height=16 > rgb(255, 160, 122).
	static CL_Colord lightsalmon;

	/// \brief <img src="../images/lightseagreen-chip.png" width=16 height=16 > rgb( 32, 178, 170).
	static CL_Colord lightseagreen;

	/// \brief <img src="../images/lightskyblue-chip.png" width=16 height=16 > rgb(135, 206, 250).
	static CL_Colord lightskyblue;

	/// \brief <img src="../images/lightslategray-chip.png" width=16 height=16 > rgb(119, 136, 153).
	static CL_Colord lightslategray;

	/// \brief <img src="../images/lightslategrey-chip.png" width=16 height=16 > rgb(119, 136, 153).
	static CL_Colord lightslategrey;

	/// \brief <img src="../images/lightsteelblue-chip.png" width=16 height=16 > rgb(176, 196, 222).
	static CL_Colord lightsteelblue;

	/// \brief <img src="../images/lightyellow-chip.png" width=16 height=16 > rgb(255, 255, 224).
	static CL_Colord lightyellow;

	/// \brief <img src="../images/lime-chip.png" width=16 height=16 > rgb( 0, 255, 0).
	static CL_Colord lime;

	/// \brief <img src="../images/limegreen-chip.png" width=16 height=16 > rgb( 50, 205, 50).
	static CL_Colord limegreen;

	/// \brief <img src="../images/linen-chip.png" width=16 height=16 > rgb(250, 240, 230).
	static CL_Colord linen;

	/// \brief <img src="../images/magenta-chip.png" width=16 height=16 > rgb(255, 0, 255).
	static CL_Colord magenta;

	/// \brief <img src="../images/maroon-chip.png" width=16 height=16 > rgb(128, 0, 0).
	static CL_Colord maroon;

	/// \brief <img src="../images/mediumaquamarine-chip.png" width=16 height=16 > rgb(102, 205, 170).
	static CL_Colord mediumaquamarine;

	/// \brief <img src="../images/mediumblue-chip.png" width=16 height=16 > rgb( 0, 0, 205).
	static CL_Colord mediumblue;

	/// \brief <img src="../images/mediumorchid-chip.png" width=16 height=16 > rgb(186, 85, 211).
	static CL_Colord mediumorchid;

	/// \brief <img src="../images/mediumpurple-chip.png" width=16 height=16 > rgb(147, 112, 219).
	static CL_Colord mediumpurple;

	/// \brief <img src="../images/mediumseagreen-chip.png" width=16 height=16 > rgb( 60, 179, 113).
	static CL_Colord mediumseagreen;

	/// \brief <img src="../images/mediumslateblue-chip.png" width=16 height=16 > rgb(123, 104, 238).
	static CL_Colord mediumslateblue;

	/// \brief <img src="../images/mediumspringgreen-chip.png" width=16 height=16 > rgb( 0, 250, 154).
	static CL_Colord mediumspringgreen;

	/// \brief <img src="../images/mediumturquoise-chip.png" width=16 height=16 > rgb( 72, 209, 204).
	static CL_Colord mediumturquoise;

	/// \brief <img src="../images/mediumvioletred-chip.png" width=16 height=16 > rgb(199, 21, 133).
	static CL_Colord mediumvioletred;

	/// \brief <img src="../images/midnightblue-chip.png" width=16 height=16 > rgb( 25, 25, 112).
	static CL_Colord midnightblue;

	/// \brief <img src="../images/mintcream-chip.png" width=16 height=16 > rgb(245, 255, 250).
	static CL_Colord mintcream;

	/// \brief <img src="../images/mistyrose-chip.png" width=16 height=16 > rgb(255, 228, 225).
	static CL_Colord mistyrose;

	/// \brief <img src="../images/moccasin-chip.png" width=16 height=16 > rgb(255, 228, 181).
	static CL_Colord moccasin;

	/// \brief <img src="../images/navajowhite-chip.png" width=16 height=16 > rgb(255, 222, 173).
	static CL_Colord navajowhite;

	/// \brief <img src="../images/navy-chip.png" width=16 height=16 > rgb( 0, 0, 128).
	static CL_Colord navy;

	/// \brief <img src="../images/oldlace-chip.png" width=16 height=16 > rgb(253, 245, 230).
	static CL_Colord oldlace;

	/// \brief <img src="../images/olive-chip.png" width=16 height=16 > rgb(128, 128, 0).
	static CL_Colord olive;

	/// \brief <img src="../images/olivedrab-chip.png" width=16 height=16 > rgb(107, 142, 35).
	static CL_Colord olivedrab;

	/// \brief <img src="../images/orange-chip.png" width=16 height=16 > rgb(255, 165, 0).
	static CL_Colord orange;

	/// \brief <img src="../images/orangered-chip.png" width=16 height=16 > rgb(255, 69, 0).
	static CL_Colord orangered;

	/// \brief <img src="../images/orchid-chip.png" width=16 height=16 > rgb(218, 112, 214).
	static CL_Colord orchid;

	/// \brief <img src="../images/palegoldenrod-chip.png" width=16 height=16 > rgb(238, 232, 170).
	static CL_Colord palegoldenrod;

	/// \brief <img src="../images/palegreen-chip.png" width=16 height=16 > rgb(152, 251, 152).
	static CL_Colord palegreen;

	/// \brief <img src="../images/paleturquoise-chip.png" width=16 height=16 > rgb(175, 238, 238).
	static CL_Colord paleturquoise;

	/// \brief <img src="../images/palevioletred-chip.png" width=16 height=16 > rgb(219, 112, 147).
	static CL_Colord palevioletred;

	/// \brief <img src="../images/papayawhip-chip.png" width=16 height=16 > rgb(255, 239, 213).
	static CL_Colord papayawhip;

	/// \brief <img src="../images/peachpuff-chip.png" width=16 height=16 > rgb(255, 218, 185).
	static CL_Colord peachpuff;

	/// \brief <img src="../images/peru-chip.png" width=16 height=16 > rgb(205, 133, 63).
	static CL_Colord peru;

	/// \brief <img src="../images/pink-chip.png" width=16 height=16 > rgb(255, 192, 203).
	static CL_Colord pink;

	/// \brief <img src="../images/plum-chip.png" width=16 height=16 > rgb(221, 160, 221).
	static CL_Colord plum;

	/// \brief <img src="../images/powderblue-chip.png" width=16 height=16 > rgb(176, 224, 230).
	static CL_Colord powderblue;

	/// \brief <img src="../images/purple-chip.png" width=16 height=16 > rgb(128, 0, 128).
	static CL_Colord purple;

	/// \brief <img src="../images/red-chip.png" width=16 height=16 > rgb(255, 0, 0).
	static CL_Colord red;

	/// \brief <img src="../images/rosybrown-chip.png" width=16 height=16 > rgb(188, 143, 143).
	static CL_Colord rosybrown;

	/// \brief <img src="../images/royalblue-chip.png" width=16 height=16 > rgb( 65, 105, 225).
	static CL_Colord royalblue;

	/// \brief <img src="../images/saddlebrown-chip.png" width=16 height=16 > rgb(139, 69, 19).
	static CL_Colord saddlebrown;

	/// \brief <img src="../images/salmon-chip.png" width=16 height=16 > rgb(250, 128, 114).
	static CL_Colord salmon;

	/// \brief <img src="../images/sandybrown-chip.png" width=16 height=16 > rgb(244, 164, 96).
	static CL_Colord sandybrown;

	/// \brief <img src="../images/seagreen-chip.png" width=16 height=16 > rgb( 46, 139, 87).
	static CL_Colord seagreen;

	/// \brief <img src="../images/seashell-chip.png" width=16 height=16 > rgb(255, 245, 238).
	static CL_Colord seashell;

	/// \brief <img src="../images/sienna-chip.png" width=16 height=16 > rgb(160, 82, 45).
	static CL_Colord sienna;

	/// \brief <img src="../images/silver-chip.png" width=16 height=16 > rgb(192, 192, 192).
	static CL_Colord silver;

	/// \brief <img src="../images/skyblue-chip.png" width=16 height=16 > rgb(135, 206, 235).
	static CL_Colord skyblue;

	/// \brief <img src="../images/slateblue-chip.png" width=16 height=16 > rgb(106, 90, 205).
	static CL_Colord slateblue;

	/// \brief <img src="../images/slategray-chip.png" width=16 height=16 > rgb(112, 128, 144).
	static CL_Colord slategray;

	/// \brief <img src="../images/slategrey-chip.png" width=16 height=16 > rgb(112, 128, 144).
	static CL_Colord slategrey;

	/// \brief <img src="../images/snow-chip.png" width=16 height=16 > rgb(255, 250, 250).
	static CL_Colord snow;

	/// \brief <img src="../images/springgreen-chip.png" width=16 height=16 > rgb( 0, 255, 127).
	static CL_Colord springgreen;

	/// \brief <img src="../images/steelblue-chip.png" width=16 height=16 > rgb( 70, 130, 180).
	static CL_Colord steelblue;

	/// \brief <img src="../images/tan-chip.png" width=16 height=16 > rgb(210, 180, 140).
	static CL_Colord tan;

	/// \brief <img src="../images/teal-chip.png" width=16 height=16 > rgb( 0, 128, 128).
	static CL_Colord teal;

	/// \brief <img src="../images/thistle-chip.png" width=16 height=16 > rgb(216, 191, 216).
	static CL_Colord thistle;

	/// \brief <img src="../images/tomato-chip.png" width=16 height=16 > rgb(255, 99, 71).
	static CL_Colord tomato;

	/// \brief <img src="../images/turquoise-chip.png" width=16 height=16 > rgb( 64, 224, 208).
	static CL_Colord turquoise;

	/// \brief <img src="../images/violet-chip.png" width=16 height=16 > rgb(238, 130, 238).
	static CL_Colord violet;

	/// \brief <img src="../images/wheat-chip.png" width=16 height=16 > rgb(245, 222, 179).
	static CL_Colord wheat;

	/// \brief <img src="../images/white-chip.png" width=16 height=16 > rgb(255, 255, 255).
	static CL_Colord white;

	/// \brief <img src="../images/whitesmoke-chip.png" width=16 height=16 > rgb(245, 245, 245).
	static CL_Colord whitesmoke;

	/// \brief <img src="../images/yellow-chip.png" width=16 height=16 > rgb(255, 255, 0).
	static CL_Colord yellow;

	/// \brief <img src="../images/yellowgreen-chip.png" width=16 height=16 > rgb(154, 205, 50).
	static CL_Colord yellowgreen;

	/// \brief rgba(0, 0, 0, 0).
	static CL_Colord transparent;

	static CL_Colord gray10;
	static CL_Colord gray20;
	static CL_Colord gray30;
	static CL_Colord gray40;
	static CL_Colord gray50;
	static CL_Colord gray60;
	static CL_Colord gray70;
	static CL_Colord gray80;
	static CL_Colord gray90;
/// \}
};

/// \brief Floating point color description class (for float).
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class CL_Colorf
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a color.
	CL_Colorf() : r(0.0f), g(0.0f), b(0.0f), a(0.0f)
	{
	}

	/// \brief Constructs a color.
	///
	/// Color components are specified in the range 0 to 1.\n
	/// An alpha value of 0 means complete transparency, while 1 means completely opaque (solid).
	///
	/// \param r = Red color component.
	/// \param g = Green color component.
	/// \param b = Blue color component.
	/// \param a = Alpha (transparency) color component.
	CL_Colorf(float r, float g, float b, float a = 1.0f)
		: r(r), g(g), b(b), a(a)
	{
	}

	/// \brief Constructs a color.
	///
	/// Color components are specified in the range 0 to 1.\n
	/// An alpha value of 0 means complete transparency, while 1 means completely opaque (solid).
	///
	/// \param array_rgba = Red,Green,Blue,Alpha color component.
	CL_Colorf(const float *array_rgba)
		: r(array_rgba[0]), g(array_rgba[1]), b(array_rgba[2]), a(array_rgba[3])
	{
	}

	/// \brief Constructs a color.
	///
	/// Color components are specified in the range 0 to 255.\n
	/// An alpha value of 0 means complete transparency, while 255 means completely opaque (solid).
	///
	/// \param r = Red color component.
	/// \param g = Green color component.
	/// \param b = Blue color component.
	/// \param a = Alpha (transparency) color component.
	CL_Colorf(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255)
		: r(r/255.0f),
		  g(g/255.0f),
		  b(b/255.0f),
		  a(a/255.0f)
	{
	}

	/// \brief Constructs a color.
	///
	/// Color components are specified in the range 0 to 255.\n
	/// An alpha value of 0 means complete transparency, while 255 means completely opaque (solid).
	///
	/// \param r = Red color component.
	/// \param g = Green color component.
	/// \param b = Blue color component.
	/// \param a = Alpha (transparency) color component.
	CL_Colorf(int r, int g, int b, int a=255)
		: r(r/255.0f),
		  g(g/255.0f),
		  b(b/255.0f),
		  a(a/255.0f)
	{
	}

	explicit CL_Colorf(const CL_Color& color)
		: r(color.get_red()/255.0f),
		  g(color.get_green()/255.0f),
		  b(color.get_blue()/255.0f),
		  a(color.get_alpha()/255.0f)
	{
	}

	/// \brief Constructs a color.
	///
	/// Color components are specified in the range 0 to 255.\n
	/// An alpha value of 0 means complete transparency, while 255 means completely opaque (solid).
	///
	/// \param hexstr The colors rgba components as a hexadecimal string of the format "#rrggbbaa", where the '#' and "aa" parts are optional.
	CL_Colorf(const CL_StringRef &hexstr)
	{
		*this = (CL_Colorf)CL_Color::find_color(hexstr);
	}

/// \}
/// \name Attributes
/// \{

public:
	float r, g, b, a;

	/// \brief Get Red
	///
	/// \return red
	float get_red() const { return r; }

	/// \brief Get Green
	///
	/// \return green
	float get_green() const { return g; }

	/// \brief Get Blue
	///
	/// \return blue
	float get_blue() const { return b; }

	/// \brief Get Alpha
	///
	/// \return alpha
	float get_alpha() const { return a; }

/// \}
/// \name Operations
/// \{

public:
	/// \brief Normalize the color by ensuring that all color values lie inbetween (0.0, 1.0)
	void normalize()
	{
		r = (r < 0.0f) ? 0.0f : ((r > 1.0f) ? 1.0f : r);
		g = (g < 0.0f) ? 0.0f : ((g > 1.0f) ? 1.0f : g);
		b = (b < 0.0f) ? 0.0f : ((b > 1.0f) ? 1.0f : b);
		a = (a < 0.0f) ? 0.0f : ((a > 1.0f) ? 1.0f : a);
	}

	/// \brief Set alpha color component, in the range 0-1.
	void set_alpha(float value) { a = value; }

	/// \brief Set red color component, in the range 0-1.
	void set_red(float value) {  r = value; }

	/// \brief Set green color component, in the range 0-1.
	void set_green(float value) { g= value; }

	/// \brief Set blue color component, in the range 0-1.
	void set_blue(float value) { b = value; }

	/// \brief Color == Color operator (deep compare)
	bool operator==(const CL_Colorf &c) const
	{
		return (r == c.r) && (g == c.g) && (b == c.b) && (a == c.a);
	}

	/// \brief Color != Color operator (deep compare)
	bool operator!=(const CL_Colorf &c) const
	{
		return (r != c.r) || (g != c.g) || (b != c.b) || (a != c.a);
	}

	/// \brief Type conversion operator.
	operator CL_Color() const
	{
		return CL_Color(*this);
	}

/// \}
/// \name Statics
/// \{

public:
	/// \brief <img src="../images/aliceblue-chip.png" width=16 height=16 > rgb(240, 248, 255).
	static CL_Colorf aliceblue;

	/// \brief <img src="../images/antiquewhite-chip.png" width=16 height=16 > rgb(250, 235, 215).
	static CL_Colorf antiquewhite;

	/// \brief <img src="../images/aqua-chip.png" width=16 height=16 > rgb( 0, 255, 255).
	static CL_Colorf aqua;

	/// \brief <img src="../images/aquamarine-chip.png" width=16 height=16 > rgb(127, 255, 212).
	static CL_Colorf aquamarine;

	/// \brief <img src="../images/azure-chip.png" width=16 height=16 > rgb(240, 255, 255).
	static CL_Colorf azure;

	/// \brief <img src="../images/beige-chip.png" width=16 height=16 > rgb(245, 245, 220).
	static CL_Colorf beige;

	/// \brief <img src="../images/bisque-chip.png" width=16 height=16 > rgb(255, 228, 196).
	static CL_Colorf bisque;

	/// \brief <img src="../images/black-chip.png" width=16 height=16 > rgb( 0, 0, 0).
	static CL_Colorf black;

	/// \brief <img src="../images/blanchedalmond-chip.png" width=16 height=16 > rgb(255, 235, 205).
	static CL_Colorf blanchedalmond;

	/// \brief <img src="../images/blue-chip.png" width=16 height=16 > rgb( 0, 0, 255).
	static CL_Colorf blue;

	/// \brief <img src="../images/blueviolet-chip.png" width=16 height=16 > rgb(138, 43, 226).
	static CL_Colorf blueviolet;

	/// \brief <img src="../images/brown-chip.png" width=16 height=16 > rgb(165, 42, 42).
	static CL_Colorf brown;

	/// \brief <img src="../images/burlywood-chip.png" width=16 height=16 > rgb(222, 184, 135).
	static CL_Colorf burlywood;

	/// \brief <img src="../images/cadetblue-chip.png" width=16 height=16 > rgb( 95, 158, 160).
	static CL_Colorf cadetblue;

	/// \brief <img src="../images/chartreuse-chip.png" width=16 height=16 > rgb(127, 255, 0).
	static CL_Colorf chartreuse;

	/// \brief <img src="../images/chocolate-chip.png" width=16 height=16 > rgb(210, 105, 30).
	static CL_Colorf chocolate;

	/// \brief <img src="../images/coral-chip.png" width=16 height=16 > rgb(255, 127, 80).
	static CL_Colorf coral;

	/// \brief <img src="../images/cornflowerblue-chip.png" width=16 height=16 > rgb(100, 149, 237).
	static CL_Colorf cornflowerblue;

	/// \brief <img src="../images/cornsilk-chip.png" width=16 height=16 > rgb(255, 248, 220).
	static CL_Colorf cornsilk;

	/// \brief <img src="../images/crimson-chip.png" width=16 height=16 > rgb(220, 20, 60).
	static CL_Colorf crimson;

	/// \brief <img src="../images/cyan-chip.png" width=16 height=16 > rgb( 0, 255, 255).
	static CL_Colorf cyan;

	/// \brief <img src="../images/darkblue-chip.png" width=16 height=16 > rgb( 0, 0, 139).
	static CL_Colorf darkblue;

	/// \brief <img src="../images/darkcyan-chip.png" width=16 height=16 > rgb( 0, 139, 139).
	static CL_Colorf darkcyan;

	/// \brief <img src="../images/darkgoldenrod-chip.png" width=16 height=16 > rgb(184, 134, 11).
	static CL_Colorf darkgoldenrod;

	/// \brief <img src="../images/darkgray-chip.png" width=16 height=16 > rgb(169, 169, 169).
	static CL_Colorf darkgray;

	/// \brief <img src="../images/darkgreen-chip.png" width=16 height=16 > rgb( 0, 100, 0).
	static CL_Colorf darkgreen;

	/// \brief <img src="../images/darkgrey-chip.png" width=16 height=16 > rgb(169, 169, 169).
	static CL_Colorf darkgrey;

	/// \brief <img src="../images/darkkhaki-chip.png" width=16 height=16 > rgb(189, 183, 107).
	static CL_Colorf darkkhaki;

	/// \brief <img src="../images/darkmagenta-chip.png" width=16 height=16 > rgb(139, 0, 139).
	static CL_Colorf darkmagenta;

	/// \brief <img src="../images/darkolivegreen-chip.png" width=16 height=16 > rgb( 85, 107, 47).
	static CL_Colorf darkolivegreen;

	/// \brief <img src="../images/darkorange-chip.png" width=16 height=16 > rgb(255, 140, 0).
	static CL_Colorf darkorange;

	/// \brief <img src="../images/darkorchid-chip.png" width=16 height=16 > rgb(153, 50, 204).
	static CL_Colorf darkorchid;

	/// \brief <img src="../images/darkred-chip.png" width=16 height=16 > rgb(139, 0, 0).
	static CL_Colorf darkred;

	/// \brief <img src="../images/darksalmon-chip.png" width=16 height=16 > rgb(233, 150, 122).
	static CL_Colorf darksalmon;

	/// \brief <img src="../images/darkseagreen-chip.png" width=16 height=16 > rgb(143, 188, 143).
	static CL_Colorf darkseagreen;

	/// \brief <img src="../images/darkslateblue-chip.png" width=16 height=16 > rgb( 72, 61, 139).
	static CL_Colorf darkslateblue;

	/// \brief <img src="../images/darkslategray-chip.png" width=16 height=16 > rgb( 47, 79, 79).
	static CL_Colorf darkslategray;

	/// \brief <img src="../images/darkslategrey-chip.png" width=16 height=16 > rgb( 47, 79, 79).
	static CL_Colorf darkslategrey;

	/// \brief <img src="../images/darkturquoise-chip.png" width=16 height=16 > rgb( 0, 206, 209).
	static CL_Colorf darkturquoise;

	/// \brief <img src="../images/darkviolet-chip.png" width=16 height=16 > rgb(148, 0, 211).
	static CL_Colorf darkviolet;

	/// \brief <img src="../images/deeppink-chip.png" width=16 height=16 > rgb(255, 20, 147).
	static CL_Colorf deeppink;

	/// \brief <img src="../images/deepskyblue-chip.png" width=16 height=16 > rgb( 0, 191, 255).
	static CL_Colorf deepskyblue;

	/// \brief <img src="../images/dimgray-chip.png" width=16 height=16 > rgb(105, 105, 105).
	static CL_Colorf dimgray;

	/// \brief <img src="../images/dimgrey-chip.png" width=16 height=16 > rgb(105, 105, 105).
	static CL_Colorf dimgrey;

	/// \brief <img src="../images/dodgerblue-chip.png" width=16 height=16 > rgb( 30, 144, 255).
	static CL_Colorf dodgerblue;

	/// \brief <img src="../images/firebrick-chip.png" width=16 height=16 > rgb(178, 34, 34).
	static CL_Colorf firebrick;

	/// \brief <img src="../images/floralwhite-chip.png" width=16 height=16 > rgb(255, 250, 240).
	static CL_Colorf floralwhite;

	/// \brief <img src="../images/forestgreen-chip.png" width=16 height=16 > rgb( 34, 139, 34).
	static CL_Colorf forestgreen;

	/// \brief <img src="../images/fuchsia-chip.png" width=16 height=16 > rgb(255, 0, 255).
	static CL_Colorf fuchsia;

	/// \brief <img src="../images/gainsboro-chip.png" width=16 height=16 > rgb(220, 220, 220).
	static CL_Colorf gainsboro;

	/// \brief <img src="../images/ghostwhite-chip.png" width=16 height=16 > rgb(248, 248, 255).
	static CL_Colorf ghostwhite;

	/// \brief <img src="../images/gold-chip.png" width=16 height=16 > rgb(255, 215, 0).
	static CL_Colorf gold;

	/// \brief <img src="../images/goldenrod-chip.png" width=16 height=16 > rgb(218, 165, 32).
	static CL_Colorf goldenrod;

	/// \brief <img src="../images/gray-chip.png" width=16 height=16 > rgb(128, 128, 128).
	static CL_Colorf gray;

	/// \brief <img src="../images/grey-chip.png" width=16 height=16 > rgb(128, 128, 128).
	static CL_Colorf grey;

	/// \brief <img src="../images/green-chip.png" width=16 height=16 > rgb( 0, 128, 0).
	static CL_Colorf green;

	/// \brief <img src="../images/greenyellow-chip.png" width=16 height=16 > rgb(173, 255, 47).
	static CL_Colorf greenyellow;

	/// \brief <img src="../images/honeydew-chip.png" width=16 height=16 > rgb(240, 255, 240).
	static CL_Colorf honeydew;

	/// \brief <img src="../images/hotpink-chip.png" width=16 height=16 > rgb(255, 105, 180).
	static CL_Colorf hotpink;

	/// \brief <img src="../images/indianred-chip.png" width=16 height=16 > rgb(205, 92, 92).
	static CL_Colorf indianred;

	/// \brief <img src="../images/indigo-chip.png" width=16 height=16 > rgb( 75, 0, 130).
	static CL_Colorf indigo;

	/// \brief <img src="../images/ivory-chip.png" width=16 height=16 > rgb(255, 255, 240).
	static CL_Colorf ivory;

	/// \brief <img src="../images/khaki-chip.png" width=16 height=16 > rgb(240, 230, 140).
	static CL_Colorf khaki;

	/// \brief <img src="../images/lavender-chip.png" width=16 height=16 > rgb(230, 230, 250).
	static CL_Colorf lavender;

	/// \brief <img src="../images/lavenderblush-chip.png" width=16 height=16 > rgb(255, 240, 245).
	static CL_Colorf lavenderblush;

	/// \brief <img src="../images/lawngreen-chip.png" width=16 height=16 > rgb(124, 252, 0).
	static CL_Colorf lawngreen;

	/// \brief <img src="../images/lemonchiffon-chip.png" width=16 height=16 > rgb(255, 250, 205).
	static CL_Colorf lemonchiffon;

	/// \brief <img src="../images/lightblue-chip.png" width=16 height=16 > rgb(173, 216, 230).
	static CL_Colorf lightblue;

	/// \brief <img src="../images/lightcoral-chip.png" width=16 height=16 > rgb(240, 128, 128).
	static CL_Colorf lightcoral;

	/// \brief <img src="../images/lightcyan-chip.png" width=16 height=16 > rgb(224, 255, 255).
	static CL_Colorf lightcyan;

	/// \brief <img src="../images/lightgoldenrodyellow-chip.png" width=16 height=16 > rgb(250, 250, 210).
	static CL_Colorf lightgoldenrodyellow;

	/// \brief <img src="../images/lightgray-chip.png" width=16 height=16 > rgb(211, 211, 211).
	static CL_Colorf lightgray;

	/// \brief <img src="../images/lightgreen-chip.png" width=16 height=16 > rgb(144, 238, 144).
	static CL_Colorf lightgreen;

	/// \brief <img src="../images/lightgrey-chip.png" width=16 height=16 > rgb(211, 211, 211).
	static CL_Colorf lightgrey;

	/// \brief <img src="../images/lightpink-chip.png" width=16 height=16 > rgb(255, 182, 193).
	static CL_Colorf lightpink;

	/// \brief <img src="../images/lightsalmon-chip.png" width=16 height=16 > rgb(255, 160, 122).
	static CL_Colorf lightsalmon;

	/// \brief <img src="../images/lightseagreen-chip.png" width=16 height=16 > rgb( 32, 178, 170).
	static CL_Colorf lightseagreen;

	/// \brief <img src="../images/lightskyblue-chip.png" width=16 height=16 > rgb(135, 206, 250).
	static CL_Colorf lightskyblue;

	/// \brief <img src="../images/lightslategray-chip.png" width=16 height=16 > rgb(119, 136, 153).
	static CL_Colorf lightslategray;

	/// \brief <img src="../images/lightslategrey-chip.png" width=16 height=16 > rgb(119, 136, 153).
	static CL_Colorf lightslategrey;

	/// \brief <img src="../images/lightsteelblue-chip.png" width=16 height=16 > rgb(176, 196, 222).
	static CL_Colorf lightsteelblue;

	/// \brief <img src="../images/lightyellow-chip.png" width=16 height=16 > rgb(255, 255, 224).
	static CL_Colorf lightyellow;

	/// \brief <img src="../images/lime-chip.png" width=16 height=16 > rgb( 0, 255, 0).
	static CL_Colorf lime;

	/// \brief <img src="../images/limegreen-chip.png" width=16 height=16 > rgb( 50, 205, 50).
	static CL_Colorf limegreen;

	/// \brief <img src="../images/linen-chip.png" width=16 height=16 > rgb(250, 240, 230).
	static CL_Colorf linen;

	/// \brief <img src="../images/magenta-chip.png" width=16 height=16 > rgb(255, 0, 255).
	static CL_Colorf magenta;

	/// \brief <img src="../images/maroon-chip.png" width=16 height=16 > rgb(128, 0, 0).
	static CL_Colorf maroon;

	/// \brief <img src="../images/mediumaquamarine-chip.png" width=16 height=16 > rgb(102, 205, 170).
	static CL_Colorf mediumaquamarine;

	/// \brief <img src="../images/mediumblue-chip.png" width=16 height=16 > rgb( 0, 0, 205).
	static CL_Colorf mediumblue;

	/// \brief <img src="../images/mediumorchid-chip.png" width=16 height=16 > rgb(186, 85, 211).
	static CL_Colorf mediumorchid;

	/// \brief <img src="../images/mediumpurple-chip.png" width=16 height=16 > rgb(147, 112, 219).
	static CL_Colorf mediumpurple;

	/// \brief <img src="../images/mediumseagreen-chip.png" width=16 height=16 > rgb( 60, 179, 113).
	static CL_Colorf mediumseagreen;

	/// \brief <img src="../images/mediumslateblue-chip.png" width=16 height=16 > rgb(123, 104, 238).
	static CL_Colorf mediumslateblue;

	/// \brief <img src="../images/mediumspringgreen-chip.png" width=16 height=16 > rgb( 0, 250, 154).
	static CL_Colorf mediumspringgreen;

	/// \brief <img src="../images/mediumturquoise-chip.png" width=16 height=16 > rgb( 72, 209, 204).
	static CL_Colorf mediumturquoise;

	/// \brief <img src="../images/mediumvioletred-chip.png" width=16 height=16 > rgb(199, 21, 133).
	static CL_Colorf mediumvioletred;

	/// \brief <img src="../images/midnightblue-chip.png" width=16 height=16 > rgb( 25, 25, 112).
	static CL_Colorf midnightblue;

	/// \brief <img src="../images/mintcream-chip.png" width=16 height=16 > rgb(245, 255, 250).
	static CL_Colorf mintcream;

	/// \brief <img src="../images/mistyrose-chip.png" width=16 height=16 > rgb(255, 228, 225).
	static CL_Colorf mistyrose;

	/// \brief <img src="../images/moccasin-chip.png" width=16 height=16 > rgb(255, 228, 181).
	static CL_Colorf moccasin;

	/// \brief <img src="../images/navajowhite-chip.png" width=16 height=16 > rgb(255, 222, 173).
	static CL_Colorf navajowhite;

	/// \brief <img src="../images/navy-chip.png" width=16 height=16 > rgb( 0, 0, 128).
	static CL_Colorf navy;

	/// \brief <img src="../images/oldlace-chip.png" width=16 height=16 > rgb(253, 245, 230).
	static CL_Colorf oldlace;

	/// \brief <img src="../images/olive-chip.png" width=16 height=16 > rgb(128, 128, 0).
	static CL_Colorf olive;

	/// \brief <img src="../images/olivedrab-chip.png" width=16 height=16 > rgb(107, 142, 35).
	static CL_Colorf olivedrab;

	/// \brief <img src="../images/orange-chip.png" width=16 height=16 > rgb(255, 165, 0).
	static CL_Colorf orange;

	/// \brief <img src="../images/orangered-chip.png" width=16 height=16 > rgb(255, 69, 0).
	static CL_Colorf orangered;

	/// \brief <img src="../images/orchid-chip.png" width=16 height=16 > rgb(218, 112, 214).
	static CL_Colorf orchid;

	/// \brief <img src="../images/palegoldenrod-chip.png" width=16 height=16 > rgb(238, 232, 170).
	static CL_Colorf palegoldenrod;

	/// \brief <img src="../images/palegreen-chip.png" width=16 height=16 > rgb(152, 251, 152).
	static CL_Colorf palegreen;

	/// \brief <img src="../images/paleturquoise-chip.png" width=16 height=16 > rgb(175, 238, 238).
	static CL_Colorf paleturquoise;

	/// \brief <img src="../images/palevioletred-chip.png" width=16 height=16 > rgb(219, 112, 147).
	static CL_Colorf palevioletred;

	/// \brief <img src="../images/papayawhip-chip.png" width=16 height=16 > rgb(255, 239, 213).
	static CL_Colorf papayawhip;

	/// \brief <img src="../images/peachpuff-chip.png" width=16 height=16 > rgb(255, 218, 185).
	static CL_Colorf peachpuff;

	/// \brief <img src="../images/peru-chip.png" width=16 height=16 > rgb(205, 133, 63).
	static CL_Colorf peru;

	/// \brief <img src="../images/pink-chip.png" width=16 height=16 > rgb(255, 192, 203).
	static CL_Colorf pink;

	/// \brief <img src="../images/plum-chip.png" width=16 height=16 > rgb(221, 160, 221).
	static CL_Colorf plum;

	/// \brief <img src="../images/powderblue-chip.png" width=16 height=16 > rgb(176, 224, 230).
	static CL_Colorf powderblue;

	/// \brief <img src="../images/purple-chip.png" width=16 height=16 > rgb(128, 0, 128).
	static CL_Colorf purple;

	/// \brief <img src="../images/red-chip.png" width=16 height=16 > rgb(255, 0, 0).
	static CL_Colorf red;

	/// \brief <img src="../images/rosybrown-chip.png" width=16 height=16 > rgb(188, 143, 143).
	static CL_Colorf rosybrown;

	/// \brief <img src="../images/royalblue-chip.png" width=16 height=16 > rgb( 65, 105, 225).
	static CL_Colorf royalblue;

	/// \brief <img src="../images/saddlebrown-chip.png" width=16 height=16 > rgb(139, 69, 19).
	static CL_Colorf saddlebrown;

	/// \brief <img src="../images/salmon-chip.png" width=16 height=16 > rgb(250, 128, 114).
	static CL_Colorf salmon;

	/// \brief <img src="../images/sandybrown-chip.png" width=16 height=16 > rgb(244, 164, 96).
	static CL_Colorf sandybrown;

	/// \brief <img src="../images/seagreen-chip.png" width=16 height=16 > rgb( 46, 139, 87).
	static CL_Colorf seagreen;

	/// \brief <img src="../images/seashell-chip.png" width=16 height=16 > rgb(255, 245, 238).
	static CL_Colorf seashell;

	/// \brief <img src="../images/sienna-chip.png" width=16 height=16 > rgb(160, 82, 45).
	static CL_Colorf sienna;

	/// \brief <img src="../images/silver-chip.png" width=16 height=16 > rgb(192, 192, 192).
	static CL_Colorf silver;

	/// \brief <img src="../images/skyblue-chip.png" width=16 height=16 > rgb(135, 206, 235).
	static CL_Colorf skyblue;

	/// \brief <img src="../images/slateblue-chip.png" width=16 height=16 > rgb(106, 90, 205).
	static CL_Colorf slateblue;

	/// \brief <img src="../images/slategray-chip.png" width=16 height=16 > rgb(112, 128, 144).
	static CL_Colorf slategray;

	/// \brief <img src="../images/slategrey-chip.png" width=16 height=16 > rgb(112, 128, 144).
	static CL_Colorf slategrey;

	/// \brief <img src="../images/snow-chip.png" width=16 height=16 > rgb(255, 250, 250).
	static CL_Colorf snow;

	/// \brief <img src="../images/springgreen-chip.png" width=16 height=16 > rgb( 0, 255, 127).
	static CL_Colorf springgreen;

	/// \brief <img src="../images/steelblue-chip.png" width=16 height=16 > rgb( 70, 130, 180).
	static CL_Colorf steelblue;

	/// \brief <img src="../images/tan-chip.png" width=16 height=16 > rgb(210, 180, 140).
	static CL_Colorf tan;

	/// \brief <img src="../images/teal-chip.png" width=16 height=16 > rgb( 0, 128, 128).
	static CL_Colorf teal;

	/// \brief <img src="../images/thistle-chip.png" width=16 height=16 > rgb(216, 191, 216).
	static CL_Colorf thistle;

	/// \brief <img src="../images/tomato-chip.png" width=16 height=16 > rgb(255, 99, 71).
	static CL_Colorf tomato;

	/// \brief <img src="../images/turquoise-chip.png" width=16 height=16 > rgb( 64, 224, 208).
	static CL_Colorf turquoise;

	/// \brief <img src="../images/violet-chip.png" width=16 height=16 > rgb(238, 130, 238).
	static CL_Colorf violet;

	/// \brief <img src="../images/wheat-chip.png" width=16 height=16 > rgb(245, 222, 179).
	static CL_Colorf wheat;

	/// \brief <img src="../images/white-chip.png" width=16 height=16 > rgb(255, 255, 255).
	static CL_Colorf white;

	/// \brief <img src="../images/whitesmoke-chip.png" width=16 height=16 > rgb(245, 245, 245).
	static CL_Colorf whitesmoke;

	/// \brief <img src="../images/yellow-chip.png" width=16 height=16 > rgb(255, 255, 0).
	static CL_Colorf yellow;

	/// \brief <img src="../images/yellowgreen-chip.png" width=16 height=16 > rgb(154, 205, 50).
	static CL_Colorf yellowgreen;

	/// \brief rgba(0, 0, 0, 0).
	static CL_Colorf transparent;

	static CL_Colorf gray10;
	static CL_Colorf gray20;
	static CL_Colorf gray30;
	static CL_Colorf gray40;
	static CL_Colorf gray50;
	static CL_Colorf gray60;
	static CL_Colorf gray70;
	static CL_Colorf gray80;
	static CL_Colorf gray90;
/// \}
};

/// \}
