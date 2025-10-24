/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "API/Core/System/mutex.h"
#include "API/Core/System/event.h"
#include "API/Core/System/interlocked_variable.h"

class FragmentBuffer
{
public:
	FragmentBuffer();

	void set_num_readers(int num_readers);

	bool readers_finished();

	int get_writer_fragment();
	void finish_writer_fragment(int num_vertices);

	int get_reader_fragment(int reader);
	int get_fragment_vertices(int fragment);
	void finish_reader_fragment(int reader);

	CL_Event &get_writer_event() { return writer_event; }
	CL_Event &get_reader_event(int reader) { return reader_events[reader]; }

	enum { num_fragments = 4, max_readers = 32 };

private:
	int get_free_fragments();

	CL_InterlockedVariable writer_position;
	CL_InterlockedVariable reader_positions[max_readers];
	CL_InterlockedVariable fragment_vertices[num_fragments];
	int num_readers;
	CL_Event reader_events[max_readers];
	CL_Event writer_event;
};
