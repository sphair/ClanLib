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
**    Emanuel Griesen
**    (if your name is missing here, please add it)
*/

//! clanCore="Math"
//! header=core.h

#ifndef header_pointset_math
#define header_pointset_math

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#include <vector>
#include "point.h"
#include "circle.h"

//: Math operations related to point sets.
//- !group=Core/Math!
//- !header=core.h!
class CL_API_CORE CL_PointSetMath
{
	public:
//! Operations:
 public:
	//: Find minimum spanning circle for the set of points
	//param std::vector<CL_Pointf> &points: the points to calulate on
	static CL_Circlef minimum_enclosing_disc(
		const std::vector<CL_Pointf> &points);

	//: Return the convex hull of the given set of points
	//param std::vector<CL_Pointf> P: the set of points
	static std::vector<CL_Pointf> convex_hull_from_polygon(std::vector<CL_Pointf> &points);

 private:

	static void calculate_minimum_enclosing_disc(
		CL_Circlef &smalldisc,
		const std::vector<CL_Pointf> &points,
		int start,
		int end);

	static void minimum_disc_with_1point(
		CL_Circlef &smalldisc,
		const std::vector<CL_Pointf> &points,
		int start,
		unsigned int i);

	static void minimum_disc_with_2points(
		CL_Circlef &smalldisc,
		const std::vector<CL_Pointf> &points,
		int start,
		unsigned int i,
		unsigned int j);

	static void minimum_disc_with_3points(
		CL_Circlef &smalldisc,
		const std::vector<CL_Pointf> &points,
		unsigned int i,
		unsigned int j,
		unsigned int k);

	friend class CL_OutlineMath;
};

#endif
