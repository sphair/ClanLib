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
*/


#pragma once

#include "../CollisionAPI/outline_accuracy.h"
#include <vector>

class Contour;
class OutlineProviderBitmap_Impl;

/// \brief Bitmap outline provider.
///
///  <p>A OutlineProviderBitmap is used to find outlines based on the alpha channel of images.</p> 
class OutlineProviderBitmap
{
/// \name Construction
/// \{

 public:
	/// \brief Construct a contour
	///
	/// \param pbuf  PixelBuffer containing image data.
	/// \param alpha_limit  Minimum alpha value for a colliding pixel
	/// \param get_insides Get Insides
	OutlineProviderBitmap(const clan::PixelBuffer &pbuf, int alpha_limit=128, bool get_insides=true);
	~OutlineProviderBitmap();

/// \}
/// \name Attributes
/// \{

 public:
	/// \brief return the countours that make up the outline
	std::vector<Contour> get_contours();

	/// \brief return the size of the image used as basis for outline creation.
	clan::Size get_size();
/// \}
/// \name Operations
/// \{

public:

/// \}
/// \name Implementation
/// \{

 private:
 	std::shared_ptr<OutlineProviderBitmap_Impl> impl;
/// \}
};

