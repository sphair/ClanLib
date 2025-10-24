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

//! clanDisplay="Input"
//! header=display.h

#ifndef header_inputbuffer
#define header_inputbuffer

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include <queue>
#include "input_event.h"
#include "../signals.h"

class CL_Component;
class CL_InputDevice;
class CL_InputContext;
class CL_InputBuffer_Generic;

//: Input buffer interface.
//- !group=Display/Input!
//- !header=display.h!
//- <p> Use this class to read buffered keys from the keyboard or any other device. </p>
class CL_API_DISPLAY CL_InputBuffer
{
//! Construction:
public:
	//: Constructs an input buffer.
	//param ic: Input context that input buffer should connect to get keys.
	//param sig_key_down: Signal input buffer should connect to get key presses.
	//param sig_key_up: Signal input buffer should connect to get key released.
	//param device: Input device that input buffer should connect to get keys.
	//- <p>Detailed description that explains what this damn constructors really do.</p>
	CL_InputBuffer();

	CL_InputBuffer(CL_InputDevice &device);

	CL_InputBuffer(CL_InputContext *ic);

	CL_InputBuffer(const CL_InputBuffer &copy);

	//: Input Buffer destructor.
	virtual ~CL_InputBuffer();

//! Attributes:
public:
	//: Returns number of keys still available in the buffer.
	//return: Number of key values left in buffer.
	int keys_left() const;

	//: Returns the next key in the buffer, but unlike pop_key() it does not
	//: pop it from the input buffer.
	CL_InputEvent peek_key() const;

//! Operations:
public:
	//: Copy assignment operator.
	CL_InputBuffer &operator =(const CL_InputBuffer &copy);

	//: Adds a device that input buffer should connect to get keys.
	void add_device(CL_InputDevice &device);

	//: Adds all devices from a inputcontext that input buffer should connect to get keys.
	void add_devices(CL_InputContext *ic);
	
	//: Pops the next key in the buffer and returns it.
	CL_InputEvent pop_key();

	//: Clears the buffer.
	void clear();
	
//! Implementation:
private:
	CL_InputBuffer_Generic *impl;
};

#endif
