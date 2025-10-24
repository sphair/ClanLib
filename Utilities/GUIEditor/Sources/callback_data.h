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

#pragma once

class CallbackData_Impl;

class CallbackData
{
public:
	CallbackData(
		const CL_StringRef &comp_type, 
		const CL_StringRef &cb_name,
		const CL_StringRef &param1,
		const CL_StringRef &param2);

//!Attributes:
public:
	CL_StringRef get_component_type() const;
	CL_StringRef get_callback_name() const;
	CL_StringRef get_param1() const;
	CL_StringRef get_param2() const;
	CL_StringRef get_return_type() const;
	CL_StringRef get_handler_function_name() const;
	
//!Operations:
public:
	void set_component_type(const CL_StringRef &str);
	void set_callback_name(const CL_StringRef &str);
	void set_param1(const CL_StringRef &str);
	void set_param2(const CL_StringRef &str);
	void set_return_type(const CL_StringRef &str);
	void set_handler_function_name(const CL_StringRef &str);
	
//!Implementation:
private:
	CL_SharedPtr<CallbackData_Impl> impl;
};
