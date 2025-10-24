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

#include "../Render/texture.h"
#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Text/string_types.h"

enum CL_TextureWrapMode;
enum CL_TextureFilter;
enum CL_TextureCompareMode;
enum CL_CompareFunction;
class CL_PixelBuffer;
class CL_PixelFormat;
class CL_GraphicContextProvider;

/// \brief Interface for implementing a CL_Texture target.
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_TextureProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_TextureProvider() { return; }

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	/// \brief Destroys the texture provider.
	virtual void destroy() = 0;

	/// \brief Create texture.
	virtual void create(int width, int height, CL_TextureFormat internal_format, int depth) = 0;

	/// \brief Retrieve image data from texture.
	virtual CL_PixelBuffer get_pixeldata(CL_TextureFormat sized_format, int level) const = 0;

	/// \brief Generate the mipmap
	virtual void generate_mipmap() = 0;

	/// \brief Upload image to texture.
	virtual void set_image(CL_PixelBuffer &image, int level) = 0;

	/// \brief Upload cube map.
	virtual void set_cube_map(
		CL_PixelBuffer &cube_map_positive_x,
		CL_PixelBuffer &cube_map_negative_x,
		CL_PixelBuffer &cube_map_positive_y,
		CL_PixelBuffer &cube_map_negative_y,
		CL_PixelBuffer &cube_map_positive_z,
		CL_PixelBuffer &cube_map_negative_z,
		int level) = 0;

	virtual void set_compressed_image(
		int level,
		CL_TextureFormat internal_format,
		int width,
		int height,
		CL_DataBuffer &image) = 0;

	/// \brief Upload image to sub texture.
	virtual void set_subimage(
		int x,
		int y,
		const CL_PixelBuffer &image,
		const CL_Rect &src_rect,
		int level) = 0;

	/// \brief Copy image data from a graphic context.
	virtual void copy_image_from(
		int x,
		int y,
		int width,
		int height,
		int level,
		CL_TextureFormat internal_format,
		CL_GraphicContextProvider *gc) = 0;

	/// \brief Copy sub image data from a graphic context.
	virtual void copy_subimage_from(
		int offset_x,
		int offset_y,
		int x,
		int y,
		int width,
		int height,
		int level,
		CL_GraphicContextProvider *gc) = 0;

	/// \brief Set the minimum level of detail texture parameter.
	virtual void set_min_lod(double min_lod) = 0;

	/// \brief Set the maximum level of detail texture parameter.
	virtual void set_max_lod(double max_lod) = 0;

	/// \brief Sets the level of detail bias constant.
	virtual void set_lod_bias(double lod_bias) = 0;

	/// \brief Sets the texture base level texture parameter.
	virtual void set_base_level(int base_level) = 0;

	/// \brief Sets the texture max level texture parameter.
	virtual void set_max_level(int max_level) = 0;

	/// \brief Set the texture wrapping mode.
	virtual void set_wrap_mode(
		CL_TextureWrapMode wrap_s,
		CL_TextureWrapMode wrap_t,
		CL_TextureWrapMode wrap_r) = 0;

	virtual void set_wrap_mode(
		CL_TextureWrapMode wrap_s,
		CL_TextureWrapMode wrap_t) = 0;

	virtual void set_wrap_mode(
		CL_TextureWrapMode wrap_s) = 0;

	/// \brief Set the minification filter.
	virtual void set_min_filter(CL_TextureFilter filter) = 0;

	/// \brief Set the magnification filter.
	virtual void set_mag_filter(CL_TextureFilter filter) = 0;

	/// \brief Set the maximum degree of anisotropy.
	virtual void set_max_anisotropy(float v) = 0;

	/// \brief Sets the texture compare mode and compare function texture parameters.
	virtual void set_texture_compare(CL_TextureCompareMode mode, CL_CompareFunction func) = 0;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
