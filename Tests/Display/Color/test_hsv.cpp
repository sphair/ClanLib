/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

#include <ClanLib/display.h>

void TestApp::test_hsv()
{
	Console::write_line(" Header: color_hsv.h");
	Console::write_line("  Class: ColorHSVi");
	
	Console::write_line("   Function: ColorHSVi()");
	{
		ColorHSVi hsv;
		if (hsv.h !=0) fail();
		if (hsv.s !=0) fail();
		if (hsv.v !=0) fail();
		if (hsv.a !=0) fail();
	}
	
	Console::write_line("   Function: ColorHSVi(int h, int s, int v, int a)");
	{
		ColorHSVi hsv(1,2,3,4);
		if (hsv.h != 1) fail();
		if (hsv.s != 2) fail();
		if (hsv.v != 3) fail();
		if (hsv.a != 4) fail();
	}
	
	Console::write_line("   Function: ColorHSVi(const ColorHSVi &copy)");
	{
		ColorHSVi hsv(1,2,3,4);
		ColorHSVi hsv_copy(hsv);
		if (hsv_copy.h != 1) fail();
		if (hsv_copy.s != 2) fail();
		if (hsv_copy.v != 3) fail();
		if (hsv_copy.a != 4) fail();
	}
	
	Console::write_line("   Function: ColorHSVi(const Color &color)");
	{
		Color color(255, 0, 0, 64);
		ColorHSVi hsv(color);
		if (hsv.h != 0) fail();
		if (hsv.s != 255) fail();
		if (hsv.v != 255) fail();
		if (hsv.a != 64) fail();
	}
	
	
	
	{
		Color color(128, 255, 128, 64);
		ColorHSVi hsv(color);
		if (hsv.h != 120) fail();
		if (hsv.s != 127) fail();
		if (hsv.v != 255) fail();
		if (hsv.a != 64) fail();
	}
	
	{
		Colorf colorf(0.5f, 1.0f, 0.5f, 1.0f);
		ColorHSVi hsv(colorf);
		if (hsv.h != 120) fail();
		if (hsv.s != 128) fail();
		if (hsv.v != 255) fail();
		if (hsv.a != 255) fail();
	}

	{
		Color color(0, 0, 128, 64);
		ColorHSVi hsv(color);
		if (hsv.h != 240) fail();
		if (hsv.s != 255) fail();
		if (hsv.v != 128) fail();
		if (hsv.a != 64) fail();
	}
	{
		Color color(64, 90, 136, 90);
		ColorHSVi hsv(color);
		if (hsv.h != 218) fail();
		if (hsv.s != 135) fail();
		if (hsv.v != 136) fail();
		if (hsv.a != 90) fail();
	}
	{
		Color color(128, 128, 128, 90);
		ColorHSVi hsv(color);
		if (hsv.h != 0) fail();
		if (hsv.s != 0) fail();
		if (hsv.v != 128) fail();
		if (hsv.a != 90) fail();
	}

	{
		Color color(64, 90, 136, 90);
		ColorHSVi hsv(color);
		Color color2(hsv);
		int red = color2.get_red();
		int green = color2.get_green();
		int blue = color2.get_blue();
		int alpha = color2.get_alpha();
		if (red != 64) fail();
		if (green != 90) fail();
		if (blue != 136) fail();
		if (alpha != 90) fail();

	}
	

	
	Console::write_line("  Class: ColorHSVf");

	Console::write_line("   Function: ColorHSVf()");
	{
		ColorHSVf hsv;
		if (hsv.h !=0.0f) fail();
		if (hsv.s !=0.0f) fail();
		if (hsv.v !=0.0f) fail();
		if (hsv.a !=0.0f) fail();
	}

	Console::write_line("   Function: ColorHSVf(float h, float s, float v, float a)");
	{
		ColorHSVf hsv(0.1f, 0.2f, 0.3f, 0.4f);
		if (hsv.h != 0.1f) fail();
		if (hsv.s != 0.2f) fail();
		if (hsv.v != 0.3f) fail();
		if (hsv.a != 0.4f) fail();
	}

	Console::write_line("   Function: ColorHSVf(const ColorHSVf &copy)");
	{
		ColorHSVf hsv(0.1f, 0.2f, 0.3f, 0.4f);
		ColorHSVf hsv_copy(hsv);
		if (hsv_copy.h != 0.1f) fail();
		if (hsv_copy.s != 0.2f) fail();
		if (hsv_copy.v != 0.3f) fail();
		if (hsv_copy.a != 0.4f) fail();
	}
	Console::write_line("   Function: ColorHSVf(const Colorf &color)");
	{
		Colorf color(1.0f, 0.0f, 0.0f, 0.2f);
		ColorHSVf hsv(color);
		if (hsv.h != 0.0f) fail();
		if (hsv.s != 1.0f) fail();
		if (hsv.v != 1.0f) fail();
		if (hsv.a != 0.2f) fail();
	}

	{
		Colorf color(0.5f, 1.0f, 0.5f, 0.2f);
		ColorHSVf hsv(color);
		if (hsv.h != 120.0f) fail();
		if (hsv.s != 0.5f) fail();
		if (hsv.v != 1.0f) fail();
		if (hsv.a != 0.2f) fail();
	}

	{
		Color color(127, 255, 127, 255);
		ColorHSVf hsv(color);
		if (hsv.h != 120.0f) fail();
		if (hsv.s < 0.49f) fail();
		if (hsv.s > 0.51f) fail();
		if (hsv.v != 1.0f) fail();
		if (hsv.a != 1.0f) fail();
	}

	{
		Colorf color(0.0f, 0.0f, 0.5, 0.3f);
		ColorHSVf hsv(color);
		if (hsv.h != 240.0f) fail();
		if (hsv.s != 1.0f) fail();
		if (hsv.v != 0.5f) fail();
		if (hsv.a != 0.3f) fail();
	}
	
	{
		Colorf color(0.2f, 0.4f, 0.7f, 0.3f);
		ColorHSVf hsv(color);
		Colorf color2(hsv);
		if (color.r < 0.1999f) fail();
		if (color.r > 0.2001f) fail();
		if (color.g < 0.3999f) fail();
		if (color.g > 0.4001f) fail();
		if (color.b < 0.6999f) fail();
		if (color.b > 0.7001f) fail();
		if (color.a < 0.2999f) fail();
		if (color.a > 0.3001f) fail();
	}
	
	Console::write_line("  Class: ColorHSVd");


}
