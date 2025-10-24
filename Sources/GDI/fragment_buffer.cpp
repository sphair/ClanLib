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

#include "GDI/precomp.h"
#include "fragment_buffer.h"

FragmentBuffer::FragmentBuffer()
: num_readers(0)
{
	writer_position.set(0);
	for (int i = 0; i < max_readers; i++)
		reader_positions[i].set(0);
}

void FragmentBuffer::set_num_readers(int new_num_readers)
{
	num_readers = new_num_readers;
}

bool FragmentBuffer::readers_finished()
{
	return get_free_fragments() == num_fragments-1;
}

int FragmentBuffer::get_writer_fragment()
{
	if (get_free_fragments() > 0)
		return writer_position.get();
	else
		return -1;
}

void FragmentBuffer::finish_writer_fragment(int num_vertices)
{
	int pos = writer_position.get();
	fragment_vertices[pos].set(num_vertices);

	pos++;
	if (pos == num_fragments)
		pos = 0;
	writer_position.set(pos);
	for (int i = 0; i < num_readers; i++)
		reader_events[i].set();
}

int FragmentBuffer::get_reader_fragment(int i)
{
	int writer = writer_position.get();
	int reader = reader_positions[i].get();
	if (writer != reader)
		return reader;
	else
		return -1;
}

int FragmentBuffer::get_fragment_vertices(int fragment)
{
	return fragment_vertices[fragment].get();
}

void FragmentBuffer::finish_reader_fragment(int i)
{
	int pos = reader_positions[i].get();
	pos++;
	if (pos == num_fragments)
		pos = 0;
	reader_positions[i].set(pos);

	if (get_free_fragments() == num_fragments-1)
		writer_event.set();
}

int FragmentBuffer::get_free_fragments()
{
	int writer = writer_position.get();
	int free = num_fragments-1;
	for (int i = 0; i < num_readers; i++)
	{
		int reader = reader_positions[i].get();
		int delta = 0;
		if (reader <= writer)
			delta = writer-reader;
		else
			delta = writer+num_fragments-reader;
		int available = num_fragments-1 - delta;
		if (available < free)
			free = available;
	}
	return free;
}
