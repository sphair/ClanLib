/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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


#pragma once

#include "../api_display.h"
#include <memory>
#include "../../Core/Signals/callback.h"

namespace clan
{
/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

class Size;
class Rect;
class ImageImportDescription_Impl;
class PixelBuffer;

/// \brief Image Import Description Class.
///
/// This class allows you to setup a more advanced description when importing images.
class CL_API_DISPLAY ImageImportDescription
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a image import description with default values.
	ImageImportDescription();

	~ImageImportDescription();

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns the premultiply alpha setting
	bool get_premultiply_alpha() const;

	/// \brief Returns the flip vertical setting
	bool flip_vertical() const;

	/// \brief Returns the sRGB setting
	bool is_srgb() const;

	/// \brief Returns if this image should be cached
	bool is_cached() const;
/// \}
/// \name Operations
/// \{
public:
	/// \brief Process the pixel buffers depending of the chosen settings
	///
	/// Note, the output may point to a different pixel buffer than the input\n
	/// The input image may be written to.
	PixelBuffer process(PixelBuffer &image) const;

	/// \brief Set the premultiply alpha setting
	///
	/// (This defaults to off)
	void set_premultiply_alpha(bool enable);

	/// \brief Set the flip vertical setting
	///
	/// (This defaults to off)
	void set_flip_vertical(bool enable);

	/// \brief Controls if the image is uploaded as a sRGB texture or not
	///
	/// (This defaults to off)
	void set_srgb(bool enable);

	/// \brief Controls if this image can be cached
	///
	/// (This defaults to true)
	void set_cached(bool enable);

/// \}
/// \name Callbacks
/// \{
public:
	/// \brief User defined fine control of the pixel buffer
	///
	/// Note, the output maybe different to the input, if desired\n
	/// \n
	/// PixelBuffer func_process(PixelBuffer &input)
	Callback<PixelBuffer(PixelBuffer &)> &func_process();

/// \}
/// \name Implementation
/// \{
private:
	std::shared_ptr<ImageImportDescription_Impl> impl;
/// \}
};

}

/// \}
