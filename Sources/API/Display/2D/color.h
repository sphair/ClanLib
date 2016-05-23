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
	};

	/// Standard X11/HTML named colors
	class StandardColor
	{
	public:
		static Color aliceblue() { return Color(240, 248, 255); }
		static Color antiquewhite() { return Color(250, 235, 215); }
		static Color aqua() { return Color(0, 255, 255); }
		static Color aquamarine() { return Color(127, 255, 212); }
		static Color azure() { return Color(240, 255, 255); }
		static Color beige() { return Color(245, 245, 220); }
		static Color bisque() { return Color(255, 228, 196); }
		static Color black() { return Color(0, 0, 0); }
		static Color blanchedalmond() { return Color(255, 235, 205); }
		static Color blue() { return Color(0, 0, 255); }
		static Color blueviolet() { return Color(138, 43, 226); }
		static Color brown() { return Color(165, 42, 42); }
		static Color burlywood() { return Color(222, 184, 135); }
		static Color cadetblue() { return Color(95, 158, 160); }
		static Color chartreuse() { return Color(127, 255, 0); }
		static Color chocolate() { return Color(210, 105, 30); }
		static Color coral() { return Color(255, 127, 80); }
		static Color cornflowerblue() { return Color(100, 149, 237); }
		static Color cornsilk() { return Color(255, 248, 220); }
		static Color crimson() { return Color(220, 20, 60); }
		static Color cyan() { return Color(0, 255, 255); }
		static Color darkblue() { return Color(0, 0, 139); }
		static Color darkcyan() { return Color(0, 139, 139); }
		static Color darkgoldenrod() { return Color(184, 134, 11); }
		static Color darkgray() { return Color(169, 169, 169); }
		static Color darkgreen() { return Color(0, 100, 0); }
		static Color darkgrey() { return Color(169, 169, 169); }
		static Color darkkhaki() { return Color(189, 183, 107); }
		static Color darkmagenta() { return Color(139, 0, 139); }
		static Color darkolivegreen() { return Color(85, 107, 47); }
		static Color darkorange() { return Color(255, 140, 0); }
		static Color darkorchid() { return Color(153, 50, 204); }
		static Color darkred() { return Color(139, 0, 0); }
		static Color darksalmon() { return Color(233, 150, 122); }
		static Color darkseagreen() { return Color(143, 188, 143); }
		static Color darkslateblue() { return Color(72, 61, 139); }
		static Color darkslategray() { return Color(47, 79, 79); }
		static Color darkslategrey() { return Color(47, 79, 79); }
		static Color darkturquoise() { return Color(0, 206, 209); }
		static Color darkviolet() { return Color(148, 0, 211); }
		static Color deeppink() { return Color(255, 20, 147); }
		static Color deepskyblue() { return Color(0, 191, 255); }
		static Color dimgray() { return Color(105, 105, 105); }
		static Color dimgrey() { return Color(105, 105, 105); }
		static Color dodgerblue() { return Color(30, 144, 255); }
		static Color firebrick() { return Color(178, 34, 34); }
		static Color floralwhite() { return Color(255, 250, 240); }
		static Color forestgreen() { return Color(34, 139, 34); }
		static Color fuchsia() { return Color(255, 0, 255); }
		static Color gainsboro() { return Color(220, 220, 220); }
		static Color ghostwhite() { return Color(248, 248, 255); }
		static Color gold() { return Color(255, 215, 0); }
		static Color goldenrod() { return Color(218, 165, 32); }
		static Color gray() { return Color(128, 128, 128); }
		static Color grey() { return Color(128, 128, 128); }
		static Color green() { return Color(0, 128, 0); }
		static Color greenyellow() { return Color(173, 255, 47); }
		static Color honeydew() { return Color(240, 255, 240); }
		static Color hotpink() { return Color(255, 105, 180); }
		static Color indianred() { return Color(205, 92, 92); }
		static Color indigo() { return Color(75, 0, 130); }
		static Color ivory() { return Color(255, 255, 240); }
		static Color khaki() { return Color(240, 230, 140); }
		static Color lavender() { return Color(230, 230, 250); }
		static Color lavenderblush() { return Color(255, 240, 245); }
		static Color lawngreen() { return Color(124, 252, 0); }
		static Color lemonchiffon() { return Color(255, 250, 205); }
		static Color lightblue() { return Color(173, 216, 230); }
		static Color lightcoral() { return Color(240, 128, 128); }
		static Color lightcyan() { return Color(224, 255, 255); }
		static Color lightgoldenrodyellow() { return Color(250, 250, 210); }
		static Color lightgray() { return Color(211, 211, 211); }
		static Color lightgreen() { return Color(144, 238, 144); }
		static Color lightgrey() { return Color(211, 211, 211); }
		static Color lightpink() { return Color(255, 182, 193); }
		static Color lightsalmon() { return Color(255, 160, 122); }
		static Color lightseagreen() { return Color(32, 178, 170); }
		static Color lightskyblue() { return Color(135, 206, 250); }
		static Color lightslategray() { return Color(119, 136, 153); }
		static Color lightslategrey() { return Color(119, 136, 153); }
		static Color lightsteelblue() { return Color(176, 196, 222); }
		static Color lightyellow() { return Color(255, 255, 224); }
		static Color lime() { return Color(0, 255, 0); }
		static Color limegreen() { return Color(50, 205, 50); }
		static Color linen() { return Color(250, 240, 230); }
		static Color magenta() { return Color(255, 0, 255); }
		static Color maroon() { return Color(128, 0, 0); }
		static Color mediumaquamarine() { return Color(102, 205, 170); }
		static Color mediumblue() { return Color(0, 0, 205); }
		static Color mediumorchid() { return Color(186, 85, 211); }
		static Color mediumpurple() { return Color(147, 112, 219); }
		static Color mediumseagreen() { return Color(60, 179, 113); }
		static Color mediumslateblue() { return Color(123, 104, 238); }
		static Color mediumspringgreen() { return Color(0, 250, 154); }
		static Color mediumturquoise() { return Color(72, 209, 204); }
		static Color mediumvioletred() { return Color(199, 21, 133); }
		static Color midnightblue() { return Color(25, 25, 112); }
		static Color mintcream() { return Color(245, 255, 250); }
		static Color mistyrose() { return Color(255, 228, 225); }
		static Color moccasin() { return Color(255, 228, 181); }
		static Color navajowhite() { return Color(255, 222, 173); }
		static Color navy() { return Color(0, 0, 128); }
		static Color oldlace() { return Color(253, 245, 230); }
		static Color olive() { return Color(128, 128, 0); }
		static Color olivedrab() { return Color(107, 142, 35); }
		static Color orange() { return Color(255, 165, 0); }
		static Color orangered() { return Color(255, 69, 0); }
		static Color orchid() { return Color(218, 112, 214); }
		static Color palegoldenrod() { return Color(238, 232, 170); }
		static Color palegreen() { return Color(152, 251, 152); }
		static Color paleturquoise() { return Color(175, 238, 238); }
		static Color palevioletred() { return Color(219, 112, 147); }
		static Color papayawhip() { return Color(255, 239, 213); }
		static Color peachpuff() { return Color(255, 218, 185); }
		static Color peru() { return Color(205, 133, 63); }
		static Color pink() { return Color(255, 192, 203); }
		static Color plum() { return Color(221, 160, 221); }
		static Color powderblue() { return Color(176, 224, 230); }
		static Color purple() { return Color(128, 0, 128); }
		static Color red() { return Color(255, 0, 0); }
		static Color rosybrown() { return Color(188, 143, 143); }
		static Color royalblue() { return Color(65, 105, 225); }
		static Color saddlebrown() { return Color(139, 69, 19); }
		static Color salmon() { return Color(250, 128, 114); }
		static Color sandybrown() { return Color(244, 164, 96); }
		static Color seagreen() { return Color(46, 139, 87); }
		static Color seashell() { return Color(255, 245, 238); }
		static Color sienna() { return Color(160, 82, 45); }
		static Color silver() { return Color(192, 192, 192); }
		static Color skyblue() { return Color(135, 206, 235); }
		static Color slateblue() { return Color(106, 90, 205); }
		static Color slategray() { return Color(112, 128, 144); }
		static Color slategrey() { return Color(112, 128, 144); }
		static Color snow() { return Color(255, 250, 250); }
		static Color springgreen() { return Color(0, 255, 127); }
		static Color steelblue() { return Color(70, 130, 180); }
		static Color tan() { return Color(210, 180, 140); }
		static Color teal() { return Color(0, 128, 128); }
		static Color thistle() { return Color(216, 191, 216); }
		static Color tomato() { return Color(255, 99, 71); }
		static Color transparent() { return Color(0, 0, 0, 0); }
		static Color turquoise() { return Color(64, 224, 208); }
		static Color violet() { return Color(238, 130, 238); }
		static Color wheat() { return Color(245, 222, 179); }
		static Color white() { return Color(255, 255, 255); }
		static Color whitesmoke() { return Color(245, 245, 245); }
		static Color yellow() { return Color(255, 255, 0); }
		static Color yellowgreen() { return Color(154, 205, 50); }
		static Color gray10() { return Color(26, 26, 26); }
		static Color gray20() { return Color(51, 51, 51); }
		static Color gray30() { return Color(77, 77, 77); }
		static Color gray40() { return Color(104, 104, 104); }
		static Color gray50() { return Color(128, 128, 128); }
		static Color gray60() { return Color(153, 153, 153); }
		static Color gray70() { return Color(178, 178, 178); }
		static Color gray80() { return Color(204, 204, 204); }
		static Color gray90() { return Color(230, 230, 230); }
	};

	/// Standard X11/HTML named colors (for float)
	class StandardColorf
	{
	public:
		/// \brief Find and returns the static color matching a string.
		///
		/// \param name Name of color to match, eg. "blue".
		///
		/// \param out_color =  Matching static color, or transparent (rgba(0,0,0,0)) if there was no match.
		/// \return false = No match found
		static bool parse(const std::string &name, Colorf &out_color);

		static Colorf aliceblue() { return Colorf(40.0f / 255.0f, 248.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf antiquewhite() { return Colorf(250.0f / 255.0f, 235.0f / 255.0f, 215.0f / 255.0f); }
		static Colorf aqua() { return Colorf(0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf aquamarine() { return Colorf(127.0f / 255.0f, 255.0f / 255.0f, 212.0f / 255.0f); }
		static Colorf azure() { return Colorf(240.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf beige() { return Colorf(245.0f / 255.0f, 245.0f / 255.0f, 220.0f / 255.0f); }
		static Colorf bisque() { return Colorf(255.0f / 255.0f, 228.0f / 255.0f, 196.0f / 255.0f); }
		static Colorf black() { return Colorf(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf blanchedalmond() { return Colorf(255.0f / 255.0f, 235.0f / 255.0f, 205.0f / 255.0f); }
		static Colorf blue() { return Colorf(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf blueviolet() { return Colorf(138.0f / 255.0f, 43.0f / 255.0f, 226.0f / 255.0f); }
		static Colorf brown() { return Colorf(165.0f / 255.0f, 42.0f / 255.0f, 42.0f / 255.0f); }
		static Colorf burlywood() { return Colorf(222.0f / 255.0f, 184.0f / 255.0f, 135.0f / 255.0f); }
		static Colorf cadetblue() { return Colorf(95.0f / 255.0f, 158.0f / 255.0f, 160.0f / 255.0f); }
		static Colorf chartreuse() { return Colorf(127.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf chocolate() { return Colorf(210.0f / 255.0f, 105.0f / 255.0f, 30.0f / 255.0f); }
		static Colorf coral() { return Colorf(255.0f / 255.0f, 127.0f / 255.0f, 80.0f / 255.0f); }
		static Colorf cornflowerblue() { return Colorf(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f); }
		static Colorf cornsilk() { return Colorf(255.0f / 255.0f, 248.0f / 255.0f, 220.0f / 255.0f); }
		static Colorf crimson() { return Colorf(220.0f / 255.0f, 20.0f / 255.0f, 60.0f / 255.0f); }
		static Colorf cyan() { return Colorf(0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf darkblue() { return Colorf(0.0f / 255.0f, 0.0f / 255.0f, 139.0f / 255.0f); }
		static Colorf darkcyan() { return Colorf(0.0f / 255.0f, 139.0f / 255.0f, 139.0f / 255.0f); }
		static Colorf darkgoldenrod() { return Colorf(184.0f / 255.0f, 134.0f / 255.0f, 11.0f / 255.0f); }
		static Colorf darkgray() { return Colorf(169.0f / 255.0f, 169.0f / 255.0f, 169.0f / 255.0f); }
		static Colorf darkgreen() { return Colorf(0.0f / 255.0f, 100.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf darkgrey() { return Colorf(169.0f / 255.0f, 169.0f / 255.0f, 169.0f / 255.0f); }
		static Colorf darkkhaki() { return Colorf(189.0f / 255.0f, 183.0f / 255.0f, 107.0f / 255.0f); }
		static Colorf darkmagenta() { return Colorf(139.0f / 255.0f, 0.0f / 255.0f, 139.0f / 255.0f); }
		static Colorf darkolivegreen() { return Colorf(85.0f / 255.0f, 107.0f / 255.0f, 47.0f / 255.0f); }
		static Colorf darkorange() { return Colorf(255.0f / 255.0f, 140.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf darkorchid() { return Colorf(153.0f / 255.0f, 50.0f / 255.0f, 204.0f / 255.0f); }
		static Colorf darkred() { return Colorf(139.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf darksalmon() { return Colorf(233.0f / 255.0f, 150.0f / 255.0f, 122.0f / 255.0f); }
		static Colorf darkseagreen() { return Colorf(143.0f / 255.0f, 188.0f / 255.0f, 143.0f / 255.0f); }
		static Colorf darkslateblue() { return Colorf(72.0f / 255.0f, 61.0f / 255.0f, 139.0f / 255.0f); }
		static Colorf darkslategray() { return Colorf(47.0f / 255.0f, 79.0f / 255.0f, 79.0f / 255.0f); }
		static Colorf darkslategrey() { return Colorf(47.0f / 255.0f, 79.0f / 255.0f, 79.0f / 255.0f); }
		static Colorf darkturquoise() { return Colorf(0.0f / 255.0f, 206.0f / 255.0f, 209.0f / 255.0f); }
		static Colorf darkviolet() { return Colorf(148.0f / 255.0f, 0.0f / 255.0f, 211.0f / 255.0f); }
		static Colorf deeppink() { return Colorf(255.0f / 255.0f, 20.0f / 255.0f, 147.0f / 255.0f); }
		static Colorf deepskyblue() { return Colorf(0.0f / 255.0f, 191.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf dimgray() { return Colorf(105.0f / 255.0f, 105.0f / 255.0f, 105.0f / 255.0f); }
		static Colorf dimgrey() { return Colorf(105.0f / 255.0f, 105.0f / 255.0f, 105.0f / 255.0f); }
		static Colorf dodgerblue() { return Colorf(30.0f / 255.0f, 144.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf firebrick() { return Colorf(178.0f / 255.0f, 34.0f / 255.0f, 34.0f / 255.0f); }
		static Colorf floralwhite() { return Colorf(255.0f / 255.0f, 250.0f / 255.0f, 240.0f / 255.0f); }
		static Colorf forestgreen() { return Colorf(34.0f / 255.0f, 139.0f / 255.0f, 34.0f / 255.0f); }
		static Colorf fuchsia() { return Colorf(255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf gainsboro() { return Colorf(220.0f / 255.0f, 220.0f / 255.0f, 220.0f / 255.0f); }
		static Colorf ghostwhite() { return Colorf(248.0f / 255.0f, 248.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf gold() { return Colorf(255.0f / 255.0f, 215.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf goldenrod() { return Colorf(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f); }
		static Colorf gray() { return Colorf(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f); }
		static Colorf grey() { return Colorf(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f); }
		static Colorf green() { return Colorf(0.0f / 255.0f, 128.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf greenyellow() { return Colorf(173.0f / 255.0f, 255.0f / 255.0f, 47.0f / 255.0f); }
		static Colorf honeydew() { return Colorf(240.0f / 255.0f, 255.0f / 255.0f, 240.0f / 255.0f); }
		static Colorf hotpink() { return Colorf(255.0f / 255.0f, 105.0f / 255.0f, 180.0f / 255.0f); }
		static Colorf indianred() { return Colorf(205.0f / 255.0f, 92.0f / 255.0f, 92.0f / 255.0f); }
		static Colorf indigo() { return Colorf(75.0f / 255.0f, 0.0f / 255.0f, 130.0f / 255.0f); }
		static Colorf ivory() { return Colorf(255.0f / 255.0f, 255.0f / 255.0f, 240.0f / 255.0f); }
		static Colorf khaki() { return Colorf(240.0f / 255.0f, 230.0f / 255.0f, 140.0f / 255.0f); }
		static Colorf lavender() { return Colorf(230.0f / 255.0f, 230.0f / 255.0f, 250.0f / 255.0f); }
		static Colorf lavenderblush() { return Colorf(255.0f / 255.0f, 240.0f / 255.0f, 245.0f / 255.0f); }
		static Colorf lawngreen() { return Colorf(124.0f / 255.0f, 252.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf lemonchiffon() { return Colorf(255.0f / 255.0f, 250.0f / 255.0f, 205.0f / 255.0f); }
		static Colorf lightblue() { return Colorf(173.0f / 255.0f, 216.0f / 255.0f, 230.0f / 255.0f); }
		static Colorf lightcoral() { return Colorf(240.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f); }
		static Colorf lightcyan() { return Colorf(224.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf lightgoldenrodyellow() { return Colorf(250.0f / 255.0f, 250.0f / 255.0f, 210.0f / 255.0f); }
		static Colorf lightgray() { return Colorf(211.0f / 255.0f, 211.0f / 255.0f, 211.0f / 255.0f); }
		static Colorf lightgreen() { return Colorf(144.0f / 255.0f, 238.0f / 255.0f, 144.0f / 255.0f); }
		static Colorf lightgrey() { return Colorf(211.0f / 255.0f, 211.0f / 255.0f, 211.0f / 255.0f); }
		static Colorf lightpink() { return Colorf(255.0f / 255.0f, 182.0f / 255.0f, 193.0f / 255.0f); }
		static Colorf lightsalmon() { return Colorf(255.0f / 255.0f, 160.0f / 255.0f, 122.0f / 255.0f); }
		static Colorf lightseagreen() { return Colorf(32.0f / 255.0f, 178.0f / 255.0f, 170.0f / 255.0f); }
		static Colorf lightskyblue() { return Colorf(135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f); }
		static Colorf lightslategray() { return Colorf(119.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f); }
		static Colorf lightslategrey() { return Colorf(119.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f); }
		static Colorf lightsteelblue() { return Colorf(176.0f / 255.0f, 196.0f / 255.0f, 222.0f / 255.0f); }
		static Colorf lightyellow() { return Colorf(255.0f / 255.0f, 255.0f / 255.0f, 224.0f / 255.0f); }
		static Colorf lime() { return Colorf(0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf limegreen() { return Colorf(50.0f / 255.0f, 205.0f / 255.0f, 50.0f / 255.0f); }
		static Colorf linen() { return Colorf(250.0f / 255.0f, 240.0f / 255.0f, 230.0f / 255.0f); }
		static Colorf magenta() { return Colorf(255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf maroon() { return Colorf(128.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf mediumaquamarine() { return Colorf(102.0f / 255.0f, 205.0f / 255.0f, 170.0f / 255.0f); }
		static Colorf mediumblue() { return Colorf(0.0f / 255.0f, 0.0f / 255.0f, 205.0f / 255.0f); }
		static Colorf mediumorchid() { return Colorf(186.0f / 255.0f, 85.0f / 255.0f, 211.0f / 255.0f); }
		static Colorf mediumpurple() { return Colorf(147.0f / 255.0f, 112.0f / 255.0f, 219.0f / 255.0f); }
		static Colorf mediumseagreen() { return Colorf(60.0f / 255.0f, 179.0f / 255.0f, 113.0f / 255.0f); }
		static Colorf mediumslateblue() { return Colorf(123.0f / 255.0f, 104.0f / 255.0f, 238.0f / 255.0f); }
		static Colorf mediumspringgreen() { return Colorf(0.0f / 255.0f, 250.0f / 255.0f, 154.0f / 255.0f); }
		static Colorf mediumturquoise() { return Colorf(72.0f / 255.0f, 209.0f / 255.0f, 204.0f / 255.0f); }
		static Colorf mediumvioletred() { return Colorf(199.0f / 255.0f, 21.0f / 255.0f, 133.0f / 255.0f); }
		static Colorf midnightblue() { return Colorf(25.0f / 255.0f, 25.0f / 255.0f, 112.0f / 255.0f); }
		static Colorf mintcream() { return Colorf(245.0f / 255.0f, 255.0f / 255.0f, 250.0f / 255.0f); }
		static Colorf mistyrose() { return Colorf(255.0f / 255.0f, 228.0f / 255.0f, 225.0f / 255.0f); }
		static Colorf moccasin() { return Colorf(255.0f / 255.0f, 228.0f / 255.0f, 181.0f / 255.0f); }
		static Colorf navajowhite() { return Colorf(255.0f / 255.0f, 222.0f / 255.0f, 173.0f / 255.0f); }
		static Colorf navy() { return Colorf(0.0f / 255.0f, 0.0f / 255.0f, 128.0f / 255.0f); }
		static Colorf oldlace() { return Colorf(253.0f / 255.0f, 245.0f / 255.0f, 230.0f / 255.0f); }
		static Colorf olive() { return Colorf(128.0f / 255.0f, 128.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf olivedrab() { return Colorf(107.0f / 255.0f, 142.0f / 255.0f, 35.0f / 255.0f); }
		static Colorf orange() { return Colorf(255.0f / 255.0f, 165.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf orangered() { return Colorf(255.0f / 255.0f, 69.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf orchid() { return Colorf(218.0f / 255.0f, 112.0f / 255.0f, 214.0f / 255.0f); }
		static Colorf palegoldenrod() { return Colorf(238.0f / 255.0f, 232.0f / 255.0f, 170.0f / 255.0f); }
		static Colorf palegreen() { return Colorf(152.0f / 255.0f, 251.0f / 255.0f, 152.0f / 255.0f); }
		static Colorf paleturquoise() { return Colorf(175.0f / 255.0f, 238.0f / 255.0f, 238.0f / 255.0f); }
		static Colorf palevioletred() { return Colorf(219.0f / 255.0f, 112.0f / 255.0f, 147.0f / 255.0f); }
		static Colorf papayawhip() { return Colorf(255.0f / 255.0f, 239.0f / 255.0f, 213.0f / 255.0f); }
		static Colorf peachpuff() { return Colorf(255.0f / 255.0f, 218.0f / 255.0f, 185.0f / 255.0f); }
		static Colorf peru() { return Colorf(205.0f / 255.0f, 133.0f / 255.0f, 63.0f / 255.0f); }
		static Colorf pink() { return Colorf(255.0f / 255.0f, 192.0f / 255.0f, 203.0f / 255.0f); }
		static Colorf plum() { return Colorf(221.0f / 255.0f, 160.0f / 255.0f, 221.0f / 255.0f); }
		static Colorf powderblue() { return Colorf(176.0f / 255.0f, 224.0f / 255.0f, 230.0f / 255.0f); }
		static Colorf purple() { return Colorf(128.0f / 255.0f, 0.0f / 255.0f, 128.0f / 255.0f); }
		static Colorf red() { return Colorf(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf rosybrown() { return Colorf(188.0f / 255.0f, 143.0f / 255.0f, 143.0f / 255.0f); }
		static Colorf royalblue() { return Colorf(65.0f / 255.0f, 105.0f / 255.0f, 225.0f / 255.0f); }
		static Colorf saddlebrown() { return Colorf(139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f); }
		static Colorf salmon() { return Colorf(250.0f / 255.0f, 128.0f / 255.0f, 114.0f / 255.0f); }
		static Colorf sandybrown() { return Colorf(244.0f / 255.0f, 164.0f / 255.0f, 96.0f / 255.0f); }
		static Colorf seagreen() { return Colorf(46.0f / 255.0f, 139.0f / 255.0f, 87.0f / 255.0f); }
		static Colorf seashell() { return Colorf(255.0f / 255.0f, 245.0f / 255.0f, 238.0f / 255.0f); }
		static Colorf sienna() { return Colorf(160.0f / 255.0f, 82.0f / 255.0f, 45.0f / 255.0f); }
		static Colorf silver() { return Colorf(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f); }
		static Colorf skyblue() { return Colorf(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f); }
		static Colorf slateblue() { return Colorf(106.0f / 255.0f, 90.0f / 255.0f, 205.0f / 255.0f); }
		static Colorf slategray() { return Colorf(112.0f / 255.0f, 128.0f / 255.0f, 144.0f / 255.0f); }
		static Colorf slategrey() { return Colorf(112.0f / 255.0f, 128.0f / 255.0f, 144.0f / 255.0f); }
		static Colorf snow() { return Colorf(255.0f / 255.0f, 250.0f / 255.0f, 250.0f / 255.0f); }
		static Colorf springgreen() { return Colorf(0.0f / 255.0f, 255.0f / 255.0f, 127.0f / 255.0f); }
		static Colorf steelblue() { return Colorf(70.0f / 255.0f, 130.0f / 255.0f, 180.0f / 255.0f); }
		static Colorf tan() { return Colorf(210.0f / 255.0f, 180.0f / 255.0f, 140.0f / 255.0f); }
		static Colorf teal() { return Colorf(0.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f); }
		static Colorf thistle() { return Colorf(216.0f / 255.0f, 191.0f / 255.0f, 216.0f / 255.0f); }
		static Colorf tomato() { return Colorf(255.0f / 255.0f, 99.0f / 255.0f, 71.0f / 255.0f); }
		static Colorf transparent() { return Colorf(0.0f, 0.0f, 0.0f, 0.0f); }
		static Colorf turquoise() { return Colorf(64.0f / 255.0f, 224.0f / 255.0f, 208.0f / 255.0f); }
		static Colorf violet() { return Colorf(238.0f / 255.0f, 130.0f / 255.0f, 238.0f / 255.0f); }
		static Colorf wheat() { return Colorf(245.0f / 255.0f, 222.0f / 255.0f, 179.0f / 255.0f); }
		static Colorf white() { return Colorf(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f); }
		static Colorf whitesmoke() { return Colorf(245.0f / 255.0f, 245.0f / 255.0f, 245.0f / 255.0f); }
		static Colorf yellow() { return Colorf(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f); }
		static Colorf yellowgreen() { return Colorf(154.0f / 255.0f, 205.0f / 255.0f, 50.0f / 255.0f); }
		static Colorf gray10() { return Colorf(0.1f, 0.1f, 0.1f); }
		static Colorf gray20() { return Colorf(0.2f, 0.2f, 0.2f); }
		static Colorf gray30() { return Colorf(0.3f, 0.3f, 0.3f); }
		static Colorf gray40() { return Colorf(0.4f, 0.4f, 0.4f); }
		static Colorf gray50() { return Colorf(0.5f, 0.5f, 0.5f); }
		static Colorf gray60() { return Colorf(0.6f, 0.6f, 0.6f); }
		static Colorf gray70() { return Colorf(0.7f, 0.7f, 0.7f); }
		static Colorf gray80() { return Colorf(0.8f, 0.8f, 0.8f); }
		static Colorf gray90() { return Colorf(0.9f, 0.9f, 0.9f); }
	};

	/// \}
}
