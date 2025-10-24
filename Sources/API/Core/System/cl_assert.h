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

//! clanCore="System"
//! header=core.h

#ifndef header_cl_assert
#define header_cl_assert

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __PRETTY_FUNCTION__ 
#define __PRETTY_FUNCTION__ NULL
#endif

#ifndef __STRING
#define cl_assert(a) CL_Assert::die(a, __FILE__, __LINE__, __PRETTY_FUNCTION__, #a);
#else
#define cl_assert(a) CL_Assert::die(a, __FILE__, __LINE__, __PRETTY_FUNCTION__, __STRING(a));
#endif

//: This is ClanLib's assertion class.
//- !group=Core/System!
//- !header=core.h!

class CL_API_CORE CL_Assert
{
//! Operations:
public:
	//: Produces an assert.
	//- <p>This function shouldn't be called directly -
	//- instead, use the cl_assert macro.
	//-  For example: cl_assert(should_not_be_zero != 0)</p>
	//param a: Asserts if false.
	//param file: Name of the file where the assert occurred.
	//param line: Line in the file where the assert occurred.
	//param func: Function in which the assert occurred.
	//param assert_str: String that describe the assertion check.
	static void die(bool a, const char *file, int line, const char *func, const char *assert_str);
};

#endif
