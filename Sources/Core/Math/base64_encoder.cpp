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
#include "API/Core/Math/base64_encoder.h"
#include "API/Core/System/databuffer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Base64Encoder_Impl class:

static unsigned char cl_base64char[64] =
{
	'A','B','C','D','E','F','G','H','I','J','K','L','M',
	'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'a','b','c','d','e','f','g','h','i','j','k','l','m',
	'n','o','p','q','r','s','t','u','v','w','x','y','z',
	'0','1','2','3','4','5','6','7','8','9','+','/'
};

class CL_Base64Encoder_Impl
{
//! Construction:
public:
	CL_Base64Encoder_Impl() : chunk_filled(0)
	{
	}

//! Attributes:
public:
	CL_DataBuffer result;

	unsigned char chunk[3];

	int chunk_filled;

//! Operations:
public:
	static void encode(unsigned char *output, const unsigned char *input, int size_input)
	{
		int i, o;
		for (i = 0, o = 0; i < size_input; i+=3, o+=4)
		{
			unsigned int v1 = input[i+0];
			unsigned int v2 = input[i+1];
			unsigned int v3 = input[i+2];
			unsigned int value = (v1 << 16) + (v2 << 8) + v3;

			output[o+0] = cl_base64char[(value >> 18) & 63];
			output[o+1] = cl_base64char[(value >> 12) & 63];
			output[o+2] = cl_base64char[(value >> 6) & 63];
			output[o+3] = cl_base64char[value & 63];
		}
	}
};

/////////////////////////////////////////////////////////////////////////////
// CL_Base64Encoder Construction:

CL_Base64Encoder::CL_Base64Encoder()
: impl(new CL_Base64Encoder_Impl)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Base64Encoder Attributes:

CL_DataBuffer &CL_Base64Encoder::get_result()
{
	return impl->result;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Base64Encoder Operations:

void CL_Base64Encoder::reset()
{
	impl->result.set_size(0);
	impl->chunk_filled = 0;
}

void CL_Base64Encoder::feed(const void *_data, int size, bool append_result)
{
	int pos = 0;
	const unsigned char *data = (const unsigned char *) _data;

	if (!append_result)
		impl->result.set_size(0);

	// Handle any left-over data from last encode:

	if (impl->chunk_filled > 0)
	{
		int needed = 3 - impl->chunk_filled;
		if (size >= needed)
		{
			memcpy(impl->chunk + impl->chunk_filled, data, needed);
			int out_pos = impl->result.get_size();
			impl->result.set_size(out_pos + 4);
			CL_Base64Encoder_Impl::encode((unsigned char *) impl->result.get_data() + out_pos, impl->chunk, 3);
			pos += needed;
			impl->chunk_filled = 0;
		}
		else
		{
			memcpy(impl->chunk + impl->chunk_filled, data, size);
			impl->chunk_filled += size;
			return;
		}
	}

	// Base64 encode what's available to us now:

	int blocks = (size-pos) / 3;
	int out_pos = impl->result.get_size();
	impl->result.set_size(out_pos + blocks*4);
	CL_Base64Encoder_Impl::encode((unsigned char *) impl->result.get_data() + out_pos, data + pos, blocks*3);
	pos += blocks*3;

	// Save data for last incomplete block:

	int leftover = size-pos;
	if (leftover > 3)
		throw CL_Exception("Base64 encoder is broken!");
	impl->chunk_filled = leftover;
	memcpy(impl->chunk, data + pos, leftover);
}

void CL_Base64Encoder::finalize(bool append_result)
{
	if (!append_result)
		impl->result.set_size(0);
	if (impl->chunk_filled == 0)
		return;

	// Allocate memory for last block:

	int pos = impl->result.get_size();
	impl->result.set_size(pos + 4);
	unsigned char *output = (unsigned char *) impl->result.get_data() + pos;
	unsigned char *input = impl->chunk;
	int size = impl->chunk_filled;

	// Base64 last block:

	memset(input + size, 0, 3-size);

	unsigned int v1 = input[0];
	unsigned int v2 = input[1];
	unsigned int v3 = input[2];
	unsigned int value = (v1 << 16) + (v2 << 8) + v3;

	output[0] = cl_base64char[(value >> 18) & 63];
	output[1] = cl_base64char[(value >> 12) & 63];
	output[2] = cl_base64char[(value >> 6) & 63];
	output[3] = cl_base64char[value & 63];

	if (impl->chunk_filled == 1)
	{
		output[2] = '=';
		output[3] = '=';
	}
	else if (impl->chunk_filled == 2)
	{
		output[3] = '=';
	}
}

CL_String8 CL_Base64Encoder::encode(const void *data, int size)
{
	CL_Base64Encoder encoder;
	encoder.feed(data, size);
	encoder.finalize(true);
	return CL_String8(encoder.get_result().get_data(), encoder.get_result().get_size());
}

CL_String8 CL_Base64Encoder::encode(const CL_StringRef8 &data)
{
	return encode(data.data(), data.length());
}
	
CL_String8 CL_Base64Encoder::encode(const CL_DataBuffer &data)
{
	return encode(data.get_data(), data.get_size());
}

/////////////////////////////////////////////////////////////////////////////
// CL_Base64Encoder Implementation:
