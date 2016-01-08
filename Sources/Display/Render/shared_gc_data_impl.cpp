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

#include "Display/precomp.h"
#include "API/Core/System/disposable_object.h"
#include "API/Display/Render/shared_gc_data.h"
#include <algorithm>
#include "shared_gc_data_impl.h"
#include "API/Display/Window/display_window_description.h"

namespace clan
{
	SharedGCData_Impl::SharedGCData_Impl() : reference_count(0)
	{
	}

	SharedGCData_Impl::~SharedGCData_Impl()
	{
	}

	std::vector<GraphicContextProvider*> &SharedGCData_Impl::get_gc_providers()
	{
		return graphic_context_providers;
	}

	void SharedGCData_Impl::add_provider(GraphicContextProvider *provider)
	{
		graphic_context_providers.push_back(provider);
	}
	void SharedGCData_Impl::remove_provider(GraphicContextProvider *provider)
	{
		if (graphic_context_providers.size() == 1)
			dispose_objects();

		for (auto it = graphic_context_providers.begin(); it != graphic_context_providers.end(); ++it)
		{
			if (provider == (*it))
			{
				graphic_context_providers.erase(it);
				break;
			}
		}
	}

	GraphicContextProvider *SharedGCData_Impl::get_provider()
	{
		if (graphic_context_providers.empty())
			return nullptr;
		return graphic_context_providers.back();
	}

	void SharedGCData_Impl::dispose_objects()
	{
		std::vector<DisposableObject*>::iterator it;
		for (it = disposable_objects.begin(); it != disposable_objects.end(); ++it)
			(*it)->dispose();
	}

	void SharedGCData_Impl::add_disposable(DisposableObject *disposable)
	{
		disposable_objects.push_back(disposable);
	}

	void SharedGCData_Impl::remove_disposable(DisposableObject *disposable)
	{
		std::vector<DisposableObject*>::iterator it;
		it = std::find(disposable_objects.begin(), disposable_objects.end(), disposable);
		if (it != disposable_objects.end())
			disposable_objects.erase(it);
	}
}
