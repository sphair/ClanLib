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

#pragma once

#include "API/Network/Socket/tcp_connection.h"
#include "API/Network/IRC/dcc_download.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/event.h"
#include "API/Core/IOData/virtual_directory.h"

class CL_DCCDownload_Generic
{
/// \name Construction
/// \{

public:
	CL_DCCDownload_Generic(
		const CL_String &server,
		const CL_String &port,
		const CL_String &filename,
		int total_size,
		CL_VirtualDirectory directory);

	~CL_DCCDownload_Generic();


/// \}
/// \name Attributes
/// \{

public:
	CL_Thread thread;

	CL_Mutex mutex;

	CL_String server, port, filename;

	int total_size;

	CL_VirtualDirectory directory;

	CL_DCCDownload::DCCStatus status;

	CL_Event event_shutdown, event_reconnect;

	enum
	{
		no_signal,
		lost_signal,
		complete_signal
	} send_signal;

	CL_String error;

	/// \brief sig_connection_lost(error_message)
	CL_Signal_v1<const CL_String &> sig_connection_lost;

	/// \brief sig_download_complete()
	CL_Signal_v0 sig_download_complete;


/// \}
/// \name Operations
/// \{

public:
	void update();


/// \}
/// \name Implementation
/// \{

private:
	void thread_main();
/// \}
};


