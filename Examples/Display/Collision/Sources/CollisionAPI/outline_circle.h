/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Harry Storbacka
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/


#pragma once

/// \brief Collision detection outline segmentation circle.
///
/// <p>Outline circles split the line segments into groups for faster collision detection.
///    If two circles encapsulating the lines don't collide, none of the lines inside them can
///    collide either. </p> 
class OutlineCircle
{
/// \name Construction
/// \{

 public:
	/// \brief Construct a outline circle
	OutlineCircle() :
		position(-1.0,-1.0),
		radius(0),
		start(0),
		end(0)
	{
		return;
	}

/// \}
/// \name Operations
/// \{

 public:
	/// \brief Construct a outline circle
	///
	/// \param other  Other circle to test.
	bool collide(const OutlineCircle &other) const
	{
		if( &other == this )
			return false;

		clan::Pointf pos = position - other.position;
		float dist = float(sqrt( pos.x*pos.x+pos.y*pos.y ));

		if( dist > (radius + other.radius) )
			return false;

		return true;
	}

/// \}
/// \name Attributes
/// \{

 public:
	clan::Pointf position;
	float radius;
	unsigned int start;
	unsigned int end;
/// \}
};
