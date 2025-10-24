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

/// \addtogroup clanNetwork_Internet_Relay_Chat clanNetwork Internet Relay Chat
/// \{

#pragma once

#include "../api_network.h"
#include "../../Core/Signals/signal_v0.h"
#include "../../Core/Signals/signal_v1.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/IOData/virtual_directory.h"

class CL_DCCDownload_Generic;
class CL_VirtualDirectory;

/// \brief IRC DCC download session object.
///
///    <p>This class connects to an IRC client listening for an incoming DCC
///    connection and then downloads the file offered. Should be used in
///    response to a DCC SEND CTCP command.</p> 
/// \xmlonly !group=Network/Internet Relay Chat! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_DCCDownload
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a new DCC download session.
	CL_DCCDownload();

	/// \brief Constructs a DCCDownload
	///
	/// \param copy = DCCDownload
	CL_DCCDownload(const CL_DCCDownload &copy);

	CL_DCCDownload(
		const CL_String &server,
		const CL_String &port,
		const CL_String &filename,
		int total_size,
		CL_VirtualDirectory directory = CL_VirtualDirectory());

	virtual ~CL_DCCDownload();

/// \}
/// \name Attributes
/// \{

public:
	// cannot use "Status" due to brain dead Xlib header #define'ing it to an "int"
	enum DCCStatus
	{
		/// \brief Connecting to DCC server.
		connecting,

		/// \brief Currently downloading.
		downloading,

		/// \brief Connection was lost to DCC server.
		connection_lost,

		/// \brief Download was completed.
		finished
	};

	/// \brief Returns the number of bytes transfered so far.
	int get_bytes_transfered();

	/// \brief Returns the total size of file.
	/** <p>If the total size is unknown, it returns 0.</p>*/
	int get_total_size();

	/// \brief Returns the current status of the download.
	DCCStatus get_status();

	/// \brief sig_connection_lost(error_message)
	CL_Signal_v1<const CL_String &> &sig_connection_lost();

	/// \brief sig_download_complete()
	CL_Signal_v0 &sig_download_complete();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Copy assignment operator.
	CL_DCCDownload &operator =(const CL_DCCDownload &copy);

	/// \brief Attemp to reconnect and resume download.
	void reconnect();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_DCCDownload_Generic> impl;
/// \}
};

/// \}
