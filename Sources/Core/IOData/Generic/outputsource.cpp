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

#include "Core/precomp.h"
#include "API/Core/IOData/outputsource.h"
#include "API/Core/IOData/cl_endian.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource operations:

void CL_OutputSource::set_system_mode()
{
	little_endian_mode = !CL_Endian::is_system_big();
}

void CL_OutputSource::set_big_endian_mode()
{
	little_endian_mode = false;
}

void CL_OutputSource::set_little_endian_mode()
{
	little_endian_mode = true;
}

void CL_OutputSource::write_int64(cl_int64 data)
{
	cl_int64 final = data;
	if (little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(cl_int64));
}

void CL_OutputSource::write_uint64(cl_uint64 data)
{
	cl_uint64 final = data;
	if (little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(cl_uint64));
}

void CL_OutputSource::write_int32(cl_int32 data)
{
	cl_int32 final = data;
	if (little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(cl_int32));
}

void CL_OutputSource::write_uint32(cl_uint32 data)
{
	cl_uint32 final = data;
	if (little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(cl_uint32));
}

void CL_OutputSource::write_int16(cl_int16 data)
{
	cl_int16 final = data;
	if (little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(cl_int16));
}

void CL_OutputSource::write_uint16(cl_uint16 data)
{
	cl_uint16 final = data;
	if (little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(cl_uint16));
}

void CL_OutputSource::write_int8(cl_int8 data)
{
	write(&data, sizeof(cl_int8));
}

void CL_OutputSource::write_uint8(cl_uint8 data)
{
	write(&data, sizeof(cl_uint8));
}

void CL_OutputSource::write_short16(short data)
{
	short final = data;
	if (little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(short));
}

void CL_OutputSource::write_ushort16(unsigned short data)
{
	unsigned short final = data;
	if (little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(unsigned short));
}

void CL_OutputSource::write_char8(char data)
{
	write(&data, sizeof(char));
}

void CL_OutputSource::write_uchar8(unsigned char data)
{
	write(&data, sizeof(unsigned char));
}

void CL_OutputSource::write_float32(float data)
{
	float final = data;
	if (little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(float));
}

void CL_OutputSource::write_float64(double data)
{
	double final = data;
	if (little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(double));
}

void CL_OutputSource::write_bool8(bool data)
{
	bool final = data;
	if (little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(bool));
}

void CL_OutputSource::write_string(const std::string &str)
{
	int size = str.length();
	write_int32(size);
	write(str.data(), size);
}
