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

#include "Display/display_precomp.h"
#include "API/GL/texture.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl_state.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/color.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Math/point.h"
#include "API/Core/System/error.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Texture_Generic:

class CL_Texture_Generic
{
//! Construction:
public:
	CL_Texture_Generic(int target) : handle(0), target(target)
	{
		clGenTextures(1, &handle);
	}
	
	~CL_Texture_Generic()
	{
		clDeleteTextures(1, &handle);
	}

//! Attributes:
public:
	CLuint handle;

	int target;

//! Operations:
public:
	void make_current() const
	{
		clBindTexture(target, handle);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CL_Texture Construction:

CL_Texture::CL_Texture()
{
}

CL_Texture::CL_Texture(int target_gl) 
: impl(new CL_Texture_Generic(target_gl))
{
}

CL_Texture::CL_Texture(
	int target_gl,
	CL_PixelBuffer &image,
	bool autogenerate_mipmaps,
	int border,
	int format)
: impl(new CL_Texture_Generic(target_gl))
{
	if (autogenerate_mipmaps)
		set_generate_mipmap(true);

	set_image(image, 0, border, format);
}

CL_Texture::CL_Texture(
	int target_gl,
	int width,
	int height,
	int depth,
	int border,
	int format,
	int level)
: impl(new CL_Texture_Generic(target_gl))
{
	set_size(
		format,
		width,
		height,
		depth,
		border,
		level);
}

CL_Texture::~CL_Texture()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Texture Attributes:

int CL_Texture::get_handle()
{
	return (int) impl->handle;
}

int CL_Texture::get_target_gl()
{
	return impl->target;
}

int CL_Texture::get_format_gl()
{
	impl->make_current();
	return 0;
}

int CL_Texture::get_width(int level)
{
	impl->make_current();
	return 0 / (level+1);
}

int CL_Texture::get_height(int level)
{
	impl->make_current();
	return 0 / (level+1);
}

int CL_Texture::get_depth()
{
	impl->make_current();
	return 0;
}

int CL_Texture::get_border()
{
	impl->make_current();
	return 0;
}

CL_PixelBuffer CL_Texture::get_pixeldata(int level)
{
	return get_pixeldata(CL_Rect(0, 0, get_width(level), get_height(level)), level);
}

CL_PixelBuffer CL_Texture::get_pixeldata(CL_PixelFormat &format, int level)
{
	return get_pixeldata(format, CL_Rect(0, 0, get_width(level), get_height(level)), level);
}

CL_PixelBuffer CL_Texture::get_pixeldata(const CL_Rect &rect, int level)
{
	return CL_PixelBuffer();
}

CL_PixelBuffer CL_Texture::get_pixeldata(CL_PixelFormat &format, const CL_Rect &rect, int level)
{
	return CL_PixelBuffer();
}

float CL_Texture::get_min_lod() const
{
	impl->make_current();
	CLfloat min_lod = 0.0f;
	clGetTexParameterfv(impl->target, CL_TEXTURE_MIN_LOD, &min_lod);
	return min_lod;
}

float CL_Texture::get_max_lod() const
{
	impl->make_current();
	CLfloat max_lod = 0.0f;
	clGetTexParameterfv(impl->target, CL_TEXTURE_MAX_LOD, &max_lod);
	return max_lod;
}

float CL_Texture::get_lod_bias() const
{
	impl->make_current();
	CLfloat lod_bias = 0.0f;
	clGetTexParameterfv(impl->target, CL_TEXTURE_LOD_BIAS, &lod_bias);
	return lod_bias;
}

int CL_Texture::get_base_level() const
{
	impl->make_current();
	CLint base_level = 0;
	clGetTexParameteriv(impl->target, CL_TEXTURE_BASE_LEVEL, &base_level);
	return base_level;
}

int CL_Texture::get_max_level() const
{
	impl->make_current();
	CLint max_level = 0;
	clGetTexParameteriv(impl->target, CL_TEXTURE_MAX_LEVEL, &max_level);
	return max_level;
}

bool CL_Texture::get_generate_mipmap() const
{
	impl->make_current();
	CLint generate_mipmap = CL_FALSE;
	clGetTexParameteriv(impl->target, CL_GENERATE_MIPMAP, &generate_mipmap);
	return (generate_mipmap != CL_FALSE);
}

CL_TextureWrapMode CL_Texture::get_wrap_mode_s() const
{
	return gl_to_wrap_mode(get_wrap_mode_s_gl());
}

int CL_Texture::get_wrap_mode_s_gl() const
{
	impl->make_current();
	CLint wrap_mode = CL_CLAMP;
	clGetTexParameteriv(impl->target, CL_TEXTURE_WRAP_S, &wrap_mode);
	return wrap_mode;
}

CL_TextureWrapMode CL_Texture::get_wrap_mode_t() const
{
	return gl_to_wrap_mode(get_wrap_mode_t_gl());
}

int CL_Texture::get_wrap_mode_t_gl() const
{
	impl->make_current();
	CLint wrap_mode = CL_CLAMP;
	clGetTexParameteriv(impl->target, CL_TEXTURE_WRAP_T, &wrap_mode);
	return wrap_mode;
}

CL_TextureWrapMode CL_Texture::get_wrap_mode_r() const
{
	return gl_to_wrap_mode(get_wrap_mode_r_gl());
}

int CL_Texture::get_wrap_mode_r_gl() const
{
	impl->make_current();
	CLint wrap_mode = CL_CLAMP;
	clGetTexParameteriv(impl->target, CL_TEXTURE_WRAP_R, &wrap_mode);
	return wrap_mode;
}

CL_TextureFilter CL_Texture::get_min_filter() const
{
	return gl_to_filter(get_min_filter_gl());
}

int CL_Texture::get_min_filter_gl() const
{
	impl->make_current();
	CLint filter = CL_LINEAR;
	clGetTexParameteriv(impl->target, CL_TEXTURE_MIN_FILTER, &filter);
	return filter;
}

CL_TextureFilter CL_Texture::get_mag_filter() const
{
	return gl_to_filter(get_mag_filter_gl());
}

int CL_Texture::get_mag_filter_gl() const
{
	impl->make_current();
	CLint filter = CL_LINEAR;
	clGetTexParameteriv(impl->target, CL_TEXTURE_MAG_FILTER, &filter);
	return filter;
}

CL_Color CL_Texture::get_border_color() const
{
	impl->make_current();
	return CL_Color::black;
}

float CL_Texture::get_priority() const
{
	impl->make_current();
	CLfloat priority = 0.0f;
	clGetTexParameterfv(impl->target, CL_TEXTURE_PRIORITY, &priority);
	return priority;
}

bool CL_Texture::is_resident() const
{
	impl->make_current();
	CLint resident = CL_FALSE;
	clGetTexParameteriv(impl->target, CL_TEXTURE_RESIDENT, &resident);
	return (resident != CL_FALSE);
}

CL_TextureDepthMode CL_Texture::get_depth_mode() const
{
	return gl_to_depth_mode(get_depth_mode_gl());
}

int CL_Texture::get_depth_mode_gl() const
{
	impl->make_current();
	CLint depth_mode = CL_LUMINANCE;
	clGetTexParameteriv(impl->target, CL_DEPTH_TEXTURE_MODE, &depth_mode);
	return depth_mode;
}

CL_TextureCompareMode CL_Texture::get_compare_mode() const
{
	return gl_to_compare_mode(get_compare_mode_gl());
}

int CL_Texture::get_compare_mode_gl() const
{
	impl->make_current();
	CLint compare_mode = CL_NONE;
	clGetTexParameteriv(impl->target, CL_TEXTURE_COMPARE_MODE, &compare_mode);
	return compare_mode;
}

CL_TextureCompareFunction CL_Texture::get_compare_function() const
{
	return gl_to_compare_function(get_compare_function_gl());
}

int CL_Texture::get_compare_function_gl() const
{
	impl->make_current();
	CLint compare_function = CL_LEQUAL;
	clGetTexParameteriv(impl->target, CL_TEXTURE_COMPARE_FUNC, &compare_function);
	return compare_function;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Texture Operations:

void CL_Texture::bind()
{
	clBindTexture(impl->target, impl->handle);
}

void CL_Texture::set_size(
	int format,
	int width,
	int height,
	int depth,
	int border,
	int level)
{
	switch (impl->target)
	{
	case CL_TEXTURE_3D:
		set_image3d_gl(
			impl->target,
			level,
			format,
			width,
			height,
			depth,
			border,
			CL_RGBA,
			CL_BYTE,
			0);
		break;

	case CL_TEXTURE_2D:
		set_image2d_gl(
			impl->target,
			level,
			format,
			width,
			height,
			border,
			CL_RGBA,
			CL_BYTE,
			0);
		break;

	case CL_TEXTURE_1D:
		set_image2d_gl(
			impl->target,
			level,
			format,
			width,
			border,
			CL_RGBA,
			CL_BYTE,
			0);
		break;

	default:
		throw CL_Error("CL_Texture::set_image does not support this texture format");
	}
}

void CL_Texture::set_image(
	CL_PixelBuffer &image,
	int level,
	int border,
	int format)
{
	CLenum native_format = 0, native_type = 0;
	bool is_native_format = CL_OpenGL::to_opengl_pixelformat(image.get_format(), native_format, native_type);
	if (is_native_format)
	{
		switch (impl->target)
		{
/*		case CL_TEXTURE_3D:
			image.lock();
			set_image3d_gl(
				impl->target,
				level,
				(format == 0) ? native_format : format,
				image.get_width(),
				image.get_height(),
				image.get_depth(),
				border,
				native_format,
				native_type,
				image.get_data());
			image.unlock();
			break;
*/
		case CL_TEXTURE_2D:
			image.lock();
			set_image2d_gl(
				impl->target,
				level,
				(format == 0) ? native_format : format,
				image.get_width(),
				image.get_height(),
				border,
				native_format,
				native_type,
				image.get_data());
			image.unlock();
			break;

		case CL_TEXTURE_1D:
			image.lock();
			set_image1d_gl(
				impl->target,
				level,
				(format == 0) ? native_format : format,
				image.get_width(),
				border,
				native_format,
				native_type,
				image.get_data());
			image.unlock();
			break;

		default:
			throw CL_Error("CL_Texture::set_image does not support this texture format");
		}
	}
	else
	{
		// For now, upload all non-opengl compatible formats in rgba8888 format:
		
		CL_PixelBuffer gl_image = image.to_format(CL_PixelFormat::rgba8888);
		set_image(gl_image, format, level, border);
	}
}

void CL_Texture::set_cube_map(
	CL_PixelBuffer &cube_map_positive_x,
	CL_PixelBuffer &cube_map_negative_x,
	CL_PixelBuffer &cube_map_positive_y,
	CL_PixelBuffer &cube_map_negative_y,
	CL_PixelBuffer &cube_map_positive_z,
	CL_PixelBuffer &cube_map_negative_z,
	int level,
	int border,
	int format)
{
	if (impl->target != CL_TEXTURE_CUBE_MAP)
		throw CL_Error("CL_Texture::set_cube_map does not support this texture format");

	CL_PixelBuffer cube_maps[6];
	cube_maps[0] = cube_map_positive_x;
	cube_maps[1] = cube_map_negative_x;
	cube_maps[2] = cube_map_positive_y;
	cube_maps[3] = cube_map_negative_y;
	cube_maps[4] = cube_map_positive_z;
	cube_maps[5] = cube_map_negative_z;
	CLenum native_format[6];
	CLenum native_type[6];
	int index_maps;

	// Make sure our images are in opengl compatible formats:
	for (index_maps = 0; index_maps < 6; index_maps++)
	{
		bool is_native_format = CL_OpenGL::to_opengl_pixelformat(
			cube_maps[index_maps].get_format(),
			native_format[index_maps],
			native_type[index_maps]);
		if (!is_native_format)
		{
			cube_maps[index_maps] = cube_maps[index_maps].to_format(CL_PixelFormat::rgba8888);
			is_native_format = CL_OpenGL::to_opengl_pixelformat(
				cube_maps[index_maps].get_format(),
				native_format[index_maps],
				native_type[index_maps]);
		}
	}
	
	// Upload cube maps:
	int targets[6] =
	{
		CL_TEXTURE_CUBE_MAP_POSITIVE_X,
		CL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		CL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		CL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		CL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		CL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	
	for (index_maps = 0; index_maps < 6; index_maps++)
	{
		cube_maps[index_maps].lock();
		set_image2d_gl(
			targets[index_maps],
			level,
			(format == 0) ? native_format[index_maps] : format,
			cube_maps[index_maps].get_width(),
			cube_maps[index_maps].get_height(),
			border,
			native_format[index_maps],
			native_type[index_maps],
			cube_maps[index_maps].get_data());
		cube_maps[index_maps].unlock();
	}
}

void CL_Texture::set_image3d_gl(
	int gl_target,
	int gl_level,
	int gl_internal_format,
	int gl_width,
	int gl_height,
	int gl_depth,
	int gl_border,
	int gl_format,
	int gl_type,
	void *gl_data)
{
	impl->make_current();
	clTexImage3D(
		gl_target,
		gl_level,
		gl_internal_format,
		gl_width,
		gl_height,
		gl_depth,
		gl_border,
		gl_format,
		gl_type,
		gl_data);
}

void CL_Texture::set_image2d_gl(
	int gl_target,
	int gl_level,
	int gl_internal_format,
	int gl_width,
	int gl_height,
	int gl_border,
	int gl_format,
	int gl_type,
	void *gl_data)
{
	impl->make_current();
	clTexImage2D(
		gl_target,
		gl_level,
		gl_internal_format,
		gl_width,
		gl_height,
		gl_border,
		gl_format,
		gl_type,
		gl_data);
}
		
void CL_Texture::set_image1d_gl(
	int gl_target,
	int gl_level,
	int gl_internal_format,
	int gl_width,
	int gl_border,
	int gl_format,
	int gl_type,
	void *gl_data)
{
	impl->make_current();
	clTexImage1D(
		gl_target,
		gl_level,
		gl_internal_format,
		gl_width,
		gl_border,
		gl_format,
		gl_type,
		gl_data);
}

void CL_Texture::set_subimage(
	int x,
	int y,
	CL_PixelBuffer &image,
	int level)
{
}

void CL_Texture::set_subimage(
	const CL_Point &point,
	CL_PixelBuffer &image,
	int level)
{
}

void CL_Texture::set_subimage3d_gl(
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
	void *gl_data)
{
	impl->make_current();
	clTexSubImage3D(
		gl_target,
		gl_level,
		gl_xoffset,
		gl_yoffset,
		gl_zoffset,
		gl_width,
		gl_height,
		gl_depth,
		gl_format,
		gl_type,
		gl_data);
}

void CL_Texture::set_subimage2d_gl(
	int gl_target,
	int gl_level,
	int gl_xoffset,
	int gl_yoffset,
	int gl_width,
	int gl_height,
	int gl_format,
	int gl_type,
	void *gl_data)
{
	impl->make_current();
	clTexSubImage2D(
		gl_target,
		gl_level,
		gl_xoffset,
		gl_yoffset,
		gl_width,
		gl_height,
		gl_format,
		gl_type,
		gl_data);
}

void CL_Texture::set_subimage1d_gl(
	int gl_target,
	int gl_level,
	int gl_xoffset,
	int gl_width,
	int gl_format,
	int gl_type,
	void *gl_data)
{
	impl->make_current();
	clTexSubImage1D(
		gl_target,
		gl_level,
		gl_xoffset,
		gl_width,
		gl_format,
		gl_type,
		gl_data);
}

void CL_Texture::copy_image(
	int level,
	int border,
	int format,
	CL_GraphicContext *gc)
{
}

void CL_Texture::copy_image(
	int x,
	int y,
	int width,
	int height,
	int level,
	int border,
	int format,
	CL_GraphicContext *gc)
{
}

void CL_Texture::copy_image(
	const CL_Rect &pos,
	int level,
	int border,
	int format,
	CL_GraphicContext *gc)
{
}

void CL_Texture::copy_image2d_gl(
	int gl_target,
	int gl_level,
	int gl_internal_format,
	int gl_x,
	int gl_y,
	int gl_width,
	int gl_height,
	int gl_border,
	CL_GraphicContext *gc)
{
	CL_OpenGLState state(gc);
	state.set_active();
	clEnable(impl->target);
	clBindTexture(impl->target, impl->handle);
	clCopyTexImage2D(
		gl_target,
		gl_level,
		gl_internal_format,
		gl_x,
		gl_y,
		gl_width,
		gl_height,
		gl_border);
}

void CL_Texture::copy_image1d_gl(
	int gl_target,
	int gl_level,
	int gl_internal_format,
	int gl_x,
	int gl_y,
	int gl_width,
	int gl_border,
	CL_GraphicContext *gc)
{
	CL_OpenGLState state(gc);
	state.set_active();
	clEnable(impl->target);
	clBindTexture(impl->target, impl->handle);
	clCopyTexImage1D(
		gl_target,
		gl_level,
		gl_internal_format,
		gl_x,
		gl_y,
		gl_width,
		gl_border);
}

void CL_Texture::copy_subimage(
	int offset_x,
	int offset_y,
	int level,
	CL_GraphicContext *gc)
{
}

void CL_Texture::copy_subimage(
	int offset_x,
	int offset_y,
	int x,
	int y,
	int width,
	int height,
	int level,
	CL_GraphicContext *gc)
{
}

void CL_Texture::copy_subimage(
	const CL_Point &offset,
	const CL_Rect &pos,
	int level,
	CL_GraphicContext *gc)
{
}

void CL_Texture::copy_subimage3d_gl(
	int gl_target,
	int gl_level,
	int gl_xoffset,
	int gl_yoffset,
	int gl_zoffset,
	int gl_x,
	int gl_y,
	int gl_width,
	int gl_height,
	CL_GraphicContext *gc)
{
	CL_OpenGLState state(gc);
	state.set_active();
	clEnable(impl->target);
	clBindTexture(impl->target, impl->handle);
	clCopyTexSubImage3D(
		gl_target,
		gl_level,
		gl_xoffset,
		gl_yoffset,
		gl_zoffset,
		gl_x,
		gl_y,
		gl_width,
		gl_height);
}

void CL_Texture::copy_subimage2d_gl(
	int gl_target,
	int gl_level,
	int gl_xoffset,
	int gl_yoffset,
	int gl_x,
	int gl_y,
	int gl_width,
	int gl_height,
	CL_GraphicContext *gc)
{
	CL_OpenGLState state(gc);
	state.set_active();
	clEnable(impl->target);
	clBindTexture(impl->target, impl->handle);
	clCopyTexSubImage2D(
		gl_target,
		gl_level,
		gl_xoffset,
		gl_yoffset,
		gl_x,
		gl_y,
		gl_width,
		gl_height);
}

void CL_Texture::copy_subimage1d_gl(
	int gl_target,
	int gl_level,
	int gl_xoffset,
	int gl_internal_format,
	int gl_x,
	int gl_y,
	int gl_width,
	CL_GraphicContext *gc)
{
	CL_OpenGLState state(gc);
	state.set_active();
	clEnable(impl->target);
	clBindTexture(impl->target, impl->handle);
	clCopyTexSubImage1D(
		gl_target,
		gl_level,
		gl_xoffset,
		gl_x,
		gl_y,
		gl_width);
}

void CL_Texture::set_min_lod(float min_lod)
{
	impl->make_current();
	clTexParameterf(impl->target, CL_TEXTURE_MIN_LOD, min_lod);
}

void CL_Texture::set_max_lod(float max_lod)
{
	impl->make_current();
	clTexParameterf(impl->target, CL_TEXTURE_MAX_LOD, max_lod);
}

void CL_Texture::set_lod_bias(float lod_bias)
{
	impl->make_current();
	clTexParameterf(impl->target, CL_TEXTURE_LOD_BIAS, lod_bias);
}

void CL_Texture::set_base_level(int base_level)
{
	impl->make_current();
	clTexParameteri(impl->target, CL_TEXTURE_BASE_LEVEL, base_level);
}

void CL_Texture::set_max_level(int max_level)
{
	impl->make_current();
	clTexParameteri(impl->target, CL_TEXTURE_MAX_LEVEL, max_level);
}

void CL_Texture::set_generate_mipmap(bool generate_mipmap)
{
	impl->make_current();
	clTexParameteri(impl->target, CL_GENERATE_MIPMAP, generate_mipmap ? CL_TRUE : CL_FALSE);
}

void CL_Texture::set_wrap_mode(
	CL_TextureWrapMode wrap_s,
	CL_TextureWrapMode wrap_t,
	CL_TextureWrapMode wrap_r)
{
	set_wrap_mode_gl(
		wrap_mode_to_gl(wrap_s),
		wrap_mode_to_gl(wrap_t),
		wrap_mode_to_gl(wrap_r));
}

void CL_Texture::set_wrap_mode(
	CL_TextureWrapMode wrap_s,
	CL_TextureWrapMode wrap_t)
{
	set_wrap_mode_gl(
		wrap_mode_to_gl(wrap_s),
		wrap_mode_to_gl(wrap_t));
}

void CL_Texture::set_wrap_mode(CL_TextureWrapMode wrap_s)
{
	set_wrap_mode_gl(wrap_mode_to_gl(wrap_s));
}

void CL_Texture::set_wrap_mode_gl(
	int gl_wrap_s,
	int gl_wrap_t,
	int gl_wrap_r)
{
	impl->make_current();
	clTexParameteri(impl->target, CL_TEXTURE_WRAP_S, gl_wrap_s);
	clTexParameteri(impl->target, CL_TEXTURE_WRAP_T, gl_wrap_t);
	clTexParameteri(impl->target, CL_TEXTURE_WRAP_R, gl_wrap_r);
}

void CL_Texture::set_wrap_mode_gl(
	int gl_wrap_s,
	int gl_wrap_t)
{
	impl->make_current();
	clTexParameteri(impl->target, CL_TEXTURE_WRAP_S, gl_wrap_s);
	clTexParameteri(impl->target, CL_TEXTURE_WRAP_T, gl_wrap_t);
}

void CL_Texture::set_wrap_mode_gl(int gl_wrap_s)
{
	impl->make_current();
	clTexParameteri(impl->target, CL_TEXTURE_WRAP_S, gl_wrap_s);
}

void CL_Texture::set_min_filter(CL_TextureFilter filter)
{
	set_min_filter_gl(filter_to_gl(filter));
}

void CL_Texture::set_min_filter_gl(int gl_filter)
{
	impl->make_current();
	clTexParameteri(impl->target, CL_TEXTURE_MIN_FILTER, gl_filter);
}

void CL_Texture::set_mag_filter(CL_TextureFilter filter)
{
	set_mag_filter_gl(filter_to_gl(filter));
}

void CL_Texture::set_mag_filter_gl(int gl_filter)
{
	impl->make_current();
	clTexParameteri(impl->target, CL_TEXTURE_MAG_FILTER, gl_filter);
}

void CL_Texture::set_border_color(float red, float green, float blue, float alpha)
{
	impl->make_current();
	CLfloat color[4] = { red, green, blue, alpha };
	clTexParameterfv(impl->target, CL_TEXTURE_BORDER_COLOR, color);
}

void CL_Texture::set_border_color(const CL_Color &color)
{
	set_border_color(
		color.get_red()/255.0f,
		color.get_green()/255.0f,
		color.get_blue()/255.0f,
		color.get_alpha()/255.0f);
}

void CL_Texture::set_priority(float priority)
{
	impl->make_current();
	clTexParameterf(impl->target, CL_TEXTURE_PRIORITY, priority);
}

void CL_Texture::set_depth_mode(CL_TextureDepthMode depth_mode)
{
	set_depth_mode_gl(depth_mode_to_gl(depth_mode));
}

void CL_Texture::set_depth_mode_gl(int gl_depth_mode)
{
	impl->make_current();
	clTexParameteri(impl->target, CL_DEPTH_TEXTURE_MODE, gl_depth_mode);
}

void CL_Texture::set_texture_compare(CL_TextureCompareMode mode, CL_TextureCompareFunction func)
{
	set_texture_compare_gl(
		compare_mode_to_gl(mode),
		compare_function_to_gl(func));
}

void CL_Texture::set_texture_compare_gl(int gl_mode, int gl_func)
{
	impl->make_current();
	clTexParameteri(impl->target, CL_TEXTURE_COMPARE_MODE, gl_mode);
	clTexParameteri(impl->target, CL_TEXTURE_COMPARE_FUNC, gl_func);
}

int CL_Texture::wrap_mode_to_gl(CL_TextureWrapMode wrap)
{
	switch (wrap)
	{
	case cl_wrap_clamp:
		return CL_CLAMP;
	case cl_wrap_clamp_to_edge:
		return CL_CLAMP_TO_EDGE;
	case cl_wrap_clamp_to_border:
		return CL_CLAMP_TO_BORDER;
	case cl_wrap_repeat:
		return CL_REPEAT;
	case cl_wrap_mirrored_repeat:
		return CL_MIRRORED_REPEAT;
	}
	return -1;
}

CL_TextureWrapMode CL_Texture::gl_to_wrap_mode(int gl_wrap)
{
	switch (gl_wrap)
	{
	case CL_CLAMP:
		return cl_wrap_clamp;
	case CL_CLAMP_TO_EDGE:
		return cl_wrap_clamp_to_edge;
	case CL_CLAMP_TO_BORDER:
		return cl_wrap_clamp_to_border;
	case CL_REPEAT:
		return cl_wrap_repeat;
	case CL_MIRRORED_REPEAT:
		return cl_wrap_mirrored_repeat;
	}
	return cl_wrap_clamp;
}

int CL_Texture::filter_to_gl(CL_TextureFilter filter)
{
	switch (filter)
	{
	case cl_filter_nearest:
		return CL_NEAREST;
	case cl_filter_linear:
		return CL_LINEAR;
	case cl_filter_nearest_mipmap_nearest:
		return CL_NEAREST_MIPMAP_NEAREST;
	case cl_filter_nearest_mipmap_linear:
		return CL_NEAREST_MIPMAP_LINEAR;
	case cl_filter_linear_mipmap_nearest:
		return CL_LINEAR_MIPMAP_NEAREST;
	case cl_filter_linear_mipmap_linear:
		return CL_LINEAR_MIPMAP_LINEAR;
	}
	return -1;
}

CL_TextureFilter CL_Texture::gl_to_filter(int gl_filter)
{
	switch (gl_filter)
	{
	case CL_NEAREST:
		return cl_filter_nearest;
	case CL_LINEAR:
		return cl_filter_linear;
	case CL_NEAREST_MIPMAP_NEAREST:
		return cl_filter_nearest_mipmap_nearest;
	case CL_NEAREST_MIPMAP_LINEAR:
		return cl_filter_nearest_mipmap_linear;
	case CL_LINEAR_MIPMAP_NEAREST:
		return cl_filter_linear_mipmap_nearest;
	case CL_LINEAR_MIPMAP_LINEAR:
		return cl_filter_linear_mipmap_linear;
	}
	return cl_filter_nearest;
}

int CL_Texture::depth_mode_to_gl(CL_TextureDepthMode depth_mode)
{
	switch (depth_mode)
	{
	case cl_depthmode_luminance:
		return CL_LUMINANCE;
	case cl_depthmode_intensity:
		return CL_INTENSITY;
	case cl_depthmode_alpha:
		return CL_ALPHA;
	}
	return -1;
}

CL_TextureDepthMode CL_Texture::gl_to_depth_mode(int gl_depth_mode)
{
	switch (gl_depth_mode)
	{
	case CL_LUMINANCE:
		return cl_depthmode_luminance;
	case CL_INTENSITY:
		return cl_depthmode_intensity;
	case CL_ALPHA:
		return cl_depthmode_alpha;
	}
	return cl_depthmode_luminance;
}

int CL_Texture::compare_mode_to_gl(CL_TextureCompareMode mode)
{
	switch (mode)
	{
	case cl_comparemode_none:
		return CL_NONE;
	case cl_comparemode_compare_r_to_texture:
		return CL_COMPARE_R_TO_TEXTURE;
	}
	return -1;
}

CL_TextureCompareMode CL_Texture::gl_to_compare_mode(int gl_mode)
{
	switch (gl_mode)
	{
	case CL_NONE:
		return cl_comparemode_none;
	case CL_COMPARE_R_TO_TEXTURE:
		return cl_comparemode_compare_r_to_texture;
	}
        return cl_comparemode_none;
}

int CL_Texture::compare_function_to_gl(CL_TextureCompareFunction func)
{
	switch (func)
	{
	case cl_comparefunc_lequal:
		return CL_LEQUAL;
	case cl_comparefunc_gequal:
		return CL_GEQUAL;
	case cl_comparefunc_less:
		return CL_LESS;
	case cl_comparefunc_greater:
		return CL_GREATER;
	case cl_comparefunc_equal:
		return CL_EQUAL;
	case cl_comparefunc_notequal:
		return CL_NOTEQUAL;
	case cl_comparefunc_always:
		return CL_ALWAYS;
	case cl_comparefunc_never:
		return CL_NEVER;
	}
	return -1;
}

CL_TextureCompareFunction CL_Texture::gl_to_compare_function(int gl_func)
{
	switch (gl_func)
	{
	case CL_LEQUAL:
		return cl_comparefunc_lequal;
	case CL_GEQUAL:
		return cl_comparefunc_gequal;
	case CL_LESS:
		return cl_comparefunc_less;
	case CL_GREATER:
		return cl_comparefunc_greater;
	case CL_EQUAL:
		return cl_comparefunc_equal;
	case CL_NOTEQUAL:
		return cl_comparefunc_notequal;
	case CL_ALWAYS:
		return cl_comparefunc_always;
	case CL_NEVER:
		return cl_comparefunc_never;
	}
	return cl_comparefunc_lequal;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Texture Implementation:
