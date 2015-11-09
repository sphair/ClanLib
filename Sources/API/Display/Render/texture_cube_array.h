/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "texture_cube.h"

namespace clan
{
	/// \addtogroup clanDisplay_Display clanDisplay Display
	/// \{

	/// \brief 2D texture cube array object class.
	class TextureCubeArray : public Texture
	{
	public:
		/// \brief Constructs a null instance.
		TextureCubeArray();

		/// \brief Constructs a texture from an implementation
		///
		/// \param impl = The implementation
		TextureCubeArray(const std::shared_ptr<Texture_Impl> &impl) : Texture(impl) { }

		/// \brief Constructs a Texture
		///
		/// \param context = Graphic Context
		/// \param width = value
		/// \param height = value
		/// \param internal_format = Texture Format
		/// \param levels = Mipmap levels for the texture. 0 = all levels
		TextureCubeArray(GraphicContext &context, int width, int height, int array_size, TextureFormat texture_format = tf_rgba8, int levels = 1);

		/// \brief Constructs a Texture
		///
		/// \param context = Graphic Context
		/// \param size = Size
		/// \param internal_format = Texture Format
		/// \param levels = Mipmap levels for the texture. 0 = all levels
		TextureCubeArray(GraphicContext &context, const Size &size, int array_size, TextureFormat texture_format = tf_rgba8, int levels = 1);

		/// \brief Get the texture width.
		int get_width() const;

		/// \brief Get the texture height.
		int get_height() const;

		/// \brief Get the texture size.
		Size get_size() const;

		/// \brief Get the texture array size
		int get_array_size() const;

		/// \brief Upload image to texture.
		///
		/// \param context Graphic context to use for the request
		/// \param array_index Index in the array
		/// \param image Image to upload.
		/// \param level Mipmap level-of-detail number.
		void set_image(
			GraphicContext &context,
			int array_index,
			TextureCubeDirection cube_direction,
			PixelBuffer &image,
			int level = 0);

		/// \brief Upload image to sub texture.
		///
		/// \param context Graphic context to use for the request
		/// \param array_index Index in the array
		/// \param image Image to upload.
		/// \param level Mipmap level-of-detail number.
		void set_subimage(
			GraphicContext &context,
			int array_index,
			TextureCubeDirection cube_direction,
			int x,
			int y,
			const PixelBuffer &image,
			const Rect &src_rect,
			int level = 0);

		void set_subimage(
			GraphicContext &context,
			int array_index,
			TextureCubeDirection cube_direction,
			const Point &point,
			const PixelBuffer &image,
			const Rect &src_rect,
			int level = 0);
	};

	/// \}
}
