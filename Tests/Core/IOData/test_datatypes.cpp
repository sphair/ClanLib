/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

void TestApp::test_datatypes(void)
{
	Console::write_line(" Header: datatypes.h");
	Console::write_line("  Ensuring that the datatypes are the correct size");

	if (sizeof(byte8) != 1) fail();
	if (sizeof(byte16) != 2) fail();
	if (sizeof(byte32) != 4) fail();
	if (sizeof(byte64) != 8) fail();

	Console::write_line("  Ensuring that the datatypes have the correct sign");
	byte8 dts8 = 2;
	byte16 dts16 = 2;
	byte32 dts32 = 2;
	byte64 dts64 = 2;
	ubyte8 dtu8 = 2;
	ubyte16 dtu16 = 2;
	ubyte32 dtu32 = 2;
	ubyte64 dtu64 = 2;

	if (dts8 < 2) fail();
	if (dts16 < 2) fail();
	if (dts32 < 2) fail();
	if (dts64 < 2) fail();
	if (dtu8 < 2) fail();
	if (dtu16 < 2) fail();
	if (dtu32 < 2) fail();
	if (dtu64 < 2) fail();

	dts8-=4;
	dts16-=4;
	dts32-=4;
	dts64-=4;
	dtu8-=4;
	dtu16-=4;
	dtu32-=4;
	dtu64-=4;

	if (dts8 > 2) fail();
	if (dts16 > 2) fail();
	if (dts32 > 2) fail();
	if (dts64 > 2) fail();
	if (dtu8 < 2) fail();
	if (dtu16 < 2) fail();
	if (dtu32 < 2) fail();
	if (dtu64 < 2) fail();

}

