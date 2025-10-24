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
#include "iodevice.h"

class CL_Event;

/// \brief Pipe connection I/O device.
///
/// \xmlonly !group=Core/IO Data! !header=core.h! \endxmlonly
class CL_API_CORE CL_PipeConnection : public CL_IODevice
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a pipe connection.
	CL_PipeConnection(const CL_StringRef &pipe_name);

	/** <p>The 'dummy' boolean for the platform specific constructor is required
	    to avoid situations where strings accidentally becomes a HANDLE parameter
	    instead of a string reference.  Always set the dummy to true.</p>*/
#ifdef WIN32
	CL_PipeConnection(HANDLE pipe_handle, bool dummy);
#else
	CL_PipeConnection(int pipe_handle, bool dummy);
#endif

	~CL_PipeConnection();


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
	/// \brief Disconnect pipe.
	void disconnect();


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


/// \}
