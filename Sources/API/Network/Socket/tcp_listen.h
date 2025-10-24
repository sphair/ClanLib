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

/// \addtogroup clanNetwork_Socket clanNetwork Socket
/// \{

#pragma once

#include "../api_network.h"
#include "../../Core/System/sharedptr.h"

class CL_Event;
class CL_SocketName;
class CL_TCPConnection;
class CL_TCPListen_Impl;

/// \brief TCP listen socket.
///
/// \xmlonly !group=Network/Socket! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_TCPListen
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a TCPListen
	///
	/// \param name = Socket Name
	/// \param queue_size = value
	/// \param force_bind = bool
	CL_TCPListen(const CL_SocketName &name, int queue_size = 5, bool force_bind = true);

	~CL_TCPListen();

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Get Accept event
	///
	/// \return accept_event
	CL_Event get_accept_event() const;

	/// \brief Get Handle
	///
	/// \return handle
	int get_handle() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Accept
	///
	/// \return TCPConnection
	CL_TCPConnection accept();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_TCPListen_Impl> impl;
/// \}
};

/// \}
