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
**    Harry Storbacka
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "pixel_format.h"
#include "../../Core/Math/rect.h"

class CL_Size;
class CL_Rect;
class CL_PixelFormat;
class CL_Palette;
class CL_Color;
class CL_Colorf;
class CL_PixelBuffer;
class CL_PixelBuffer_Impl;
class CL_ResourceManager;
class CL_VirtualDirectory;
class CL_IODevice;

/// \brief Pixel data reference class.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_PixelBufferRef
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a pixel buffer.
	///
	/// \param width Width of pixel buffer, in pixels.
	/// \param height Height of pixel buffer, in pixels.
	/// \param pitch Bytes per line in pixel buffer.
	/// \param format Pixel format of pixel buffer.
	/// \param palette Palette used in pixel buffer.
	/// \param data Data pointer to pixel data.
	CL_PixelBufferRef();

	/// \brief Constructs a PixelBufferRef
	///
	/// \param buffer = Pixel Buffer
	CL_PixelBufferRef(const CL_PixelBuffer &buffer);

	/// \brief Constructs a PixelBufferRef
	///
	/// \param width = value
	/// \param height = value
	/// \param pitch = value
	/// \param format = Pixel Format
	/// \param data = void
	CL_PixelBufferRef(int width, int height, int pitch, const CL_PixelFormat &format, const void *data);

	/// \brief Constructs a PixelBufferRef
	///
	/// \param width = value
	/// \param height = value
	/// \param pitch = value
	/// \param format = Pixel Format
	/// \param palette = Palette
	/// \param data = void
	CL_PixelBufferRef(int width, int height, int pitch, const CL_PixelFormat &format, const CL_Palette *palette, const void *data);

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the pixel format of the pixel buffer.
	const CL_PixelFormat &get_format() const { return format; }

	/// \brief Returns the palette of the pixel buffer.
	const CL_Palette *get_palette() const { return palette; }

	/// \brief Returns the buffer width.
	int get_width() const { return width; }

	/// \brief Returns the buffer height.
	int get_height() const { return height; }

	/// \brief Returns the width and height of the buffer.
	CL_Size get_size() const { return CL_Size(width, height); }

	/// \brief Returns the pitch (bytes per scanline).
	unsigned int get_pitch() const { return pitch; }

	/// \brief Returns a pointer to the beginning of the pixel buffer.
	void *get_data() { return data; }

	const void *get_data() const { return data; }

/// \}
/// \name Operations
/// \{

public:

	/// \brief Get subimage
	///
	/// \param rect = Rect
	///
	/// \return Pixel Buffer Ref
	CL_PixelBufferRef get_subimage(const CL_Rect &rect) const;

	/// \brief Create a copy of the pixelbuffer that doesn't share data with the original pixel buffer.
	CL_PixelBuffer copy() const;

	/// \brief Convert pixel buffer to the pixel format of the target buffer, storing
	/// \brief the result in the target buffer.
	/// \param target Target pixel buffer.
	/// \param buffer Buffer getting stored pixels in the specified format and pitch.
	/// \param format Pixel format of target buffer.
	/// \param dest_pitch Bytes per line of target buffer.
	/// \param dest_rect Destination rectangle for conversion.
	/// \param src_rect Source rectangle for conversion.
	void convert(CL_PixelBuffer target) const;

	void convert(void *buffer, const CL_PixelFormat &format, int dest_pitch,
		const CL_Rect &dest_rect, const CL_Rect &src_rect = CL_Rect(0,0,0,0)) const;

	/// \brief Convert one line of pixel data to target buffer.
	///
	/// \param buffer Buffer getting stored pixels in the specified format
	/// \param format Pixel format of target buffer.
	/// \param y Specifies which line to convert when converting one single line.
	void convert_line(void *buffer, const CL_PixelFormat &format, int y) const;

	/// \brief Converts current buffer to a new pixel format and returns the result.
	CL_PixelBuffer to_format(const CL_PixelFormat &format) const;

private:
	CL_PixelFormat format;
	CL_Palette *palette;
	int width;
	int height;
	unsigned int pitch;
	void *data;
/// \}
};

/// \brief Pixel data container.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_PixelBuffer
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a pixel buffer.
	///
	/// \param width Width of pixel buffer, in pixels.
	/// \param height Height of pixel buffer, in pixels.
	/// \param pitch Bytes per line in pixel buffer.
	/// \param format Pixel format of pixel buffer.
	/// \param palette Palette used in pixel buffer.
	/// \param data Data pointer to pixel data. If null, will construct a memory pixel buffer with the given dimensions.
	CL_PixelBuffer();

	/// \brief Constructs a PixelBuffer
	///
	/// \param other = Pixel Buffer Ref
	CL_PixelBuffer(const CL_PixelBufferRef &other);

	/// \brief Constructs a PixelBuffer
	///
	/// \param width = value
	/// \param height = value
	/// \param pitch = value
	/// \param format = Pixel Format
	/// \param data = void
	CL_PixelBuffer(int width, int height, int pitch, const CL_PixelFormat &format, const void *data = 0);

	/// \brief Constructs a PixelBuffer
	///
	/// \param width = value
	/// \param height = value
	/// \param pitch = value
	/// \param format = Pixel Format
	/// \param palette = Palette
	/// \param data = void
	CL_PixelBuffer(int width, int height, int pitch, const CL_PixelFormat &format, const CL_Palette &palette, const void *data = 0);

	/// \brief Constructs a PixelBuffer
	///
	/// \param copy = Pixel Buffer
	CL_PixelBuffer(const CL_PixelBuffer &copy);

	/// \brief Constructs a PixelBuffer
	///
	/// \param fullname = String Ref
	CL_PixelBuffer(const CL_StringRef &fullname);

	/// \brief Constructs a PixelBuffer
	///
	/// \param filename = String Ref
	/// \param dir = Virtual Directory
	CL_PixelBuffer(const CL_StringRef &filename, const CL_VirtualDirectory &dir);

	/// \brief Constructs a PixelBuffer
	///
	/// \param file = IODevice
	/// \param image_type = String
	CL_PixelBuffer(CL_IODevice &file, const CL_String &image_type);

	virtual ~CL_PixelBuffer();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if the PixelBuffer doesn't contain any data.
	bool is_null() const;

	/// \brief Create a copy of the pixelbuffer that doesn't share data with the original pixel buffer.
	CL_PixelBuffer copy() const;

	/// \brief Returns the pixel format of the pixel buffer.
	const CL_PixelFormat &get_format() const;

	/// \brief Returns the palette of the pixel buffer.
	const CL_Palette *get_palette() const;

	/// \brief Returns the buffer width.
	int get_width() const;

	/// \brief Returns the buffer height.
	int get_height() const;

	/// \brief Returns the width and height of the buffer.
	CL_Size get_size() const;

	/// \brief Returns the pitch (bytes per scanline).
	unsigned int get_pitch() const;

	/// \brief Returns a pointer to the beginning of the pixel buffer.
	void *get_data();

	const void *get_data() const;

	/// \brief Returns the pixel at coordinates x and y.
	CL_Color get_pixel(int x, int y);

	/// \brief Returns a new pixel buffer, copying the area specified by rect.
	CL_PixelBufferRef get_subimage(const CL_Rect &rect) const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Copy assignment operator.
	CL_PixelBuffer &operator =(const CL_PixelBuffer &copy);

	/// \brief Return true if the CL_PixelBuffer is valid and usable
	operator bool () const;

	/// \brief Convert pixel buffer to the pixel format of the target buffer, storing
	/// \brief the result in the target buffer.
	/// \param target Target pixel buffer.
	/// \param buffer Buffer getting stored pixels in the specified format and pitch.
	/// \param format Pixel format of target buffer.
	/// \param dest_pitch Bytes per line of target buffer.
	/// \param dest_rect Destination rectangle for conversion.
	/// \param src_rect Source rectangle for conversion.
	void convert(CL_PixelBuffer target) const;

	void convert(void *buffer, const CL_PixelFormat &format, int dest_pitch,
		const CL_Rect &dest_rect, const CL_Rect &src_rect = CL_Rect(0,0,0,0)) const;

	/// \brief Convert one line of pixel data to target buffer.
	///
	/// \param buffer Buffer getting stored pixels in the specified format
	/// \param format Pixel format of target buffer.
	/// \param y Specifies which line to convert when converting one single line.
	void convert_line(void *buffer, const CL_PixelFormat &format, int y) const;

	/// \brief Converts current buffer to a new pixel format and returns the result.
	CL_PixelBuffer to_format(const CL_PixelFormat &format) const;

	/// \brief Sets a new colorkey without converting the buffer.
	///
	/// \param enabled Enable or disable the colorkey.
	/// \param colorkey New colorkey to use.
	void set_colorkey(bool enabled, unsigned int colorkey);

	/// \brief Draw a pixel at (x, y) using the specified color.
	void draw_pixel(int x, int y, const CL_Colorf &color);

	/// \brief Flip the entire image vertically (turn it upside down)
	void flip_vertical();

	/// \brief Mutiply the RGB components by the Alpha component
	///
	/// This is useful with certain blending functions
	void premultiply_alpha();
/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_PixelBuffer_Impl> impl;
/// \}
};

/// \}
