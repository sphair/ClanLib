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

#include "Core/precomp.h"
#include "API/Core/IOData/file.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/security_descriptor.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_help.h"
#include "iodevice_impl.h"
#include "iodevice_provider_file.h"

/////////////////////////////////////////////////////////////////////////////
// CL_File Construction:

CL_File::CL_File()
: CL_IODevice(new CL_IODeviceProvider_File())
{
}

CL_File::CL_File(
	const CL_String &filename)
: CL_IODevice(new CL_IODeviceProvider_File(CL_PathHelp::normalize(filename, CL_PathHelp::path_type_file), open_existing, access_read, share_all, 0))
{
}

CL_File::CL_File(
	const CL_String &filename,
	OpenMode open_mode,
	unsigned int access,
	unsigned int share,
	unsigned int flags)
: CL_IODevice(new CL_IODeviceProvider_File(CL_PathHelp::normalize(filename, CL_PathHelp::path_type_file), open_mode, access, share, flags))
{
}
CL_File::CL_File(
	const CL_String &filename,
	OpenMode mode,
	const CL_SecurityDescriptor &permissions,
	unsigned int access,
	unsigned int share,
	unsigned int flags)
: CL_IODevice(new CL_IODeviceProvider_File(CL_PathHelp::normalize(filename, CL_PathHelp::path_type_file), mode, permissions, access, share, flags))
{
}

CL_File::~CL_File()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_File Attributes:

CL_SecurityDescriptor CL_File::get_permissions() const
{
	const CL_IODeviceProvider_File *provider = dynamic_cast<const CL_IODeviceProvider_File*>(impl->provider);
	return provider->get_permissions();
}

/////////////////////////////////////////////////////////////////////////////
// CL_File Operations:

bool CL_File::open(
	const CL_String &filename)
{
	CL_IODeviceProvider_File *provider = dynamic_cast<CL_IODeviceProvider_File*>(impl->provider);
	return provider->open(CL_PathHelp::normalize(filename, CL_PathHelp::path_type_file), open_existing, access_read, share_all, 0);
}

bool CL_File::open(
	const CL_String &filename,
	OpenMode open_mode,
	unsigned int access,
	unsigned int share,
	unsigned int flags)
{
	CL_IODeviceProvider_File *provider = dynamic_cast<CL_IODeviceProvider_File*>(impl->provider);
	return provider->open(CL_PathHelp::normalize(filename, CL_PathHelp::path_type_file), open_mode, access, share, flags);
}

bool CL_File::open(
	const CL_String &filename,
	OpenMode mode,
	const CL_SecurityDescriptor &permissions,
	unsigned int access,
	unsigned int share,
	unsigned int flags)
{
	CL_IODeviceProvider_File *provider = dynamic_cast<CL_IODeviceProvider_File*>(impl->provider);
	return provider->open(CL_PathHelp::normalize(filename, CL_PathHelp::path_type_file), mode, permissions, access, share, flags);
}
	
void CL_File::close()
{
	CL_IODeviceProvider_File *provider = dynamic_cast<CL_IODeviceProvider_File*>(impl->provider);
	provider->close();
}

bool CL_File::set_permissions(const CL_SecurityDescriptor &permissions)
{
	CL_IODeviceProvider_File *provider = dynamic_cast<CL_IODeviceProvider_File*>(impl->provider);
	return provider->set_permissions(permissions);
}

/////////////////////////////////////////////////////////////////////////////
// CL_File Implementation:

