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

#ifndef header_dcc_download_generic
#define header_dcc_download_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Network/Socket/socket.h"
#include "API/Network/IRC/dcc_download.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/event_trigger.h"

class CL_DCCDownload_Generic : private CL_Runnable, private CL_KeepAlive
{
//! Construction:
public:
	CL_DCCDownload_Generic(
		const std::string &server,
		const std::string &port,
		const std::string &filename,
		int total_size,
		CL_OutputSourceProvider *provider,
		bool delete_provider);

	~CL_DCCDownload_Generic();

//! Attributes:
public:
	CL_Thread thread;
	
	CL_Mutex mutex;
	
	std::string server, port, filename;
	
	int total_size;
	
	CL_OutputSourceProvider *provider;

	bool delete_provider;
	
	CL_DCCDownload::DCCStatus status;
	
	CL_EventTrigger shutdown_trigger, reconnect_trigger;
	
	enum
	{
		no_signal,
		lost_signal,
		complete_signal
	} send_signal;
	
	std::string error;

	//: sig_connection_lost(error_message)
	CL_Signal_v1<const std::string &> sig_connection_lost;

	//: sig_download_complete()
	CL_Signal_v0 sig_download_complete;

//! Operations:
public:
	void add_ref();

	void release_ref();

//! Implementation:
private:
	virtual void run();
	
	virtual void keep_alive();
	
	int ref_count;
};

#endif
