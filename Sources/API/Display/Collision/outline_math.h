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
**    Emanuel Greisen
**    (if your name is missing here, please add it)
*/

//! clanDisplay="Collision"
//! header=display.h

#ifndef header_outline_math
#define header_outline_math

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>

class CL_Pointf;
class CL_OutlineCircle;

//: Math operations on outlines
//- !group=Core/Collision !
//- !header=display.h!
//- <p>This class provides general methods for calulating things on outlines.</p>
class CL_OutlineMath
{
//! Operations:
 public:
	//: Find minimum spanning circle for the set of points
	//param std::vector<CL_Pointf> &points: the points to calulate on
	static CL_OutlineCircle minimum_enclosing_disc(
		const std::vector<CL_Pointf> &points);
	static void minimum_enclosing_disc(
		CL_OutlineCircle &smalldisc,
		const std::vector<CL_Pointf> &points);

 private:
	static void minimum_disc_with_1point(
		CL_OutlineCircle &smalldisc,
		const std::vector<CL_Pointf> &points,
		unsigned int i);
	static void minimum_disc_with_2points(
		CL_OutlineCircle &smalldisc,
		const std::vector<CL_Pointf> &points ,
		unsigned int i,
		unsigned int j);
	static void minimum_disc_with_3points(
		CL_OutlineCircle &smalldisc,
		const std::vector<CL_Pointf> &points ,
		unsigned int i,
		unsigned int j,
		unsigned int k);

	public:
		static void minimum_enclosing_sub_circle(
			CL_OutlineCircle &smalldisc,
			const std::vector<CL_Pointf> &points,
			float maxradius);
	private:
		static void minimum_enclosing_sub_circle_with_1point(
			CL_OutlineCircle &smalldisc,
			const std::vector<CL_Pointf> &points);
		static void minimum_enclosing_sub_circle_with_2points(
			CL_OutlineCircle &smalldisc,
			const std::vector<CL_Pointf> &points,
			unsigned int j);

};

#endif
