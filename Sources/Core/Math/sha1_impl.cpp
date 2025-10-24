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
#include "sha1_impl.h"

#ifndef WIN32
#include <cstring>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_SHA1_Impl Construction:

CL_SHA1_Impl::CL_SHA1_Impl()
{
	reset();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SHA1_Impl Attributes:

CL_String8 CL_SHA1_Impl::get_hash(bool uppercase)
{
	if (calculated == false)
		throw CL_Exception("SHA-1 hash has not been calculated yet!");
	
	char digest[41];
	memset(digest, 0, 41);
	to_hex(digest, h0, uppercase);
	to_hex(digest+8, h1, uppercase);
	to_hex(digest+16, h2, uppercase);
	to_hex(digest+24, h3, uppercase);
	to_hex(digest+32, h4, uppercase);
	return digest;
}

void CL_SHA1_Impl::get_hash(unsigned char out_hash[20])
{
	if (calculated == false)
		throw CL_Exception("SHA-1 hash has not been calculated yet!");

	out_hash[0] = (unsigned char) ((h0 >> 24) & 0xff);
	out_hash[1] = (unsigned char) ((h0 >> 16) & 0xff);
	out_hash[2] = (unsigned char) ((h0 >> 8) & 0xff);
	out_hash[3] = (unsigned char) (h0 & 0xff);
	out_hash[4] = (unsigned char) ((h1 >> 24) & 0xff);
	out_hash[5] = (unsigned char) ((h1 >> 16) & 0xff);
	out_hash[6] = (unsigned char) ((h1 >> 8) & 0xff);
	out_hash[7] = (unsigned char) (h1 & 0xff);
	out_hash[8] = (unsigned char) ((h2 >> 24) & 0xff);
	out_hash[9] = (unsigned char) ((h2 >> 16) & 0xff);
	out_hash[10] = (unsigned char) ((h2 >> 8) & 0xff);
	out_hash[11] = (unsigned char) (h2 & 0xff);
	out_hash[12] = (unsigned char) ((h3 >> 24) & 0xff);
	out_hash[13] = (unsigned char) ((h3 >> 16) & 0xff);
	out_hash[14] = (unsigned char) ((h3 >> 8) & 0xff);
	out_hash[15] = (unsigned char) (h3 & 0xff);
	out_hash[16] = (unsigned char) ((h4 >> 24) & 0xff);
	out_hash[17] = (unsigned char) ((h4 >> 16) & 0xff);
	out_hash[18] = (unsigned char) ((h4 >> 8) & 0xff);
	out_hash[19] = (unsigned char) (h4 & 0xff);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SHA1_Impl Operations:

void CL_SHA1_Impl::reset()
{
	h0 = 0x67452301;
	h1 = 0xEFCDAB89;
	h2 = 0x98BADCFE;
	h3 = 0x10325476;
	h4 = 0xC3D2E1F0;
	memset(chunk, 0, 64);
	chunk_filled = 0;
	length_message = 0;
	calculated = false;
}

void CL_SHA1_Impl::add(const void *_data, int size)
{
	if (calculated)
		reset();
		
	#define cl_min(a,b) ((a) < (b) ? (a) : (b))

	const unsigned char *data = (const unsigned char *) _data;
	int pos = 0;
	while (pos < size)
	{
		int data_left = size - pos;
		int buffer_space = 64 - chunk_filled;
		int data_used = cl_min(buffer_space, data_left);
		memcpy(chunk + chunk_filled, data + pos, data_used);
		chunk_filled += data_used;
		pos += data_used;
		if (chunk_filled == 64)
		{
			process_chunk();
			chunk_filled = 0;
		}
	}
	length_message += size * (cl_uint64) 8;
}

void CL_SHA1_Impl::calculate()
{
	if (calculated)
		reset();

	// append a single "1" bit to message
	// append "0" bits until message length ≡ 448 ≡ -64 (mod 512)
	// append length of message, in bits as 64-bit big-endian integer to message
	
	unsigned char end_data[128];
	memset(end_data, 0, 128);
	end_data[0] = 128;
	
	int size = 64 - chunk_filled;
	if (size < 9)
		size += 64;

	unsigned int length_upper = (unsigned int) (length_message >> 32);
	unsigned int length_lower = (unsigned int) (length_message & 0xffffffff);
	
	end_data[size-8] = (length_upper & 0xff000000) >> 24;
	end_data[size-7] = (length_upper & 0x00ff0000) >> 16;
	end_data[size-6] = (length_upper & 0x0000ff00) >> 8;
	end_data[size-5] = (length_upper & 0x000000ff);
	end_data[size-4] = (length_lower & 0xff000000) >> 24;
	end_data[size-3] = (length_lower & 0x00ff0000) >> 16;
	end_data[size-2] = (length_lower & 0x0000ff00) >> 8;
	end_data[size-1] = (length_lower & 0x000000ff);
		
	add(end_data, size);
	
	if (chunk_filled != 0)
		throw CL_Exception("Error in CL_SHA1_Impl class. Still chunk data at end of calculate");

	calculated = true;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SHA1_Impl Implementation:

void CL_SHA1_Impl::process_chunk()
{
	int i;
	unsigned int w[80];

	for (i = 0; i < 16; i++)
	{
		unsigned int b1 = chunk[i*4];
		unsigned int b2 = chunk[i*4+1];
		unsigned int b3 = chunk[i*4+2];
		unsigned int b4 = chunk[i*4+3];
		w[i] = (b1 << 24) + (b2 << 16) + (b3 << 8) + b4;
	}
	
	for (i = 16; i < 80; i++)
		w[i] = leftrotate_uint32(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
		
	cl_uint32 a = h0;
	cl_uint32 b = h1;
	cl_uint32 c = h2;
	cl_uint32 d = h3;
	cl_uint32 e = h4;
	
	for (i = 0; i < 80; i++)
	{
		cl_uint32 f, k;
		if (i < 20)
		{
			f = (b & c) | ((~b) & d);
			k = 0x5A827999;
		}
		else if (i < 40)
		{
			f = b ^ c ^ d;
			k = 0x6ED9EBA1;
		}
		else if (i < 60)
		{
			f = (b & c) | (b & d) | (c & d);
			k = 0x8F1BBCDC;
		}
		else
		{
			f =  b ^ c ^ d;
			k = 0xCA62C1D6;
		}
		
		cl_uint32 temp = leftrotate_uint32(a, 5) + f + e + k + w[i];
		e = d;
		d = c;
		c = leftrotate_uint32(b, 30);
		b = a;
		a = temp;
	}
	
	h0 += a;
	h1 += b;
	h2 += c;
	h3 += d;
	h4 += e;
}

void CL_SHA1_Impl::to_hex(char *buffer, cl_uint32 value, bool uppercase)
{
	cl_uint32 values[4];
	values[0] = ((value & 0xff000000) >> 24);
	values[1] = ((value & 0x00ff0000) >> 16);
	values[2] = ((value & 0x0000ff00) >> 8);
	values[3] = (value & 0x000000ff);
	
	cl_uint32 low = '0';
	cl_uint32 high = uppercase ? 'A' : 'a';
	for (int i = 0; i < 4; i++)
	{
		cl_uint32 a = ((values[i] & 0xf0) >> 4);
		cl_uint32 b = (values[i] & 0x0f);
		buffer[i*2+0] = (a < 10) ? (low + a) : (high + a - 10);
		buffer[i*2+1] = (b < 10) ? (low + b) : (high + b - 10);
	}
}

inline unsigned int CL_SHA1_Impl::leftrotate_uint32(unsigned int value, int shift)
{
	return (value << shift) + (value >> (32-shift));
}
