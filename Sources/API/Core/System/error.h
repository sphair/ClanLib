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

#ifndef header_error
#define header_error

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

#include <string>

//: Exception class in ClanLib.
//- !group=Core/System!
//- !header=core.h!
//- <p>If something goes wrong in a clanlib function, it will throw an exception of
//- this class.</p>
class CL_API_CORE CL_Error
{
public:
//! Construction:
	//: Error Constructor.
	CL_Error(const char *message) { this->message = message; }

	CL_Error(const std::string &message) { this->message = message; }

	//: The error message
	std::string message;
};

//: This function throws an error.
void CL_API_CORE cl_throw_error(const char *message);
void CL_API_CORE cl_throw_error(const std::string &message);

#endif
