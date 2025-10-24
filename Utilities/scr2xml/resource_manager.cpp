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
**    (if your name is missing here, please add it)
*/

#include "resource_manager.h"
#include "resource_tokenizer.h"

#include "resource_tree.h"

#include <ClanLib/Core/System/clanstring.h>
#include <ClanLib/Core/System/error.h>
#include <ClanLib/Core/System/cl_assert.h>

#include <algorithm>
#include <ctype.h>
#include <iostream>
#include <map>

ResourceManager::ResourceManager(std::vector<char>& buffer, ResourceTree* t)
	: config_file_data(buffer), tree(t)
{
	parse();
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::parse()
{
	try
	{
		ResourceTokenizer lexer(config_file_data);

		while (true)
		{
			std::string token = lexer.get_next_token();
			if (token == "") break;
			
			if (token == "location") // obsolete. Will read it but issues a warning.
			{
				token = lexer.get_next_token();
				if (token != "=")
					throw CL_Error(lexer.write_error("Missing '=' following global 'location'-declaration"));
				
				token = lexer.get_next_token();
				if (token != "datafile")
					throw CL_Error(lexer.write_error("Syntax error following global 'location'-declaration"));
				
				token = lexer.get_next_token();
				if (token != "(")
					throw CL_Error(lexer.write_error("Missing '(' following 'datafile' keyword"));

				token = lexer.get_next_token();

				token = lexer.get_next_token();
				if (token != ")")
					throw CL_Error(lexer.write_error("Missing ')' following datafile filename"));

				token = lexer.get_next_token();
				if (token != ";")
					throw CL_Error(lexer.write_error("Missing ';' following datafile filename"));
			}
			else // assume it is a section body (resource or section)
			{
				parse_section_body(token, lexer, "");
			}
		}
	}
	catch (...)
	{
		throw;
	}
}

void ResourceManager::parse_section_body(
	std::string token,
	ResourceTokenizer &lexer,
	std::string prefix)
{
	if (token == "section")
	{
		std::string section_name = lexer.get_next_token();
		std::string section_prefix = prefix + section_name;  

		token = lexer.get_next_token();
		if (token != "{")
			throw CL_Error(lexer.write_error("Missing '{'"));

		tree->add_section(section_name);
			
		while (true)
		{
			token = lexer.get_next_token();
			if (token == "}") break;
			parse_section_body(token, lexer, section_prefix);
		}
		
		tree->end_section();
	}
	else // must be a resource then
	{
		std::string name = token;

		token = lexer.get_next_token();
		if (token != "=")
		{
			std::string err;
			err = err + "Missing '=' following declaration of resource '" + name + "'";
			throw CL_Error(lexer.write_error(err.c_str()));
		}

		// read the location (if available):
		std::string location;
		token = lexer.get_next_token();
		if(token == "(")
			location = "";
		else {
			location = token;
			token = lexer.get_next_token();
		}

		// read resource options:
		if (token != "(")
		{
			std::string err;
			err = err + "Missing '(' following declaration of resource '" + name + "'";
			throw CL_Error(lexer.write_error(err.c_str()));
		}

		ResourceOptions options;
		while (true)
		{
			token = lexer.get_next_token();
			if (token == ")") break;
			
			std::string option_name = token;
			
			token = lexer.get_next_token();
			if (token != "=")
			{
				std::string err;
				err = err + "Syntax error in options following declaration of resource '" + name + "'";
				throw CL_Error(lexer.write_error(err.c_str()));
			}
			
			token = lexer.get_next_token();
			if (token == "(") // it is a value list
			{
				ResourceOption option(option_name);

				while (true)
				{
					token = lexer.get_next_token();
					if (token == ")") break;

					option.add_value(token);
					
					token = lexer.get_next_token();
					if (token == ")") break;
					if (token != ",")
					{
						std::string err;
						err = err + "Missing ',' or ')' following declaration of multiple option values in option '" +
							   option_name + "' in resource '" + name + "' (found '" + token + "')";
						throw CL_Error(lexer.write_error(err.c_str()));
					}
				}
				
				options.add(option);
			}
			else
			{
				std::string option_value = token;
				
				options.add(ResourceOption(option_name, option_value));
			}

			token = lexer.get_next_token();
			if (token == ")") break;
			if (token != ",")
			{
				std::string err;
				err = err + "Syntax error in options following declaration of resource '" + name + "'";
				throw CL_Error(lexer.write_error(err.c_str()));
			}
		}
		
		token = lexer.get_next_token();
		if (token != ";")
		{
			std::string err;
			err = err + "Missing ';' following declaration of resource '" + name +"'";
			throw CL_Error(lexer.write_error(err.c_str()));
		}
		
		if (options.exists("type") == false)
		{
			std::string err;
			err = err + "No type declared for resource '" + name + "'";
			throw CL_Error(lexer.write_error(err.c_str()));
		}

		tree->add_resource(name, location, options);
	}
}

std::string ResourceManager::get_filename(const std::string &pathname)
{
	std::string::size_type pos1 = pathname.find_last_of('/');
	std::string::size_type pos2 = pathname.find_last_of('\\');

	if (pos1 == std::string::npos && pos2 == std::string::npos)
	{
		return pathname;
	}
	else if (pos1 != std::string::npos && pos2 == std::string::npos)
	{
		return pathname.substr(pos1 + 1);
	}
	else if (pos1 == std::string::npos && pos2 != std::string::npos)
	{
		return pathname.substr(pos2 + 1);
	}
	else // (pos1 != std::string::npos && pos2 != std::string::npos)
	{
		return pathname.substr(((pos1>pos2) ? pos1 : pos2) + 1);
	}
}

std::string ResourceManager::get_path(const std::string &pathname)
{
	std::string::size_type length = pathname.length();
 	std::string::size_type pos1 = pathname.find_last_of('/');
	std::string::size_type pos2 = pathname.find_last_of('\\');
	if (pos1 == std::string::npos) pos1 = length;
	if (pos2 == std::string::npos) pos2 = length;
	if (pos1 < 0) pos1 = length;
	if (pos2 < 0) pos2 = length;

	if (pos1 == length && pos2 == length) return std::string();

	int pos = (pos1 < pos2) ? pos1 : pos2;
	return pathname.substr(0, pos);
}
