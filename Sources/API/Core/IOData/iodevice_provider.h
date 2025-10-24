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

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{


#pragma once


#include "../api_core.h"
#include "iodevice.h"

/// \brief I/O device provider interface.
///
/// \xmlonly !group=Core/IO Data! !header=core.h! \endxmlonly
class CL_API_CORE CL_IODeviceProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_IODeviceProvider() { return; }


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the size of data stream.
	/** <p>Returns -1 if the size is unknown.</p>*/
	virtual int get_size() const { return -1; }

	/// \brief Returns the position in the data stream.
	/** <p>Returns -1 if the position is unknown.</p>*/
	virtual int get_position() const { return -1; }


/// \}
/// \name Operations
/// \{

public:
	/// \brief Send data to device.
	virtual int send(const void *data, int len, bool send_all = true) = 0;

	/// \brief Receive data from device.
	virtual int receive(void *data, int len, bool receive_all = true) = 0;

	/// \brief Peek data from device.
	virtual int peek(void *data, int len) = 0;

	/// \brief Returns a new provider to the same resource.
	virtual CL_IODeviceProvider *duplicate() = 0;

	/// \brief Seek in data stream.
	virtual bool seek(int position, CL_IODevice::SeekMode mode) { return false; }


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


/// \}
