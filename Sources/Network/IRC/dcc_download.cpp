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

#include "API/Network/IRC/dcc_download.h"
#include "dcc_download_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DCCDownload Construction:

CL_DCCDownload::CL_DCCDownload() : impl(0)
{
}

CL_DCCDownload::CL_DCCDownload(
	const std::string &server,
	const std::string &port,
	const std::string &filename,
	int total_size,
	CL_OutputSourceProvider *output,
	bool delete_provider) : impl(0)
{
	impl = new CL_DCCDownload_Generic(server, port, filename, total_size, output, delete_provider);
	impl->add_ref();
}

CL_DCCDownload::CL_DCCDownload(const CL_DCCDownload &copy) : impl(copy.impl)
{
	if (impl) impl->add_ref();
}
	
CL_DCCDownload::~CL_DCCDownload()
{
	if (impl) impl->release_ref();
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_DCCDownload Attributes:

int CL_DCCDownload::get_bytes_transfered()
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return 0;
}
	
int CL_DCCDownload::get_total_size()
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->total_size;
}
	
CL_DCCDownload::DCCStatus CL_DCCDownload::get_status()
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->status;
}

CL_Signal_v1<const std::string &> &CL_DCCDownload::sig_connection_lost()
{
	return impl->sig_connection_lost;
}

CL_Signal_v0 &CL_DCCDownload::sig_download_complete()
{
	return impl->sig_download_complete;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DCCDownload Operations:

CL_DCCDownload &CL_DCCDownload::operator =(const CL_DCCDownload &copy)
{
	if (impl == copy.impl) return *this;
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
	return *this;
}

void CL_DCCDownload::reconnect()
{
	impl->reconnect_trigger.set_flag();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DCCDownload Implementation:
