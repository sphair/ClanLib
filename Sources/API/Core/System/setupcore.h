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

#ifndef header_setupcore
#define header_setupcore

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

#ifdef WIN32
#include <windows.h>
#endif

//: ClanCore initialization functions.
//- !group=Core/System!
//- !header=core.h!
class CL_API_CORE CL_SetupCore
{
//! Construction:
public:
	//: Initializes clanCore.
	//- <p>Constructing a CL_SetupCore object is equalent to calling CL_SetupCore::init().</p>
	//- <p>When the CL_SetupCore instance is destroyed, CL_SetupCore::deinit() is called.</p>
	CL_SetupCore(bool register_resources_only = false);

	~CL_SetupCore();

public:
//! Operations:
	//: Initialize clanCore.
	//- <p>If you are using CL_ClanApplication to boot your
	//- application, you do not need to call this function.</p>
	//- <p>If using Win32 (and without CL_ClanApplication) -
	//- You must call CL_SetupCore::set_instance(HINSTANCE hInstance)
	//- before this calling this function</p>
	static void init(bool register_resources_only = false);

	//: Deinitialize clanCore.
	//- <p>If you are using CL_ClanApplication to boot your
	//- application, you do not need to call this function.</p>
	static void deinit();

#ifdef WIN32
	//: Set instance.
	//- <p>If doing win32 initialization without using clanApp, you must call
	//- this function before calling CL_SetupCore::init().</p>
	static void set_instance(HINSTANCE hInstance);
#endif
};

#endif
