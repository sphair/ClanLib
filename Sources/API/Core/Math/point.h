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
**    Kenneth Gangstoe
**    Harry Storbacka
**    Mark Page
*/

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"
#include "vec2.h"
#include "angle.h"
#include "origin.h"
#include "size.h"

class CL_Pointf;
class CL_Pointd;

/// \brief 2D (x,y) point structure.
///
/// These point templates are defined for: int (CL_Point), float (CL_Pointf), double (CL_Pointd)
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
template<typename Type>
class CL_Pointx : public CL_Vec2<Type>
{
public:
	CL_Pointx() : CL_Vec2<Type>() {}
	CL_Pointx(Type x, Type y) : CL_Vec2<Type>(x, y) {}
	CL_Pointx(const CL_Pointx<Type> &p) : CL_Vec2<Type>(p.x, p.y) {}
	CL_Pointx(const CL_Vec2<Type> &p) : CL_Vec2<Type>(p.x, p.y) {}
};

/// \brief 2D (x,y) point structure - Integer
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Point : public CL_Pointx<int>
{
public:
	CL_Point() : CL_Pointx<int>() {}
	CL_Point(int x, int y) : CL_Pointx<int>(x, y) {}
	CL_Point(const CL_Pointx<int> &p) : CL_Pointx<int>(p.x, p.y) {}
	CL_Point(const CL_Vec2<int> &p) : CL_Pointx<int>(p.x, p.y) {}
};

/// \brief 2D (x,y) point structure - Float
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Pointf : public CL_Pointx<float>
{
public:
	CL_Pointf() : CL_Pointx<float>() {}
	CL_Pointf(float x, float y) : CL_Pointx<float>(x, y) {}
	CL_Pointf(const CL_Pointx<float> &p) : CL_Pointx<float>(p.x, p.y) {}
	CL_Pointf(const CL_Vec2<float> &p) : CL_Pointx<float>(p.x, p.y) {}
};

/// \brief 2D (x,y) point structure - Double
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_Pointd : public CL_Pointx<double>
{
public:
	CL_Pointd() : CL_Pointx<double>() {}
	CL_Pointd(double x, double y) : CL_Pointx<double>(x, y) {}
	CL_Pointd(const CL_Pointx<double> &p) : CL_Pointx<double>(p.x, p.y) {}
	CL_Pointd(const CL_Vec2<double> &p) : CL_Pointx<double>(p.x, p.y) {}
};

/// \}

