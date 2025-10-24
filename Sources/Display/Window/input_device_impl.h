/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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


#include "API/Core/System/mutex.h"
#include "API/Core/Signals/signal_v1.h"
#include "API/Display/TargetProviders/input_device_provider.h"
#include "input_context_impl.h"

class CL_InputDevice_Impl
{
public:
	CL_InputDevice_Impl()
	: provider(0)
	{
		slot_provider_event = sig_provider_event.connect(this, &CL_InputDevice_Impl::on_provider_event);
	}

	~CL_InputDevice_Impl()
	{
		if (provider)
			provider->destroy();
	}

	void on_provider_event(const CL_InputEvent &e)
	{
		CL_MutexSection mutex_lock(&CL_InputContext_Impl::mutex);
		std::vector< CL_WeakPtr<CL_InputContext_Impl> >::size_type pos, size;
		size = input_contexts.size();
		for (pos = 0; pos < size; pos++)
		{
			CL_WeakPtr<CL_InputContext_Impl> &ic = input_contexts[pos];
			if (!ic.is_null())
				ic->received_event(e, input_device);
		}
	}

	std::vector< CL_WeakPtr<CL_InputContext_Impl> > input_contexts;

	CL_WeakPtr<CL_InputDevice_Impl> input_device;

	CL_InputDeviceProvider *provider;

	CL_Signal_v1<const CL_InputEvent &> sig_provider_event;

	CL_Slot slot_provider_event;

	CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> sig_key_down;

	CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> sig_key_up;

	CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> sig_pointer_move;

	CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> sig_axis_move;

	CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> sig_key_dblclk;

	CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> sig_proximity_change;
};



