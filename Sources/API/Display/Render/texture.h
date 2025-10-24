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
#include "../../Core/Text/string_types.h"
#include "../../Core/IOData/virtual_directory.h"
#include "../../Core/Resources/resource_data_session.h"
#include "graphic_context.h"
#include "compare_function.h"
#include "../Image/image_import_description.h"
#include "../Image/texture_format.h"

class CL_Color;
class CL_Point;
class CL_PixelBuffer;
class CL_PixelFormat;
class CL_TextureProvider;
class CL_DataBuffer;
class CL_Texture_Impl;
class CL_SharedGCData_Impl;

/// \brief Texture coordinate wrapping modes.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_TextureWrapMode
{
	cl_wrap_clamp_to_edge,
	cl_wrap_repeat,
	cl_wrap_mirrored_repeat
};

/// \brief Texture filters.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_TextureFilter
{
	cl_filter_nearest,
	cl_filter_linear,
	cl_filter_nearest_mipmap_nearest,
	cl_filter_nearest_mipmap_linear,
	cl_filter_linear_mipmap_nearest,
	cl_filter_linear_mipmap_linear
};

/// \brief Texture compare modes.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_TextureCompareMode
{
	cl_comparemode_none,
	cl_comparemode_compare_r_to_texture
};

/// \brief Texture dimensions.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_TextureDimensions
{
	cl_texture_1d,
	cl_texture_2d,
	cl_texture_3d,
	cl_texture_cube_map
};

/// \brief Texture object class.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Texture
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	CL_Texture();

	/// \brief Constructs a Texture
	///
	/// \param context = Graphic Context
	/// \param texture_dimensions = Texture Dimensions
	CL_Texture(CL_GraphicContext &context, CL_TextureDimensions texture_dimensions);

	/// \brief Constructs a Texture
	///
	/// \param context = Graphic Context
	/// \param width = value
	/// \param height = value
	/// \param internal_format = Texture Format
	CL_Texture(CL_GraphicContext &context, int width, int height, CL_TextureFormat internal_format = cl_rgba);

	/// \brief Constructs a 3D Texture
	///
	/// \param context = Graphic Context
	/// \param width = value
	/// \param height = value
	/// \param depth = value
	/// \param internal_format = Texture Format
	CL_Texture(CL_GraphicContext &context, int width, int height, int depth, CL_TextureFormat internal_format = cl_rgba);

	/// \brief Constructs a Texture
	///
	/// \param context = Graphic Context
	/// \param size = Size
	/// \param internal_format = Texture Format
	CL_Texture(CL_GraphicContext &context, const CL_Size &size, CL_TextureFormat internal_format = cl_rgba);

	CL_Texture(
		CL_GraphicContext &context,
		const CL_StringRef &fullname, const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	CL_Texture(
		CL_GraphicContext &context,
		const CL_StringRef &filename,
		const CL_VirtualDirectory &directory, const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	CL_Texture(
		CL_GraphicContext &context,
		CL_IODevice &file, const CL_String &image_type, const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	CL_Texture(
		const CL_StringRef &resource_id,
		CL_ResourceManager *resources,
		CL_GraphicContext &gc, const CL_ImageImportDescription &import_desc = CL_ImageImportDescription ());

	/// \brief Constructs a texture from an implementation
	///
	/// \param impl = The implementation
	CL_Texture(CL_SharedPtr<CL_Texture_Impl> &impl);

	virtual ~CL_Texture();

/// \}
/// \name Operators
/// \{
public:
	/// \brief Equality operator
	bool operator==(const CL_Texture &other) const
	{
		return impl==other.impl;
	}

	/// \brief Inequality operator
	bool operator!=(const CL_Texture &other) const
	{
		return impl!=other.impl;
	}

	/// \brief Less than operator
	bool operator<(const CL_Texture &other) const
	{
		return impl < other.impl;
	}

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return impl.is_null(); }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get the texture width.
	int get_width() const;

	/// \brief Get the texture height.
	int get_height() const;

	/// \brief Get the texture size.
	CL_Size get_size() const;

	/// \brief Get the texture depth.
	int get_depth() const;

	/// \brief Retrieve image data from texture.
	CL_PixelBuffer get_pixeldata(int level = 0) const;

	/// \brief Get pixeldata
	///
	/// \param format = Pixel Format
	/// \param level = value
	///
	/// \return Pixel Buffer
	CL_PixelBuffer get_pixeldata(CL_TextureFormat sized_format, int level = 0) const;

	/// \brief Get the minimum level of detail.
	float get_min_lod() const;

	/// \brief Get the maximum level of detail.
	float get_max_lod() const;

	/// \brief Get the level of detail bias constant.
	float get_lod_bias() const;

	/// \brief Get the texture base level.
	int get_base_level() const;

	/// \brief Get the texture max level.
	int get_max_level() const;

	/// \brief Get the texture wrap mode for the s coordinate.
	CL_TextureWrapMode get_wrap_mode_s() const;

	/// \brief Get the texture wrap mode for the t coordinate.
	CL_TextureWrapMode get_wrap_mode_t() const;

	/// \brief Get the texture wrap mode for the r coordinate.
	CL_TextureWrapMode get_wrap_mode_r() const;

	/// \brief Get the texture minification filter.
	CL_TextureFilter get_min_filter() const;

	/// \brief Get the texture magnification filter.
	CL_TextureFilter get_mag_filter() const;

	/// \brief Returns true if texture is resident in texture memory.
	bool is_resident() const;

	/// \brief Get the texture compare mode.
	CL_TextureCompareMode get_compare_mode() const;

	/// \brief Get the texture compare function.
	CL_CompareFunction get_compare_function() const;

	/// \brief Get Provider
	///
	/// \return provider
	CL_TextureProvider *get_provider() const;

	/// \brief Get the implementation weakptr
	///
	/// This is used to assist is creating CL_Texture caches internally within clanlib
	CL_WeakPtr<CL_Texture_Impl> get_impl() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Generate the mipmap
	void generate_mipmap();

	/// \brief Upload image to texture.
	///
	/// \param image Image to upload.
	/// \param level Mipmap level-of-detail number.
	void set_image(
		CL_PixelBuffer &image,
		int level = 0);

	/// \brief Upload cube map.
	void set_cube_map(
		CL_PixelBuffer &cube_map_positive_x,
		CL_PixelBuffer &cube_map_negative_x,
		CL_PixelBuffer &cube_map_positive_y,
		CL_PixelBuffer &cube_map_negative_y,
		CL_PixelBuffer &cube_map_positive_z,
		CL_PixelBuffer &cube_map_negative_z,
		int level = 0);

	void set_compressed_image(
		int level,
		CL_TextureFormat internal_format,
		int width,
		int height,
		CL_DataBuffer &image);

	/// \brief Upload image to sub texture.
	///
	/// \param image Image to upload.
	/// \param level Mipmap level-of-detail number.
	void set_subimage(
		int x,
		int y,
		const CL_PixelBuffer &image,
		const CL_Rect &src_rect,
		int level = 0);

	void set_subimage(
		const CL_Point &point,
		const CL_PixelBuffer &image,
		const CL_Rect &src_rect,
		int level = 0);

	/// \brief Copy image data from a graphic context.
	void copy_image_from(
		CL_GraphicContext &context,
		int level,
		CL_TextureFormat internal_format = cl_rgba);

	void copy_image_from(
		CL_GraphicContext &context,
		int x,
		int y,
		int width,
		int height,
		int level = 0,
		CL_TextureFormat internal_format = cl_rgba);

	void copy_image_from(
		CL_GraphicContext &context,
		const CL_Rect &pos,
		int level = 0,
		CL_TextureFormat internal_format = cl_rgba);

	/// \brief Copy sub image data from a graphic context.
	void copy_subimage_from(
		CL_GraphicContext &context,
		int offset_x,
		int offset_y,
		int x,
		int y,
		int width,
		int height,
		int level = 0);

	void copy_subimage_from(
		CL_GraphicContext &context,
		const CL_Point &offset,
		const CL_Rect &pos,
		int level = 0);

	/// \brief Set the minimum level of detail texture parameter.
	void set_min_lod(float min_lod);

	/// \brief Set the maximum level of detail texture parameter.
	void set_max_lod(float max_lod);

	/// \brief Sets the level of detail bias constant.
	void set_lod_bias(float lod_bias);

	/// \brief Sets the texture base level texture parameter.
	void set_base_level(int base_level);

	/// \brief Sets the texture max level texture parameter.
	void set_max_level(int max_level);

	/// \brief Set the texture wrapping mode.
	void set_wrap_mode(
		CL_TextureWrapMode wrap_s,
		CL_TextureWrapMode wrap_t,
		CL_TextureWrapMode wrap_r);

	void set_wrap_mode(
		CL_TextureWrapMode wrap_s,
		CL_TextureWrapMode wrap_t);

	void set_wrap_mode(
		CL_TextureWrapMode wrap_s);

	/// \brief Set the minification filter.
	void set_min_filter(CL_TextureFilter filter);

	/// \brief Set the magnification filter.
	void set_mag_filter(CL_TextureFilter filter);

	/// \brief Set the maximum degree of anisotropy.
	void set_max_anisotropy(float max_anisotropy);

	/// \brief Sets the texture compare mode and compare function texture parameters.
	void set_texture_compare(CL_TextureCompareMode mode, CL_CompareFunction func);

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_Texture_Impl> impl;
/// \}
};

/// \}
