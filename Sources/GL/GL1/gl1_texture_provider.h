/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/TargetProviders/primitives_array_provider.h"
#include "API/Core/System/disposable_object.h"
#include "API/GL/opengl.h"

namespace clan
{
	class GL1GraphicContextProvider;

	class GL1TextureProvider : public TextureProvider, DisposableObject
	{
	public:
		GL1TextureProvider(TextureDimensions texture_dimensions);
		~GL1TextureProvider();

		GLuint get_handle() const { return handle; }
		GLuint get_texture_type() const { return texture_type; }

		bool is_power_of_two_texture() const { return power_of_two_texture; }

		Size get_surface_size() const { return Size(width, height); }
		Size get_texture_size() const { return Size(pot_width, pot_height); }

		/// \brief Get a OpenGL format out of a pixel format.
		/** <p>The function returns false if pixelformat color depth is not convertible to
			OpenGL pixel format, otherwise the format and type are returned with values in format and type.</p>*/
		static bool to_opengl_pixelformat(const PixelBuffer &pbuffer, GLenum &format, GLenum &type);

		/// \brief Get a OpenGL format out of a pixel format.
		/** <p>The function returns false if pixelformat color depth is not convertible to
			OpenGL pixel format, otherwise the format and type are returned with values in format and type.</p>*/
		static bool to_opengl_pixelformat(TextureFormat texture_format, GLenum &format, GLenum &type);

		/// \brief To opengl textureformat
		///
		/// \param format = Texture Format
		/// \param gl_internal_format = GLint
		/// \param gl_pixel_format = GLenum
		static void to_opengl_textureformat(TextureFormat format, GLint &gl_internal_format, GLenum &gl_pixel_format);

		void generate_mipmap() override;
		void create(int width, int height, int depth, int array_size, TextureFormat texture_format, int levels) override;
		PixelBuffer get_pixeldata(GraphicContext &gc, TextureFormat texture_format, int level) const override;

		void copy_from(GraphicContext &gc, int x, int y, int slice, int level, const PixelBuffer &src, const Rect &src_rect) override;

		void copy_image_from(
			int x,
			int y,
			int width,
			int height,
			int level,
			TextureFormat texture_format,
			GraphicContextProvider *gc) override;

		void copy_subimage_from(
			int offset_x,
			int offset_y,
			int x,
			int y,
			int width,
			int height,
			int level,
			GraphicContextProvider *gc) override;

		void set_min_lod(double min_lod) override;
		void set_max_lod(double max_lod) override;
		void set_lod_bias(double lod_bias) override;
		void set_base_level(int base_level) override;
		void set_max_level(int max_level) override;

		void set_wrap_mode(
			TextureWrapMode wrap_s,
			TextureWrapMode wrap_t,
			TextureWrapMode wrap_r) override;

		void set_wrap_mode(
			TextureWrapMode wrap_s,
			TextureWrapMode wrap_t) override;

		void set_wrap_mode(
			TextureWrapMode wrap_s) override;

		void set_min_filter(TextureFilter filter) override;
		void set_mag_filter(TextureFilter filter) override;
		void set_max_anisotropy(float v) override;

		void set_texture_compare(TextureCompareMode mode, CompareFunction func) override;

		TextureProvider *create_view(TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers) override;

		/// \brief Transform a non-power-of-two coordinate
		///
		/// \param attribute = Source attribute
		/// \param transformed_data = Destination (this will be set at a minimum of total_vertices*attribute.size
		/// \param vertex_offset = vertex offset
		/// \param num_vertices = Number of vertices
		/// \param total_vertices = Size to set the destination to
		void transform_coordinate(const PrimitivesArrayProvider::VertexData &attribute, std::vector<float> &transformed_data, int vertex_offset, int num_vertices, int total_vertices);

	private:
		void on_dispose() override;
		void set_texture_image2d(GLuint target, PixelBuffer &image, int level);
		void set_texture_image3d(GLuint target, PixelBuffer &image, int image_depth, int level);
		int get_next_power_of_two(int value);

		int width, height, depth;
		int pot_width, pot_height, pot_depth;
		float pot_ratio_width, pot_ratio_height, pot_ratio_depth;
		bool power_of_two_texture;

		/// \brief OpenGL texture handle.
		GLuint handle;

		GLuint texture_type;
	};

	class GL1TextureStateTracker
	{
	public:
		GL1TextureStateTracker(GLuint texture_type, GLuint handle);
		~GL1TextureStateTracker();

	private:
		GLboolean last_is_enabled_texture1d;
		GLboolean last_is_enabled_texture2d;
		GLboolean last_is_enabled_texture3d;
		GLboolean last_is_enabled_texture_cube_map;
		GLuint last_bound_texture1d;
		GLuint last_bound_texture2d;
		GLuint last_bound_texture3d;
		GLuint last_bound_texture_cube_map;
	};
}
