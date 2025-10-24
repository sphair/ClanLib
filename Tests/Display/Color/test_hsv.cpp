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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

#include <ClanLib/display.h>

void TestApp::test_hsv()
{
	CL_Console::write_line(" Header: color_hsv.h");
	CL_Console::write_line("  Class: CL_ColorHSVi");

	CL_Console::write_line("   Function: CL_ColorHSVi()");
	{
		CL_ColorHSVi hsv;
		if (hsv.h !=0) fail();
		if (hsv.s !=0) fail();
		if (hsv.v !=0) fail();
		if (hsv.a !=0) fail();
	}

	CL_Console::write_line("   Function: CL_ColorHSVi(int h, int s, int v, int a)");
	{
		CL_ColorHSVi hsv(1,2,3,4);
		if (hsv.h != 1) fail();
		if (hsv.s != 2) fail();
		if (hsv.v != 3) fail();
		if (hsv.a != 4) fail();
	}

	CL_Console::write_line("   Function: CL_ColorHSVi(const CL_ColorHSVi &copy)");
	{
		CL_ColorHSVi hsv(1,2,3,4);
		CL_ColorHSVi hsv_copy(hsv);
		if (hsv_copy.h != 1) fail();
		if (hsv_copy.s != 2) fail();
		if (hsv_copy.v != 3) fail();
		if (hsv_copy.a != 4) fail();
	}
	CL_Console::write_line("   Function: CL_ColorHSVi(const CL_Color &color)");
	{
		CL_Color color(255, 0, 0, 64);
		CL_ColorHSVi hsv(color);
		if (hsv.h != 0) fail();
		if (hsv.s != 255) fail();
		if (hsv.v != 255) fail();
		if (hsv.a != 64) fail();
	}

	{
		CL_Color color(128, 255, 128, 64);
		CL_ColorHSVi hsv(color);
		if (hsv.h != 120) fail();
		if (hsv.s != 127) fail();
		if (hsv.v != 255) fail();
		if (hsv.a != 64) fail();
	}

	{
		CL_Colorf colorf(0.5f, 1.0f, 0.5f, 1.0f);
		CL_ColorHSVi hsv(colorf);
		if (hsv.h != 120) fail();
		if (hsv.s != 128) fail();
		if (hsv.v != 255) fail();
		if (hsv.a != 255) fail();
	}

	{
		CL_Color color(0, 0, 128, 64);
		CL_ColorHSVi hsv(color);
		if (hsv.h != 240) fail();
		if (hsv.s != 255) fail();
		if (hsv.v != 128) fail();
		if (hsv.a != 64) fail();
	}
	{
		CL_Color color(64, 90, 136, 90);
		CL_ColorHSVi hsv(color);
		if (hsv.h != 218) fail();
		if (hsv.s != 135) fail();
		if (hsv.v != 136) fail();
		if (hsv.a != 90) fail();
	}
	{
		CL_Color color(128, 128, 128, 90);
		CL_ColorHSVi hsv(color);
		if (hsv.h != 0) fail();
		if (hsv.s != 0) fail();
		if (hsv.v != 128) fail();
		if (hsv.a != 90) fail();
	}

	{
		CL_Color color(64, 90, 136, 90);
		CL_ColorHSVi hsv(color);
		CL_Color color2(hsv);
		int red = color2.get_red();
		int green = color2.get_green();
		int blue = color2.get_blue();
		int alpha = color2.get_alpha();
		if (red != 64) fail();
		if (green != 90) fail();
		if (blue != 136) fail();
		if (alpha != 90) fail();

	}

	CL_Console::write_line("  Class: CL_ColorHSVf");

	CL_Console::write_line("   Function: CL_ColorHSVf()");
	{
		CL_ColorHSVf hsv;
		if (hsv.h !=0.0f) fail();
		if (hsv.s !=0.0f) fail();
		if (hsv.v !=0.0f) fail();
		if (hsv.a !=0.0f) fail();
	}

	CL_Console::write_line("   Function: CL_ColorHSVf(float h, float s, float v, float a)");
	{
		CL_ColorHSVf hsv(0.1f, 0.2f, 0.3f, 0.4f);
		if (hsv.h != 0.1f) fail();
		if (hsv.s != 0.2f) fail();
		if (hsv.v != 0.3f) fail();
		if (hsv.a != 0.4f) fail();
	}

	CL_Console::write_line("   Function: CL_ColorHSVf(const CL_ColorHSVf &copy)");
	{
		CL_ColorHSVf hsv(0.1f, 0.2f, 0.3f, 0.4f);
		CL_ColorHSVf hsv_copy(hsv);
		if (hsv_copy.h != 0.1f) fail();
		if (hsv_copy.s != 0.2f) fail();
		if (hsv_copy.v != 0.3f) fail();
		if (hsv_copy.a != 0.4f) fail();
	}
	CL_Console::write_line("   Function: CL_ColorHSVf(const CL_Colorf &color)");
	{
		CL_Colorf color(1.0f, 0.0f, 0.0f, 0.2f);
		CL_ColorHSVf hsv(color);
		if (hsv.h != 0.0f) fail();
		if (hsv.s != 1.0f) fail();
		if (hsv.v != 1.0f) fail();
		if (hsv.a != 0.2f) fail();
	}

	{
		CL_Colorf color(0.5f, 1.0f, 0.5f, 0.2f);
		CL_ColorHSVf hsv(color);
		if (hsv.h != 120.0f) fail();
		if (hsv.s != 0.5f) fail();
		if (hsv.v != 1.0f) fail();
		if (hsv.a != 0.2f) fail();
	}

	{
		CL_Color color(127, 255, 127, 255);
		CL_ColorHSVf hsv(color);
		if (hsv.h != 120.0f) fail();
		if (hsv.s < 0.49f) fail();
		if (hsv.s > 0.51f) fail();
		if (hsv.v != 1.0f) fail();
		if (hsv.a != 1.0f) fail();
	}

	{
		CL_Colorf color(0.0f, 0.0f, 0.5, 0.3f);
		CL_ColorHSVf hsv(color);
		if (hsv.h != 240.0f) fail();
		if (hsv.s != 1.0f) fail();
		if (hsv.v != 0.5f) fail();
		if (hsv.a != 0.3f) fail();
	}

	{
		CL_Colorf color(0.2f, 0.4f, 0.7f, 0.3f);
		CL_ColorHSVf hsv(color);
		CL_Colorf color2(hsv);
		if (color.r < 0.1999f) fail();
		if (color.r > 0.2001f) fail();
		if (color.g < 0.3999f) fail();
		if (color.g > 0.4001f) fail();
		if (color.b < 0.6999f) fail();
		if (color.b > 0.7001f) fail();
		if (color.a < 0.2999f) fail();
		if (color.a > 0.3001f) fail();
	}

	CL_Console::write_line("  Class: CL_ColorHSVd");

	CL_Console::write_line("   Function: CL_ColorHSVd()");
	{
		CL_ColorHSVd hsv;
		if (hsv.h !=0.0) fail();
		if (hsv.s !=0.0) fail();
		if (hsv.v !=0.0) fail();
		if (hsv.a !=0.0) fail();
	}

	CL_Console::write_line("   Function: CL_ColorHSVd(float h, float s, float v, float a)");
	{
		CL_ColorHSVd hsv(0.1, 0.2, 0.3, 0.4);
		if (hsv.h != 0.1) fail();
		if (hsv.s != 0.2) fail();
		if (hsv.v != 0.3) fail();
		if (hsv.a != 0.4) fail();
	}

	CL_Console::write_line("   Function: CL_ColorHSVd(const CL_ColorHSVd &copy)");
	{
		CL_ColorHSVd hsv(0.1, 0.2, 0.3, 0.4);
		CL_ColorHSVd hsv_copy(hsv);
		if (hsv_copy.h != 0.1) fail();
		if (hsv_copy.s != 0.2) fail();
		if (hsv_copy.v != 0.3) fail();
		if (hsv_copy.a != 0.4) fail();
	}
	CL_Console::write_line("   Function: CL_ColorHSVd(const CL_Colorf &color)");
	{
		CL_Colord color(1.0, 0.0, 0.0, 0.2);
		CL_ColorHSVd hsv(color);
		if (hsv.h != 0.0) fail();
		if (hsv.s != 1.0) fail();
		if (hsv.v != 1.0) fail();
		if (hsv.a != 0.2) fail();
	}

	{
		CL_Colord color(0.5, 1.0, 0.5, 0.2);
		CL_ColorHSVd hsv(color);
		if (hsv.h != 120.0) fail();
		if (hsv.s != 0.5) fail();
		if (hsv.v != 1.0) fail();
		if (hsv.a != 0.2) fail();
	}

	{
		CL_Color color(127, 255, 127, 255);
		CL_ColorHSVd hsv(color);
		if (hsv.h != 120.0) fail();
		if (hsv.s < 0.49) fail();
		if (hsv.s > 0.51) fail();
		if (hsv.v != 1.0) fail();
		if (hsv.a != 1.0) fail();
	}

	{
		CL_Colord color(0.0, 0.0, 0.5, 0.3);
		CL_ColorHSVd hsv(color);
		if (hsv.h != 240.0) fail();
		if (hsv.s != 1.0) fail();
		if (hsv.v != 0.5) fail();
		if (hsv.a != 0.3) fail();
	}

	{
		CL_Colord color(0.2, 0.4, 0.7, 0.3);
		CL_ColorHSVd hsv(color);
		CL_Colord color2(hsv);
		if (color.r < 0.1999) fail();
		if (color.r > 0.2001) fail();
		if (color.g < 0.3999) fail();
		if (color.g > 0.4001) fail();
		if (color.b < 0.6999) fail();
		if (color.b > 0.7001) fail();
		if (color.a < 0.2999) fail();
		if (color.a > 0.3001) fail();
	}

}
