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

//! clanCore="XML"
//! header=core.h

#ifndef header_dom_implementation
#define header_dom_implementation

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

#include "../System/sharedptr.h"

class CL_DomImplementation_Generic;
class CL_DomDocument;

//: DOM Implementation class.
//- !group=Core/XML!
//- !header=core.h!
//- <p>The DOM Implementation interface provides a number of methods for performing
//- operations that are independent of any particular instance of the document object model.</p>
class CL_API_CORE CL_DomImplementation
{
//! Construction:
public:
	//: Constructs a DOM Implementation handle.
	CL_DomImplementation();

	CL_DomImplementation(CL_DomDocument &doc);

	~CL_DomImplementation();

//! Attributes:
public:

//! Operations:
public:
	//: Test if the DOM implementation implements a specific feature.
	//param feature: The package name of the feature to test. In Level 1, the legal values are "HTML" and "XML" (case-insensitive).
	//param version: This is the version number of the package name to test. In Level 1, this is the string "1.0". If the version is not specified, supporting any version of the feature will cause the method to return true.
	//retval: true if the feature is implemented in the specified version, false otherwise.
	bool has_feature(const std::string &feature, const std::string &version);

//! Implementation:
private:
	CL_SharedPtr<CL_DomImplementation_Generic> impl;
};

#endif
