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
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/cl_endian.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource operations:

void CL_InputSource::set_system_mode()
{
	little_endian_mode = !CL_Endian::is_system_big();
}

void CL_InputSource::set_big_endian_mode()
{
	little_endian_mode = false;
}

void CL_InputSource::set_little_endian_mode()
{
	little_endian_mode = true;
}

cl_int64 CL_InputSource::read_int64()
{
	cl_int64 answer;
	if (read(&answer, sizeof(cl_int64)) != sizeof(cl_int64)) throw CL_Error("CL_InputSource_Datafile::read_int64() failed");
	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_uint64 CL_InputSource::read_uint64()
{
	cl_uint64 answer;
	if (read(&answer, sizeof(cl_uint64)) != sizeof(cl_uint64)) throw CL_Error("CL_InputSource_Datafile::read_uint64() failed");
	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_int32 CL_InputSource::read_int32()
{
	cl_int32 answer;
	if (read(&answer, sizeof(cl_int32)) != sizeof(cl_int32)) throw CL_Error("CL_InputSource_Datafile::read_int32() failed");
	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_uint32 CL_InputSource::read_uint32()
{
	cl_uint32 answer;
	if (read(&answer, sizeof(cl_uint32)) != sizeof(cl_uint32)) throw CL_Error("CL_InputSource_Datafile::read_uint32() failed");
	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_int16 CL_InputSource::read_int16()
{
	cl_int16 answer;
	if (read(&answer, sizeof(cl_int16)) != sizeof(cl_int16)) throw CL_Error("CL_InputSource_Datafile::read_int16() failed");
	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_uint16 CL_InputSource::read_uint16()
{
	cl_uint16 answer;
	if (read(&answer, sizeof(cl_uint16)) != sizeof(cl_uint16)) throw CL_Error("CL_InputSource_Datafile::read_uint16() failed");
	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_int8 CL_InputSource::read_int8()
{
	cl_int8 answer;
	if (read(&answer, sizeof(cl_int8)) != sizeof(cl_int8)) throw CL_Error("CL_InputSource_Datafile::read_int8() failed");
	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_uint8 CL_InputSource::read_uint8()
{
	cl_uint8 answer;
	if (read(&answer, sizeof(cl_uint8)) != sizeof(cl_uint8)) throw CL_Error("CL_InputSource_Datafile::read_uint8() failed");
	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

short CL_InputSource::read_short16()
{
	short answer;
	if (read(&answer, sizeof(short)) != sizeof(short)) throw CL_Error("CL_InputSource_Datafile::read_short16() failed");

	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}

	return answer;
}

unsigned short CL_InputSource::read_ushort16()
{
	unsigned short answer;
	if (read(&answer, sizeof(unsigned short)) != sizeof(unsigned short)) throw CL_Error("CL_InputSource_Datafile::read_ushort16() failed");

	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}

	return answer;
}

char CL_InputSource::read_char8()
{
	char answer;
	if (read(&answer, sizeof(char)) != sizeof(char)) throw CL_Error("CL_InputSource_Datafile::read_char8() failed");

	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}

	return answer;
}

unsigned char CL_InputSource::read_uchar8()
{
	unsigned char answer;
	if (read(&answer, sizeof(unsigned char)) != sizeof(unsigned char)) throw CL_Error("CL_InputSource_Datafile::read_uchar8() failed");

	return answer;
}

float CL_InputSource::read_float32()
{
	float answer;
	if (read(&answer, sizeof(float)) != sizeof(float)) throw CL_Error("CL_InputSource_Datafile::read_float32() failed");

	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}

	return answer;
}

double CL_InputSource::read_float64()
{
	double answer;
	if (read(&answer, sizeof(double)) != sizeof(double)) throw CL_Error("CL_InputSource_Datafile::read_float64() failed");

	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}

	return answer;
}

bool CL_InputSource::read_bool8()
{
	bool answer;
	if (read(&answer, sizeof(bool)) != sizeof(bool)) throw CL_Error("CL_InputSource_Datafile::read_bool8() failed");

	if (little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}

	return answer;
}

std::string CL_InputSource::read_string()
{
	int size = read_int32();

	char *str = new char[size];
	try
	{
		read(str, size);
		
		std::string ret(str, size);
		delete[] str;

		return ret;
	}
	catch (...)
	{
		delete[] str;
		throw;
	}
}
