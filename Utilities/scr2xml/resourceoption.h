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

//! clanCore="Resources"
//! header=core.h

#ifndef RESOURCEOPTION_H_
#define RESOURCEOPTION_H_

#include <vector>
#include <string>

//: Resource option class.
//- <p>Holds the particular option for a resource.</p>
class ResourceOption
{
//! Construction:
public:
	//: Constructs a resource option.
	//param name: The name of the option
	//param value: Value of the option as a string
	ResourceOption(const std::string &name, const std::string &value) : name(name)
	{
		values.push_back(value);
	}

	ResourceOption(const std::string &name) : name(name)
	{
	}

	ResourceOption()
	{
	}

//! Attributes:
public:
	//: Gets the name of the resource option.
	//return: The name of the option.
	const std::string &get_name() const { return name; }

	//: Gets all values as a vector.
	//return: An std::vector<std::string> with all the values.
	std::vector<std::string> &get_values() { return values; }

//! Operations:
public:
	//: Add a value.
	//param value: The string value to add.
	//return: The index of the added value.
	int add_value(const std::string &value) { values.push_back(value); return values.size();}

//! Implementation:
private:
	std::string name;
	std::vector<std::string> values;
};

#endif
