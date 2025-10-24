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
*/

#ifndef header_workspace
#define header_workspace

#if _MSC_VER > 1000
#pragma once
#endif

#include "project.h"

class Workspace
{
// Construction:
public:

// Attributes:
public:
	std::list<Project> projects;
	// Projects in workspace.

	std::string output_include_dir;
	// Output include directory. Project headers should be copied to this destination.

	std::string output_lib_dir;
	// Output library directory. Project library files should be copied to this destination.

	std::string input_include_dir;
	// Input include directory path.

	std::string input_lib_dir;
	// Input library directory path.

// Operations:
public:

// Implementation:
private:
};

#endif
