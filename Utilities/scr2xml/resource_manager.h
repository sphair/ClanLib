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

#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_


#include <vector>
#include "resource_manager.h"
#include "resourceoptions.h"
#include "resource_tokenizer.h"

#define default_scriptfile_id "_script_section"

class ResourceTree;

class ResourceManager
{
public:
	ResourceManager(std::vector<char>& buffer, ResourceTree* t);

	~ResourceManager();

private:
	void parse();

	void parse_include(ResourceTokenizer &lexer);
	
	void parse_section_body(
		std::string token,
		ResourceTokenizer &lexer,
		std::string prefix);

	//: Returns the filename part of a pathname. Eg. returns "filename.tga" if given string "/blabla/filename.tga".
	static std::string get_filename(const std::string &pathname);

	//: Returns the path part of a pathname. Eg. returns "/blabla/" if given string "/blabla/filename.tga".
	static std::string get_path(const std::string &pathname);

	std::vector<char> config_file_data;

	ResourceTree* tree;
};

#endif
