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
*/

#pragma once

#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl.h"
#include "API/Display/TargetProviders/primitives_array_provider.h"
#include "API/Core/System/disposable_object.h"

namespace clan
{
	class GL3GraphicContextProvider;

	class GL3PrimitivesArrayProvider : public PrimitivesArrayProvider, DisposableObject
	{
	public:
		GL3PrimitivesArrayProvider(GL3GraphicContextProvider *gc_provider);
		~GL3PrimitivesArrayProvider();

		GLuint handle;

		GL3GraphicContextProvider *get_gc_provider() { return gc_provider; }

		void set_attribute(int index, const VertexData &data, bool normalize) override;

	private:
		void on_dispose() override;

		GL3GraphicContextProvider *gc_provider;
	};

	class PrimitivesArrayStateTracker
	{
	public:
		PrimitivesArrayStateTracker(GL3GraphicContextProvider *gc_provider, GLuint handle);
		~PrimitivesArrayStateTracker();

	private:
		GLuint last_vao;
		bool vao_set;
	};
}
