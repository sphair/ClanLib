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

#include "Core/precomp.h"

#include <iomanip>
#include <iostream>
#include <stdio.h>

#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"
#include "command_line_generic.h"


#ifdef WIN32
#define snprintf _snprintf
#endif

CL_CommandLine_Generic::CL_CommandLine_Generic()
{
	help_indent = 18;
}

void CL_CommandLine_Generic::parse_args(int argc, char** argv)
{
	programm = argv[0];

	for(int i = 1; i < argc; ++i) 
	{
		if (argv[i][0] == '-') 
		{
			if (argv[i][1] == '-') 
			{
				// We got a long option
				if (argv[i][2] == '\0') { 
					// Got a '--', so we stop evaluating arguments
					++i;
					while(i < argc) 
					{
						read_option(CL_CommandLine::REST_ARG, argv[i]);
						++i;
					}
				} 
				else
				{
					std::string opt = argv[i] + 2;
					std::string long_opt;
					std::string long_opt_arg;

					std::string::size_type pos = opt.find('=');

					if (pos != std::string::npos) 
					{
						long_opt = opt.substr(0, pos);
						long_opt_arg = opt.substr(pos+1);
					}
					else 
					{
						long_opt = opt;
					}

					// Long Option
					Option* option = lookup_long_option(long_opt);

					if (option) 
					{
						if (option->argument.empty()) 
						{
							read_option(option->key, "");
						} 
						else
						{
							if (pos != std::string::npos) 
							{
								read_option(option->key, long_opt_arg);
							}
							else
							{            
								if (i == argc - 1) 
								{
									throw CL_Error("option '" + std::string(argv[i]) + "' requires an argument");
								}
								else 
								{
									read_option(option->key, argv[i + 1]);
									++i;
								}
							}
						}
					}
					else
					{
						throw CL_Error("unrecognized option '" + std::string(argv[i]) + "'");
					}
				}
			} 
			else 
			{
				// We got a short option
				char* p = argv[i] + 1;
          
				if (*p != '\0') {
					// Handle option chains
					while (*p) 
					{
						// Short option(s)
						Option* option = lookup_short_option(*p);

						if (option) 
						{
							if (option->argument.empty()) 
							{
								read_option(option->key, "");
							} 
							else 
							{
								if (i == argc - 1 || *(p+1) != '\0') 
								{
									// No more arguments
									throw CL_Error("option requires an argument -- " + std::string(1, *p));
								}
								else
								{
									read_option(option->key, argv[i + 1]);
									++i;
								}
							}
						} 
						else 
						{
							throw CL_Error("invalid option -- " + std::string(1, *p));
						}
						++p; 
					}
				} 
				else
				{
					read_option(CL_CommandLine::REST_ARG, "-");
				} 
			}
		} 
		else
		{
			read_option(CL_CommandLine::REST_ARG, argv[i]);
		}
	}

	current_option = parsed_options.end();
}

CL_CommandLine_Generic::Option *CL_CommandLine_Generic::lookup_short_option(char short_option)
{
	for(Options::iterator i = options.begin(); i != options.end(); ++i)
	{
		if (i->key == short_option)
			return &(*i);
	}
	return 0;
}

CL_CommandLine_Generic::Option *CL_CommandLine_Generic::lookup_long_option(const std::string& long_option)
{
	for(Options::iterator i = options.begin(); i != options.end(); ++i)
	{
		if (i->long_option == long_option)
			return &*i;
	}
	return 0;
}

void CL_CommandLine_Generic::read_option(int key, const std::string& argument)
{
	ParsedOption parsed_option;
  
	parsed_option.key = key;
	parsed_option.argument = argument;

	parsed_options.push_back(parsed_option);
}

void CL_CommandLine_Generic::print_help()
{
	bool first_usage = true;
	for(Options::iterator i = options.begin(); i != options.end(); ++i)
	{
		if (i->visible)
		{
			if (i->key == USAGE) 
			{
				if (first_usage) 
				{
					std::cout << "Usage: " << programm << " " <<  i->help << std::endl; 
					first_usage = false;
				}
				else
				{
					std::cout << "or:    " << programm << " " << i->help << std::endl; 
				}
			} 
			else if (i->key == GROUP) 
			{
				if (i != options.begin())
					std::cout << std::endl;
				std::cout << i->help << std::endl;
			}
			else if (i->key == DOC) 
			{
				if (i != options.begin())
					std::cout << std::endl;
				std::cout << i->help << std::endl;
			}
			else 
			{
				char option[256]   = { 0 };
				char argument[256] = { 0 };

				if (i->key > 255 || i->key < 0)
					snprintf(option, 256, "--%s", i->long_option.c_str());
				else if (i->long_option.empty())
					snprintf(option, 256, "-%c", char(i->key));
				else
					snprintf(option, 256, "-%c, --%s", char(i->key), i->long_option.c_str());

				if (!i->argument.empty())
				{
					if (i->long_option.empty())
						snprintf(argument, 256, " %s", i->argument.c_str());
					else
						snprintf(argument, 256, "=%s", i->argument.c_str());
				}

				std::cout << "  " 
							 << std::setiosflags(std::ios::left) << std::setw(help_indent)
							 << (std::string(option) + std::string(argument)) << std::setw(0)
							 << " " << i->help << std::endl;
			}
		}
	}
	std::cout << std::endl;
}

void CL_CommandLine_Generic::add_usage(const std::string& usage)
{
	Option option;

	option.key          = USAGE;
	option.help         = usage;
	option.visible      = true;

	options.push_back(option);   
}

void CL_CommandLine_Generic::add_doc(const std::string& grouptopic)
{
	Option option;

	option.key          = DOC;
	option.help         = grouptopic;
	option.visible      = true;

	options.push_back(option);  
}

void CL_CommandLine_Generic::add_group(const std::string& grouptopic)
{
	Option option;

	option.key          = GROUP;
	option.help         = grouptopic;
	option.visible      = true;

	options.push_back(option);  
}

void CL_CommandLine_Generic::add_option(int key, 
                      const std::string& long_option, 
                      const std::string& argument,
                      const std::string& help,
                      bool visible)
{
	Option option;

	option.key          = key;
	option.help         = help;
	option.long_option  = long_option;
	option.argument     = argument;
	option.visible      = visible;

	options.push_back(option);
}

bool CL_CommandLine_Generic::next()
{
	if (current_option == parsed_options.end()) 
	{
		(current_option = parsed_options.begin());
		return current_option != parsed_options.end();
	}
	else
	{
		return (++current_option) != parsed_options.end();
	}
}

int CL_CommandLine_Generic::get_key()
{
	return current_option->key;
}

std::string CL_CommandLine_Generic::get_argument()
{
	return current_option->argument;
}
