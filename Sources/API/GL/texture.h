/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

//! clanGL="Display 3D"
//! header=gl.h

#ifndef header_texture
#define header_texture

#ifdef CL_API_DLL
#ifdef CL_GL_EXPORT
#define CL_API_GL __declspec(dllexport)
#else
#define CL_API_GL __declspec(dllimport)
#endif
#else
#define CL_API_GL
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Core/System/sharedptr.h"

enum CL_TextureWrapMode
{
	cl_wrap_clamp,
	cl_wrap_clamp_to_edge,
	cl_wrap_clamp_to_border,
	cl_wrap_repeat,
	cl_wrap_mirrored_repeat
};

enum CL_TextureFilter
{
	cl_filter_nearest,
	cl_filter_linear,
	cl_filter_nearest_mipmap_nearest,
	cl_filter_nearest_mipmap_linear,
	cl_filter_linear_mipmap_nearest,
	cl_filter_linear_mipmap_linear
};

enum CL_TextureDepthMode
{
	cl_depthmode_luminance,
	cl_depthmode_intensity,
	cl_depthmode_alpha
};

enum CL_TextureCompareMode
{
	cl_comparemode_none,
	cl_comparemode_compare_r_to_texture
};

enum CL_TextureCompareFunction
{
	cl_comparefunc_lequal,
	cl_comparefunc_gequal,
	cl_comparefunc_less,
	cl_comparefunc_greater,
	cl_comparefunc_equal,
	cl_comparefunc_notequal,
	cl_comparefunc_always,
	cl_comparefunc_never
};

class CL_Color;
class CL_PixelFormat;
class CL_PixelBuffer;
class CL_Rect;
class CL_Point;
class CL_Size;
class CL_GraphicContext;
class CL_Texture_Generic;

//: OpenGL texture container.
//- <p>This class is ClanLib's C++ interface to an OpenGL texture.</p>
class CL_API_GL CL_Texture
{
//! Construction:
public:
	//: Construct a texture object.
	//param target_gl: OpenGL texture target. Can be CL_TEXTURE_1D, CL_TEXTURE_2D, CL_TEXTURE_3D or CL_TEXTURE_CUBE_MAP.
	//param image: Initial image to upload as level-of-detail 0.
	//param autogenerate_mipmap: If true, enables mipmap autogeneration before uploading image.
	//param border: Border width of texture.
	//param format: Internal texture format of texture.
	CL_Texture();

	CL_Texture(int target_gl);

	CL_Texture(
		int target_gl,
		CL_PixelBuffer &image,
		bool autogenerate_mipmaps = false,
		int border = 0,
		int format = 0);
	
	CL_Texture(
		int target_gl,
		int format,
		int width,
		int height = 1,
		int depth = 1,
		int border = 0,
		int level = 0);
	
	~CL_Texture();

//! Attributes:
public:
	//: Returns the OpenGL texture handle.
	int get_handle();

	//: Get the texture target as OpenGL constant.
	int get_target_gl();

	//: Get the internal texture format as OpenGL constant.
	int get_format_gl();

	//: Get the texture width.
	//param level: Mipmap level to get width for.
	int get_width(int level);

	//: Get the texture height.
	//param level: Mipmap level to get height for.
	int get_height(int level);

	//: Get the texture depth.
	int get_depth();

	//: Get the texture border size.
	int get_border();

	//: Retrieve image data from texture.
	CL_PixelBuffer get_pixeldata(int level = 0);

	CL_PixelBuffer get_pixeldata(CL_PixelFormat &format, int level = 0);

	CL_PixelBuffer get_pixeldata(const CL_Rect &rect, int level = 0);

	CL_PixelBuffer get_pixeldata(CL_PixelFormat &format, const CL_Rect &rect, int level = 0);

	//: Get the minimum level of detail.
	float get_min_lod() const;

	//: Get the maximum level of detail.
	float get_max_lod() const;

	//: Get the level of detail bias constant.
	float get_lod_bias() const;

	//: Get the texture base level.
	int get_base_level() const;

	//: Get the texture max level.
	int get_max_level() const;

	//: Get if automatic mipmap generation is enabled.
	bool get_generate_mipmap() const;

	//: Get the texture wrap mode for the s coordinate.
	CL_TextureWrapMode get_wrap_mode_s() const;

	//: Get the texture wrap mode for the s coordinate as OpenGL constant.
	int get_wrap_mode_s_gl() const;

	//: Get the texture wrap mode for the t coordinate.
	CL_TextureWrapMode get_wrap_mode_t() const;

	//: Get the texture wrap mode for the t coordinate as OpenGL constant.
	int get_wrap_mode_t_gl() const;

	//: Get the texture wrap mode for the r coordinate.
	CL_TextureWrapMode get_wrap_mode_r() const;

	//: Get the texture wrap mode for the r coordinate as OpenGL constant.
	int get_wrap_mode_r_gl() const;

	//: Get the texture minification filter.
	CL_TextureFilter get_min_filter() const;

	//: Get the texture minification filter as OpenGL constant.
	int get_min_filter_gl() const;

	//: Get the texture magnification filter.
	CL_TextureFilter get_mag_filter() const;

	//: Get the texture magnification filter as OpenGL constant.
	int get_mag_filter_gl() const;

	//: Get the texture border color.
	CL_Color get_border_color() const;

	//: Get the texture priority.
	float get_priority() const;
	
	//: Returns true if texture is resident in texture memory.
	bool is_resident() const;

	//: Get the texture depth mode.
	CL_TextureDepthMode get_depth_mode() const;

	//: Get the texture depth mode as OpenGL constant.
	int get_depth_mode_gl() const;

	//: Get the texture compare mode.
	CL_TextureCompareMode get_compare_mode() const;

	//: Get the texture compare mode as OpenGL constant.
	int get_compare_mode_gl() const;

	//: Get the texture compare function.
	CL_TextureCompareFunction get_compare_function() const;

	//: Get the texture compare function as OpenGL constant.
	int get_compare_function_gl() const;

//! Operations:
public:
	//: Bind texture on current active CL_OpenGLState.
	void bind();

	//: Set texture size, without uploading any image data.
	void set_size(
		int format,
		int width,
		int height = 1,
		int depth = 1,
		int border = 0,
		int level = 0);

	//: Upload image to texture.
	//param image: Image to upload.
	//param level: Mipmap level-of-detail number.
	//param border: Border width of texture.
	//param format: Internal texture format of texture.
	void set_image(
		CL_PixelBuffer &image,
		int level = 0,
		int border = 0,
		int format = 0);

	//: Upload cube map.
	void set_cube_map(
		CL_PixelBuffer &cube_map_positive_x,
		CL_PixelBuffer &cube_map_negative_x,
		CL_PixelBuffer &cube_map_positive_y,
		CL_PixelBuffer &cube_map_negative_y,
		CL_PixelBuffer &cube_map_positive_z,
		CL_PixelBuffer &cube_map_negative_z,
		int level = 0,
		int border = 0,
		int format = 0);

	//: Upload 3D image to texture with OpenGL constants.
	void set_image3d_gl(
		int gl_target,
		int gl_level,
		int gl_internal_format,
		int gl_width,
		int gl_height,
		int gl_depth,
		int gl_border,
		int gl_format,
		int gl_type = 0,
		void *gl_data = 0);

	//: Upload 2D image to texture with OpenGL constants.
	void set_image2d_gl(
		int gl_target,
		int gl_level,
		int gl_internal_format,
		int gl_width,
		int gl_height,
		int gl_border,
		int gl_format,
		int gl_type = 0,
		void *gl_data = 0);
		
	//: Upload 1D image to texture with OpenGL constants.
	void set_image1d_gl(
		int gl_target,
		int gl_level,
		int gl_internal_format,
		int gl_width,
		int gl_border,
		int gl_format,
		int gl_type = 0,
		void *gl_data = 0);

	//: Upload image to sub texture.
	//param image: Image to upload.
	//param level: Mipmap level-of-detail number.
	void set_subimage(
		int x,
		int y,
		CL_PixelBuffer &image,
		int level = 0);

	void set_subimage(
		const CL_Point &point,
		CL_PixelBuffer &image,
		int level = 0);

	//: Upload 3D sub image to texture with OpenGL constants.
	void set_subimage3d_gl(
		int gl_target,
		int gl_level,
		int gl_xoffset,
		int gl_yoffset,
		int gl_zoffset,
		int gl_width,
		int gl_height,
		int gl_depth,
		int gl_format,
		int gl_type,
		void *gl_data);

	//: Upload 2D sub image to texture with OpenGL constants.
	void set_subimage2d_gl(
		int gl_target,
		int gl_level,
		int gl_xoffset,
		int gl_yoffset,
		int gl_width,
		int gl_height,
		int gl_format,
		int gl_type,
		void *gl_data);
		
	//: Upload 1D sub image to texture with OpenGL constants.
	void set_subimage1d_gl(
		int gl_target,
		int gl_level,
		int gl_xoffset,
		int gl_width,
		int gl_format,
		int gl_type,
		void *gl_data);

	//: Copy image data from a graphic context.
	void copy_image(
		int level,
		int border = 0,
		int format = 0,
		CL_GraphicContext *gc = 0);

	void copy_image(
		int x,
		int y,
		int width,
		int height,
		int level = 0,
		int border = 0,
		int format = 0,
		CL_GraphicContext *gc = 0);
	
	void copy_image(
		const CL_Rect &pos,
		int level = 0,
		int border = 0,
		int format = 0,
		CL_GraphicContext *gc = 0);

	//: Copy 2D image data from a graphic context with OpenGL constants.
	void copy_image2d_gl(
		int gl_target,
		int gl_level,
		int gl_internal_format,
		int gl_x,
		int gl_y,
		int gl_width,
		int gl_height,
		int gl_border = 0,
		CL_GraphicContext *gc = 0);
	
	//: Copy 1D image data from a graphic context with OpenGL constants.
	void copy_image1d_gl(
		int gl_target,
		int gl_level,
		int gl_internal_format,
		int gl_x,
		int gl_y,
		int gl_width,
		int gl_border,
		CL_GraphicContext *gc = 0);

	//: Copy sub image data from a graphic context.
	void copy_subimage(
		int offset_x,
		int offset_y,
		int level,
		CL_GraphicContext *gc = 0);

	void copy_subimage(
		int offset_x,
		int offset_y,
		int x,
		int y,
		int width,
		int height,
		int level = 0,
		CL_GraphicContext *gc = 0);
	
	void copy_subimage(
		const CL_Point &offset,
		const CL_Rect &pos,
		int level = 0,
		CL_GraphicContext *gc = 0);

	//: Copy 3D sub image data from a graphic context with OpenGL constants.
	void copy_subimage3d_gl(
		int gl_target,
		int gl_level,
		int gl_xoffset,
		int gl_yoffset,
		int gl_zoffset,
		int gl_x,
		int gl_y,
		int gl_width,
		int gl_height,
		CL_GraphicContext *gc = 0);

	//: Copy 2D sub image data from a graphic context with OpenGL constants.
	void copy_subimage2d_gl(
		int gl_target,
		int gl_level,
		int gl_xoffset,
		int gl_yoffset,
		int gl_x,
		int gl_y,
		int gl_width,
		int gl_height,
		CL_GraphicContext *gc = 0);
	
	//: Copy 1D sub image data from a graphic context with OpenGL constants.
	void copy_subimage1d_gl(
		int gl_target,
		int gl_level,
		int gl_xoffset,
		int gl_internal_format,
		int gl_x,
		int gl_y,
		int gl_width,
		CL_GraphicContext *gc = 0);

	//: Set the minimum level of detail texture parameter.
	void set_min_lod(float min_lod);

	//: Set the maximum level of detail texture parameter.
	void set_max_lod(float max_lod);

	//: Sets the level of detail bias constant.
	void set_lod_bias(float lod_bias);

	//: Sets the texture base level texture parameter.
	void set_base_level(int base_level);

	//: Sets the texture max level texture parameter.
	void set_max_level(int max_level);

	//: Enables or disables automatic mipmap generation when uploading image data.
	void set_generate_mipmap(bool generate_mipmap = true);

	//: Set the texture wrapping mode.
	void set_wrap_mode(
		CL_TextureWrapMode wrap_s,
		CL_TextureWrapMode wrap_t,
		CL_TextureWrapMode wrap_r);
	
	void set_wrap_mode(
		CL_TextureWrapMode wrap_s,
		CL_TextureWrapMode wrap_t);

	void set_wrap_mode(
		CL_TextureWrapMode wrap_s);

	//: Set the texture wrapping mode with OpenGL constants.
	void set_wrap_mode_gl(
		int gl_wrap_s,
		int gl_wrap_t,
		int gl_wrap_r);

	void set_wrap_mode_gl(
		int gl_wrap_s,
		int gl_wrap_t);

	void set_wrap_mode_gl(
		int gl_wrap_s);

	//: Set the minification filter.
	void set_min_filter(CL_TextureFilter filter);

	//: Set the minification filter with an OpenGL constant.
	void set_min_filter_gl(int filter);

	//: Set the magnification filter.
	void set_mag_filter(CL_TextureFilter filter);

	//: Set the magnification filter with an OpenGL constant.
	void set_mag_filter_gl(int filter);

	//: Set texture border color.
	void set_border_color(float red, float green, float blue, float alpha);

	void set_border_color(const CL_Color &color);

	//: Set texture priority.
	void set_priority(float priority);

	//: Set the depth texture mode parameter.
	void set_depth_mode(CL_TextureDepthMode depth_mode);

	//: Set the depth texture mode with an OpenGL constant.
	void set_depth_mode_gl(int depth_mode);

	//: Sets the texture compare mode and compare function texture parameters.
	void set_texture_compare(CL_TextureCompareMode mode, CL_TextureCompareFunction func);

	//: Sets the texture compare mode and function with an OpenGL constant.
	void set_texture_compare_gl(int mode, int func);

	//: Returns wrap mode as OpenGL constant.
	static int wrap_mode_to_gl(CL_TextureWrapMode wrap);

	//: Returns OpenGL constant as wrap mode.
	static CL_TextureWrapMode gl_to_wrap_mode(int gl_wrap);

	//: Converts CL_TextureFilter enum to OpenGL constant.	
	static int filter_to_gl(CL_TextureFilter filter);

	//: Converts OpenGL constant to CL_Texturefilter enum.
	static CL_TextureFilter gl_to_filter(int gl_filter);
	
	//: Converts CL_TextureDepthMode enum to OpenGL constant.
	static int depth_mode_to_gl(CL_TextureDepthMode depth_mode);
	
	//: Converts OpenGL constant to CL_TextureDepthMode enum.
	static CL_TextureDepthMode gl_to_depth_mode(int gl_depth_mode);
	
	//: Converts CL_TextureCompareMode enum to OpenGL constant.
	static int compare_mode_to_gl(CL_TextureCompareMode mode);
	
	//: Converts OpenGL constant to CL_TextureCompareMode enum.
	static CL_TextureCompareMode gl_to_compare_mode(int gl_mode);
	
	//: Converts CL_TextureCompareFunction to OpenGL constant.
	static int compare_function_to_gl(CL_TextureCompareFunction func);

	//: Converts OpenGL constant to CL_TextureCompareFunction.
	static CL_TextureCompareFunction gl_to_compare_function(int gl_func);

//! Implementation:
private:
	CL_SharedPtr<CL_Texture_Generic> impl;
};

#endif
