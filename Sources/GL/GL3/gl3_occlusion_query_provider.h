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
**    Harry Storbacka
*/

#pragma once


#include "API/Display/TargetProviders/occlusion_query_provider.h"
#include "API/GL/opengl.h"
#include "API/Core/System/disposable_object.h"

namespace clan
{
	class GL3GraphicContextProvider;

	class GL3OcclusionQueryProvider : public OcclusionQueryProvider, DisposableObject
	{
	public:
		GL3OcclusionQueryProvider(GL3GraphicContextProvider *gc_provider);
		~GL3OcclusionQueryProvider();

		GLint get_result() const override;
		bool is_result_ready() const override;

		void begin() override;
		void end() override;
		void create() override;

	private:
		void on_dispose() override;

		/// \brief OpenGL occlusion query handle.
		GLuint handle;

		GL3GraphicContextProvider *gc_provider;
	};
}
