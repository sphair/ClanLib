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
**    Harry Storbacka
**    Mark Page
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "pixel_format.h"
#include "../../Core/Math/rect.h"
#include "texture_format.h"
#include "buffer_usage.h"

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
class CL_GraphicContext;
class CL_PixelBufferProvider;

/// \brief Pixel buffer prefered direction
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_PixelBufferDirection
{
	/// \brief Use of the pixel buffer is to send data to the gpu
	cl_data_to_gpu,

	/// \brief Use of the pixel buffer is to retrieve data from the gpu
	cl_data_from_gpu
};

/// \brief Pixel data container.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_PixelBuffer
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance
	CL_PixelBuffer();

	/// \brief Constructs a PixelBuffer
	///
	/// \param width = value
	/// \param height = value
	/// \param sized_format = Pixel Format
	/// \param data = The data (0 = Allocate automatically to a boundary of 16 bytes)
	/// \param only_reference_data : true = Reference the data. false = Copy the data
	CL_PixelBuffer(int width, int height, CL_TextureFormat sized_format, const void *data = 0, bool only_reference_data = false);

	/// \brief Constructs a GPU PixelBuffer
	///
	/// \param gc = Graphic Context
	/// \param width = value
	/// \param height = value
	/// \param direction = prefered direction that you will use tranferring the pixel buffer data
	/// \param sized_format = Texture Format
	/// \param data = data to copy from (May be NULL)
	/// \param usage = Buffer Usage
	CL_PixelBuffer(CL_GraphicContext &gc, int width, int height, CL_PixelBufferDirection direction = cl_data_to_gpu, CL_TextureFormat sized_format = cl_rgba8, const void *data = 0, CL_BufferUsage usage = cl_usage_stream_draw);

	/// \brief Constructs a GPU PixelBuffer from an existing pixelbuffer
	///
	/// \param gc = Graphic Context
	/// \param pbuff = The pixelbuffer to copy from
	/// \param direction = prefered direction that you will use tranferring the pixel buffer data
	/// \param usage = Buffer Usage
	CL_PixelBuffer(CL_GraphicContext &gc, const CL_PixelBuffer &pbuff, CL_PixelBufferDirection direction = cl_data_to_gpu, CL_BufferUsage usage = cl_usage_stream_draw);

	/// \brief Constructs a PixelBuffer
	///
	/// \param width = value
	/// \param height = value
	/// \param sized_format = Pixel Format
	/// \param palette = Palette
	/// \param data = void
	CL_PixelBuffer(int width, int height, CL_TextureFormat sized_format, const CL_Palette &palette, const void *data = 0);

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
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Create a copy of the pixelbuffer that doesn't share data with the original pixel buffer.
	CL_PixelBuffer copy() const;

	/// \brief Create a copy of the pixelbuffer that doesn't share data with the original pixel buffer.
	CL_PixelBuffer copy(const CL_Rect &rect) const;

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

	/// \brief Returns a pointer to the beginning of the pixel buffer as 8 bit data.
	unsigned char *get_data_uint8() { return reinterpret_cast<unsigned char*>(get_data()); }
	const unsigned char *get_data_uint8() const { return reinterpret_cast<const unsigned char*>(get_data()); }

	/// \brief Returns a pointer to the beginning of the pixel buffer as 16 bit data.
	unsigned short *get_data_uint16() { return reinterpret_cast<unsigned short*>(get_data()); }
	const unsigned short *get_data_uint16() const { return reinterpret_cast<const unsigned short*>(get_data()); }

	/// \brief Returns a pointer to the beginning of the pixel buffer as 32 bit data.
	unsigned int *get_data_uint32() { return reinterpret_cast<unsigned int*>(get_data()); }
	const unsigned int *get_data_uint32() const { return reinterpret_cast<const unsigned int*>(get_data()); }

	/// \brief Returns a pointer to the beginning of a specific line.
	void *get_line(int line) { unsigned char *d = get_data_uint8(); return d + line * get_pitch(); }
	const void *get_line(int line) const { const unsigned char *d = get_data_uint8(); return d + line * get_pitch(); }

	/// \brief Returns a pointer to the beginning of a specific line as 8 bit data.
	unsigned char *get_line_uint8(int line) { return reinterpret_cast<unsigned char*>(get_line(line)); }
	const unsigned char *get_line_uint8(int line) const { return reinterpret_cast<const unsigned char*>(get_line(line)); }

	/// \brief Returns a pointer to the beginning of a specific line as 16 bit data.
	unsigned short *get_line_uint16(int line) { return reinterpret_cast<unsigned short*>(get_line(line)); }
	const unsigned short *get_line_uint16(int line) const { return reinterpret_cast<const unsigned short*>(get_line(line)); }

	/// \brief Returns a pointer to the beginning of a specific line as 32 bit data.
	unsigned int *get_line_uint32(int line) { return reinterpret_cast<unsigned int*>(get_data()); }
	const unsigned int *get_line_uint32(int line) const { return reinterpret_cast<const unsigned int*>(get_line(line)); }

	/// \brief Returns true if format uses a source color key.
	bool has_colorkey() const;

	/// \brief Returns the source color key.
	unsigned int get_colorkey() const;

	/// \brief Returns the number of bytes per pixel
	///
	/// \return Bytes per pixel. 0 = Unknown
	int get_bytes_per_pixel() const;

	/// \brief Returns the red component color mask (if available)
	unsigned int get_red_mask() const;

	/// \brief Returns the green component color mask (if available)
	unsigned int get_green_mask() const;

	/// \brief Returns the blue component color mask (if available)
	unsigned int get_blue_mask() const;

	/// \brief Returns the alpha component color mask (if available)
	unsigned int get_alpha_mask() const;

	/// \brief Returns the pixel format
	CL_TextureFormat get_format() const;

	/// \brief Get Provider
	///
	/// \return provider
	CL_PixelBufferProvider *get_provider() const;

	/// \brief Return color of pixel at the specified coordinates.
	CL_Colorf get_pixel(int x, int y);

/// \}
/// \name Operations
/// \{

public:

	/// \brief Maps buffer into system memory.
	void lock(CL_BufferAccess access);

	/// \brief Unmaps element buffer.
	void unlock();

	/// \brief Uploads data to buffer.
	void upload_data(const CL_Rect &dest_rect, const void *data);

	/// \brief Return true if the CL_PixelBuffer is valid and usable
	operator bool () const;

	/// \brief Convert pixel buffer to the pixel format of the target buffer, storing
	///  the result in the target buffer.
	///
	/// \param target Target pixel buffer.
	void convert(CL_PixelBuffer &target) const;

	/// \brief Convert pixel buffer to the pixel format of the target buffer, storing
	///  the result in the target buffer.
	///
	/// \param target Buffer getting stored pixels in the specified format and pitch.
	/// \param dest_rect Destination rectangle for conversion.
	/// \param src_rect Source rectangle for conversion.
	void convert(CL_PixelBuffer &target, const CL_Rect &dest_rect, const CL_Rect &src_rect = CL_Rect()) const;

	/// \brief Converts current buffer to a new pixel format and returns the result.
	CL_PixelBuffer to_format(CL_TextureFormat sized_format) const;

	/// \brief Sets a new colorkey without converting the buffer.
	///
	/// (Used when converting from a format without an alpha channel) 
	///
	/// \param enabled Enable or disable the colorkey.
	/// \param colorkey New colorkey to use.
	void set_colorkey(bool enabled, unsigned int colorkey);

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
	friend class CL_PixelBuffer_Impl;
/// \}
};

/// \}
