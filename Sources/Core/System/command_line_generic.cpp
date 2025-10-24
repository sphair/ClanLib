/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Ingo Ruhnke
*/

#include "Core/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/console.h"
#include "API/Core/Text/string_help.h"

#include <iomanip>
#include <iostream>
#include <cstdio>

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
				if (argv[i][2] == '\0')
				{ 
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
					CL_String8 opt = argv[i] + 2;
					CL_String8 long_opt;
					CL_String8 long_opt_arg;

					CL_String8::size_type pos = opt.find('=');

					if (pos != CL_String8::npos) 
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
							if (pos != CL_String8::npos) 
							{
								read_option(option->key, long_opt_arg);
							}
							else
							{            
								if (i == argc - 1) 
								{
									CL_StringFormat str("option '%1' requires an argument");
									str.set_arg(1, argv[i]);
									throw CL_Exception(str.get_result());
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
						CL_StringFormat str("unrecognized option '%1'");
						str.set_arg(1, argv[i]);
						throw CL_Exception(str.get_result());
					}
				}
			} 
			else 
			{
				// We got a short option
				char* p = argv[i] + 1;
          
				if (*p != '\0')
				{
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
									CL_StringFormat str("option requires an argument -- %1");
									str.set_arg(1, CL_StringHelp::local8_to_text(CL_String8(1, *p)));
									throw CL_Exception(str.get_result());
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
							CL_StringFormat str("invalid option -- %1");
							str.set_arg(1, CL_StringHelp::local8_to_text(CL_String8(1, *p)));
							throw CL_Exception(str.get_result());
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

CL_CommandLine_Generic::Option *CL_CommandLine_Generic::lookup_long_option(const CL_String8& long_option)
{
	for(Options::iterator i = options.begin(); i != options.end(); ++i)
	{
		if (i->long_option == long_option)
			return &*i;
	}
	return 0;
}

void CL_CommandLine_Generic::read_option(int key, const CL_String8& argument)
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
					CL_Console::write_line("Usage: %1 %2", CL_StringHelp::local8_to_text(programm), CL_StringHelp::local8_to_text(i->help));
					first_usage = false;
				}
				else
				{
					CL_Console::write_line("or:    %1 %2", CL_StringHelp::local8_to_text(programm), CL_StringHelp::local8_to_text(i->help));
				}
			} 
			else if (i->key == GROUP) 
			{
				if (i != options.begin())
					CL_Console::write_line("");
				CL_Console::write_line(CL_StringHelp::local8_to_text(i->help));
			}
			else if (i->key == DOC) 
			{
				if (i != options.begin())
					CL_Console::write_line("");
				CL_Console::write_line(CL_StringHelp::local8_to_text(i->help));
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

				CL_String8 syntax = CL_StringRef8(option) + CL_StringRef8(argument);
				syntax.resize(help_indent, ' ');
				CL_Console::write_line("  %1 %2", CL_StringHelp::local8_to_text(syntax), CL_StringHelp::local8_to_text(i->help));
			}
		}
	}
	CL_Console::write_line("");
}

void CL_CommandLine_Generic::add_usage(const CL_String8& usage)
{
	Option option;

	option.key          = USAGE;
	option.help         = usage;
	option.visible      = true;

	options.push_back(option);   
}

void CL_CommandLine_Generic::add_doc(const CL_String8& grouptopic)
{
	Option option;

	option.key          = DOC;
	option.help         = grouptopic;
	option.visible      = true;

	options.push_back(option);  
}

void CL_CommandLine_Generic::add_group(const CL_String8& grouptopic)
{
	Option option;

	option.key          = GROUP;
	option.help         = grouptopic;
	option.visible      = true;

	options.push_back(option);  
}

void CL_CommandLine_Generic::add_option(int key, 
                      const CL_String8& long_option, 
                      const CL_String8& argument,
                      const CL_String8& help,
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

CL_String8 CL_CommandLine_Generic::get_argument()
{
	return current_option->argument;
}
