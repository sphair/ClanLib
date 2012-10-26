/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

void TestApp::test_resources(void)
{
	Console::write_line(" Header: resource.h");
	Console::write_line("  Class: Resource");

	// Construct resource manager. This will cause sig_resource_added() to be signalled for each
	// resource object located in the resource file. This means ResourceApp::on_resource_added will
	// get called for each resource.
	ResourceManager resources("resources.xml");

	// Lets try to access some of the clanlib objects in the resource file:
	std::string config_name = resources.get_string_resource("Configuration/name", "");
	int config_width = resources.get_integer_resource("Configuration/width", 0 );
	int config_height = resources.get_integer_resource("Configuration/height", 0 );

	Console::write_line(std::string("name = ") + config_name );
	Console::write_line(std::string("width = ") + StringHelp::int_to_text(config_width));
	Console::write_line(std::string("height = ") + StringHelp::int_to_text(config_height));

	// Get a list over all classes
	std::vector<std::string> v = resources.get_resource_names_of_type("uclass", "Classes");
	std::vector<std::string>::iterator it;
	for(it = v.begin(); it != v.end(); ++it)
	{
		// Get one of our custom resources:
		Resource resource = resources.get_resource(*it);

		// Its possible to access the dom element object in the resource:
		DomElement element = resource.get_element();
		int strength = StringHelp::text_to_int(element.get_attribute("strength"));
		int magic = StringHelp::text_to_int(element.get_attribute("magic"));

		Console::write_line(std::string("\n") + "name = " + resource.get_name());
		Console::write_line(std::string("strength = ") + StringHelp::int_to_text(strength));
		Console::write_line(std::string("magic = ") + StringHelp::int_to_text(magic) );
	}

}

