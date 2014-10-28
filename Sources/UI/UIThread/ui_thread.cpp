/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "UI/precomp.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/2D/canvas.h"
#include "API/UI/UIThread/ui_thread.h"

namespace clan
{
	class UIThreadImpl
	{
	public:
		ResourceManager resources;
	};

	static UIThread *ui_thread_instance = 0;

	UIThread::UIThread(ResourceManager manager) : impl(std::make_shared<UIThreadImpl>())
	{
		if (ui_thread_instance != 0) throw Exception("Only one UIThread is allowed");
		ui_thread_instance = this;
	}

	UIThread::~UIThread()
	{
		ui_thread_instance = 0;
	}

	UIThread *UIThread::get_instance()
	{
		if (ui_thread_instance == 0) throw Exception("No UIThread created");
		return ui_thread_instance;
	}

	ResourceManager UIThread::get_resources()
	{
		return get_instance()->impl->resources;
	}

	Canvas UIThread::get_resource_canvas()
	{
		return SharedGCData::get_resource_canvas();
	}
}
