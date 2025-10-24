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

#ifndef header_input_buffer_generic
#define header_input_buffer_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <queue>
#include "API/Display/input_event.h"
#include "API/signals.h"

class CL_InputBuffer_Generic
{
//! Construction:
public:
	CL_InputBuffer_Generic() : ref_count(0) { return; }

//! Attributes:
public:
	std::queue<CL_InputEvent> buffer;

	CL_SlotContainer slots;

//! Operations:
public:
	void add_ref() { ref_count++; }

	void release_ref() { ref_count--; if (ref_count == 0) delete this; }

	void on_key_down(const CL_InputEvent &key) { buffer.push(key); }

	void on_key_up(const CL_InputEvent &key) { buffer.push(key); }

//! Implementation:
private:
	int ref_count;
};

#endif
