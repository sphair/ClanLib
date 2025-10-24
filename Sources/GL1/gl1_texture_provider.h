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
*/

#pragma once


#include "API/Display/TargetProviders/texture_provider.h"
#include "API/Display/Render/compare_function.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/System/disposable_object.h"
#include "API/GL1/opengl1.h"

class CL_GL1GraphicContextProvider;

class CL_GL1TextureProvider : public CL_TextureProvider, CL_DisposableObject
{
/// \name Construction
/// \{
public:
	CL_GL1TextureProvider(CL_GL1GraphicContextProvider *gc_provider, CL_TextureDimensions texture_dimensions);
	~CL_GL1TextureProvider();
/// \}

/// \name Attributes
/// \{
public:
	CLuint get_handle() const { return handle; }
	CLuint get_texture_type() const { return texture_type; }

	bool is_power_of_two_texture() const { return power_of_two_texture; }

	CL_Size get_surface_size() const { return CL_Size(width, height); }
	CL_Size get_texture_size() const { return CL_Size(pot_width, pot_height); }

/// \}

/// \name Operations
/// \{
public:
	void generate_mipmap();
	void create(int width, int height, CL_TextureFormat internal_format, int depth);
	void destroy();
	CL_PixelBuffer get_pixeldata(CL_TextureFormat sized_format, int level) const;
	void set_image(CL_PixelBuffer &image, int level);

	void set_cube_map(
		CL_PixelBuffer &cube_map_positive_x,
		CL_PixelBuffer &cube_map_negative_x,
		CL_PixelBuffer &cube_map_positive_y,
		CL_PixelBuffer &cube_map_negative_y,
		CL_PixelBuffer &cube_map_positive_z,
		CL_PixelBuffer &cube_map_negative_z,
		int level);

	void set_compressed_image(
		int level,
		CL_TextureFormat internal_format,
		int width,
		int height,
		CL_DataBuffer &image);

	void set_subimage(
		int x,
		int y,
		const CL_PixelBuffer &image,
		const CL_Rect &src_rect,
		int level);

	void copy_image_from(
		int x,
		int y,
		int width,
		int height,
		int level,
		CL_TextureFormat internal_format,
		CL_GraphicContextProvider *gc);

	void copy_subimage_from(
		int offset_x,
		int offset_y,
		int x,
		int y,
		int width,
		int height,
		int level,
		CL_GraphicContextProvider *gc);

	void set_min_lod(double min_lod);
	void set_max_lod(double max_lod);
	void set_lod_bias(double lod_bias);
	void set_base_level(int base_level);
	void set_max_level(int max_level);

	void set_wrap_mode(
		CL_TextureWrapMode wrap_s,
		CL_TextureWrapMode wrap_t,
		CL_TextureWrapMode wrap_r);

	void set_wrap_mode(
		CL_TextureWrapMode wrap_s,
		CL_TextureWrapMode wrap_t);

	void set_wrap_mode(
		CL_TextureWrapMode wrap_s);

	void set_min_filter(CL_TextureFilter filter);
	void set_mag_filter(CL_TextureFilter filter);
	void set_max_anisotropy(float v);
	void set_texture_compare(CL_TextureCompareMode mode, CL_CompareFunction func);

	/// \brief Transform a non-power-of-two coordinate
	///
	/// \param attribute = Source attribute
	/// \param transformed_data = Destination (this will be set at a minimum of total_vertices*attribute.size
	/// \param vertex_offset = vertex offset
	/// \param num_vertices = Number of vertices
	/// \param total_vertices = Size to set the destination to
	void transform_coordinate(const CL_PrimitivesArrayData::VertexData &attribute, std::vector<float> &transformed_data, int vertex_offset, int num_vertices, int total_vertices);

/// \}

/// \name Implementation
/// \{
private:
	void on_dispose();
	void set_texture_image2d(CLuint target, CL_PixelBuffer &image, int level);
	void set_texture_image3d(CLuint target, CL_PixelBuffer &image, int image_depth, int level);
	int get_next_power_of_two(int value);

	CLenum to_enum(CL_TextureFilter filter);
	CLenum to_enum(CL_TextureWrapMode mode);
	CLenum to_enum(CL_CompareFunction func);
	CLenum to_enum(CL_TextureCompareMode mode);

	int width, height, depth;
	int pot_width, pot_height, pot_depth;
	float pot_ratio_width, pot_ratio_height, pot_ratio_depth;
	bool power_of_two_texture;

	/// \brief OpenGL texture handle.
	CLuint handle;

	CLuint texture_type;
/// \}
};

class CL_GL1TextureStateTracker
{
public:
	CL_GL1TextureStateTracker(CLuint texture_type, CLuint handle, CL_GL1GraphicContextProvider *gc_provider);
	~CL_GL1TextureStateTracker();

private:
	CLboolean last_is_enabled_texture1d;
	CLboolean last_is_enabled_texture2d;
	CLboolean last_is_enabled_texture3d;
	CLboolean last_is_enabled_texture_cube_map;
	CLuint last_bound_texture1d;
	CLuint last_bound_texture2d;
	CLuint last_bound_texture3d;
	CLuint last_bound_texture_cube_map;
};


