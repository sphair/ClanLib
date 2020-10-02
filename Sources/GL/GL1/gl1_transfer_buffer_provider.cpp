/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Mark Page
*/

#include "GL/precomp.h"
#include "gl1_transfer_buffer_provider.h"

namespace clan
{
	GL1TransferBufferProvider::GL1TransferBufferProvider()
		: data(nullptr), size(0)
	{
	}

	GL1TransferBufferProvider::~GL1TransferBufferProvider()
	{
		delete[] data;
	}

	void GL1TransferBufferProvider::create(int new_size, BufferUsage usage)
	{
		delete[] data;
		data = nullptr;
		size = 0;
		data = new char[new_size];
		size = new_size;
	}

	void GL1TransferBufferProvider::create(void *init_data, int new_size, BufferUsage usage)
	{
		delete[] data;
		data = nullptr;
		size = 0;
		data = new char[new_size];
		size = new_size;
		memcpy(data, init_data, size);
	}

	void GL1TransferBufferProvider::upload_data(GraphicContext &gc, int offset, const void *data, int size)
	{
		if ((size < 0) || (offset < 0) || ((size + offset) > this->size))
			throw Exception("Transfer buffer, invalid size");

		memcpy(this->data + offset, data, size);
	}
}
