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

/// \addtogroup clanCore_System clanCore System
/// \{

#pragma once

#include "../api_core.h"
#include "../Text/string_types.h"
#include <vector>
#include <stdexcept>

/// \brief Top-level exception class.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_Exception : public std::exception
{
/// \name Construction
/// \{
public:
	/// \brief Constructs an exception object.
	CL_Exception(const CL_String &message);

	/// \brief Destructs an exception object
	~CL_Exception() throw() {}

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Description of exception.
	CL_String message;

	/// \brief Returns description of exception
	virtual const char* what() const throw();

/// \}
/// \name Operations
/// \{
public:
	/// \brief Returns the call stack present when the exception object was created.
	/** <p>On Linux, to obtain function names, remember to link with the -rdynamic flag </p>*/
	std::vector<CL_String> get_stack_trace();

	/// \brief Returns the message and call stack present when the exception object was created, formatted using newlines.
	/** <p>On Linux, to obtain function names, remember to link with the -rdynamic flag </p>*/
	CL_String get_message_and_stack_trace();

/// \}
/// \name Implementation
/// \{
private:
	enum { max_frames = 32 };
	void *frames[max_frames];
	int num_frames;
	mutable CL_String8 buffer;
/// \}
};

/// \}
