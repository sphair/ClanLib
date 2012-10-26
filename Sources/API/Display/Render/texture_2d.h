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
**    Magnus Norddahl
**    Harry Storbacka
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../api_display.h"
#include "texture.h"

namespace clan
{

/// \brief 2D texture object class.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY Texture2D : public Texture
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	Texture2D();

	/// \brief Constructs a texture from an implementation
	///
	/// \param impl = The implementation
	Texture2D(const std::shared_ptr<Texture_Impl> &impl) : Texture(impl) { }

	/// \brief Constructs a Texture
	///
	/// \param context = Graphic Context
	/// \param width = value
	/// \param height = value
	/// \param internal_format = Texture Format
	/// \param levels = Mipmap levels for the texture. 0 = all levels
	Texture2D(GraphicContext &context, int width, int height, TextureFormat texture_format = tf_rgba8, int levels = 1);

	/// \brief Constructs a Texture
	///
	/// \param context = Graphic Context
	/// \param size = Size
	/// \param internal_format = Texture Format
	/// \param levels = Mipmap levels for the texture. 0 = all levels
	Texture2D(GraphicContext &context, const Size &size, TextureFormat texture_format = tf_rgba8, int levels = 1);

	Texture2D(
		GraphicContext &context,
		const std::string &fullname, const ImageImportDescription &import_desc = ImageImportDescription ());

	Texture2D(
		GraphicContext &context,
		const std::string &filename,
		const VirtualDirectory &directory, const ImageImportDescription &import_desc = ImageImportDescription ());

	Texture2D(
		GraphicContext &context,
		IODevice &file, const std::string &image_type, const ImageImportDescription &import_desc = ImageImportDescription ());

	Texture2D(
		GraphicContext &gc,
		const std::string &resource_id,
		ResourceManager *resources,
		const ImageImportDescription &import_desc = ImageImportDescription ());
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Get the texture width.
	int get_width() const;

	/// \brief Get the texture height.
	int get_height() const;

	/// \brief Get the texture size.
	Size get_size() const;

	/// \brief Retrieve image data from texture.
	PixelBuffer get_pixeldata(GraphicContext &gc, int level = 0) const;

	/// \brief Get pixeldata
	///
	/// \param format = Pixel Format
	/// \param level = value
	///
	/// \return Pixel Buffer
	PixelBuffer get_pixeldata(GraphicContext &gc, TextureFormat texture_format, int level = 0) const;

	/// \brief Get the texture wrap mode for the s coordinate.
	TextureWrapMode get_wrap_mode_s() const;

	/// \brief Get the texture wrap mode for the t coordinate.
	TextureWrapMode get_wrap_mode_t() const;
/// \}

/// \name Operations
/// \{
public:
	/// \brief Upload image to texture.
	///
	/// \param context Graphic context to use for the request
	/// \param image Image to upload.
	/// \param level Mipmap level-of-detail number.
	void set_image(
		GraphicContext &context,
		const PixelBuffer &image,
		int level = 0);

	/// \brief Upload image to sub texture.
	///
	/// \param context Graphic context to use for the request
	/// \param image Image to upload.
	/// \param level Mipmap level-of-detail number.
	void set_subimage(
		GraphicContext &context,
		int x,
		int y,
		const PixelBuffer &image,
		const Rect &src_rect,
		int level = 0);

	void set_subimage(
		GraphicContext &context,
		const Point &point,
		const PixelBuffer &image,
		const Rect &src_rect,
		int level = 0);

	/// \brief Copy image data from a graphic context.
	void copy_image_from(
		GraphicContext &context,
		int level,
		TextureFormat texture_format = tf_rgba8);

	void copy_image_from(
		GraphicContext &context,
		int x,
		int y,
		int width,
		int height,
		int level = 0,
		TextureFormat texture_format = tf_rgba8);

	void copy_image_from(
		GraphicContext &context,
		const Rect &pos,
		int level = 0,
		TextureFormat texture_format = tf_rgba8);

	/// \brief Copy sub image data from a graphic context.
	void copy_subimage_from(
		GraphicContext &context,
		int offset_x,
		int offset_y,
		int x,
		int y,
		int width,
		int height,
		int level = 0);

	void copy_subimage_from(
		GraphicContext &context,
		const Point &offset,
		const Rect &pos,
		int level = 0);

	void set_wrap_mode(
		TextureWrapMode wrap_s,
		TextureWrapMode wrap_t);
/// \}
};

}

/// \}
