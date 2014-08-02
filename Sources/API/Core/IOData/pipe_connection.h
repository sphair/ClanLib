/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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


#pragma once

#include "iodevice.h"

namespace clan
{
/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{

class Event;

/// \brief Pipe connection I/O device.
class PipeConnection : public IODevice
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a pipe connection.
	PipeConnection(const std::string &pipe_name);

	/** <p>The 'dummy' boolean for the platform specific constructor is required
	    to avoid situations where strings accidentally becomes a HANDLE parameter
	    instead of a string reference.  Always set the dummy to true.</p>*/
#ifdef WIN32

	/// \brief Constructs a PipeConnection
	///
	/// \param pipe_handle = HANDLE
	/// \param dummy = bool
	PipeConnection(HANDLE pipe_handle, bool dummy);
#else

	/// \brief Constructs a PipeConnection
	///
	/// \param pipe_handle = value
	/// \param dummy = bool
	PipeConnection(int pipe_handle, bool dummy);
#endif

	~PipeConnection();

/// \}
/// \name Attributes
/// \{

public:
#ifdef WIN32
	/// \brief Returns the OS handle for the pipe.
	HANDLE get_handle() const;
#else

	/// \brief Get Handle
	///
	/// \return handle
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

}

/// \}
