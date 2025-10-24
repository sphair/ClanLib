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

/// \addtogroup clanDisplay_Image_Providers clanDisplay Image Providers
/// \{

#pragma once

#include "../api_display.h"
#include "../Image/pixel_buffer.h"
#include "../../Core/Text/string_types.h"
#include <vector>

class CL_IODevice;
class CL_JPEGDecompressor_Impl;

/// \brief JPEG Decompressor.
///
/// \xmlonly !group=Display/Image Providers! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_JPEGDecompressor
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a JPEGDecompressor
	///
	/// \param input_source = IODevice
	CL_JPEGDecompressor(CL_IODevice input_source);

/// \}
/// \name Attributes
/// \{

public:
	enum SpecialMarker
	{
		marker_rst0 = 0xD0,
		marker_eoi  = 0xD9,
		marker_app0 = 0xe0,
		marker_app1 = 0xe1, // EXIF data
		marker_app2 = 0xe2, // FlashPix Ready data (is that a fancy word for thumbnail?)
		marker_app3 = 0xe3,
		marker_app4 = 0xe4,
		marker_app5 = 0xe5,
		marker_app6 = 0xe6,
		marker_app7 = 0xe7,
		marker_app8 = 0xe8,
		marker_app9 = 0xe9,
		marker_app10 = 0xea,
		marker_app11 = 0xeb,
		marker_app12 = 0xec,
		marker_app13 = 0xed,
		marker_app14 = 0xee,
		marker_com  = 0xfe
	};

	struct SavedMarker
	{
		unsigned char marker;
		unsigned int original_length;
		unsigned int data_length;
		void *data;
	};

	/// \brief Get Output width
	///
	/// \return output_width
	int get_output_width() const;

	/// \brief Get Output height
	///
	/// \return output_height
	int get_output_height() const;

	/// \brief Get Output components
	///
	/// \return output_components
	int get_output_components() const;

	std::vector<SavedMarker> get_saved_markers() const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Record marker
	///
	/// \param marker_code = Special Marker
	/// \param length_limit = value
	void record_marker(SpecialMarker marker_code, unsigned int length_limit = 0xffff);

	/// \brief Start
	///
	/// \param raw_data = bool
	void start(bool raw_data = false);

	unsigned int read_scanlines(unsigned char **data, unsigned int lines);

	unsigned int read_raw_data(const unsigned char ***data, unsigned int lines);

	/// \brief Finish
	void finish();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_JPEGDecompressor_Impl> impl;
/// \}
};

/// \}
