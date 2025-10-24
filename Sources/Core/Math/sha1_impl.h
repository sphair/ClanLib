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


#include "API/Core/IOData/datatypes.h"

class CL_SHA1_Impl
{
/// \name Construction
/// \{

public:
	CL_SHA1_Impl();


/// \}
/// \name Attributes
/// \{

public:
	CL_String8 get_hash(bool uppercase = false);

	void get_hash(unsigned char out_hash[20]);


/// \}
/// \name Operations
/// \{

public:
	void reset();

	void add(const void *data, int size);

	void calculate();


/// \}
/// \name Implementation
/// \{

private:
	void process_chunk();

	void to_hex(char *buffer, cl_uint32 value, bool uppercase);

	unsigned int leftrotate_uint32(unsigned int value, int shift);

	cl_uint32 h0, h1, h2, h3, h4;

	unsigned char chunk[64];

	int chunk_filled;

	cl_uint64 length_message;

	bool calculated;
/// \}
};


