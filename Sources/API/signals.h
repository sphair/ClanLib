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

//: <p>ClanLib signal system. Signals is an other word for callback functions. A
//: C++ signal library provides a typecast clean interface to allow for
//: callbacks to be called, without doing nasty pointer casts all the time. You
//: can use this library totally independent of the rest of clanlib, if you
//: want (as soon as you've learned about signals, you will love them - trust me).</p>
//! Global=Signal

#ifndef header_signals
#define header_signals

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include "Signals/slot.h"
#include "Signals/signal_v0.h"
#include "Signals/signal_v1.h"
#include "Signals/signal_v2.h"
#include "Signals/signal_v3.h"
#include "Signals/signal_v4.h"
#include "Signals/signal_v5.h"
#include "Signals/slotbuffer_v0.h"
#include "Signals/slotbuffer_v1.h"
#include "Signals/slot_container.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanSignals-static-mt.lib")
#else
#pragma comment(lib, "clanSignals-static-mt-debug.lib")
#endif
#endif

#endif
