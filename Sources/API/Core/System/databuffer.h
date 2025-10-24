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

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once


#include "../api_core.h"
#include "sharedptr.h"

class CL_MemoryPool;
class CL_DataBuffer_Impl;

/// \brief General purpose data buffer.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_DataBuffer
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a data buffer of 0 size.
	CL_DataBuffer();

	CL_DataBuffer(int size, CL_MemoryPool *pool = 0);

	CL_DataBuffer(const void *data, int size, CL_MemoryPool *pool = 0);

	CL_DataBuffer(const CL_DataBuffer &data, int pos, int size = -1, CL_MemoryPool *pool = 0);

	~CL_DataBuffer();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns a pointer to the data.
	char *get_data();

	const char *get_data() const;

	/// \brief Returns the size of the data.
	int get_size() const;

	/// \brief Returns the capacity of the data buffer object.
	int get_capacity() const;

	/// \brief Returns a char in the buffer.
	char &operator[](int i);

	const char &operator[](int i) const;

	char &operator[](unsigned int i);

	const char &operator[](unsigned int i) const;

	/// \brief Returns true if the buffer is 0 in size.
	bool is_null() const;


/// \}
/// \name Operations
/// \{

public:
	CL_DataBuffer &operator =(const CL_DataBuffer &copy);

	/// \brief Resize the buffer.
	void set_size(int size);

	/// \brief Preallocate enough memory.
	void set_capacity(int capacity);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_DataBuffer_Impl> impl;
/// \}
};


/// \}
