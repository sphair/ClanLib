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
**    Magnus Norddahl
*/

#include "Core/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/exception.h"

/////////////////////////////////////////////////////////////////////////////
// CL_StringFormat Construction:

CL_StringFormat::CL_StringFormat(const CL_StringRef &format_string)
: string(format_string)
{
	bool arg_mode = false;
	unsigned int arg_start = 0;
	int arg_value = 0;
	CL_StringRef::size_type index, size;
	size = string.size();
	for (index = 0; index < size; index++)
	{
		if (!arg_mode)
		{
			if (string[index] == '%')
			{
				arg_mode = true;
				arg_value = 0;
				arg_start = index;
			}
		}
		else
		{
			switch (string[index])
			{
			case '%':
				if (arg_start == index-1)
				{
					arg_mode = false;
					string = string.substr(0, index) + string.substr(index + 1);
					index--;
					size--;
				}
				else
				{
					create_arg(arg_value, arg_start, index-arg_start);
					arg_start = index;
					arg_value = 0;
				}
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				arg_value *= 10;
				arg_value += string[index]-'0';
				break;
			default:
				if (arg_start != index-1)
					create_arg(arg_value, arg_start, index-arg_start);
				arg_mode = false;
				break;
			}
		}
	}
	
	if (arg_mode && arg_start != size-1)
		create_arg(arg_value, arg_start, size-arg_start);
}

CL_StringFormat::~CL_StringFormat()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_StringFormat Attributes:

const CL_String &CL_StringFormat::get_result() const
{
	return string;
}

/////////////////////////////////////////////////////////////////////////////
// CL_StringFormat Operations:

void CL_StringFormat::set_arg(int index, const CL_StringRef &text)
{
	if (index >= (int) args.size())
		return;
		
	ArgPosition pos = args[index];
	if (pos.length == 0)
		return;
		
	int delta_size = ((int) text.length()) - pos.length;
	string = string.substr(0, pos.start) + text + string.substr(pos.start + pos.length);
	args[index].length = text.length();
	
	std::vector<ArgPosition>::size_type i, size;
	size = args.size();
	for (i = 0; i < size; i++)
	{
		if (args[i].start > pos.start)
			args[i].start += delta_size;
	}
}
	
void CL_StringFormat::set_arg(int index, int value, int min_length)
{
	CL_String t = CL_StringHelp::int_to_text(value);
	if ((int) t.length() < min_length)
		t = CL_String(min_length-t.length(), '0') + t;
	set_arg(index, t);
}

void CL_StringFormat::set_arg(int index, unsigned int value, int min_length)
{
	CL_String t = CL_StringHelp::uint_to_text(value);
	if ((int) t.length() < min_length)
		t = CL_String(min_length-t.length(), '0') + t;
	set_arg(index, t);
}

void CL_StringFormat::set_arg(int index, float value)
{
	set_arg(index, CL_StringHelp::float_to_text(value));
}

void CL_StringFormat::set_arg(int index, double value)
{
	set_arg(index, CL_StringHelp::double_to_text(value));
}

/////////////////////////////////////////////////////////////////////////////
// CL_StringFormat Implementation:

void CL_StringFormat::create_arg(int index, int start, int length)
{
	if (index > 256)
		throw CL_Exception("Encountered more than 256 indexes in a formatted string!");

	ArgPosition pos(start, length);
	while (index >= (int) args.size())
		args.push_back(ArgPosition());
	
	args[index].start = start;
	args[index].length = length;
}
