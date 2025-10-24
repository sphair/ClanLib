/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

//! clanNetwork="Internet Relay Chat"
//! header=network.h

#ifndef header_dcc_download
#define header_dcc_download

#ifdef CL_API_DLL
#ifdef CL_NETWORK_EXPORT
#define CL_API_NETWORK __declspec(dllexport)
#else
#define CL_API_NETWORK __declspec(dllimport)
#endif
#else
#define CL_API_NETWORK
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../signals.h"
#include <string>

class CL_DCCDownload_Generic;
class CL_OutputSourceProvider;

//: IRC DCC download session object.
//- !group=Network/Internet Relay Chat!
//- !header=network.h!
//- <p>This class connects to an IRC client listening for an incoming DCC
//- connection and then downloads the file offered. Should be used in
//- response to a DCC SEND CTCP command.</p>
class CL_API_NETWORK CL_DCCDownload
{
//! Construction:
public:
	//: Constructs a new DCC download session.
	CL_DCCDownload();
	
	CL_DCCDownload(const CL_DCCDownload &copy);

	CL_DCCDownload(
		const std::string &server,
		const std::string &port,
		const std::string &filename,
		int total_size = 0,
		CL_OutputSourceProvider *output = 0,
		bool delete_provider = false);
	
	virtual ~CL_DCCDownload();
	
//! Attributes:
public:
	// cannot use "Status" due to brain dead Xlib header #define'ing it to an "int"
	enum DCCStatus
	{
		//: Connecting to DCC server.
		connecting,
		
		//: Currently downloading.
		downloading,
		
		//: Connection was lost to DCC server.
		connection_lost,
		
		//: Download was completed.
		finished
	};

	//: Returns the number of bytes transfered so far.
	int get_bytes_transfered();
	
	//: Returns the total size of file.
	//- <p>If the total size is unknown, it returns 0.</p>
	int get_total_size();
	
	//: Returns the current status of the download.
	DCCStatus get_status();

	//: sig_connection_lost(error_message)
	CL_Signal_v1<const std::string &> &sig_connection_lost();

	//: sig_download_complete()
	CL_Signal_v0 &sig_download_complete();

//! Operations:
public:
	//: Copy assignment operator.
	CL_DCCDownload &operator =(const CL_DCCDownload &copy);

	//: Attemp to reconnect and resume download.
	void reconnect();

//! Implementation:
private:
	CL_DCCDownload_Generic *impl;
};

#endif
