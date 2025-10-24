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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

void TestApp::test_weakptr(void)
{
	CL_Console::write_line(" Header: weakptr.h");
	CL_Console::write_line("  Class: CL_WeakPtr");

	g_bConstructor = 0;
	g_bDestructor = 0;

//*** testing is_null()

	CL_Console::write_line("   Function: bool is_null()");

	CL_SharedPtr<MyClass> myshared( new(MyClass) );

	CL_WeakPtr<MyClass> myptr_a;
	CL_WeakPtr<MyClass> myptr_b(myshared);

	CL_WeakPtr<MyClass> myptr_c = myptr_b;
	CL_WeakPtr<MyClass> myptr_d = myptr_c;
	CL_WeakPtr<MyClass> myptr_e = myptr_b;
	if (!myptr_a.is_null()) fail();
	if (myptr_b.is_null()) fail();
	if (myptr_c.is_null()) fail();
	if (myptr_d.is_null()) fail();
	if (myptr_e.is_null()) fail();

	myptr_c.disconnect();

	if (!myptr_a.is_null()) fail();
	if (myptr_b.is_null()) fail();
	if (!myptr_c.is_null()) fail();
	if (myptr_d.is_null()) fail();
	if (myptr_e.is_null()) fail();

	if (g_bConstructor != 1) fail();
	if (g_bDestructor != 0) fail();

	CL_Console::write_line("   Function: bool disconnect()");
	myptr_c = myptr_b;
	if (!myptr_a.is_null()) fail();
	if (myptr_b.is_null()) fail();
	if (myptr_c.is_null()) fail();
	if (myptr_d.is_null()) fail();
	if (myptr_e.is_null()) fail();

	myptr_c.disconnect();
	myptr_d.disconnect();
	myptr_e.disconnect();

	if (!myptr_a.is_null()) fail();
	if (myptr_b.is_null()) fail();
	if (!myptr_c.is_null()) fail();
	if (!myptr_d.is_null()) fail();
	if (!myptr_e.is_null()) fail();

	if (g_bConstructor != 1) fail();
	if (g_bDestructor != 0) fail();

	myptr_b.disconnect();
	if (!myptr_b.is_null()) fail();
	if (g_bConstructor != 1) fail();
	if (g_bDestructor != 0) fail();

	myptr_b = myshared;
	myptr_c = myptr_b;
	myptr_d = myptr_c;

	myptr_c.disconnect();
	myshared.disconnect();
	if (!myptr_a.is_null()) fail();
	if (!myptr_b.is_null()) fail();
	if (!myptr_c.is_null()) fail();
	if (!myptr_d.is_null()) fail();

	if (g_bConstructor != 1) fail();
	if (g_bDestructor != 1) fail();

}


