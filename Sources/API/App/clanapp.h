/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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


#pragma once

#include "api_app.h"
#include <vector>
#include <string>

namespace clan
{
/// \addtogroup clanApp_System clanApp System
/// \{

/// \brief Helper class to create cross platform main functions
///
/// To use this class, create a static main function in your application class,
/// then make a single global instance of Application.
///
/// \code
/// class MyApplication
/// {
/// public:
///   static int main(std::vector<std::string> &args);
/// };
///
/// Application clanapp(&MyApplication::main);
/// \endcode
/// If you do not want exceptions to be automatically caught, pass "false" to the optional catch_exceptions parameter in Application\n
/// Your program does not have to use this class.\n
class CL_API_APP Application
{
/// \name Construction
/// \{

public:
	typedef int (MainFunction)(const std::vector<std::string> &args);

	/// \brief Clan Application constructor
	Application(MainFunction *main_function, bool catch_exceptions = true)
	{
		main = main_function;
		enable_catch_exceptions = catch_exceptions;
	}

/// \}
/// \name Attributes
/// \{
public:
	static MainFunction *main;
	static bool enable_catch_exceptions;
/// \}
};

}	// namespace clan

/// \}
