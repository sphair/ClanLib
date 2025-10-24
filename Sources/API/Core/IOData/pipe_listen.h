/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{


#pragma once


#include "../api_core.h"
#include "../System/sharedptr.h"

class CL_Event;
class CL_PipeConnection;
class CL_PipeListen_Impl;

/// \brief Pipe listen class.
///
/// \xmlonly !group=Core/IO Data! !header=core.h! \endxmlonly
class CL_API_CORE CL_PipeListen
{
/// \name Construction
/// \{

public:
	CL_PipeListen(const CL_String &name);

	~CL_PipeListen();


/// \}
/// \name Attributes
/// \{

public:
#ifdef WIN32
	/// \brief Returns the OS handle for the pipe.
	HANDLE get_handle() const;
#else
	int get_handle() const;
#endif


/// \}
/// \name Operations
/// \{

public:
	/// \brief Begin listening for pipe connection attempts.
	CL_Event begin_accept();

	/// \brief Establish connection.
	CL_PipeConnection complete_accept();

	/// \brief Stop listening for connection attempts..
	void cancel_accept();

	/// \brief Block until a connection is established.
	CL_PipeConnection accept();


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_PipeListen_Impl> impl;
/// \}
};


/// \}
