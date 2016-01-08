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
**    Mark Page
*/

#pragma once

#include "API/Display/2D/canvas.h"
#include "API/Display/Window/display_window.h"

namespace clan
{
	class SharedGCData_Impl
	{
	public:
		SharedGCData_Impl();
		~SharedGCData_Impl();

		void add_provider(GraphicContextProvider *provider);
		void remove_provider(GraphicContextProvider *provider);
		std::vector<GraphicContextProvider*> &get_gc_providers();
		GraphicContextProvider *get_provider();

		void dispose_objects();
		void add_disposable(DisposableObject *disposable);
		void remove_disposable(DisposableObject *disposable);

		Canvas &get_resource_canvas();

		int reference_count;
		static std::recursive_mutex cl_sharedgc_mutex;
		static SharedGCData *cl_sharedgc;

	private:
		Signal<void()> sig_destruction_imminent;

		std::vector<GraphicContextProvider*> graphic_context_providers;
		std::vector<DisposableObject*> disposable_objects;
	};
}
