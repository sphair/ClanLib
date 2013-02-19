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

#pragma once


#include "API/Display/TargetProviders/texture_provider.h"
#include "API/Core/System/disposable_object.h"
#include "API/GL/opengl.h"

namespace clan
{

class GL3GraphicContextProvider;

class GL3TextureProvider : public TextureProvider, DisposableObject
{
/// \name Construction
/// \{
public:
	GL3TextureProvider(TextureDimensions texture_dimensions);
	GL3TextureProvider(GLuint texture_type, GLuint handle);
	~GL3TextureProvider();
/// \}

/// \name Attributes
/// \{
public:
	GLuint get_handle() const { return handle; }
	GLuint get_texture_type() const { return texture_type; }
	GLint get_internal_format() const { return gl_internal_format; }
/// \}

/// \name Operations
/// \{
public:
	void generate_mipmap();
	void create(int width, int height, int depth, int array_size, TextureFormat texture_format, int levels);

	PixelBuffer get_pixeldata(GraphicContext &gc, TextureFormat texture_format, int level) const;

	void copy_from(GraphicContext &gc, int x, int y, int slice, int level, const PixelBuffer &src, const Rect &src_rect);

	void copy_image_from(
		int x,
		int y,
		int width,
		int height,
		int level,
		TextureFormat texture_format,
		GraphicContextProvider *gc);

	void copy_subimage_from(
		int offset_x,
		int offset_y,
		int x,
		int y,
		int width,
		int height,
		int level,
		GraphicContextProvider *gc);

	void set_min_lod(double min_lod);
	void set_max_lod(double max_lod);
	void set_lod_bias(double lod_bias);
	void set_base_level(int base_level);
	void set_max_level(int max_level);

	void set_wrap_mode(
		TextureWrapMode wrap_s,
		TextureWrapMode wrap_t,
		TextureWrapMode wrap_r);

	void set_wrap_mode(
		TextureWrapMode wrap_s,
		TextureWrapMode wrap_t);

	void set_wrap_mode(
		TextureWrapMode wrap_s);

	void set_min_filter(TextureFilter filter);
	void set_mag_filter(TextureFilter filter);
	void set_max_anisotropy(float v);
	void set_texture_compare(TextureCompareMode mode, CompareFunction func);
	TextureProvider *create_view(TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers);
/// \}

/// \name Implementation
/// \{
private:
	void create_initial(TextureDimensions texture_dimensions);
	GL3TextureProvider(GL3TextureProvider *orig_texture, TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers);

	void on_dispose();
	static GLenum to_enum(TextureFilter filter);
	static GLenum to_enum(TextureWrapMode mode);
	static GLenum to_enum(CompareFunction func);
	static GLenum to_enum(TextureCompareMode mode);
	static GLenum to_cube_target(int index);

	int width, height, depth, array_size;

	/// \brief OpenGL texture handle.
	GLuint handle;
	GLuint texture_type;
	GLint gl_internal_format;
/// \}
};

class TextureStateTracker
{
public:
	TextureStateTracker(GLuint target, GLuint handle);
	~TextureStateTracker();

private:
	GLuint target;
	GLuint previous_handle;
};

}
