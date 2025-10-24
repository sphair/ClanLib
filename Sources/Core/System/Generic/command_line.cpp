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
#include "API/Core/System/command_line.h"
#include "command_line_generic.h"

CL_CommandLine::CL_CommandLine()
{
	impl = new CL_CommandLine_Generic();
}

CL_CommandLine::~CL_CommandLine()
{
	delete impl;
}

void CL_CommandLine::parse_args(int argc, char** argv)
{
	impl->parse_args(argc, argv);
}

void CL_CommandLine::print_help()
{
	impl->print_help();
}

void CL_CommandLine::set_help_indent(int i)
{
	impl->set_help_indent(i);
}

void CL_CommandLine::add_usage(const std::string& usage)
{
	impl->add_usage(usage);
}

void CL_CommandLine::add_doc(const std::string& doc)
{
	impl->add_doc(doc);
}

void CL_CommandLine::add_group(const std::string& grouptopic)
{
	impl->add_group(grouptopic);
}

void CL_CommandLine::add_option(
	int key, 
	const std::string& long_option, 
	const std::string& argument,
	const std::string& help,
	bool visible)
{
	impl->add_option(key, long_option, argument, help, visible);
}

bool CL_CommandLine::next()
{
	return impl->next();
}

int CL_CommandLine::get_key()
{
	return impl->get_key();
}

std::string CL_CommandLine::get_argument()
{
	return impl->get_argument();
}
