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
**    Kenneth Gangstoe
*/

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#include "../api_display.h"
#include "../Render/graphic_context.h"
#include "../Render/texture_2d.h"
#include "../Image/pixel_buffer.h"
#include <vector>

namespace clan
{

class ImageDescription_Impl;
class XMLResourceDocument;

/// \brief This class describes a single frame in a image description.
class ImageDescriptionFrame
{
public:

public:

	/// \brief Constructs a ImageDescriptionFrame
	///
	/// \param texture = Texture
	/// \param rect = Rect
	ImageDescriptionFrame() { }
	ImageDescriptionFrame(Texture2D texture, Rect rect) : texture(texture), rect(rect) { }

	Texture2D texture;
	Rect rect;
};

/// \brief This class contains everything to construct a image - its data, default settings etc.
///
///    <p>ImageDescription is used by Image to construct itself based
///    on the description. The description class give a big flexibility
///    creating a image, individual frames can come from different image
///    sources, or be cut out from an image source using various different
///    techniques.</p>
class CL_API_DISPLAY ImageDescription
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a image description.
	///
	/// \param resource_id Resource name of a image description resource.
	/// \param resources Resource manager used to load resource.
	ImageDescription();

	/// \brief Constructs a ImageDescription
	///
	/// \param gc = Graphic Context
	/// \param resource_id = String Ref
	/// \param resources = Resource Manager
	ImageDescription(GraphicContext &gc, const std::string &resource_id, XMLResourceDocument *resources, const ImageImportDescription &import_desc = ImageImportDescription ());

	/// \brief Constructs a ImageDescription
	///
	/// \param copy = Image Description
	ImageDescription(const ImageDescription &copy);

	~ImageDescription();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the available frame
	const ImageDescriptionFrame &get_frame() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Copy assignment operator.
	ImageDescription &operator =(const ImageDescription &copy);

	/// \brief Add frame
	///
	/// \param texture = Texture
	void add_frame(const Texture2D &texture);

	/// \brief Add frame
	///
	/// \param fullname = String Ref
	void add_frame(GraphicContext &gc, const std::string &fullname, const ImageImportDescription &import_desc = ImageImportDescription ());

	/// \brief Add frame
	///
	/// \param file = IODevice
	/// \param image_type = String
	void add_frame(GraphicContext &gc, IODevice &file, const std::string &image_type, const ImageImportDescription &import_desc = ImageImportDescription ());

	/// \brief Add frame
	///
	/// \param filename = String Ref
	/// \param dir = Virtual Directory
	void add_frame(GraphicContext &gc, const std::string &filename, const FileSystem &fs, const ImageImportDescription &import_desc = ImageImportDescription ());

	/// \brief Add frame
	///
	/// \param texture = Texture
	/// \param frame = Rect
	void add_frame(const Texture2D &texture, const Rect &frame);

/// \}
/// \name Implementation
/// \{
private:
	/// \brief ImageDescription implementation.
	std::shared_ptr<ImageDescription_Impl> impl;
/// \}
};

}

/// \}
