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

#include "API/Core/IOData/iodevice_provider.h"
#include "API/Core/System/databuffer.h"

class CL_IODeviceProvider_Memory : public CL_IODeviceProvider
{
/// \name Construction
/// \{

public:
	CL_IODeviceProvider_Memory();

	CL_IODeviceProvider_Memory(CL_DataBuffer &data);


/// \}
/// \name Attributes
/// \{

public:
	virtual int get_size() const;

	virtual int get_position() const;

	const CL_DataBuffer &get_data() const;

	CL_DataBuffer &get_data();


/// \}
/// \name Operations
/// \{

public:
	virtual int send(const void *data, int len, bool send_all = true);

	virtual int receive(void *data, int len, bool receive_all = true);

	virtual int peek(void *data, int len);

	virtual bool seek(int position, CL_IODevice::SeekMode mode);

	CL_IODeviceProvider *duplicate();


/// \}
/// \name Implementation
/// \{

private:
	void validate_position() const;

	CL_DataBuffer data;

	mutable int position;
/// \}
};


