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
**    Mark Page
*/

#pragma once

#include "../../Core/Math/vec4.h"
#include <vector>

namespace clan
{
	/// \addtogroup clanDisplay_2D clanDisplay 2D
	/// \{

	class PixelFormat;
	class Colorf;

	/// \brief Color description class.
	class Color : public Vec4ub
	{
	public:
		/// \brief Constructs a color.
		Color() {}

		explicit Color(const Colorf&);

		/// \brief Constructs a color.
		///
		/// Color components are specified in the range 0 to 255.\n
		/// An alpha value of 0 means complete transparency, while 255 means completely opaque (solid).
		///
		/// \param red Red color component.
		/// \param green Green color component.
		/// \param blue Blue color component.
		/// \param alpha Alpha (transparency) color component.
		Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
			: Vec4ub(red, green, blue, alpha) { return; }

		/// \brief Constructs a color.
		///
		/// Color components are specified in the range 0 to 255.\n
		/// An alpha value of 0 means complete transparency, while 255 means completely opaque (solid).
		///
		/// \param hexstr The colors rgba components as a hexadecimal string of the format "#rrggbbaa", where the '#' and "aa" parts are optional.
		Color(const std::string &hexstr);

		/// \brief Returns the alpha color component, in the range 0-255.
		unsigned char get_alpha() const { return a; }

		/// \brief Returns the red color component, in the range 0-255.
		unsigned char get_red() const { return r; }

		/// \brief Returns the green color component, in the range 0-255.
		unsigned char get_green() const { return g; }

		/// \brief Returns the blue color component, in the range 0-255.
		unsigned char get_blue() const { return b; }

		/// \brief Returns the alpha color component, in the range 0-1.
		float get_alpha_f() const { return float(a) / 255.0f; }

		/// \brief Returns the red color component, in the range 0-1.
		float get_red_f() const { return float(r) / 255.0f; }

		/// \brief Returns the green color component, in the range 0-1.
		float get_green_f() const { return float(g) / 255.0f; }

		/// \brief Returns the blue color component, in the range 0-1.
		float get_blue_f() const { return float(b) / 255.0f; }

		/// \brief Returns the color in ARGB8888 format.
		unsigned int get_argb8() const { return (((unsigned int)a) << 24) | (((unsigned int)r) << 16) | (((unsigned int)g) << 8) | (unsigned int)b; }

		/// \brief Returns the color in ABGR8888 format.
		unsigned int get_abgr8() const { return (((unsigned int)a) << 24) | (((unsigned int)b) << 16) | (((unsigned int)g) << 8) | (unsigned int)r; }

		/// \brief Returns the color in 0BGR8888 format.
		unsigned int get_bgr8() const { return (((unsigned int)b) << 16) | (((unsigned int)g) << 8) | (unsigned int)r; }

		/// \brief Returns the color in RGBA8888 format.
		unsigned int get_rgba8() const { return (((unsigned int)r) << 24) | (((unsigned int)g) << 16) | (((unsigned int)b) << 8) | (unsigned int)a; }

		/// \brief Returns the color in BGRA8888 format.
		unsigned int get_bgra8() const { return (((unsigned int)b) << 24) | (((unsigned int)g) << 16) | (((unsigned int)r) << 8) | (unsigned int)a; }

	// Operations:
	public:
		/// \brief Color == Color operator (deep compare)
		bool operator==(const Color &c) const
		{
			return (r == c.r) && (g == c.g) && (b == c.b) && (a == c.a);
		}

		/// \brief Color != Color operator (deep compare)
		bool operator!=(const Color &c) const
		{
			return (r != c.r) || (g != c.g) || (b != c.b) || (a != c.a);
		}

		/// \brief <img src="../../img/colors/aliceblue-chip.png" width=16 height=16 > rgb(240, 248, 255).
		static Color aliceblue;

		/// \brief <img src="../../img/colors/antiquewhite-chip.png" width=16 height=16 > rgb(250, 235, 215).
		static Color antiquewhite;

		/// \brief <img src="../../img/colors/aqua-chip.png" width=16 height=16 > rgb( 0, 255, 255).
		static Color aqua;

		/// \brief <img src="../../img/colors/aquamarine-chip.png" width=16 height=16 > rgb(127, 255, 212).
		static Color aquamarine;

		/// \brief <img src="../../img/colors/azure-chip.png" width=16 height=16 > rgb(240, 255, 255).
		static Color azure;

		/// \brief <img src="../../img/colors/beige-chip.png" width=16 height=16 > rgb(245, 245, 220).
		static Color beige;

		/// \brief <img src="../../img/colors/bisque-chip.png" width=16 height=16 > rgb(255, 228, 196).
		static Color bisque;

		/// \brief <img src="../../img/colors/black-chip.png" width=16 height=16 > rgb( 0, 0, 0).
		static Color black;

		/// \brief <img src="../../img/colors/blanchedalmond-chip.png" width=16 height=16 > rgb(255, 235, 205).
		static Color blanchedalmond;

		/// \brief <img src="../../img/colors/blue-chip.png" width=16 height=16 > rgb( 0, 0, 255).
		static Color blue;

		/// \brief <img src="../../img/colors/blueviolet-chip.png" width=16 height=16 > rgb(138, 43, 226).
		static Color blueviolet;

		/// \brief <img src="../../img/colors/brown-chip.png" width=16 height=16 > rgb(165, 42, 42).
		static Color brown;

		/// \brief <img src="../../img/colors/burlywood-chip.png" width=16 height=16 > rgb(222, 184, 135).
		static Color burlywood;

		/// \brief <img src="../../img/colors/cadetblue-chip.png" width=16 height=16 > rgb( 95, 158, 160).
		static Color cadetblue;

		/// \brief <img src="../../img/colors/chartreuse-chip.png" width=16 height=16 > rgb(127, 255, 0).
		static Color chartreuse;

		/// \brief <img src="../../img/colors/chocolate-chip.png" width=16 height=16 > rgb(210, 105, 30).
		static Color chocolate;

		/// \brief <img src="../../img/colors/coral-chip.png" width=16 height=16 > rgb(255, 127, 80).
		static Color coral;

		/// \brief <img src="../../img/colors/cornflowerblue-chip.png" width=16 height=16 > rgb(100, 149, 237).
		static Color cornflowerblue;

		/// \brief <img src="../../img/colors/cornsilk-chip.png" width=16 height=16 > rgb(255, 248, 220).
		static Color cornsilk;

		/// \brief <img src="../../img/colors/crimson-chip.png" width=16 height=16 > rgb(220, 20, 60).
		static Color crimson;

		/// \brief <img src="../../img/colors/cyan-chip.png" width=16 height=16 > rgb( 0, 255, 255).
		static Color cyan;

		/// \brief <img src="../../img/colors/darkblue-chip.png" width=16 height=16 > rgb( 0, 0, 139).
		static Color darkblue;

		/// \brief <img src="../../img/colors/darkcyan-chip.png" width=16 height=16 > rgb( 0, 139, 139).
		static Color darkcyan;

		/// \brief <img src="../../img/colors/darkgoldenrod-chip.png" width=16 height=16 > rgb(184, 134, 11).
		static Color darkgoldenrod;

		/// \brief <img src="../../img/colors/darkgray-chip.png" width=16 height=16 > rgb(169, 169, 169).
		static Color darkgray;

		/// \brief <img src="../../img/colors/darkgreen-chip.png" width=16 height=16 > rgb( 0, 100, 0).
		static Color darkgreen;

		/// \brief <img src="../../img/colors/darkgrey-chip.png" width=16 height=16 > rgb(169, 169, 169).
		static Color darkgrey;

		/// \brief <img src="../../img/colors/darkkhaki-chip.png" width=16 height=16 > rgb(189, 183, 107).
		static Color darkkhaki;

		/// \brief <img src="../../img/colors/darkmagenta-chip.png" width=16 height=16 > rgb(139, 0, 139).
		static Color darkmagenta;

		/// \brief <img src="../../img/colors/darkolivegreen-chip.png" width=16 height=16 > rgb( 85, 107, 47).
		static Color darkolivegreen;

		/// \brief <img src="../../img/colors/darkorange-chip.png" width=16 height=16 > rgb(255, 140, 0).
		static Color darkorange;

		/// \brief <img src="../../img/colors/darkorchid-chip.png" width=16 height=16 > rgb(153, 50, 204).
		static Color darkorchid;

		/// \brief <img src="../../img/colors/darkred-chip.png" width=16 height=16 > rgb(139, 0, 0).
		static Color darkred;

		/// \brief <img src="../../img/colors/darksalmon-chip.png" width=16 height=16 > rgb(233, 150, 122).
		static Color darksalmon;

		/// \brief <img src="../../img/colors/darkseagreen-chip.png" width=16 height=16 > rgb(143, 188, 143).
		static Color darkseagreen;

		/// \brief <img src="../../img/colors/darkslateblue-chip.png" width=16 height=16 > rgb( 72, 61, 139).
		static Color darkslateblue;

		/// \brief <img src="../../img/colors/darkslategray-chip.png" width=16 height=16 > rgb( 47, 79, 79).
		static Color darkslategray;

		/// \brief <img src="../../img/colors/darkslategrey-chip.png" width=16 height=16 > rgb( 47, 79, 79).
		static Color darkslategrey;

		/// \brief <img src="../../img/colors/darkturquoise-chip.png" width=16 height=16 > rgb( 0, 206, 209).
		static Color darkturquoise;

		/// \brief <img src="../../img/colors/darkviolet-chip.png" width=16 height=16 > rgb(148, 0, 211).
		static Color darkviolet;

		/// \brief <img src="../../img/colors/deeppink-chip.png" width=16 height=16 > rgb(255, 20, 147).
		static Color deeppink;

		/// \brief <img src="../../img/colors/deepskyblue-chip.png" width=16 height=16 > rgb( 0, 191, 255).
		static Color deepskyblue;

		/// \brief <img src="../../img/colors/dimgray-chip.png" width=16 height=16 > rgb(105, 105, 105).
		static Color dimgray;

		/// \brief <img src="../../img/colors/dimgrey-chip.png" width=16 height=16 > rgb(105, 105, 105).
		static Color dimgrey;

		/// \brief <img src="../../img/colors/dodgerblue-chip.png" width=16 height=16 > rgb( 30, 144, 255).
		static Color dodgerblue;

		/// \brief <img src="../../img/colors/firebrick-chip.png" width=16 height=16 > rgb(178, 34, 34).
		static Color firebrick;

		/// \brief <img src="../../img/colors/floralwhite-chip.png" width=16 height=16 > rgb(255, 250, 240).
		static Color floralwhite;

		/// \brief <img src="../../img/colors/forestgreen-chip.png" width=16 height=16 > rgb( 34, 139, 34).
		static Color forestgreen;

		/// \brief <img src="../../img/colors/fuchsia-chip.png" width=16 height=16 > rgb(255, 0, 255).
		static Color fuchsia;

		/// \brief <img src="../../img/colors/gainsboro-chip.png" width=16 height=16 > rgb(220, 220, 220).
		static Color gainsboro;

		/// \brief <img src="../../img/colors/ghostwhite-chip.png" width=16 height=16 > rgb(248, 248, 255).
		static Color ghostwhite;

		/// \brief <img src="../../img/colors/gold-chip.png" width=16 height=16 > rgb(255, 215, 0).
		static Color gold;

		/// \brief <img src="../../img/colors/goldenrod-chip.png" width=16 height=16 > rgb(218, 165, 32).
		static Color goldenrod;

		/// \brief <img src="../../img/colors/gray-chip.png" width=16 height=16 > rgb(128, 128, 128).
		static Color gray;

		/// \brief <img src="../../img/colors/grey-chip.png" width=16 height=16 > rgb(128, 128, 128).
		static Color grey;

		/// \brief <img src="../../img/colors/green-chip.png" width=16 height=16 > rgb( 0, 128, 0).
		static Color green;

		/// \brief <img src="../../img/colors/greenyellow-chip.png" width=16 height=16 > rgb(173, 255, 47).
		static Color greenyellow;

		/// \brief <img src="../../img/colors/honeydew-chip.png" width=16 height=16 > rgb(240, 255, 240).
		static Color honeydew;

		/// \brief <img src="../../img/colors/hotpink-chip.png" width=16 height=16 > rgb(255, 105, 180).
		static Color hotpink;

		/// \brief <img src="../../img/colors/indianred-chip.png" width=16 height=16 > rgb(205, 92, 92).
		static Color indianred;

		/// \brief <img src="../../img/colors/indigo-chip.png" width=16 height=16 > rgb( 75, 0, 130).
		static Color indigo;

		/// \brief <img src="../../img/colors/ivory-chip.png" width=16 height=16 > rgb(255, 255, 240).
		static Color ivory;

		/// \brief <img src="../../img/colors/khaki-chip.png" width=16 height=16 > rgb(240, 230, 140).
		static Color khaki;

		/// \brief <img src="../../img/colors/lavender-chip.png" width=16 height=16 > rgb(230, 230, 250).
		static Color lavender;

		/// \brief <img src="../../img/colors/lavenderblush-chip.png" width=16 height=16 > rgb(255, 240, 245).
		static Color lavenderblush;

		/// \brief <img src="../../img/colors/lawngreen-chip.png" width=16 height=16 > rgb(124, 252, 0).
		static Color lawngreen;

		/// \brief <img src="../../img/colors/lemonchiffon-chip.png" width=16 height=16 > rgb(255, 250, 205).
		static Color lemonchiffon;

		/// \brief <img src="../../img/colors/lightblue-chip.png" width=16 height=16 > rgb(173, 216, 230).
		static Color lightblue;

		/// \brief <img src="../../img/colors/lightcoral-chip.png" width=16 height=16 > rgb(240, 128, 128).
		static Color lightcoral;

		/// \brief <img src="../../img/colors/lightcyan-chip.png" width=16 height=16 > rgb(224, 255, 255).
		static Color lightcyan;

		/// \brief <img src="../../img/colors/lightgoldenrodyellow-chip.png" width=16 height=16 > rgb(250, 250, 210).
		static Color lightgoldenrodyellow;

		/// \brief <img src="../../img/colors/lightgray-chip.png" width=16 height=16 > rgb(211, 211, 211).
		static Color lightgray;

		/// \brief <img src="../../img/colors/lightgreen-chip.png" width=16 height=16 > rgb(144, 238, 144).
		static Color lightgreen;

		/// \brief <img src="../../img/colors/lightgrey-chip.png" width=16 height=16 > rgb(211, 211, 211).
		static Color lightgrey;

		/// \brief <img src="../../img/colors/lightpink-chip.png" width=16 height=16 > rgb(255, 182, 193).
		static Color lightpink;

		/// \brief <img src="../../img/colors/lightsalmon-chip.png" width=16 height=16 > rgb(255, 160, 122).
		static Color lightsalmon;

		/// \brief <img src="../../img/colors/lightseagreen-chip.png" width=16 height=16 > rgb( 32, 178, 170).
		static Color lightseagreen;

		/// \brief <img src="../../img/colors/lightskyblue-chip.png" width=16 height=16 > rgb(135, 206, 250).
		static Color lightskyblue;

		/// \brief <img src="../../img/colors/lightslategray-chip.png" width=16 height=16 > rgb(119, 136, 153).
		static Color lightslategray;

		/// \brief <img src="../../img/colors/lightslategrey-chip.png" width=16 height=16 > rgb(119, 136, 153).
		static Color lightslategrey;

		/// \brief <img src="../../img/colors/lightsteelblue-chip.png" width=16 height=16 > rgb(176, 196, 222).
		static Color lightsteelblue;

		/// \brief <img src="../../img/colors/lightyellow-chip.png" width=16 height=16 > rgb(255, 255, 224).
		static Color lightyellow;

		/// \brief <img src="../../img/colors/lime-chip.png" width=16 height=16 > rgb( 0, 255, 0).
		static Color lime;

		/// \brief <img src="../../img/colors/limegreen-chip.png" width=16 height=16 > rgb( 50, 205, 50).
		static Color limegreen;

		/// \brief <img src="../../img/colors/linen-chip.png" width=16 height=16 > rgb(250, 240, 230).
		static Color linen;

		/// \brief <img src="../../img/colors/magenta-chip.png" width=16 height=16 > rgb(255, 0, 255).
		static Color magenta;

		/// \brief <img src="../../img/colors/maroon-chip.png" width=16 height=16 > rgb(128, 0, 0).
		static Color maroon;

		/// \brief <img src="../../img/colors/mediumaquamarine-chip.png" width=16 height=16 > rgb(102, 205, 170).
		static Color mediumaquamarine;

		/// \brief <img src="../../img/colors/mediumblue-chip.png" width=16 height=16 > rgb( 0, 0, 205).
		static Color mediumblue;

		/// \brief <img src="../../img/colors/mediumorchid-chip.png" width=16 height=16 > rgb(186, 85, 211).
		static Color mediumorchid;

		/// \brief <img src="../../img/colors/mediumpurple-chip.png" width=16 height=16 > rgb(147, 112, 219).
		static Color mediumpurple;

		/// \brief <img src="../../img/colors/mediumseagreen-chip.png" width=16 height=16 > rgb( 60, 179, 113).
		static Color mediumseagreen;

		/// \brief <img src="../../img/colors/mediumslateblue-chip.png" width=16 height=16 > rgb(123, 104, 238).
		static Color mediumslateblue;

		/// \brief <img src="../../img/colors/mediumspringgreen-chip.png" width=16 height=16 > rgb( 0, 250, 154).
		static Color mediumspringgreen;

		/// \brief <img src="../../img/colors/mediumturquoise-chip.png" width=16 height=16 > rgb( 72, 209, 204).
		static Color mediumturquoise;

		/// \brief <img src="../../img/colors/mediumvioletred-chip.png" width=16 height=16 > rgb(199, 21, 133).
		static Color mediumvioletred;

		/// \brief <img src="../../img/colors/midnightblue-chip.png" width=16 height=16 > rgb( 25, 25, 112).
		static Color midnightblue;

		/// \brief <img src="../../img/colors/mintcream-chip.png" width=16 height=16 > rgb(245, 255, 250).
		static Color mintcream;

		/// \brief <img src="../../img/colors/mistyrose-chip.png" width=16 height=16 > rgb(255, 228, 225).
		static Color mistyrose;

		/// \brief <img src="../../img/colors/moccasin-chip.png" width=16 height=16 > rgb(255, 228, 181).
		static Color moccasin;

		/// \brief <img src="../../img/colors/navajowhite-chip.png" width=16 height=16 > rgb(255, 222, 173).
		static Color navajowhite;

		/// \brief <img src="../../img/colors/navy-chip.png" width=16 height=16 > rgb( 0, 0, 128).
		static Color navy;

		/// \brief <img src="../../img/colors/oldlace-chip.png" width=16 height=16 > rgb(253, 245, 230).
		static Color oldlace;

		/// \brief <img src="../../img/colors/olive-chip.png" width=16 height=16 > rgb(128, 128, 0).
		static Color olive;

		/// \brief <img src="../../img/colors/olivedrab-chip.png" width=16 height=16 > rgb(107, 142, 35).
		static Color olivedrab;

		/// \brief <img src="../../img/colors/orange-chip.png" width=16 height=16 > rgb(255, 165, 0).
		static Color orange;

		/// \brief <img src="../../img/colors/orangered-chip.png" width=16 height=16 > rgb(255, 69, 0).
		static Color orangered;

		/// \brief <img src="../../img/colors/orchid-chip.png" width=16 height=16 > rgb(218, 112, 214).
		static Color orchid;

		/// \brief <img src="../../img/colors/palegoldenrod-chip.png" width=16 height=16 > rgb(238, 232, 170).
		static Color palegoldenrod;

		/// \brief <img src="../../img/colors/palegreen-chip.png" width=16 height=16 > rgb(152, 251, 152).
		static Color palegreen;

		/// \brief <img src="../../img/colors/paleturquoise-chip.png" width=16 height=16 > rgb(175, 238, 238).
		static Color paleturquoise;

		/// \brief <img src="../../img/colors/palevioletred-chip.png" width=16 height=16 > rgb(219, 112, 147).
		static Color palevioletred;

		/// \brief <img src="../../img/colors/papayawhip-chip.png" width=16 height=16 > rgb(255, 239, 213).
		static Color papayawhip;

		/// \brief <img src="../../img/colors/peachpuff-chip.png" width=16 height=16 > rgb(255, 218, 185).
		static Color peachpuff;

		/// \brief <img src="../../img/colors/peru-chip.png" width=16 height=16 > rgb(205, 133, 63).
		static Color peru;

		/// \brief <img src="../../img/colors/pink-chip.png" width=16 height=16 > rgb(255, 192, 203).
		static Color pink;

		/// \brief <img src="../../img/colors/plum-chip.png" width=16 height=16 > rgb(221, 160, 221).
		static Color plum;

		/// \brief <img src="../../img/colors/powderblue-chip.png" width=16 height=16 > rgb(176, 224, 230).
		static Color powderblue;

		/// \brief <img src="../../img/colors/purple-chip.png" width=16 height=16 > rgb(128, 0, 128).
		static Color purple;

		/// \brief <img src="../../img/colors/red-chip.png" width=16 height=16 > rgb(255, 0, 0).
		static Color red;

		/// \brief <img src="../../img/colors/rosybrown-chip.png" width=16 height=16 > rgb(188, 143, 143).
		static Color rosybrown;

		/// \brief <img src="../../img/colors/royalblue-chip.png" width=16 height=16 > rgb( 65, 105, 225).
		static Color royalblue;

		/// \brief <img src="../../img/colors/saddlebrown-chip.png" width=16 height=16 > rgb(139, 69, 19).
		static Color saddlebrown;

		/// \brief <img src="../../img/colors/salmon-chip.png" width=16 height=16 > rgb(250, 128, 114).
		static Color salmon;

		/// \brief <img src="../../img/colors/sandybrown-chip.png" width=16 height=16 > rgb(244, 164, 96).
		static Color sandybrown;

		/// \brief <img src="../../img/colors/seagreen-chip.png" width=16 height=16 > rgb( 46, 139, 87).
		static Color seagreen;

		/// \brief <img src="../../img/colors/seashell-chip.png" width=16 height=16 > rgb(255, 245, 238).
		static Color seashell;

		/// \brief <img src="../../img/colors/sienna-chip.png" width=16 height=16 > rgb(160, 82, 45).
		static Color sienna;

		/// \brief <img src="../../img/colors/silver-chip.png" width=16 height=16 > rgb(192, 192, 192).
		static Color silver;

		/// \brief <img src="../../img/colors/skyblue-chip.png" width=16 height=16 > rgb(135, 206, 235).
		static Color skyblue;

		/// \brief <img src="../../img/colors/slateblue-chip.png" width=16 height=16 > rgb(106, 90, 205).
		static Color slateblue;

		/// \brief <img src="../../img/colors/slategray-chip.png" width=16 height=16 > rgb(112, 128, 144).
		static Color slategray;

		/// \brief <img src="../../img/colors/slategrey-chip.png" width=16 height=16 > rgb(112, 128, 144).
		static Color slategrey;

		/// \brief <img src="../../img/colors/snow-chip.png" width=16 height=16 > rgb(255, 250, 250).
		static Color snow;

		/// \brief <img src="../../img/colors/springgreen-chip.png" width=16 height=16 > rgb( 0, 255, 127).
		static Color springgreen;

		/// \brief <img src="../../img/colors/steelblue-chip.png" width=16 height=16 > rgb( 70, 130, 180).
		static Color steelblue;

		/// \brief <img src="../../img/colors/tan-chip.png" width=16 height=16 > rgb(210, 180, 140).
		static Color tan;

		/// \brief <img src="../../img/colors/teal-chip.png" width=16 height=16 > rgb( 0, 128, 128).
		static Color teal;

		/// \brief <img src="../../img/colors/thistle-chip.png" width=16 height=16 > rgb(216, 191, 216).
		static Color thistle;

		/// \brief <img src="../../img/colors/tomato-chip.png" width=16 height=16 > rgb(255, 99, 71).
		static Color tomato;

		/// \brief <img src="../../img/colors/turquoise-chip.png" width=16 height=16 > rgb( 64, 224, 208).
		static Color turquoise;

		/// \brief <img src="../../img/colors/violet-chip.png" width=16 height=16 > rgb(238, 130, 238).
		static Color violet;

		/// \brief <img src="../../img/colors/wheat-chip.png" width=16 height=16 > rgb(245, 222, 179).
		static Color wheat;

		/// \brief <img src="../../img/colors/white-chip.png" width=16 height=16 > rgb(255, 255, 255).
		static Color white;

		/// \brief <img src="../../img/colors/whitesmoke-chip.png" width=16 height=16 > rgb(245, 245, 245).
		static Color whitesmoke;

		/// \brief <img src="../../img/colors/yellow-chip.png" width=16 height=16 > rgb(255, 255, 0).
		static Color yellow;

		/// \brief <img src="../../img/colors/yellowgreen-chip.png" width=16 height=16 > rgb(154, 205, 50).
		static Color yellowgreen;

		/// \brief rgba(0, 0, 0, 0).
		static Color transparent;

		static Color gray10;
		static Color gray20;
		static Color gray30;
		static Color gray40;
		static Color gray50;
		static Color gray60;
		static Color gray70;
		static Color gray80;
		static Color gray90;

		static Color _aliceblue() { return Color(240,248,255); }
		static Color _antiquewhite() { return Color(250,235,215); }
		static Color _aqua() { return Color(0,255,255); }
		static Color _aquamarine() { return Color(127,255,212); }
		static Color _azure() { return Color(240,255,255); }
		static Color _beige() { return Color(245,245,220); }
		static Color _bisque() { return Color(255,228,196); }
		static Color _black() { return Color(0,0,0); }
		static Color _blanchedalmond() { return Color(255,235,205); }
		static Color _blue() { return Color(0,0,255); }
		static Color _blueviolet() { return Color(138,43,226); }
		static Color _brown() { return Color(165,42,42); }
		static Color _burlywood() { return Color(222,184,135); }
		static Color _cadetblue() { return Color(95,158,160); }
		static Color _chartreuse() { return Color(127,255,0); }
		static Color _chocolate() { return Color(210,105,30); }
		static Color _coral() { return Color(255,127,80); }
		static Color _cornflowerblue() { return Color(100,149,237); }
		static Color _cornsilk() { return Color(255,248,220); }
		static Color _crimson() { return Color(220,20,60); }
		static Color _cyan() { return Color(0,255,255); }
		static Color _darkblue() { return Color(0,0,139); }
		static Color _darkcyan() { return Color(0,139,139); }
		static Color _darkgoldenrod() { return Color(184,134,11); }
		static Color _darkgray() { return Color(169,169,169); }
		static Color _darkgreen() { return Color(0,100,0); }
		static Color _darkgrey() { return Color(169,169,169); }
		static Color _darkkhaki() { return Color(189,183,107); }
		static Color _darkmagenta() { return Color(139,0,139); }
		static Color _darkolivegreen() { return Color(85,107,47); }
		static Color _darkorange() { return Color(255,140,0); }
		static Color _darkorchid() { return Color(153,50,204); }
		static Color _darkred() { return Color(139,0,0); }
		static Color _darksalmon() { return Color(233,150,122); }
		static Color _darkseagreen() { return Color(143,188,143); }
		static Color _darkslateblue() { return Color(72,61,139); }
		static Color _darkslategray() { return Color(47,79,79); }
		static Color _darkslategrey() { return Color(47,79,79); }
		static Color _darkturquoise() { return Color(0,206,209); }
		static Color _darkviolet() { return Color(148,0,211); }
		static Color _deeppink() { return Color(255,20,147); }
		static Color _deepskyblue() { return Color(0,191,255); }
		static Color _dimgray() { return Color(105,105,105); }
		static Color _dimgrey() { return Color(105,105,105); }
		static Color _dodgerblue() { return Color(30,144,255); }
		static Color _firebrick() { return Color(178,34,34); }
		static Color _floralwhite() { return Color(255,250,240); }
		static Color _forestgreen() { return Color(34,139,34); }
		static Color _fuchsia() { return Color(255,0,255); }
		static Color _gainsboro() { return Color(220,220,220); }
		static Color _ghostwhite() { return Color(248,248,255); }
		static Color _gold() { return Color(255,215,0); }
		static Color _goldenrod() { return Color(218,165,32); }
		static Color _gray() { return Color(128,128,128); }
		static Color _grey() { return Color(128,128,128); }
		static Color _green() { return Color(0,128,0); }
		static Color _greenyellow() { return Color(173,255,47); }
		static Color _honeydew() { return Color(240,255,240); }
		static Color _hotpink() { return Color(255,105,180); }
		static Color _indianred() { return Color(205,92,92); }
		static Color _indigo() { return Color(75,0,130); }
		static Color _ivory() { return Color(255,255,240); }
		static Color _khaki() { return Color(240,230,140); }
		static Color _lavender() { return Color(230,230,250); }
		static Color _lavenderblush() { return Color(255,240,245); }
		static Color _lawngreen() { return Color(124,252,0); }
		static Color _lemonchiffon() { return Color(255,250,205); }
		static Color _lightblue() { return Color(173,216,230); }
		static Color _lightcoral() { return Color(240,128,128); }
		static Color _lightcyan() { return Color(224,255,255); }
		static Color _lightgoldenrodyellow() { return Color(250,250,210); }
		static Color _lightgray() { return Color(211,211,211); }
		static Color _lightgreen() { return Color(144,238,144); }
		static Color _lightgrey() { return Color(211,211,211); }
		static Color _lightpink() { return Color(255,182,193); }
		static Color _lightsalmon() { return Color(255,160,122); }
		static Color _lightseagreen() { return Color(32,178,170); }
		static Color _lightskyblue() { return Color(135,206,250); }
		static Color _lightslategray() { return Color(119,136,153); }
		static Color _lightslategrey() { return Color(119,136,153); }
		static Color _lightsteelblue() { return Color(176,196,222); }
		static Color _lightyellow() { return Color(255,255,224); }
		static Color _lime() { return Color(0,255,0); }
		static Color _limegreen() { return Color(50,205,50); }
		static Color _linen() { return Color(250,240,230); }
		static Color _magenta() { return Color(255,0,255); }
		static Color _maroon() { return Color(128,0,0); }
		static Color _mediumaquamarine() { return Color(102,205,170); }
		static Color _mediumblue() { return Color(0,0,205); }
		static Color _mediumorchid() { return Color(186,85,211); }
		static Color _mediumpurple() { return Color(147,112,219); }
		static Color _mediumseagreen() { return Color(60,179,113); }
		static Color _mediumslateblue() { return Color(123,104,238); }
		static Color _mediumspringgreen() { return Color(0,250,154); }
		static Color _mediumturquoise() { return Color(72,209,204); }
		static Color _mediumvioletred() { return Color(199,21,133); }
		static Color _midnightblue() { return Color(25,25,112); }
		static Color _mintcream() { return Color(245,255,250); }
		static Color _mistyrose() { return Color(255,228,225); }
		static Color _moccasin() { return Color(255,228,181); }
		static Color _navajowhite() { return Color(255,222,173); }
		static Color _navy() { return Color(0,0,128); }
		static Color _oldlace() { return Color(253,245,230); }
		static Color _olive() { return Color(128,128,0); }
		static Color _olivedrab() { return Color(107,142,35); }
		static Color _orange() { return Color(255,165,0); }
		static Color _orangered() { return Color(255,69,0); }
		static Color _orchid() { return Color(218,112,214); }
		static Color _palegoldenrod() { return Color(238,232,170); }
		static Color _palegreen() { return Color(152,251,152); }
		static Color _paleturquoise() { return Color(175,238,238); }
		static Color _palevioletred() { return Color(219,112,147); }
		static Color _papayawhip() { return Color(255,239,213); }
		static Color _peachpuff() { return Color(255,218,185); }
		static Color _peru() { return Color(205,133,63); }
		static Color _pink() { return Color(255,192,203); }
		static Color _plum() { return Color(221,160,221); }
		static Color _powderblue() { return Color(176,224,230); }
		static Color _purple() { return Color(128,0,128); }
		static Color _red() { return Color(255,0,0); }
		static Color _rosybrown() { return Color(188,143,143); }
		static Color _royalblue() { return Color(65,105,225); }
		static Color _saddlebrown() { return Color(139,69,19); }
		static Color _salmon() { return Color(250,128,114); }
		static Color _sandybrown() { return Color(244,164,96); }
		static Color _seagreen() { return Color(46,139,87); }
		static Color _seashell() { return Color(255,245,238); }
		static Color _sienna() { return Color(160,82,45); }
		static Color _silver() { return Color(192,192,192); }
		static Color _skyblue() { return Color(135,206,235); }
		static Color _slateblue() { return Color(106,90,205); }
		static Color _slategray() { return Color(112,128,144); }
		static Color _slategrey() { return Color(112,128,144); }
		static Color _snow() { return Color(255,250,250); }
		static Color _springgreen() { return Color(0,255,127); }
		static Color _steelblue() { return Color(70,130,180); }
		static Color _tan() { return Color(210,180,140); }
		static Color _teal() { return Color(0,128,128); }
		static Color _thistle() { return Color(216,191,216); }
		static Color _tomato() { return Color(255,99,71); }
		static Color _transparent() { return Color(0,0,0,0); }
		static Color _turquoise() { return Color(64,224,208); }
		static Color _violet() { return Color(238,130,238); }
		static Color _wheat() { return Color(245,222,179); }
		static Color _white() { return Color(255,255,255); }
		static Color _whitesmoke() { return Color(245,245,245); }
		static Color _yellow() { return Color(255,255,0); }
		static Color _yellowgreen() { return Color(154,205,50); }
		static Color _gray10() { return Color(26,26,26); }
		static Color _gray20() { return Color(51,51,51); }
		static Color _gray30() { return Color(77,77,77); }
		static Color _gray40() { return Color(104,104,104); }
		static Color _gray50() { return Color(128,128,128); }
		static Color _gray60() { return Color(153,153,153); }
		static Color _gray70() { return Color(178,178,178); }
		static Color _gray80() { return Color(204,204,204); }
		static Color _gray90() { return Color(230,230,230); }

		/// \brief Set alpha color component, in the range 0-255.
		void set_alpha(unsigned char value) { a = value; }

		/// \brief Set red color component, in the range 0-255.
		void set_red(unsigned char value) {  r = value; }

		/// \brief Set green color component, in the range 0-255.
		void set_green(unsigned char value) { g = value; }

		/// \brief Set blue color component, in the range 0-255.
		void set_blue(unsigned char value) { b = value; }

		/// \brief Set alpha color component, in the range 0-1.
		void set_alpha_f(float value) { a = (unsigned char) (value*255.0f); }

		/// \brief Set red color component, in the range 0-1.
		void set_red_f(float value) { r = (unsigned char) (value*255.0f); }

		/// \brief Set green color component, in the range 0-1.
		void set_green_f(float value) { g = (unsigned char) (value*255.0f); }

		/// \brief Set blue color component, in the range 0-1.
		void set_blue_f(float value) { b = (unsigned char) (value*255.0f); }

		/// \brief Set color based on rgba color components in the range 0-255.
		void set_color(unsigned char new_red, unsigned char new_green, unsigned char new_blue, unsigned char new_alpha = 255)
		{ r = new_red; g = new_green; b = new_blue;  a = new_alpha; }

		/// \brief Set color based on rgba color components
		void set_rgba8(unsigned int color);

		/// \brief Set color based on rgba color components
		void set_bgra8(unsigned int color);

		/// \brief Set color based on rgb color components. Alpha is set to 255
		void set_rgb8(unsigned int color);

		/// \brief Set color based on argb color components
		void set_argb8(unsigned int color);

		/// \brief Set color based on rgba color components in the range 0-1.
		void set_colorf(float new_red, float new_green, float new_blue, float new_alpha = 1.0f)
		{
			r = (unsigned char) (new_red * 255.0f);
			g = (unsigned char) (new_green * 255.0f);
			b = (unsigned char) (new_blue * 255.0f);
			a = (unsigned char) (new_alpha * 255.0f);
		}

	/// \}
	};

	/// \brief Floating point color description class (for float).
	class Colorf : public Vec4f
	{
	public:
		/// \brief Constructs a color.
		Colorf() : Vec4f(0.0f, 0.0f, 0.0f, 0.0f)
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
		Colorf(float r, float g, float b, float a = 1.0f) : Vec4f(r,g,b,a)
		{
		}

		/// \brief Constructs a color.
		///
		/// Color components are specified in the range 0 to 1.\n
		/// An alpha value of 0 means complete transparency, while 1 means completely opaque (solid).
		///
		/// \param array_rgba = Red,Green,Blue,Alpha color component.
		Colorf(const float *array_rgba)
			: Vec4f((array_rgba[0]), (array_rgba[1]), (array_rgba[2]), (array_rgba[3]))
		{
		}

		/// \brief Constructs a color.
		///
		/// \param color = The color
		Colorf(const Vec4f &color) : Vec4f(color)
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
		Colorf(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255)
			: Vec4f((r/255.0f), (g/255.0f), (b/255.0f),  (a/255.0f))
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
		Colorf(int r, int g, int b, int a=255)
			: Vec4f((r/255.0f), (g/255.0f), (b/255.0f), (a/255.0f))
		{
		}

		explicit Colorf(const Color& color)
			: Vec4f((color.get_red()/255.0f), (color.get_green()/255.0f), (color.get_blue()/255.0f), (color.get_alpha()/255.0f))
		{
		}

		/// \brief Constructs a color.
		///
		/// \param hexstr The colors rgba components as a hexadecimal string of the format "#rrggbbaa", where the '#' and "aa" parts are optional.
		Colorf(const std::string &hexstr)
		{
			Colorf::find_color(hexstr, *this);
		}

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
		bool operator==(const Colorf &c) const
		{
			return (r == c.r) && (g == c.g) && (b == c.b) && (a == c.a);
		}

		/// \brief Color != Color operator (deep compare)
		bool operator!=(const Colorf &c) const
		{
			return (r != c.r) || (g != c.g) || (b != c.b) || (a != c.a);
		}

		/// \brief Type conversion operator.
		operator Color() const
		{
			return Color(*this);
		}

		/// \brief Find and returns the static color matching a string.
		///
		/// \param name Name of color to match, eg. "blue".
		///
		/// \param out_color =  Matching static color, or transparent (rgba(0,0,0,0)) if there was no match.
		/// \return false = No match found
		static bool find_color(const std::string &name, Colorf &out_color);

		/// \brief <img src="../../img/colors/aliceblue-chip.png" width=16 height=16 > rgb(240, 248, 255).
		static Colorf aliceblue;

		/// \brief <img src="../../img/colors/antiquewhite-chip.png" width=16 height=16 > rgb(250, 235, 215).
		static Colorf antiquewhite;

		/// \brief <img src="../../img/colors/aqua-chip.png" width=16 height=16 > rgb( 0, 255, 255).
		static Colorf aqua;

		/// \brief <img src="../../img/colors/aquamarine-chip.png" width=16 height=16 > rgb(127, 255, 212).
		static Colorf aquamarine;

		/// \brief <img src="../../img/colors/azure-chip.png" width=16 height=16 > rgb(240, 255, 255).
		static Colorf azure;

		/// \brief <img src="../../img/colors/beige-chip.png" width=16 height=16 > rgb(245, 245, 220).
		static Colorf beige;

		/// \brief <img src="../../img/colors/bisque-chip.png" width=16 height=16 > rgb(255, 228, 196).
		static Colorf bisque;

		/// \brief <img src="../../img/colors/black-chip.png" width=16 height=16 > rgb( 0, 0, 0).
		static Colorf black;

		/// \brief <img src="../../img/colors/blanchedalmond-chip.png" width=16 height=16 > rgb(255, 235, 205).
		static Colorf blanchedalmond;

		/// \brief <img src="../../img/colors/blue-chip.png" width=16 height=16 > rgb( 0, 0, 255).
		static Colorf blue;

		/// \brief <img src="../../img/colors/blueviolet-chip.png" width=16 height=16 > rgb(138, 43, 226).
		static Colorf blueviolet;

		/// \brief <img src="../../img/colors/brown-chip.png" width=16 height=16 > rgb(165, 42, 42).
		static Colorf brown;

		/// \brief <img src="../../img/colors/burlywood-chip.png" width=16 height=16 > rgb(222, 184, 135).
		static Colorf burlywood;

		/// \brief <img src="../../img/colors/cadetblue-chip.png" width=16 height=16 > rgb( 95, 158, 160).
		static Colorf cadetblue;

		/// \brief <img src="../../img/colors/chartreuse-chip.png" width=16 height=16 > rgb(127, 255, 0).
		static Colorf chartreuse;

		/// \brief <img src="../../img/colors/chocolate-chip.png" width=16 height=16 > rgb(210, 105, 30).
		static Colorf chocolate;

		/// \brief <img src="../../img/colors/coral-chip.png" width=16 height=16 > rgb(255, 127, 80).
		static Colorf coral;

		/// \brief <img src="../../img/colors/cornflowerblue-chip.png" width=16 height=16 > rgb(100, 149, 237).
		static Colorf cornflowerblue;

		/// \brief <img src="../../img/colors/cornsilk-chip.png" width=16 height=16 > rgb(255, 248, 220).
		static Colorf cornsilk;

		/// \brief <img src="../../img/colors/crimson-chip.png" width=16 height=16 > rgb(220, 20, 60).
		static Colorf crimson;

		/// \brief <img src="../../img/colors/cyan-chip.png" width=16 height=16 > rgb( 0, 255, 255).
		static Colorf cyan;

		/// \brief <img src="../../img/colors/darkblue-chip.png" width=16 height=16 > rgb( 0, 0, 139).
		static Colorf darkblue;

		/// \brief <img src="../../img/colors/darkcyan-chip.png" width=16 height=16 > rgb( 0, 139, 139).
		static Colorf darkcyan;

		/// \brief <img src="../../img/colors/darkgoldenrod-chip.png" width=16 height=16 > rgb(184, 134, 11).
		static Colorf darkgoldenrod;

		/// \brief <img src="../../img/colors/darkgray-chip.png" width=16 height=16 > rgb(169, 169, 169).
		static Colorf darkgray;

		/// \brief <img src="../../img/colors/darkgreen-chip.png" width=16 height=16 > rgb( 0, 100, 0).
		static Colorf darkgreen;

		/// \brief <img src="../../img/colors/darkgrey-chip.png" width=16 height=16 > rgb(169, 169, 169).
		static Colorf darkgrey;

		/// \brief <img src="../../img/colors/darkkhaki-chip.png" width=16 height=16 > rgb(189, 183, 107).
		static Colorf darkkhaki;

		/// \brief <img src="../../img/colors/darkmagenta-chip.png" width=16 height=16 > rgb(139, 0, 139).
		static Colorf darkmagenta;

		/// \brief <img src="../../img/colors/darkolivegreen-chip.png" width=16 height=16 > rgb( 85, 107, 47).
		static Colorf darkolivegreen;

		/// \brief <img src="../../img/colors/darkorange-chip.png" width=16 height=16 > rgb(255, 140, 0).
		static Colorf darkorange;

		/// \brief <img src="../../img/colors/darkorchid-chip.png" width=16 height=16 > rgb(153, 50, 204).
		static Colorf darkorchid;

		/// \brief <img src="../../img/colors/darkred-chip.png" width=16 height=16 > rgb(139, 0, 0).
		static Colorf darkred;

		/// \brief <img src="../../img/colors/darksalmon-chip.png" width=16 height=16 > rgb(233, 150, 122).
		static Colorf darksalmon;

		/// \brief <img src="../../img/colors/darkseagreen-chip.png" width=16 height=16 > rgb(143, 188, 143).
		static Colorf darkseagreen;

		/// \brief <img src="../../img/colors/darkslateblue-chip.png" width=16 height=16 > rgb( 72, 61, 139).
		static Colorf darkslateblue;

		/// \brief <img src="../../img/colors/darkslategray-chip.png" width=16 height=16 > rgb( 47, 79, 79).
		static Colorf darkslategray;

		/// \brief <img src="../../img/colors/darkslategrey-chip.png" width=16 height=16 > rgb( 47, 79, 79).
		static Colorf darkslategrey;

		/// \brief <img src="../../img/colors/darkturquoise-chip.png" width=16 height=16 > rgb( 0, 206, 209).
		static Colorf darkturquoise;

		/// \brief <img src="../../img/colors/darkviolet-chip.png" width=16 height=16 > rgb(148, 0, 211).
		static Colorf darkviolet;

		/// \brief <img src="../../img/colors/deeppink-chip.png" width=16 height=16 > rgb(255, 20, 147).
		static Colorf deeppink;

		/// \brief <img src="../../img/colors/deepskyblue-chip.png" width=16 height=16 > rgb( 0, 191, 255).
		static Colorf deepskyblue;

		/// \brief <img src="../../img/colors/dimgray-chip.png" width=16 height=16 > rgb(105, 105, 105).
		static Colorf dimgray;

		/// \brief <img src="../../img/colors/dimgrey-chip.png" width=16 height=16 > rgb(105, 105, 105).
		static Colorf dimgrey;

		/// \brief <img src="../../img/colors/dodgerblue-chip.png" width=16 height=16 > rgb( 30, 144, 255).
		static Colorf dodgerblue;

		/// \brief <img src="../../img/colors/firebrick-chip.png" width=16 height=16 > rgb(178, 34, 34).
		static Colorf firebrick;

		/// \brief <img src="../../img/colors/floralwhite-chip.png" width=16 height=16 > rgb(255, 250, 240).
		static Colorf floralwhite;

		/// \brief <img src="../../img/colors/forestgreen-chip.png" width=16 height=16 > rgb( 34, 139, 34).
		static Colorf forestgreen;

		/// \brief <img src="../../img/colors/fuchsia-chip.png" width=16 height=16 > rgb(255, 0, 255).
		static Colorf fuchsia;

		/// \brief <img src="../../img/colors/gainsboro-chip.png" width=16 height=16 > rgb(220, 220, 220).
		static Colorf gainsboro;

		/// \brief <img src="../../img/colors/ghostwhite-chip.png" width=16 height=16 > rgb(248, 248, 255).
		static Colorf ghostwhite;

		/// \brief <img src="../../img/colors/gold-chip.png" width=16 height=16 > rgb(255, 215, 0).
		static Colorf gold;

		/// \brief <img src="../../img/colors/goldenrod-chip.png" width=16 height=16 > rgb(218, 165, 32).
		static Colorf goldenrod;

		/// \brief <img src="../../img/colors/gray-chip.png" width=16 height=16 > rgb(128, 128, 128).
		static Colorf gray;

		/// \brief <img src="../../img/colors/grey-chip.png" width=16 height=16 > rgb(128, 128, 128).
		static Colorf grey;

		/// \brief <img src="../../img/colors/green-chip.png" width=16 height=16 > rgb( 0, 128, 0).
		static Colorf green;

		/// \brief <img src="../../img/colors/greenyellow-chip.png" width=16 height=16 > rgb(173, 255, 47).
		static Colorf greenyellow;

		/// \brief <img src="../../img/colors/honeydew-chip.png" width=16 height=16 > rgb(240, 255, 240).
		static Colorf honeydew;

		/// \brief <img src="../../img/colors/hotpink-chip.png" width=16 height=16 > rgb(255, 105, 180).
		static Colorf hotpink;

		/// \brief <img src="../../img/colors/indianred-chip.png" width=16 height=16 > rgb(205, 92, 92).
		static Colorf indianred;

		/// \brief <img src="../../img/colors/indigo-chip.png" width=16 height=16 > rgb( 75, 0, 130).
		static Colorf indigo;

		/// \brief <img src="../../img/colors/ivory-chip.png" width=16 height=16 > rgb(255, 255, 240).
		static Colorf ivory;

		/// \brief <img src="../../img/colors/khaki-chip.png" width=16 height=16 > rgb(240, 230, 140).
		static Colorf khaki;

		/// \brief <img src="../../img/colors/lavender-chip.png" width=16 height=16 > rgb(230, 230, 250).
		static Colorf lavender;

		/// \brief <img src="../../img/colors/lavenderblush-chip.png" width=16 height=16 > rgb(255, 240, 245).
		static Colorf lavenderblush;

		/// \brief <img src="../../img/colors/lawngreen-chip.png" width=16 height=16 > rgb(124, 252, 0).
		static Colorf lawngreen;

		/// \brief <img src="../../img/colors/lemonchiffon-chip.png" width=16 height=16 > rgb(255, 250, 205).
		static Colorf lemonchiffon;

		/// \brief <img src="../../img/colors/lightblue-chip.png" width=16 height=16 > rgb(173, 216, 230).
		static Colorf lightblue;

		/// \brief <img src="../../img/colors/lightcoral-chip.png" width=16 height=16 > rgb(240, 128, 128).
		static Colorf lightcoral;

		/// \brief <img src="../../img/colors/lightcyan-chip.png" width=16 height=16 > rgb(224, 255, 255).
		static Colorf lightcyan;

		/// \brief <img src="../../img/colors/lightgoldenrodyellow-chip.png" width=16 height=16 > rgb(250, 250, 210).
		static Colorf lightgoldenrodyellow;

		/// \brief <img src="../../img/colors/lightgray-chip.png" width=16 height=16 > rgb(211, 211, 211).
		static Colorf lightgray;

		/// \brief <img src="../../img/colors/lightgreen-chip.png" width=16 height=16 > rgb(144, 238, 144).
		static Colorf lightgreen;

		/// \brief <img src="../../img/colors/lightgrey-chip.png" width=16 height=16 > rgb(211, 211, 211).
		static Colorf lightgrey;

		/// \brief <img src="../../img/colors/lightpink-chip.png" width=16 height=16 > rgb(255, 182, 193).
		static Colorf lightpink;

		/// \brief <img src="../../img/colors/lightsalmon-chip.png" width=16 height=16 > rgb(255, 160, 122).
		static Colorf lightsalmon;

		/// \brief <img src="../../img/colors/lightseagreen-chip.png" width=16 height=16 > rgb( 32, 178, 170).
		static Colorf lightseagreen;

		/// \brief <img src="../../img/colors/lightskyblue-chip.png" width=16 height=16 > rgb(135, 206, 250).
		static Colorf lightskyblue;

		/// \brief <img src="../../img/colors/lightslategray-chip.png" width=16 height=16 > rgb(119, 136, 153).
		static Colorf lightslategray;

		/// \brief <img src="../../img/colors/lightslategrey-chip.png" width=16 height=16 > rgb(119, 136, 153).
		static Colorf lightslategrey;

		/// \brief <img src="../../img/colors/lightsteelblue-chip.png" width=16 height=16 > rgb(176, 196, 222).
		static Colorf lightsteelblue;

		/// \brief <img src="../../img/colors/lightyellow-chip.png" width=16 height=16 > rgb(255, 255, 224).
		static Colorf lightyellow;

		/// \brief <img src="../../img/colors/lime-chip.png" width=16 height=16 > rgb( 0, 255, 0).
		static Colorf lime;

		/// \brief <img src="../../img/colors/limegreen-chip.png" width=16 height=16 > rgb( 50, 205, 50).
		static Colorf limegreen;

		/// \brief <img src="../../img/colors/linen-chip.png" width=16 height=16 > rgb(250, 240, 230).
		static Colorf linen;

		/// \brief <img src="../../img/colors/magenta-chip.png" width=16 height=16 > rgb(255, 0, 255).
		static Colorf magenta;

		/// \brief <img src="../../img/colors/maroon-chip.png" width=16 height=16 > rgb(128, 0, 0).
		static Colorf maroon;

		/// \brief <img src="../../img/colors/mediumaquamarine-chip.png" width=16 height=16 > rgb(102, 205, 170).
		static Colorf mediumaquamarine;

		/// \brief <img src="../../img/colors/mediumblue-chip.png" width=16 height=16 > rgb( 0, 0, 205).
		static Colorf mediumblue;

		/// \brief <img src="../../img/colors/mediumorchid-chip.png" width=16 height=16 > rgb(186, 85, 211).
		static Colorf mediumorchid;

		/// \brief <img src="../../img/colors/mediumpurple-chip.png" width=16 height=16 > rgb(147, 112, 219).
		static Colorf mediumpurple;

		/// \brief <img src="../../img/colors/mediumseagreen-chip.png" width=16 height=16 > rgb( 60, 179, 113).
		static Colorf mediumseagreen;

		/// \brief <img src="../../img/colors/mediumslateblue-chip.png" width=16 height=16 > rgb(123, 104, 238).
		static Colorf mediumslateblue;

		/// \brief <img src="../../img/colors/mediumspringgreen-chip.png" width=16 height=16 > rgb( 0, 250, 154).
		static Colorf mediumspringgreen;

		/// \brief <img src="../../img/colors/mediumturquoise-chip.png" width=16 height=16 > rgb( 72, 209, 204).
		static Colorf mediumturquoise;

		/// \brief <img src="../../img/colors/mediumvioletred-chip.png" width=16 height=16 > rgb(199, 21, 133).
		static Colorf mediumvioletred;

		/// \brief <img src="../../img/colors/midnightblue-chip.png" width=16 height=16 > rgb( 25, 25, 112).
		static Colorf midnightblue;

		/// \brief <img src="../../img/colors/mintcream-chip.png" width=16 height=16 > rgb(245, 255, 250).
		static Colorf mintcream;

		/// \brief <img src="../../img/colors/mistyrose-chip.png" width=16 height=16 > rgb(255, 228, 225).
		static Colorf mistyrose;

		/// \brief <img src="../../img/colors/moccasin-chip.png" width=16 height=16 > rgb(255, 228, 181).
		static Colorf moccasin;

		/// \brief <img src="../../img/colors/navajowhite-chip.png" width=16 height=16 > rgb(255, 222, 173).
		static Colorf navajowhite;

		/// \brief <img src="../../img/colors/navy-chip.png" width=16 height=16 > rgb( 0, 0, 128).
		static Colorf navy;

		/// \brief <img src="../../img/colors/oldlace-chip.png" width=16 height=16 > rgb(253, 245, 230).
		static Colorf oldlace;

		/// \brief <img src="../../img/colors/olive-chip.png" width=16 height=16 > rgb(128, 128, 0).
		static Colorf olive;

		/// \brief <img src="../../img/colors/olivedrab-chip.png" width=16 height=16 > rgb(107, 142, 35).
		static Colorf olivedrab;

		/// \brief <img src="../../img/colors/orange-chip.png" width=16 height=16 > rgb(255, 165, 0).
		static Colorf orange;

		/// \brief <img src="../../img/colors/orangered-chip.png" width=16 height=16 > rgb(255, 69, 0).
		static Colorf orangered;

		/// \brief <img src="../../img/colors/orchid-chip.png" width=16 height=16 > rgb(218, 112, 214).
		static Colorf orchid;

		/// \brief <img src="../../img/colors/palegoldenrod-chip.png" width=16 height=16 > rgb(238, 232, 170).
		static Colorf palegoldenrod;

		/// \brief <img src="../../img/colors/palegreen-chip.png" width=16 height=16 > rgb(152, 251, 152).
		static Colorf palegreen;

		/// \brief <img src="../../img/colors/paleturquoise-chip.png" width=16 height=16 > rgb(175, 238, 238).
		static Colorf paleturquoise;

		/// \brief <img src="../../img/colors/palevioletred-chip.png" width=16 height=16 > rgb(219, 112, 147).
		static Colorf palevioletred;

		/// \brief <img src="../../img/colors/papayawhip-chip.png" width=16 height=16 > rgb(255, 239, 213).
		static Colorf papayawhip;

		/// \brief <img src="../../img/colors/peachpuff-chip.png" width=16 height=16 > rgb(255, 218, 185).
		static Colorf peachpuff;

		/// \brief <img src="../../img/colors/peru-chip.png" width=16 height=16 > rgb(205, 133, 63).
		static Colorf peru;

		/// \brief <img src="../../img/colors/pink-chip.png" width=16 height=16 > rgb(255, 192, 203).
		static Colorf pink;

		/// \brief <img src="../../img/colors/plum-chip.png" width=16 height=16 > rgb(221, 160, 221).
		static Colorf plum;

		/// \brief <img src="../../img/colors/powderblue-chip.png" width=16 height=16 > rgb(176, 224, 230).
		static Colorf powderblue;

		/// \brief <img src="../../img/colors/purple-chip.png" width=16 height=16 > rgb(128, 0, 128).
		static Colorf purple;

		/// \brief <img src="../../img/colors/red-chip.png" width=16 height=16 > rgb(255, 0, 0).
		static Colorf red;

		/// \brief <img src="../../img/colors/rosybrown-chip.png" width=16 height=16 > rgb(188, 143, 143).
		static Colorf rosybrown;

		/// \brief <img src="../../img/colors/royalblue-chip.png" width=16 height=16 > rgb( 65, 105, 225).
		static Colorf royalblue;

		/// \brief <img src="../../img/colors/saddlebrown-chip.png" width=16 height=16 > rgb(139, 69, 19).
		static Colorf saddlebrown;

		/// \brief <img src="../../img/colors/salmon-chip.png" width=16 height=16 > rgb(250, 128, 114).
		static Colorf salmon;

		/// \brief <img src="../../img/colors/sandybrown-chip.png" width=16 height=16 > rgb(244, 164, 96).
		static Colorf sandybrown;

		/// \brief <img src="../../img/colors/seagreen-chip.png" width=16 height=16 > rgb( 46, 139, 87).
		static Colorf seagreen;

		/// \brief <img src="../../img/colors/seashell-chip.png" width=16 height=16 > rgb(255, 245, 238).
		static Colorf seashell;

		/// \brief <img src="../../img/colors/sienna-chip.png" width=16 height=16 > rgb(160, 82, 45).
		static Colorf sienna;

		/// \brief <img src="../../img/colors/silver-chip.png" width=16 height=16 > rgb(192, 192, 192).
		static Colorf silver;

		/// \brief <img src="../../img/colors/skyblue-chip.png" width=16 height=16 > rgb(135, 206, 235).
		static Colorf skyblue;

		/// \brief <img src="../../img/colors/slateblue-chip.png" width=16 height=16 > rgb(106, 90, 205).
		static Colorf slateblue;

		/// \brief <img src="../../img/colors/slategray-chip.png" width=16 height=16 > rgb(112, 128, 144).
		static Colorf slategray;

		/// \brief <img src="../../img/colors/slategrey-chip.png" width=16 height=16 > rgb(112, 128, 144).
		static Colorf slategrey;

		/// \brief <img src="../../img/colors/snow-chip.png" width=16 height=16 > rgb(255, 250, 250).
		static Colorf snow;

		/// \brief <img src="../../img/colors/springgreen-chip.png" width=16 height=16 > rgb( 0, 255, 127).
		static Colorf springgreen;

		/// \brief <img src="../../img/colors/steelblue-chip.png" width=16 height=16 > rgb( 70, 130, 180).
		static Colorf steelblue;

		/// \brief <img src="../../img/colors/tan-chip.png" width=16 height=16 > rgb(210, 180, 140).
		static Colorf tan;

		/// \brief <img src="../../img/colors/teal-chip.png" width=16 height=16 > rgb( 0, 128, 128).
		static Colorf teal;

		/// \brief <img src="../../img/colors/thistle-chip.png" width=16 height=16 > rgb(216, 191, 216).
		static Colorf thistle;

		/// \brief <img src="../../img/colors/tomato-chip.png" width=16 height=16 > rgb(255, 99, 71).
		static Colorf tomato;

		/// \brief <img src="../../img/colors/turquoise-chip.png" width=16 height=16 > rgb( 64, 224, 208).
		static Colorf turquoise;

		/// \brief <img src="../../img/colors/violet-chip.png" width=16 height=16 > rgb(238, 130, 238).
		static Colorf violet;

		/// \brief <img src="../../img/colors/wheat-chip.png" width=16 height=16 > rgb(245, 222, 179).
		static Colorf wheat;

		/// \brief <img src="../../img/colors/white-chip.png" width=16 height=16 > rgb(255, 255, 255).
		static Colorf white;

		/// \brief <img src="../../img/colors/whitesmoke-chip.png" width=16 height=16 > rgb(245, 245, 245).
		static Colorf whitesmoke;

		/// \brief <img src="../../img/colors/yellow-chip.png" width=16 height=16 > rgb(255, 255, 0).
		static Colorf yellow;

		/// \brief <img src="../../img/colors/yellowgreen-chip.png" width=16 height=16 > rgb(154, 205, 50).
		static Colorf yellowgreen;

		/// \brief rgba(0, 0, 0, 0).
		static Colorf transparent;

		static Colorf gray10;
		static Colorf gray20;
		static Colorf gray30;
		static Colorf gray40;
		static Colorf gray50;
		static Colorf gray60;
		static Colorf gray70;
		static Colorf gray80;
		static Colorf gray90;

		static Colorf _aliceblue() { return Colorf(40.0f / 255.0f, 248.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf _antiquewhite() { return Colorf(250.0f / 255.0f, 235.0f / 255.0f, 215.0f / 255.0f); }
		static Colorf _aqua() { return Colorf(0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf _aquamarine() { return Colorf(127.0f / 255.0f, 255.0f / 255.0f, 212.0f / 255.0f); }
		static Colorf _azure() { return Colorf(240.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf _beige() { return Colorf(245.0f / 255.0f, 245.0f / 255.0f, 220.0f / 255.0f); }
		static Colorf _bisque() { return Colorf(255.0f / 255.0f, 228.0f / 255.0f, 196.0f / 255.0f); }
		static Colorf _black() { return Colorf(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _blanchedalmond() { return Colorf(255.0f / 255.0f, 235.0f / 255.0f, 205.0f / 255.0f); }
		static Colorf _blue() { return Colorf(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf _blueviolet() { return Colorf(138.0f / 255.0f, 43.0f / 255.0f, 226.0f / 255.0f); }
		static Colorf _brown() { return Colorf(165.0f / 255.0f, 42.0f / 255.0f, 42.0f / 255.0f); }
		static Colorf _burlywood() { return Colorf(222.0f / 255.0f, 184.0f / 255.0f, 135.0f / 255.0f); }
		static Colorf _cadetblue() { return Colorf(95.0f / 255.0f, 158.0f / 255.0f, 160.0f / 255.0f); }
		static Colorf _chartreuse() { return Colorf(127.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _chocolate() { return Colorf(210.0f / 255.0f, 105.0f / 255.0f, 30.0f / 255.0f); }
		static Colorf _coral() { return Colorf(255.0f / 255.0f, 127.0f / 255.0f, 80.0f / 255.0f); }
		static Colorf _cornflowerblue() { return Colorf(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f); }
		static Colorf _cornsilk() { return Colorf(255.0f / 255.0f, 248.0f / 255.0f, 220.0f / 255.0f); }
		static Colorf _crimson() { return Colorf(220.0f / 255.0f, 20.0f / 255.0f, 60.0f / 255.0f); }
		static Colorf _cyan() { return Colorf(0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf _darkblue() { return Colorf(0.0f / 255.0f, 0.0f / 255.0f, 139.0f / 255.0f); }
		static Colorf _darkcyan() { return Colorf(0.0f / 255.0f, 139.0f / 255.0f, 139.0f / 255.0f); }
		static Colorf _darkgoldenrod() { return Colorf(184.0f / 255.0f, 134.0f / 255.0f, 11.0f / 255.0f); }
		static Colorf _darkgray() { return Colorf(169.0f / 255.0f, 169.0f / 255.0f, 169.0f / 255.0f); }
		static Colorf _darkgreen() { return Colorf(0.0f / 255.0f, 100.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _darkgrey() { return Colorf(169.0f / 255.0f, 169.0f / 255.0f, 169.0f / 255.0f); }
		static Colorf _darkkhaki() { return Colorf(189.0f / 255.0f, 183.0f / 255.0f, 107.0f / 255.0f); }
		static Colorf _darkmagenta() { return Colorf(139.0f / 255.0f, 0.0f / 255.0f, 139.0f / 255.0f); }
		static Colorf _darkolivegreen() { return Colorf(85.0f / 255.0f, 107.0f / 255.0f, 47.0f / 255.0f); }
		static Colorf _darkorange() { return Colorf(255.0f / 255.0f, 140.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _darkorchid() { return Colorf(153.0f / 255.0f, 50.0f / 255.0f, 204.0f / 255.0f); }
		static Colorf _darkred() { return Colorf(139.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _darksalmon() { return Colorf(233.0f / 255.0f, 150.0f / 255.0f, 122.0f / 255.0f); }
		static Colorf _darkseagreen() { return Colorf(143.0f / 255.0f, 188.0f / 255.0f, 143.0f / 255.0f); }
		static Colorf _darkslateblue() { return Colorf(72.0f / 255.0f, 61.0f / 255.0f, 139.0f / 255.0f); }
		static Colorf _darkslategray() { return Colorf(47.0f / 255.0f, 79.0f / 255.0f, 79.0f / 255.0f); }
		static Colorf _darkslategrey() { return Colorf(47.0f / 255.0f, 79.0f / 255.0f, 79.0f / 255.0f); }
		static Colorf _darkturquoise() { return Colorf(0.0f / 255.0f, 206.0f / 255.0f, 209.0f / 255.0f); }
		static Colorf _darkviolet() { return Colorf(148.0f / 255.0f, 0.0f / 255.0f, 211.0f / 255.0f); }
		static Colorf _deeppink() { return Colorf(255.0f / 255.0f, 20.0f / 255.0f, 147.0f / 255.0f); }
		static Colorf _deepskyblue() { return Colorf(0.0f / 255.0f, 191.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf _dimgray() { return Colorf(105.0f / 255.0f, 105.0f / 255.0f, 105.0f / 255.0f); }
		static Colorf _dimgrey() { return Colorf(105.0f / 255.0f, 105.0f / 255.0f, 105.0f / 255.0f); }
		static Colorf _dodgerblue() { return Colorf(30.0f / 255.0f, 144.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf _firebrick() { return Colorf(178.0f / 255.0f, 34.0f / 255.0f, 34.0f / 255.0f); }
		static Colorf _floralwhite() { return Colorf(255.0f / 255.0f, 250.0f / 255.0f, 240.0f / 255.0f); }
		static Colorf _forestgreen() { return Colorf(34.0f / 255.0f, 139.0f / 255.0f, 34.0f / 255.0f); }
		static Colorf _fuchsia() { return Colorf(255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf _gainsboro() { return Colorf(220.0f / 255.0f, 220.0f / 255.0f, 220.0f / 255.0f); }
		static Colorf _ghostwhite() { return Colorf(248.0f / 255.0f, 248.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf _gold() { return Colorf(255.0f / 255.0f, 215.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _goldenrod() { return Colorf(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f); }
		static Colorf _gray() { return Colorf(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f); }
		static Colorf _grey() { return Colorf(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f); }
		static Colorf _green() { return Colorf(0.0f / 255.0f, 128.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _greenyellow() { return Colorf(173.0f / 255.0f, 255.0f / 255.0f, 47.0f / 255.0f); }
		static Colorf _honeydew() { return Colorf(240.0f / 255.0f, 255.0f / 255.0f, 240.0f / 255.0f); }
		static Colorf _hotpink() { return Colorf(255.0f / 255.0f, 105.0f / 255.0f, 180.0f / 255.0f); }
		static Colorf _indianred() { return Colorf(205.0f / 255.0f, 92.0f / 255.0f, 92.0f / 255.0f); }
		static Colorf _indigo() { return Colorf(75.0f / 255.0f, 0.0f / 255.0f, 130.0f / 255.0f); }
		static Colorf _ivory() { return Colorf(255.0f / 255.0f, 255.0f / 255.0f, 240.0f / 255.0f); }
		static Colorf _khaki() { return Colorf(240.0f / 255.0f, 230.0f / 255.0f, 140.0f / 255.0f); }
		static Colorf _lavender() { return Colorf(230.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f); }
		static Colorf _lavenderblush() { return Colorf(255.0f / 255.0f, 240.0f / 255.0f, 245.0f / 255.0f); }
		static Colorf _lawngreen() { return Colorf(124.0f / 255.0f, 252.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _lemonchiffon() { return Colorf(255.0f / 255.0f, 250.0f / 255.0f, 205.0f / 255.0f); }
		static Colorf _lightblue() { return Colorf(173.0f / 255.0f, 216.0f / 255.0f, 230.0f / 255.0f); }
		static Colorf _lightcoral() { return Colorf(240.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f); }
		static Colorf _lightcyan() { return Colorf(224.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf _lightgoldenrodyellow() { return Colorf(250.0f / 255.0f, 250.0f / 255.0f, 210.0f / 255.0f); }
		static Colorf _lightgray() { return Colorf(211.0f / 255.0f, 211.0f / 255.0f, 211.0f / 255.0f); }
		static Colorf _lightgreen() { return Colorf(144.0f / 255.0f, 238.0f / 255.0f, 144.0f / 255.0f); }
		static Colorf _lightgrey() { return Colorf(211.0f / 255.0f, 211.0f / 255.0f, 211.0f / 255.0f); }
		static Colorf _lightpink() { return Colorf(255.0f / 255.0f, 182.0f / 255.0f, 193.0f / 255.0f); }
		static Colorf _lightsalmon() { return Colorf(255.0f / 255.0f, 160.0f / 255.0f, 122.0f / 255.0f); }
		static Colorf _lightseagreen() { return Colorf(32.0f / 255.0f, 178.0f / 255.0f, 170.0f / 255.0f); }
		static Colorf _lightskyblue() { return Colorf(135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f); }
		static Colorf _lightslategray() { return Colorf(119.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f); }
		static Colorf _lightslategrey() { return Colorf(119.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f); }
		static Colorf _lightsteelblue() { return Colorf(176.0f / 255.0f, 196.0f / 255.0f, 222.0f / 255.0f); }
		static Colorf _lightyellow() { return Colorf(255.0f / 255.0f, 255.0f / 255.0f, 224.0f / 255.0f); }
		static Colorf _lime() { return Colorf(0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _limegreen() { return Colorf(50.0f / 255.0f, 205.0f / 255.0f, 50.0f / 255.0f); }
		static Colorf _linen() { return Colorf(250.0f / 255.0f, 240.0f / 255.0f, 230.0f / 255.0f); }
		static Colorf _magenta() { return Colorf(255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf _maroon() { return Colorf(128.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _mediumaquamarine() { return Colorf(102.0f / 255.0f, 205.0f / 255.0f, 170.0f / 255.0f); }
		static Colorf _mediumblue() { return Colorf(0.0f / 255.0f, 0.0f / 255.0f, 205.0f / 255.0f); }
		static Colorf _mediumorchid() { return Colorf(186.0f / 255.0f, 85.0f / 255.0f, 211.0f / 255.0f); }
		static Colorf _mediumpurple() { return Colorf(147.0f / 255.0f, 112.0f / 255.0f, 219.0f / 255.0f); }
		static Colorf _mediumseagreen() { return Colorf(60.0f / 255.0f, 179.0f / 255.0f, 113.0f / 255.0f); }
		static Colorf _mediumslateblue() { return Colorf(123.0f / 255.0f, 104.0f / 255.0f, 238.0f / 255.0f); }
		static Colorf _mediumspringgreen() { return Colorf(0.0f / 255.0f, 250.0f / 255.0f, 154.0f / 255.0f); }
		static Colorf _mediumturquoise() { return Colorf(72.0f / 255.0f, 209.0f / 255.0f, 204.0f / 255.0f); }
		static Colorf _mediumvioletred() { return Colorf(199.0f / 255.0f, 21.0f / 255.0f, 133.0f / 255.0f); }
		static Colorf _midnightblue() { return Colorf(25.0f / 255.0f, 25.0f / 255.0f, 112.0f / 255.0f); }
		static Colorf _mintcream() { return Colorf(245.0f / 255.0f, 255.0f / 255.0f, 250.0f / 255.0f); }
		static Colorf _mistyrose() { return Colorf(255.0f / 255.0f, 228.0f / 255.0f, 225.0f / 255.0f); }
		static Colorf _moccasin() { return Colorf(255.0f / 255.0f, 228.0f / 255.0f, 181.0f / 255.0f); }
		static Colorf _navajowhite() { return Colorf(255.0f / 255.0f, 222.0f / 255.0f, 173.0f / 255.0f); }
		static Colorf _navy() { return Colorf(0.0f / 255.0f, 0.0f / 255.0f, 128.0f / 255.0f); }
		static Colorf _oldlace() { return Colorf(253.0f / 255.0f, 245.0f / 255.0f, 230.0f / 255.0f); }
		static Colorf _olive() { return Colorf(128.0f / 255.0f, 128.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _olivedrab() { return Colorf(107.0f / 255.0f, 142.0f / 255.0f, 35.0f / 255.0f); }
		static Colorf _orange() { return Colorf(255.0f / 255.0f, 165.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _orangered() { return Colorf(255.0f / 255.0f, 69.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _orchid() { return Colorf(218.0f / 255.0f, 112.0f / 255.0f, 214.0f / 255.0f); }
		static Colorf _palegoldenrod() { return Colorf(238.0f / 255.0f, 232.0f / 255.0f, 170.0f / 255.0f); }
		static Colorf _palegreen() { return Colorf(152.0f / 255.0f, 251.0f / 255.0f, 152.0f / 255.0f); }
		static Colorf _paleturquoise() { return Colorf(175.0f / 255.0f, 238.0f / 255.0f, 238.0f / 255.0f); }
		static Colorf _palevioletred() { return Colorf(219.0f / 255.0f, 112.0f / 255.0f, 147.0f / 255.0f); }
		static Colorf _papayawhip() { return Colorf(255.0f / 255.0f, 239.0f / 255.0f, 213.0f / 255.0f); }
		static Colorf _peachpuff() { return Colorf(255.0f / 255.0f, 218.0f / 255.0f, 185.0f / 255.0f); }
		static Colorf _peru() { return Colorf(205.0f / 255.0f, 133.0f / 255.0f, 63.0f / 255.0f); }
		static Colorf _pink() { return Colorf(255.0f / 255.0f, 192.0f / 255.0f, 203.0f / 255.0f); }
		static Colorf _plum() { return Colorf(221.0f / 255.0f, 160.0f / 255.0f, 221.0f / 255.0f); }
		static Colorf _powderblue() { return Colorf(176.0f / 255.0f, 224.0f / 255.0f, 230.0f / 255.0f); }
		static Colorf _purple() { return Colorf(128.0f / 255.0f, 0.0f / 255.0f, 128.0f / 255.0f); }
		static Colorf _red() { return Colorf(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _rosybrown() { return Colorf(188.0f / 255.0f, 143.0f / 255.0f, 143.0f / 255.0f); }
		static Colorf _royalblue() { return Colorf(65.0f / 255.0f, 105.0f / 255.0f, 225.0f / 255.0f); }
		static Colorf _saddlebrown() { return Colorf(139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f); }
		static Colorf _salmon() { return Colorf(250.0f / 255.0f, 128.0f / 255.0f, 114.0f / 255.0f); }
		static Colorf _sandybrown() { return Colorf(244.0f / 255.0f, 164.0f / 255.0f, 96.0f / 255.0f); }
		static Colorf _seagreen() { return Colorf(46.0f / 255.0f, 139.0f / 255.0f, 87.0f / 255.0f); }
		static Colorf _seashell() { return Colorf(255.0f / 255.0f, 245.0f / 255.0f, 238.0f / 255.0f); }
		static Colorf _sienna() { return Colorf(160.0f / 255.0f, 82.0f / 255.0f, 45.0f / 255.0f); }
		static Colorf _silver() { return Colorf(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f); }
		static Colorf _skyblue() { return Colorf(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f); }
		static Colorf _slateblue() { return Colorf(106.0f / 255.0f, 90.0f / 255.0f, 205.0f / 255.0f); }
		static Colorf _slategray() { return Colorf(112.0f / 255.0f, 128.0f / 255.0f, 144.0f / 255.0f); }
		static Colorf _slategrey() { return Colorf(112.0f / 255.0f, 128.0f / 255.0f, 144.0f / 255.0f); }
		static Colorf _snow() { return Colorf(255.0f / 255.0f, 250.0f / 255.0f, 250.0f / 255.0f); }
		static Colorf _springgreen() { return Colorf(0.0f / 255.0f, 255.0f / 255.0f, 127.0f / 255.0f); }
		static Colorf _steelblue() { return Colorf(70.0f / 255.0f, 130.0f / 255.0f, 180.0f / 255.0f); }
		static Colorf _tan() { return Colorf(210.0f / 255.0f, 180.0f / 255.0f, 140.0f / 255.0f); }
		static Colorf _teal() { return Colorf(0.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f); }
		static Colorf _thistle() { return Colorf(216.0f / 255.0f, 191.0f / 255.0f, 216.0f / 255.0f); }
		static Colorf _tomato() { return Colorf(255.0f / 255.0f, 99.0f / 255.0f, 71.0f / 255.0f); }
		static Colorf _transparent() { return Colorf(0.0f, 0.0f, 0.0f, 0.0f); }
		static Colorf _turquoise() { return Colorf(64.0f / 255.0f, 224.0f / 255.0f, 208.0f / 255.0f); }
		static Colorf _violet() { return Colorf(238.0f / 255.0f, 130.0f / 255.0f, 238.0f / 255.0f); }
		static Colorf _wheat() { return Colorf(245.0f / 255.0f, 222.0f / 255.0f, 179.0f / 255.0f); }
		static Colorf _white() { return Colorf(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf _whitesmoke() { return Colorf(245.0f / 255.0f, 245.0f / 255.0f, 245.0f / 255.0f); }
		static Colorf _yellow() { return Colorf(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf _yellowgreen() { return Colorf(154.0f / 255.0f, 205.0f / 255.0f, 50.0f / 255.0f); }
		static Colorf _gray10() { return Colorf(0.1f, 0.1f, 0.1f); }
		static Colorf _gray20() { return Colorf(0.2f, 0.2f, 0.2f); }
		static Colorf _gray30() { return Colorf(0.3f, 0.3f, 0.3f); }
		static Colorf _gray40() { return Colorf(0.4f, 0.4f, 0.4f); }
		static Colorf _gray50() { return Colorf(0.5f, 0.5f, 0.5f); }
		static Colorf _gray60() { return Colorf(0.6f, 0.6f, 0.6f); }
		static Colorf _gray70() { return Colorf(0.7f, 0.7f, 0.7f); }
		static Colorf _gray80() { return Colorf(0.8f, 0.8f, 0.8f); }
		static Colorf _gray90() { return Colorf(0.9f, 0.9f, 0.9f); }
	};

	/// \}
}
