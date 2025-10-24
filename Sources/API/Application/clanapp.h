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

//! clanApp="System"
//! header=application.h

#ifndef header_clanapp
#define header_clanapp

#if _MSC_VER > 1000
#pragma once
#endif

//: The starting place for all applications.
//- !group=App/System!
//- !header=application.h!
//- <p> The application class must be inherited by all ClanLib applications.
//- In your application create a class inheriting CL_ClanApplication, 
//- make a global instance of it, and fill in the main() function. </p>
class CL_ClanApplication
{
public:
	//: Clan Application constructor
	CL_ClanApplication();

	//: Clan Application destructor
	virtual ~CL_ClanApplication();

	//: Main application function
	//- <p> Every program made using ClanLib must implement this function. 
	//- This is the main application function, which will be called 
	//- <i>once</i> following system initialization. </p>
	//param argc: The number of arguments to the program (c-style)
	//param argv: A pointer to strings, containing the program arguments. The name of the program executable is always passed as first argument.
	//return: Program return value
	virtual int main(int argc, char** argv)=0;

	//: Static pointer to an initialized instance of this class.
	//: This is set by the constructor.
	static CL_ClanApplication *app;
};

#endif
