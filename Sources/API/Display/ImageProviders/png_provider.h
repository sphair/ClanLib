/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#pragma once

#include "../Image/pixel_buffer.h"
#include "../../Core/IOData/file_system.h"

namespace clan
{
	/// \addtogroup clanDisplay_Image_Providers clanDisplay Image Providers
	/// \{

	class FileSystem;

	/// \brief Surface provider that can load PNG (.png) files.
	class PNGProvider
	{
	public:
		/// \brief Called to load an image with this provider type.
		///
		/// \param name Name of the file to load.
		/// \param directory Directory that file name is relative to.
		static PixelBuffer load(
			const std::string &filename,
			const FileSystem &fs,
			bool srgb = false);

		static PixelBuffer load(
			const std::string &fullname,
			bool srgb = false);

		/// \brief Load
		///
		/// \param dev = IODevice
		///
		/// \return Pixel Buffer
		static PixelBuffer load(IODevice &dev, bool srgb = false);

		/// \brief Called to save a given PixelBuffer to a file
		static void save(
			PixelBuffer buffer,
			const std::string &filename,
			FileSystem &fs);

		static void save(
			PixelBuffer buffer,
			const std::string &fullname);

		/// \brief Save the given PixelBuffer to an output device.
		static void save(PixelBuffer buffer, IODevice &iodev);
	};

	/// \}
}
