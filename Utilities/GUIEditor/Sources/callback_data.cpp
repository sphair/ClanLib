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
**    Harry Storbacka
*/

#include "precomp.h"
#include "callback_data.h"


class CallbackData_Impl
{
public:
	CallbackData_Impl()
	{
	}

	CL_String component_type;
	CL_String cb_name;
	CL_String param1;
	CL_String param2;
	CL_String return_type;
	CL_String handler_function_name;
};

//////////////////////////////////////////////////////////////////////////
// CallbackData Construction:

CallbackData::CallbackData(
	const CL_StringRef &comp_type, 
	const CL_StringRef &cb_name,
	const CL_StringRef &param1,
	const CL_StringRef &param2)
 : impl(new CallbackData_Impl)
{
	impl->component_type = comp_type;
	impl->cb_name = cb_name;
	impl->param1 = param1;
	impl->param2 = param2;
}

//////////////////////////////////////////////////////////////////////////
// CallbackData Attributes:


CL_StringRef CallbackData::get_component_type() const
{
	return impl->component_type;
}

CL_StringRef CallbackData::get_callback_name() const
{
	return impl->cb_name;
}

CL_StringRef CallbackData::get_param1() const
{
	return impl->param1;

}

CL_StringRef CallbackData::get_param2() const
{
	return impl->param2;
}

CL_StringRef CallbackData::get_return_type() const
{
	return impl->return_type;
}

CL_StringRef CallbackData::get_handler_function_name() const
{
	return impl->handler_function_name;
}

//////////////////////////////////////////////////////////////////////////
// CallbackData Operations:

void CallbackData::set_component_type(const CL_StringRef &str)
{
	impl->component_type = str;
}

void CallbackData::set_callback_name(const CL_StringRef &str)
{
	impl->cb_name = str;
}

void CallbackData::set_param1(const CL_StringRef &str)
{
	impl->param1 = str;
}

void CallbackData::set_param2(const CL_StringRef &str)
{
	impl->param1 = str;
}

void CallbackData::set_return_type(const CL_StringRef &str)
{
	impl->return_type = str;
}

void CallbackData::set_handler_function_name(const CL_StringRef &str)
{
	impl->handler_function_name = str;
}

//////////////////////////////////////////////////////////////////////////
// CallbackData Implementation:
