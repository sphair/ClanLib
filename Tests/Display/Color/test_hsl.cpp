/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
**
**  This software is provifed 'as-is', without any express or implied
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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

#include <ClanLib/display.h>

void TestApp::test_hsl()
{
	Console::write_line(" Header: color_hsl.h");
	Console::write_line("  Class: ColorHSLi");

	Console::write_line("   Function: ColorHSLi()");
	{
		ColorHSLi hsl;
		if (hsl.h !=0) fail();
		if (hsl.s !=0) fail();
		if (hsl.l !=0) fail();
		if (hsl.a !=0) fail();
	}

	Console::write_line("   Function: ColorHSLi(int h, int s, int l, int a)");
	{
		ColorHSLi hsl(1,2,3,4);
		if (hsl.h != 1) fail();
		if (hsl.s != 2) fail();
		if (hsl.l != 3) fail();
		if (hsl.a != 4) fail();
	}

	Console::write_line("   Function: ColorHSLi(const ColorHSLi &copy)");
	{
		ColorHSLi hsl(1,2,3,4);
		ColorHSLi hsl_copy(hsl);
		if (hsl_copy.h != 1) fail();
		if (hsl_copy.s != 2) fail();
		if (hsl_copy.l != 3) fail();
		if (hsl_copy.a != 4) fail();
	}
	Console::write_line("   Function: ColorHSLi(const Color &color)");
	{
		Color color(255, 0, 0, 64);
		ColorHSLi hsl(color);
		if (hsl.h != 0) fail();
		if (hsl.s != 255) fail();
		if (hsl.l != 128) fail();
		if (hsl.a != 64) fail();
	}

	{
		Color color(128, 255, 128, 64);
		ColorHSLi hsl(color);
		if (hsl.h != 120) fail();
		if (hsl.s != 255) fail();
		if (hsl.l < 191) fail();
		if (hsl.l > 192) fail();
		if (hsl.a != 64) fail();
	}

	{
		Colorf colorf(0.5f, 1.0f, 0.5f, 1.0f);
		ColorHSLi hsl(colorf);
		if (hsl.h != 120) fail();
		if (hsl.s != 255) fail();
		if (hsl.l != 191) fail();
		if (hsl.a != 255) fail();
	}

	{
		Color color(0, 0, 128, 64);
		ColorHSLi hsl(color);
		if (hsl.h != 240) fail();
		if (hsl.s != 255) fail();
		if (hsl.l != 64) fail();
		if (hsl.a != 64) fail();
	}
	{
		Color color(64, 90, 136, 90);
		ColorHSLi hsl(color);
		if (hsl.h != 218) fail();
		if (hsl.s != 92) fail();
		if (hsl.l != 100) fail();
		if (hsl.a != 90) fail();
	}
	{
		Color color(128, 128, 128, 90);
		ColorHSLi hsl(color);
		if (hsl.h != 0) fail();
		if (hsl.s != 0) fail();
		if (hsl.l != 128) fail();
		if (hsl.a != 90) fail();
	}

	{
		Color color(64, 90, 136, 90);
		ColorHSLi hsl(color);
		Color color2(hsl);
		int red = color2.get_red();
		int green = color2.get_green();
		int blue = color2.get_blue();
		int alpha = color2.get_alpha();
		if (red < 63) fail();
		if (red > 65) fail();
		if (green != 90) fail();
		if (blue != 136) fail();
		if (alpha != 90) fail();

	}

	Console::write_line("  Class: ColorHSLf");

	Console::write_line("   Function: ColorHSLf()");
	{
		ColorHSLf hsl;
		if (hsl.h !=0.0f) fail();
		if (hsl.s !=0.0f) fail();
		if (hsl.l !=0.0f) fail();
		if (hsl.a !=0.0f) fail();
	}

	Console::write_line("   Function: ColorHSLf(float h, float s, float l, float a)");
	{
		ColorHSLf hsl(0.1f, 0.2f, 0.3f, 0.4f);
		if (hsl.h != 0.1f) fail();
		if (hsl.s != 0.2f) fail();
		if (hsl.l != 0.3f) fail();
		if (hsl.a != 0.4f) fail();
	}

	Console::write_line("   Function: ColorHSLf(const ColorHSLf &copy)");
	{
		ColorHSLf hsl(0.1f, 0.2f, 0.3f, 0.4f);
		ColorHSLf hsl_copy(hsl);
		if (hsl_copy.h != 0.1f) fail();
		if (hsl_copy.s != 0.2f) fail();
		if (hsl_copy.l != 0.3f) fail();
		if (hsl_copy.a != 0.4f) fail();
	}
	Console::write_line("   Function: ColorHSLf(const Colorf &color)");
	{
		Colorf color(1.0f, 0.0f, 0.0f, 0.2f);
		ColorHSLf hsl(color);
		if (hsl.h != 0.0f) fail();
		if (hsl.s != 1.0f) fail();
		if (hsl.l != 0.5f) fail();
		if (hsl.a != 0.2f) fail();
	}

	{
		Colorf color(0.5f, 1.0f, 0.5f, 0.2f);
		ColorHSLf hsl(color);
		if (hsl.h != 120.0f) fail();
		if (hsl.s != 1.0f) fail();
		if (hsl.l != 0.75f) fail();
		if (hsl.a != 0.2f) fail();
	}

	{
		Color color(127, 255, 127, 255);
		ColorHSLf hsl(color);
		if (hsl.h != 120.0f) fail();
		if (hsl.s != 1.0f) fail();
		if (hsl.l < 0.74f) fail();
		if (hsl.l > 0.76f) fail();
		if (hsl.a != 1.0f) fail();
	}

	{
		Colorf color(0.0f, 0.0f, 0.5, 0.3f);
		ColorHSLf hsl(color);
		if (hsl.h != 240.0f) fail();
		if (hsl.s != 1.0f) fail();
		if (hsl.l < 0.24f) fail();
		if (hsl.l > 0.26f) fail();
		if (hsl.a != 0.3f) fail();
	}

	{
		Colorf color(0.2f, 0.4f, 0.7f, 0.3f);
		ColorHSLf hsl(color);
		Colorf color2(hsl);
		if (color.r < 0.1999f) fail();
		if (color.r > 0.2001f) fail();
		if (color.g < 0.3999f) fail();
		if (color.g > 0.4001f) fail();
		if (color.b < 0.6999f) fail();
		if (color.b > 0.7001f) fail();
		if (color.a < 0.2999f) fail();
		if (color.a > 0.3001f) fail();
	}


}
