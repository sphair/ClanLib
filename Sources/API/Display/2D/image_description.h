/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
class ResourceManager;

/// \brief This class describes a single frame in a image description.
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class ImageDescriptionFrame
{
public:
	enum FrameType
	{
		type_pixelbuffer,
		type_texture
	};

public:

	/// \brief Constructs a ImageDescriptionFrame
	///
	/// \param pixelbuffer = Pixel Buffer
	/// \param rect = Rect
	ImageDescriptionFrame(PixelBuffer pixelbuffer, Rect rect) : pixelbuffer(pixelbuffer), rect(rect), type(type_pixelbuffer), delay(1.0) { }

	/// \brief Constructs a ImageDescriptionFrame
	///
	/// \param texture = Texture
	/// \param rect = Rect
	ImageDescriptionFrame(Texture2D texture, Rect rect) : texture(texture), rect(rect), type(type_texture), delay(1.0) { }

	PixelBuffer pixelbuffer;
	Texture2D texture;
	Rect rect;
	FrameType type;
	double delay;
};

/// \brief This class contains everything to construct a image - its data, default settings etc.
///
///    <p>ImageDescription is used by Image to construct itself based
///    on the description. The description class give a big flexibility
///    creating a image, individual frames can come from different image
///    sources, or be cut out from an image source using various different
///    techniques.</p>
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
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
	ImageDescription(GraphicContext &gc, const std::string &resource_id, ResourceManager *resources, const ImageImportDescription &import_desc = ImageImportDescription ());

	/// \brief Constructs a ImageDescription
	///
	/// \param copy = Image Description
	ImageDescription(const ImageDescription &copy);

	~ImageDescription();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns a list over all available frames.
	const std::vector<ImageDescriptionFrame> &get_frames() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Copy assignment operator.
	ImageDescription &operator =(const ImageDescription &copy);

	/// \brief Adds a single image.
	///
	/// \param pixelbuffer Image source.
	/// \param filename Filename of image.
	/// \param vfs Virtual File System to load image from.
	void add_frame(const PixelBuffer &pixelbuffer);

	/// \brief Add frame
	///
	/// \param texture = Texture
	void add_frame(const Texture2D &texture);

	/// \brief Add frame
	///
	/// \param fullname = String Ref
	void add_frame(const std::string &fullname, const ImageImportDescription &import_desc = ImageImportDescription ());

	/// \brief Add frame
	///
	/// \param file = IODevice
	/// \param image_type = String
	void add_frame(IODevice &file, const std::string &image_type, const ImageImportDescription &import_desc = ImageImportDescription ());

	/// \brief Add frame
	///
	/// \param filename = String Ref
	/// \param dir = Virtual Directory
	void add_frame(const std::string &filename, VirtualDirectory &dir, const ImageImportDescription &import_desc = ImageImportDescription ());

	/// \brief Add frames
	///
	/// \param texture = Texture
	/// \param frames = Rect
	/// \param num_frames = value
	void add_frames(const Texture2D &texture, Rect *frames, int num_frames);

	/// \brief Adds images formed in a grid.
	/** <p>This function will cut out a grid of frames from one image.</p>
	    \param pixelbuffer Image source.
	    \param texture Image source.
	    \param xpos, ypos Position of where image grid starts.
	    \param width, height Size of a frame in the grid.
	    \param xarray, yarray Number of columns and rows in grid.
	    \param array_skipframes Number of frames to skip at last gridline.
	    \param xspacing, yspacing Pixel interspacing between grid frames.*/
	void add_gridclipped_frames(
		const PixelBuffer &pixelbuffer,
		int xpos, int ypos,
		int width, int height,
		int xarray = 1, int yarray = 1,
		int array_skipframes = 0,
		int xspacing = 0, int yspacing = 0);

	void add_gridclipped_frames(GraphicContext &gc, 
		const Texture2D &texture,
		int xpos, int ypos,
		int width, int height,
		int xarray = 1, int yarray = 1,
		int array_skipframes = 0,
		int xspacing = 0, int yspacing = 0);

	/// \brief Adds images separated with pure alpha (within trans_limit).
	/** <p>The alpha clipper will cut out frames from an image based on
	    the transparency in the picture. It first determines the height
	    of a row by searching for the first line that it considers
	    completely transparent. Then it finds the width of each frame on
	    this line by looking for columns that are completely transparency.</p>
	    \param pixelbuffer Image source.
	    \param texture Image source.
	    \param xpos, ypos Upper left position where alpha cutting should begin.
	    \param trans_limit Amount of non-transparent alpha allowed before a pixel is not considered transparent.*/
	void add_alphaclipped_frames(
		const PixelBuffer &pixelbuffer,
		int xpos = 0, int ypos = 0,
		float trans_limit = 0.05f);

	void add_alphaclipped_frames(GraphicContext &gc, 
		const Texture2D &texture,
		int xpos = 0, int ypos = 0,
		float trans_limit = 0.05f);

	/// \brief Adds images separated with pure alpha (within trans_limit).
	/** <p>The alpha clipper will cut out frames from an image based on
	    the transparency in the picture. It scans the lines horizontally
	    from top to bottom. As soon as a non-transarent pixel is discovered,
	    the clipper finds the bounding box for that region and then moves on.</p>
	    \param pixelbuffer Image source.
	    \param texture Image source.
	    \param xpos, ypos Upper left position where alpha cutting should begin.
	    \param trans_limit Amount of non-transparent alpha allowed before a pixel is not considered transparent.*/
	void add_alphaclipped_frames_free(
		const PixelBuffer &pixelbuffer,
		int xpos = 0, int ypos = 0,
		float trans_limit = 0.05f);

	void add_alphaclipped_frames_free(GraphicContext &gc, 
		const Texture2D &texture,
		int xpos = 0, int ypos = 0,
		float trans_limit = 0.05f);

	/// \brief Sets the duration this frame is displayed, in seconds.
	void set_frame_delay(int frame, double delay);

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
