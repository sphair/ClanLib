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

//! clanDisplay="Display 2D"
//! header=display.h

#ifndef header_blend_func
#define header_blend_func

#if _MSC_VER > 1000
#pragma once
#endif

//: Blending functions.
//- !group=Display/Display 2D!
//- !header=display.h!
enum CL_BlendFunc
{
	blend_zero,                     // source or destination (0, 0, 0, 0)
	blend_one,                      // source or destination (1, 1, 1, 1)
	blend_dest_color,               // source                (Rd, Gd, Bd, Ad)
	blend_src_color,                // destination           (Rs, Gs, Bs, As)
	blend_one_minus_dest_color,     // source                (1, 1, 1, 1) - (Rd, Gd, Bd, Ad)
	blend_one_minus_src_color,      // destination           (1, 1, 1, 1) - (Rs, Gs, Bs, As)
	blend_src_alpha,                // source or destination (As, As, As, As)
	blend_one_minus_src_alpha,      // source or destination (1, 1, 1, 1) - (As, As, As, As)
	blend_dst_alpha,                // source or destination (Ad, Ad, Ad, Ad)
	blend_one_minus_dest_alpha,     // source or destination (1, 1, 1, 1) - (Ad, Ad, Ad, Ad)
	blend_src_alpha_saturate,       // source                (f, f, f, 1) - f = min(As, 1 - Ad)
	blend_constant_color,           // source or destination (Rc, Gc, Bc, Ac)
	blend_one_minus_constant_color, // source or destination (1, 1, 1, 1) - (Rc, Gc, Bc, Ac)
	blend_constant_alpha,           // source or destination (Ac, Ac, Ac, Ac)
	blend_one_minus_constant_alpha  // source or destination (1, 1, 1, 1) - (Ac, Ac, Ac, Ac)
};

#endif
