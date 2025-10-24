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

/// \addtogroup clanNetwork_NetSessions clanNetwork NetSessions
/// \{


#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include "../api_network.h"
#include "../../Core/IOData/iodevice.h"

class CL_Event;
class CL_NetSession;
class CL_NetComputer;

/// \brief Net stream class.
///
/// \xmlonly !group=Network/NetSessions! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_NetStream : public CL_IODevice
{
/// \name Construction
/// \{

public:
	/// \brief Connects to a remote netstream.
	///
	/// \param stream_channel Channel identifier establishing connection to.
	/// \param dest Remote computer on which to connect.
	CL_NetStream(
		const CL_String &stream_channel,
		CL_NetComputer &dest);

	CL_NetStream(const CL_NetStream &copy);

	~CL_NetStream();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Event trigger for reading.
	CL_Event get_read_event();

	/// \brief Event trigger for writing.
	CL_Event get_write_event();


/// \}
/// \name Operations
/// \{

public:
	/// \brief Copy constructor.
	CL_NetStream &operator =(const CL_NetStream &copy);


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


/// \}
