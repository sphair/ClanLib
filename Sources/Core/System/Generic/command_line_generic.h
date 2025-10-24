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

#ifndef header_core_system_command_line_generic
#define header_core_system_command_line_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <vector>
#include "API/Core/System/command_line.h"

class CL_CommandLine_Generic
{
private:
	int help_indent;

	std::string programm;

	struct Option 
	{
		int key;
		std::string help;
		std::string long_option;
		std::string argument;
		bool visible;
	};
  
	typedef std::vector<Option> Options;
	Options options;

	struct ParsedOption
	{
		int key;
		std::string argument;
	};

	typedef std::vector<ParsedOption> ParsedOptions;
	ParsedOptions parsed_options;
	ParsedOptions::iterator current_option;

	enum
	{
		GROUP     = -3,
		DOC       = -4,
		USAGE     = -5
	};

public:  
	CL_CommandLine_Generic();

	void set_help_indent(int i) { help_indent = i; }

	void add_usage(const std::string& usage);
	void add_doc(const std::string& doc);
	void add_group(const std::string& grouptopic);
  
	void add_option(int key,
						 const std::string& long_option, 
						 const std::string& argument,
						 const std::string& help,
						 bool visible = true);

	void parse_args(int argc, char** argv);
	void print_help();
  
	bool next();
	int get_key();
	std::string get_argument();

private:
	void read_option(int id, const std::string& argument);

	/** Find the Option structure that matches \a short_option */
	Option* lookup_short_option(char short_option);

	/** Find the Option structure that matches \a long_option */
	Option* lookup_long_option (const std::string& long_option);
};

#endif

/* EOF */
