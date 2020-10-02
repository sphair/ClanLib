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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Core/System/disposable_object.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/2D/canvas.h"
#include <algorithm>
#include "shared_gc_data_impl.h"

namespace clan
{
	std::recursive_mutex SharedGCData_Impl::cl_sharedgc_mutex;
	SharedGCData *SharedGCData_Impl::cl_sharedgc = nullptr;

	SharedGCData::SharedGCData() : impl(std::make_shared<SharedGCData_Impl>())
	{
	}

	SharedGCData::~SharedGCData()
	{
	}

	void SharedGCData::add_ref()
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(SharedGCData_Impl::cl_sharedgc_mutex);

		if (SharedGCData_Impl::cl_sharedgc == nullptr)
		{
			SharedGCData_Impl::cl_sharedgc = new SharedGCData;
		}
		SharedGCData_Impl::cl_sharedgc->impl->reference_count++;
	}

	void SharedGCData::release_ref()
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(SharedGCData_Impl::cl_sharedgc_mutex);

		if (SharedGCData_Impl::cl_sharedgc && --SharedGCData_Impl::cl_sharedgc->impl->reference_count == 0)
		{
			delete SharedGCData_Impl::cl_sharedgc;
			SharedGCData_Impl::cl_sharedgc = nullptr;
		}
	}

	void SharedGCData::add_provider(GraphicContextProvider *provider)
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(SharedGCData_Impl::cl_sharedgc_mutex);
		if (!SharedGCData_Impl::cl_sharedgc)
			throw Exception("Attempted to use an invalid SharedGCData");

		SharedGCData_Impl::cl_sharedgc->impl->add_provider(provider);

	}
	void SharedGCData::remove_provider(GraphicContextProvider *provider)
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(SharedGCData_Impl::cl_sharedgc_mutex);
		if (SharedGCData_Impl::cl_sharedgc)
			SharedGCData_Impl::cl_sharedgc->impl->remove_provider(provider);
	}

	std::vector<GraphicContextProvider*> &SharedGCData::get_gc_providers(std::unique_ptr<std::unique_lock<std::recursive_mutex>> &mutex_section)
	{
		mutex_section = std::unique_ptr<std::unique_lock<std::recursive_mutex>>(new std::unique_lock<std::recursive_mutex>(SharedGCData_Impl::cl_sharedgc_mutex));

		if (!SharedGCData_Impl::cl_sharedgc)
			throw Exception("Attempted to use an invalid SharedGCData");
		return SharedGCData_Impl::cl_sharedgc->impl->get_gc_providers();
	}

	GraphicContextProvider *SharedGCData::get_provider(std::unique_ptr<std::unique_lock<std::recursive_mutex>> &mutex_section)
	{
		mutex_section = std::unique_ptr<std::unique_lock<std::recursive_mutex>>(new std::unique_lock<std::recursive_mutex>(SharedGCData_Impl::cl_sharedgc_mutex));

		std::unique_lock<std::recursive_mutex> mutex_lock(SharedGCData_Impl::cl_sharedgc_mutex);
		if (SharedGCData_Impl::cl_sharedgc)
			return SharedGCData_Impl::cl_sharedgc->impl->get_provider();
		return nullptr;
	}

	void SharedGCData::add_disposable(DisposableObject *disposable)
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(SharedGCData_Impl::cl_sharedgc_mutex);
		if (!SharedGCData_Impl::cl_sharedgc)
			throw Exception("Attempted to use an invalid SharedGCData");

		SharedGCData_Impl::cl_sharedgc->impl->add_disposable(disposable);
	}

	void SharedGCData::remove_disposable(DisposableObject *disposable)
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(SharedGCData_Impl::cl_sharedgc_mutex);
		if (SharedGCData_Impl::cl_sharedgc)
		{
			SharedGCData_Impl::cl_sharedgc->impl->remove_disposable(disposable);
		}
	}
}
