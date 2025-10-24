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
**    Harry Storbacka
**    Magnus Norddahl
*/

/// \addtogroup clanDisplay_Collision clanDisplay Collision
/// \{

#pragma once

#include "../api_display.h"
#include "outline_provider.h"
#include "outline_accuracy.h"
#include <vector>

class CL_OutlineProviderBitmap_Generic;

/// \brief Bitmap outline provider.
///
///  <p>A CL_OutlineProviderBitmap is used to find outlines based on the alpha channel of images.</p> 
/// \xmlonly !group=Display/Collision! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_OutlineProviderBitmap : public CL_OutlineProvider
{
/// \name Construction
/// \{

 public:
	/// \brief Construct a contour
	///
	/// \param pbuf  PixelBuffer containing image data.
	/// \param alpha_limit  Minimum alpha value for a colliding pixel
	/// \param get_insides Get Insides
	CL_OutlineProviderBitmap(const CL_PixelBuffer &pbuf, int alpha_limit=128, bool get_insides=true);
	~CL_OutlineProviderBitmap();

/// \}
/// \name Attributes
/// \{

 public:
	/// \brief return the countours that make up the outline
	virtual std::vector<CL_Contour> get_contours();

	/// \brief return the width of the image used as basis for outline creation.
	virtual int get_width();

	/// \brief return the height of the image used as basis for outline creation.
	virtual int get_height();
/// \}
/// \name Operations
/// \{

public:
	/// \brief Destroys the provider.
	virtual void destroy();

/// \}
/// \name Implementation
/// \{

 private:
 	CL_SharedPtr<CL_OutlineProviderBitmap_Generic> impl;
/// \}
};

/// \}
