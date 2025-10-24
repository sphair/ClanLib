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
#include "API/Core/Math/base64_decoder.h"
#include "API/Core/System/databuffer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Base64Decoder_Impl class:

/*
	'A','B','C','D','E','F','G','H','I','J','K','L','M',
	'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'a','b','c','d','e','f','g','h','i','j','k','l','m',
	'n','o','p','q','r','s','t','u','v','w','x','y','z',
	'0','1','2','3','4','5','6','7','8','9','+','/'
*/

static unsigned char cl_char_to_base64[256] =
{
	 0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0, 62,  0,  0,  0, 63,
	52, 53, 54, 55, 56, 57, 58, 59,  60, 61,  0,  0,  0,  0,  0,  0,
	 0,  0,  1,  2,  3,  4,  5,  6,   7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22,  23, 24, 25,  0,  0,  0,  0,  0,
	 0, 26, 27, 28, 29, 30, 31, 32,  33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48,  49, 50, 51,  0,  0,  0,  0,  0,

	 0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0
};

class CL_Base64Decoder_Impl
{
//! Construction:
public:
	CL_Base64Decoder_Impl() : chunk_filled(0)
	{
	}

//! Attributes:
public:
	CL_DataBuffer result;

	unsigned char chunk[4];

	int chunk_filled;

//! Operations:
public:
	static void decode(unsigned char *output, const unsigned char *input, int size_input)
	{
		int i, o;
		for (i = 0, o = 0; i < size_input; i+=4, o+=3)
		{
			unsigned int v1 = cl_char_to_base64[input[i+0]];
			unsigned int v2 = cl_char_to_base64[input[i+1]];
			unsigned int v3 = cl_char_to_base64[input[i+2]];
			unsigned int v4 = cl_char_to_base64[input[i+3]];
			unsigned int value = (v1 << 18) + (v2 << 12) + (v3 << 6) + v4;

			output[o+0] = (value >> 16) & 255;
			output[o+1] = (value >> 8) & 255;
			output[o+2] = value & 255;
		}
	}
};

/////////////////////////////////////////////////////////////////////////////
// CL_Base64Decoder Construction:

CL_Base64Decoder::CL_Base64Decoder()
: impl(new CL_Base64Decoder_Impl)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Base64Decoder Attributes:

CL_DataBuffer &CL_Base64Decoder::get_result()
{
	return impl->result;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Base64Decoder Operations:

void CL_Base64Decoder::reset()
{
	impl->result.set_size(0);
	impl->chunk_filled = 0;
}

void CL_Base64Decoder::feed(const void *_data, int size, bool append_result)
{
	int pos = 0;
	const unsigned char *data = (const unsigned char *) _data;

	if (!append_result)
		impl->result.set_size(0);

	// Handle any left-over data from last encode:

	if (impl->chunk_filled > 0)
	{
		int needed = 4 - impl->chunk_filled;
		if (size >= needed)
		{
			// Finish left-over data block:

			memcpy(impl->chunk + impl->chunk_filled, data, needed);
			int out_pos = impl->result.get_size();
			impl->result.set_size(out_pos + 3);
			CL_Base64Decoder_Impl::decode((unsigned char *) impl->result.get_data() + out_pos, impl->chunk, 4);
			pos += needed;
			impl->chunk_filled = 0;

			// Shorten result if we got an end of base64 data marker:

			if (impl->chunk[2] == '=')
				impl->result.set_size(impl->result.get_size()-2);
			else if (impl->chunk[3] == '=')
				impl->result.set_size(impl->result.get_size()-1);
		}
		else
		{
			memcpy(impl->chunk + impl->chunk_filled, data, size);
			impl->chunk_filled += size;
			return;
		}
	}

	// Base64 encode what's available to us now:

	int blocks = (size-pos) / 4;
	int out_pos = impl->result.get_size();
	impl->result.set_size(out_pos + blocks*3);
	CL_Base64Decoder_Impl::decode((unsigned char *) impl->result.get_data() + out_pos, data + pos, blocks*4);
	pos += blocks*4;

	// Shorten result if we got an end of base64 data marker:

	if (blocks > 0)
	{
		if (data[pos-2] == '=')
			impl->result.set_size(impl->result.get_size()-2);
		else if (data[pos-1] == '=')
			impl->result.set_size(impl->result.get_size()-1);
	}

	// Save data for last incomplete block:

	int leftover = size-pos;
	if (leftover > 4)
		throw CL_Exception("Base64 decoder is broken!");
	impl->chunk_filled = leftover;
	memcpy(impl->chunk, data + pos, leftover);
}

CL_DataBuffer CL_Base64Decoder::decode(const void *data, int size)
{
	CL_Base64Decoder decoder;
	decoder.feed(data, size);
	return decoder.get_result();
}

CL_DataBuffer CL_Base64Decoder::decode(const CL_StringRef8 &data)
{
	return decode(data.data(), data.length());
}
	
CL_DataBuffer CL_Base64Decoder::decode(const CL_DataBuffer &data)
{
	return decode(data.get_data(), data.get_size());
}

/////////////////////////////////////////////////////////////////////////////
// CL_Base64Decoder Implementation:
