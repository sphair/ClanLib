/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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


#include "gl3_graphic_context_provider.h"
#include "API/Display/TargetProviders/render_buffer_provider.h"
#include "API/Core/System/disposable_object.h"

namespace clan
{
	class GL3RenderBufferProvider : public RenderBufferProvider, DisposableObject
	{
	public:
		GL3RenderBufferProvider();
		~GL3RenderBufferProvider() override;

		GLuint get_handle();

		/// \brief Creates a render buffer image of the specified dimensions.
		void create(int width, int height, TextureFormat texture_format, int multisample_samples) override;

	private:
		void on_dispose() override;

		GLuint handle;
	};
}
