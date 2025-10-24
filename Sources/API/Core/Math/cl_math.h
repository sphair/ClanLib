/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"
#include "../System/cl_platform.h"
#include <cmath>

#ifndef CL_PI
#define CL_PI CL_M_PI_F
#endif

template<typename T> inline T cl_pow2(T value) { return value*value; }
template<typename A, typename B> inline A cl_min(A a, B b) { return a < b ? a : b; }
template<typename A, typename B> inline A cl_max(A a, B b) { return a > b ? a : b; }
template<typename A, typename B, typename C> inline C cl_clamp(A val, B minval, C maxval) { return cl_max(minval, cl_min(maxval, val)); }

/// \}

