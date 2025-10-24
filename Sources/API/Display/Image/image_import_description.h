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
**    Mark Page
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Signals/callback_1.h"

class CL_Size;
class CL_Rect;
class CL_ImageImportDescription_Impl;
class CL_PixelBuffer;

/// \brief Image Import Description Class.
///
/// This class allows you to setup a more advanced description when importing images.
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_ImageImportDescription
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a image import description with default values.
	CL_ImageImportDescription();

	~CL_ImageImportDescription();

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns the premultiply alpha setting
	bool get_premultiply_alpha() const;

	/// \brief Returns the flip vertical setting
	bool flip_vertical() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Process the pixel buffers depending of the chosen settings
	///
	/// Note, the output may point to a different pixel buffer than the input\n
	/// The input image may be written to.
	CL_PixelBuffer process(CL_PixelBuffer &image) const;

	/// \brief Set the premultiply alpha setting
	///
	/// (This defaults to off)
	void set_premultiply_alpha(bool enable);

	/// \brief Set the flip vertical setting
	///
	/// (This defaults to off)
	void set_flip_vertical(bool enable);

/// \}
/// \name Callbacks
/// \{
public:
	/// \brief User defined fine control of the pixel buffer
	///
	/// Note, the output maybe different to the input, if desired\n
	/// \n
	/// CL_PixelBuffer func_process(CL_PixelBuffer &input)
	CL_Callback_1<CL_PixelBuffer, CL_PixelBuffer &> &func_process();

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_ImageImportDescription_Impl> impl;
/// \}
};

/// \}
