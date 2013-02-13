/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
namespace clan
{

class InstancesBuffer
{
public:
	InstancesBuffer();

	int new_offset_index();

	void clear();
	void add(int vectors_count);

	void lock(clan::GraphicContext &gc);
	clan::Vec4f *upload(int offset_index, int vectors);
	void unlock(clan::GraphicContext &gc);

	clan::Texture2D &get_indexes() { return indexes[current_buffer]; }
	clan::Texture2D &get_vectors() { return vectors[current_buffer]; }

private:
	enum { num_buffers = 4 };
	clan::PixelBuffer indexes_transfer[num_buffers];
	clan::PixelBuffer vectors_transfer[num_buffers];

	int max_offset_indexes;
	int next_offset_index;

	int max_vectors;
	int num_vectors;

	int current_buffer;
	clan::Texture2D indexes[num_buffers];
	clan::Texture2D vectors[num_buffers];
};

}

