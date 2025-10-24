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
#include "API/Core/IOData/iodevice_memory.h"
#include "iodevice_impl.h"
#include "iodevice_provider_memory.h"

/////////////////////////////////////////////////////////////////////////////
// CL_IODevice_Memory Construction:

CL_IODevice_Memory::CL_IODevice_Memory()
: CL_IODevice(new CL_IODeviceProvider_Memory())
{
}

CL_IODevice_Memory::CL_IODevice_Memory(CL_DataBuffer &data)
: CL_IODevice(new CL_IODeviceProvider_Memory(data))
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODevice_Memory Attributes:

const CL_DataBuffer &CL_IODevice_Memory::get_data() const
{
	const CL_IODeviceProvider_Memory *provider = dynamic_cast<const CL_IODeviceProvider_Memory*>(impl->provider);
	return provider->get_data();
}

CL_DataBuffer &CL_IODevice_Memory::get_data()
{
	CL_IODeviceProvider_Memory *provider = dynamic_cast<CL_IODeviceProvider_Memory*>(impl->provider);
	return provider->get_data();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODevice_Memory Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_IODevice_Memory Implementation:
